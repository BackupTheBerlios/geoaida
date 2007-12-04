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
		void ClusterAnalysis<Feature>::findClusters_AbsEdge(double minWeight)
{
	computeSpanningTree();
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
	computeSpanningTree();
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
		void ClusterAnalysis<Feature>::computeSpanningTree()
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

			EdgeDescriptor descriptor = add_edge(iter1->first, iter2->first, graph).first;
			weightmap[descriptor] = (iter1->second - iter2->second).euclideanLength();
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
