template<class Feature>
		void ClusterAnalysis<Feature>::startFresh()
{
	graph.clear();
	nodeData.clear();
	spanningTree.clear();
}

template<class Feature>
		typename ClusterAnalysis<Feature>::VertexDescriptor ClusterAnalysis<Feature>::addFeature(Feature &feature)
{
	VertexDescriptor descriptor = add_vertex(graph);
	nodeData[descriptor] = feature;
	
	return descriptor;
}


template<class Feature>
		std::vector<std::vector<Feature> > ClusterAnalysis<Feature>::getClusters()
{
	std::vector<std::vector<Feature> > returnClusters;
	returnClusters.resize(clusters.size());

	for (size_t i=0; i<clusters.size(); i++)
		for (size_t j=0; j<clusters[i].size(); j++)
			returnClusters[i].push_back(nodeData[static_cast<VertexDescriptor>(clusters[i][j])]);

	return returnClusters;
}

template<class Feature>
		void ClusterAnalysis<Feature>::findClusters_AbsEdge(double minWeight, double maxWeight)
{
	computeSpanningTree(maxWeight);
	property_map<GraphType, edge_weight_t>::type weightmap = get(edge_weight, graph);

	// Find n largest edge weights and delete the edges
	for (std::vector<EdgeDescriptor>::iterator edgeIter=spanningTree.begin(); edgeIter!=spanningTree.end(); )
	{
		if (weightmap[*edgeIter] > minWeight)
		{
			remove_edge(*edgeIter, graph);
			edgeIter = spanningTree.erase(edgeIter);
		}
		else
			edgeIter++;
	}

	// Collect remaining clusters
	std::vector<int> component(num_vertices(graph));
	int clusterCount = connected_components(graph, &component[0]);

	clusters.clear();
	clusters.resize(clusterCount);
	for (size_t i=0; i<component.size(); i++)
	{
		clusters[component[i]].push_back(i);
	}

	//printClusters();
}

template<class Feature>
		void ClusterAnalysis<Feature>::findClusters_ClusterCount(int noCluster)
{
	computeSpanningTree(-1);
	property_map<GraphType, edge_weight_t>::type weightmap = get(edge_weight, graph);

	// Find n largest edge weights and delete the edges
	for (int n=0; n<noCluster-1; n++)
	{
		float largestWeight = -1.0f;
		std::vector<EdgeDescriptor>::iterator edgeIter;
		EdgeDescriptor edge;

		for (std::vector<EdgeDescriptor>::iterator iter=spanningTree.begin(); iter!=spanningTree.end(); iter++)
		{
			if (weightmap[*iter] > largestWeight)
			{
				edgeIter = iter;
				edge = *iter;
				largestWeight = weightmap[edge];
			}
		}

		remove_edge(edge, graph);
		spanningTree.erase(edgeIter);
	}

	// Collect remaining clusters
	std::vector<int> component(num_vertices(graph));
	int clusterCount = connected_components(graph, &component[0]);

	clusters.clear();
	clusters.resize(clusterCount);
	for (size_t i=0; i<component.size(); i++)
	{
		clusters[component[i]].push_back(i);
	}

	//printClusters();
}

template<class Feature>
		void ClusterAnalysis<Feature>::findClusters_NNAbsEdge(int nnCount, double maxWeight)
{
	if (nodeData.size() == 0)
	{
		std::cout << "No nodes available!" << std::endl;
		return;
	}
	
	computeKNNTree(nnCount, maxWeight);
	
	// Collect remaining clusters
	std::vector<int> component(num_vertices(graph));
	int clusterCount = connected_components(graph, &component[0]);
	
	clusters.clear();
	clusters.resize(clusterCount);
	for (size_t i=0; i<component.size(); i++)
	{
		clusters[component[i]].push_back(i);
	}
}

