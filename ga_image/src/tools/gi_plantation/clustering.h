#ifndef _CLUSTERING_H_
#define _CLUSTERING_H_

#include <utility>
#include <vector>
#include <map>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/properties.hpp>

using namespace boost;

template<class Feature>
class ClusterAnalysis
{
	public:
		typedef adjacency_list<vecS, vecS, undirectedS, no_property, property<edge_weight_t, float> > GraphType;
		typedef std::pair<int, int> EdgeType;
		
		typedef graph_traits<GraphType>::vertex_descriptor VertexDescriptor;
		typedef graph_traits<GraphType>::edge_descriptor EdgeDescriptor;
		
		void startFresh();
		VertexDescriptor addFeature(Feature &feature);

		std::vector<std::vector<Feature> > getClusters();
		void findClusters_AbsEdge(double minWeight);
		void findClusters_ClusterCount(int noCluster);
		
	private:
		void computeSpanningTree();
		void printClusters();
		
		GraphType graph;
		std::map<VertexDescriptor, Feature> nodeData;
		
		std::vector<EdgeDescriptor> spanningTree;
		std::vector<std::vector<int> > clusters;
};

#include "clustering.cpp"

#endif
