/***************************************************************************
                          gi_plantation.cpp  -  description
                             -------------------
    begin                : Wed Nov 07 2007
    copyright            : (C) 2007 by Karsten Vogt
    email                : vogt@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <gaimage.h>

#include <cmath>
#include <iostream>
#include <vector>

#include "vertex.h"
#include "thresholding.h"
#include "segmentation.h"
#include "meanDistance.h"
#include "clustering.h"
#include "polygon.h"

using namespace std;

int main(int argc, char **argv)
{
	char *inputFilename;
	char *outputFilename;
	double maximumClusterEdge = 50.0;
	double maximumMeanDeviation = 2.0;

	// Read parameters
	if (argc < 3)
	{
		std::cout << "Not enough parameters" << std::endl;
		return EXIT_FAILURE;
	}

	inputFilename = argv[1];
	outputFilename = argv[2];

	if (argc >= 4)
		maximumClusterEdge = atof(argv[3]);
	if (argc >= 5)
		maximumMeanDeviation = atof(argv[4]);

	// Load input image
	Ga::Image inputImage;
	if (!inputImage.read(inputFilename))
	{
		std::cout << "Could not read file " << inputFilename << std::endl;
		return EXIT_FAILURE;
	}

	// Find segments by thresholding and segmentation
	Ga::Image luminanceImage = inputImage.convert2Luminance();
	thresholdImage(luminanceImage, 15);

	vector<Segment> segments;
	segmentImage(inputImage, segments);

	// Reject segments that don't represent trees, find the center of all remaining segments

	// Calculate the mean distance of its nearest neighbours for each tree
	// Place it in a 3D feature space
	vector<double> meanDistances;
	vector<Vertex<double> > features;
	
	calculateMeanDistances(segments, meanDistances, 3);
	for (size_t i=0; i<segments.size(); i++)
	{
		Vertex<double> vertex;
		vertex.at(0) = segments.at(i).center()[0];
		vertex.at(1) = segments.at(i).center()[1];
		vertex.at(2) = log(meanDistances.at(i));
		features.push_back(vertex);
	}

	// Construct the minimal spanning tree in feature space
	// Use it to separate tree clusters
	ClusterAnalysis<Vertex<double> > clusterAnalysis;
	clusterAnalysis.startFresh();

	for (size_t i=0; i<features.size(); i++)
		clusterAnalysis.addFeature(features.at(i));

	clusterAnalysis.findClusters_AbsEdge(maximumClusterEdge);
	vector<vector<Vertex<double> > > clusters = clusterAnalysis.getClusters();

	// Reject clusters that are not plantations
	for (vector<vector<Vertex<double> > >::iterator iter=clusters.begin(); iter!=clusters.end();)
	{
		bool removeCluster = false;
		
		if (iter->size() < 4)
			removeCluster = true;
		
		if (removeCluster)
			iter = clusters.erase(iter);
		else
			iter++;
	}

	// Find the convex hull of all remaining clusters and generate a label image
	Polygon<double> polygon;
	Ga::Image labelImage(typeid(int), luminanceImage.sizeX(), luminanceImage.sizeY());
	
	for (size_t i=0; i<clusters.size(); i++)
	{
		std::cout << "Cluster " << i << ": size(" << clusters[i].size() << "), label(" << (i+1) << ")" << std::endl;
		polygon.computeConvexHull(clusters[i]);
		polygon.render(labelImage, (int)i + 1);
	}

	labelImage.write(outputFilename);

	return 0;
}