template<class Feature>
		void ClusterAnalysis<Feature>::findClusters_FLAME(int nnCount, int iterations, double maxWeight, double densityThreshold)
{
	if (nodeData.size() == 0)
	{
		std::cout << "No nodes available!" << std::endl;
		return;
	}

	// Compute k-nearest-neighbours tree
	computeKNNTree(nnCount, maxWeight);
	
	// Compute density for each node
	std::map<VertexDescriptor, double> densityMap;
	for (typename std::map<VertexDescriptor, Feature>::iterator iter = nodeData.begin(); iter != nodeData.end(); iter++)
	{
		VertexDescriptor node = iter->first;
		Feature position = iter->second;

		// Build bounding-box
		Feature minPosition = position, maxPosition = position;
		std::pair<AdjacencyIterator, AdjacencyIterator> neighbourIterators = adjacent_vertices(node, graph);
		for (; neighbourIterators.first != neighbourIterators.second; neighbourIterators.first++)
		{
			minPosition = Feature::minimize(minPosition, nodeData[*neighbourIterators.first]);
			maxPosition = Feature::maximize(maxPosition, nodeData[*neighbourIterators.first]);
		}

		// Calculate approximated density
		double volume = std::max(0.00001, (maxPosition - minPosition).volume());
		densityMap[node] = static_cast<double>(nnCount) / volume;
	}
	
	// Compute cluster type for each node
	const int ClusterRest = 0;
	const int ClusterSupport = 1;
	const int ClusterOutlier = 2;

	std::map<VertexDescriptor, int> typeMap;
	for (typename std::map<VertexDescriptor, Feature>::iterator iter = nodeData.begin(); iter != nodeData.end(); iter++)
	{
		VertexDescriptor node = iter->first;
		Feature position = iter->second;
		
		// Classify node from neighbour densities
		bool support = true;
		bool outlier = true;
		std::pair<AdjacencyIterator, AdjacencyIterator> neighbourIterators = adjacent_vertices(node, graph);
		for (; neighbourIterators.first != neighbourIterators.second; neighbourIterators.first++)
		{
			if (densityMap[node] <= densityMap[*neighbourIterators.first])
				support = false;
			if (densityMap[node] >= densityMap[*neighbourIterators.first])
				outlier = false;
		}
		
		if (support)
			typeMap[node] = ClusterSupport;
		else if (outlier && (densityMap[node] < densityThreshold))
			typeMap[node] = ClusterOutlier;
		else
			typeMap[node] = ClusterRest;
	}
	
	// Compute starting information for iteration loop
	std::map<VertexDescriptor, int> clusterSupportList;
	std::map<VertexDescriptor, std::map<int, double> > clusterMembership;

	int clusterCount = 1;
	for (typename std::map<VertexDescriptor, int>::iterator iter = typeMap.begin(); iter != typeMap.end(); iter++)
	{
		if (iter->second == ClusterSupport)
			clusterSupportList[iter->first] = clusterCount++;
	}
	
	for (typename std::map<VertexDescriptor, int>::iterator iter = typeMap.begin(); iter != typeMap.end(); iter++)
	{
		VertexDescriptor node = iter->first;
		int nodeType = iter->second;
		
		if (nodeType == ClusterSupport)
			clusterMembership[node][clusterSupportList[node]] = 1.0;
		else if (nodeType == ClusterOutlier)
			clusterMembership[node][0] = 1.0;
		else
		{
			for (int i=0; i<clusterCount; i++)
				clusterMembership[node][i] = 1.0 / static_cast<double>(clusterCount);
		}
	}
	
	// Start iteration
	while (iterations--)
	{
		std::map<VertexDescriptor, std::map<int, double> > clusterMembershipCopy = clusterMembership;
		
		for (typename std::map<VertexDescriptor, int>::iterator iter = typeMap.begin(); iter != typeMap.end(); iter++)
		{
			VertexDescriptor node = iter->first;
			int nodeType = iter->second;
			
			if (nodeType != ClusterRest)
				continue;
			
			clusterMembership[node].clear();
			
			// Update the cluster membership data
			int neighbourCount = out_degree(node, graph);
			std::pair<AdjacencyIterator, AdjacencyIterator> neighbourIterators = adjacent_vertices(node, graph);
			for (; neighbourIterators.first != neighbourIterators.second; neighbourIterators.first++)
			{
				VertexDescriptor neighbourNode = *neighbourIterators.first;
				for (std::map<int, double>::iterator clusterIter=clusterMembershipCopy[neighbourNode].begin(); clusterIter!=clusterMembershipCopy[neighbourNode].end(); clusterIter++)
					clusterMembership[node][clusterIter->first] += clusterIter->second / static_cast<double>(neighbourCount);
			}
		}

		std::cout << iterations << " Iterations to go" << std::endl;
	}
	
	// Create Segments from fuzzy cluster membership
	clusters.clear();
	clusters.resize(clusterCount);
	for (typename std::map<VertexDescriptor, Feature>::iterator iter = nodeData.begin(); iter != nodeData.end(); iter++)
	{
		VertexDescriptor node = iter->first;
		
		double maxProbability = 0.0;
		int maxCluster = 0;
		for (std::map<int, double>::iterator clusterIter=clusterMembership[node].begin(); clusterIter!=clusterMembership[node].end(); clusterIter++)
		{
			if (clusterIter->second > maxProbability)
			{
				maxProbability = clusterIter->second;
				maxCluster = clusterIter->first;
			}
		}
		
		clusters[maxCluster].push_back(node);
	}
}

template<class Feature>
		void ClusterAnalysis<Feature>::computeSpanningTree(double maxWeight)
{
	// Add weighted edges
	property_map<GraphType, edge_weight_t>::type weightmap = get(edge_weight, graph);
	for (typename std::map<VertexDescriptor, Feature>::iterator iter1 = nodeData.begin(); iter1 != nodeData.end(); iter1++)
	{
		typename std::map<VertexDescriptor, Feature>::iterator iter2(iter1);
		for (typename std::map<VertexDescriptor, Feature>::iterator iter2 = iter1; iter2 != nodeData.end(); iter2++)
		{
			if (iter1 == iter2)
				continue;

			double weight = (iter1->second - iter2->second).euclideanLength();
			if ((maxWeight > 0.0) && (weight > maxWeight))
				continue;
			
			EdgeDescriptor descriptor = add_edge(iter1->first, iter2->first, graph).first;
			weightmap[descriptor] = weight;
		}
	}

	// Build minimal spanning tree
	spanningTree.clear();
	kruskal_minimum_spanning_tree(graph, std::back_inserter(spanningTree));

	// Remove all edges that are not part of the spanning Tree
	for (typename std::map<VertexDescriptor, Feature>::iterator iter1 = nodeData.begin(); iter1 != nodeData.end(); iter1++)
	{
		typename std::map<VertexDescriptor, Feature>::iterator iter2(iter1);
		for (typename std::map<VertexDescriptor, Feature>::iterator iter2 = iter1; iter2 != nodeData.end(); iter2++)
		{
			if (iter1 == iter2)
				continue;

			EdgeDescriptor descriptor;
			bool isElementOf;
			tie(descriptor, isElementOf) = edge(iter1->first, iter2->first, graph);

			if (!isElementOf)
				continue;

			if (std::find(spanningTree.begin(), spanningTree.end(), descriptor) == spanningTree.end())
				remove_edge(descriptor, graph);
		}
	}
}

template<class Feature>
		void ClusterAnalysis<Feature>::computeKNNTree(int nnCount, double maxWeight)
{
	nnCount = std::min(nnCount, (int)nodeData.size()-1);

	// Find nearest neighbours for each vertex
	property_map<GraphType, edge_weight_t>::type weightmap = get(edge_weight, graph);
	for (typename std::map<VertexDescriptor, Feature>::iterator iter1 = nodeData.begin(); iter1 != nodeData.end(); iter1++)
	{
		std::vector<std::pair<VertexDescriptor, double> > neighbours;
		
		for (typename std::map<VertexDescriptor, Feature>::iterator iter2 = nodeData.begin(); iter2 != nodeData.end(); iter2++)
		{
			if (iter1 == iter2)
				continue;
			
			double weight = (iter1->second - iter2->second).euclideanLength();
			neighbours.push_back(std::pair<VertexDescriptor, double>(iter2->first, weight));
		}

		std::sort(neighbours.begin(), neighbours.end(), NeighbourSort());
		
		// Add edges for the n nearest neighbours (if the distance is not too great)
		for (size_t k=0; k<std::min((int)neighbours.size(), nnCount); k++)
		{
			if ((maxWeight > 0.0) && (neighbours[k].second > maxWeight))
				continue;
			
			EdgeDescriptor descriptor = add_edge(iter1->first, neighbours[k].first, graph).first;
			weightmap[descriptor] = neighbours[k].second;
		}
	}

	std::cout << "Finished building NN-Graph" << std::endl;
}

template<class Feature>
		void ClusterAnalysis<Feature>::printClusters()
{
	for (size_t i=0; i<clusters.size(); i++)
	{
		std::cout << "Clusters " << i << ":" << std::endl;
		for (size_t j=0; j<clusters[i].size(); j++)
		{
			double x = (nodeData[static_cast<VertexDescriptor>(clusters[i][j])])[0];
			double y = (nodeData[static_cast<VertexDescriptor>(clusters[i][j])])[1];
			std::cout << "Vertex " << clusters[i][j] << " (" << x << ", " << y << ")" << std::endl;
		}
		std::cout << std::endl;
	}
}
