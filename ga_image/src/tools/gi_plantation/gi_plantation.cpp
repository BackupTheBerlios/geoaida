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
#include <sstream>

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
	double meanDistanceScaling = 500.0;
	double maximumClusterEdge = 50.0;
	double maximumMeanVariance = 2.0;

	// Read parameters
	if (argc < 3)
	{
		std::cout << "Not enough parameters" << std::endl;
		return EXIT_FAILURE;
	}

	inputFilename = argv[1];
	outputFilename = argv[2];

	if (argc >= 4)
		meanDistanceScaling = atof(argv[3]);
	if (argc >= 5)
		maximumClusterEdge = atof(argv[4]);
	if (argc >= 6)
		maximumMeanVariance = atof(argv[5]);

	std::cout << "Arguments read" << std::endl;

	// Load input image
	Ga::Image inputImage;
	if (!inputImage.read(inputFilename))
	{
		std::cout << "Could not read file " << inputFilename << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Input image loaded" << std::endl;
	
	// Find segments by thresholding and segmentation
	/*Ga::Image luminanceImage;
	switch (inputImage.noChannels())
	{
		case 1:
			luminanceImage = inputImage;
			break;
		case 3:
			luminanceImage = inputImage.convert2Luminance();
			break;

		default:
			return 5;
	}

	std::cout << "1" << std::endl;
	thresholdImage(luminanceImage, 20);
	luminanceImage.blow(0, 3);
	std::cout << "2" << std::endl;

	vector<Segment> segments;
	segmentImage(luminanceImage, segments);*/

	// Find segments using NCC
	double nccThreshold = 0.4;

	// Load tree masks
	vector<Ga::Image> treeMasks;
	for (int i=0;; i++)
	{
		std::stringstream filename;
		filename << "treemasks/treemask" << i << ".ppm";
		
		Ga::Image tmpImage;
		if (!tmpImage.read(filename.str().c_str()))
			break;
		else
			treeMasks.push_back(tmpImage);
	}

	if (treeMasks.size() == 0)
	{
		std::cout << "Could not find tree masks!" << std::endl;
		return EXIT_FAILURE;
	}

	// Use NCC to compute tree regions
	vector<Segment> segments;
	Ga::Image cutoutImage(typeid(unsigned char), inputImage.sizeX(), inputImage.sizeY());
	for (int i=0; i<treeMasks.size(); i++)
		segmentImageNCC(inputImage, treeMasks[i], nccThreshold, segments, cutoutImage);
	
	std::cout << "Segmentation finished" << std::endl;
	std::cout << "Number of segments found: " << segments.size() << std::endl;
	//cutoutImage.write("PlantationTreeCutoutTest.pgm");
	
	// Reject segments that don't represent trees

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
		vertex.at(2) = log(meanDistances.at(i)) * meanDistanceScaling;
		features.push_back(vertex);
	}

	std::cout << "Built feature space" << std::endl;
	
	// Construct the minimal spanning tree in feature space
	// Use it to separate tree clusters
	ClusterAnalysis<Vertex<double> > clusterAnalysis;
	clusterAnalysis.startFresh();

	for (size_t i=0; i<features.size(); i++)
		clusterAnalysis.addFeature(features.at(i));

	clusterAnalysis.findClusters_AbsEdge(maximumClusterEdge, 150.0);
	//clusterAnalysis.findClusters_NNAbsEdge(4, 50.0);
	//clusterAnalysis.findClusters_FLAME(64, 20, 150.0, 0.01);
	vector<vector<Vertex<double> > > clusters = clusterAnalysis.getClusters();

	// Reject clusters that are not plantations
	for (vector<vector<Vertex<double> > >::iterator iter=clusters.begin(); iter!=clusters.end();)
	{
		bool removeCluster = false;

		// A plantation has at least eight trees
		if (iter->size() < 8)
			removeCluster = true;

		// Check for an uneven distribution of mean distances
		double mean = 0.0;
		for (vector<Vertex<double> >::iterator clusterIter=iter->begin(); clusterIter!=iter->end(); clusterIter++)
			mean += (*clusterIter)[2];
		mean /= static_cast<double>(iter->size());
		
		double variance = 0.0;
		for (vector<Vertex<double> >::iterator clusterIter=iter->begin(); clusterIter!=iter->end(); clusterIter++)
			variance += ((*clusterIter)[2] - mean) * ((*clusterIter)[2] - mean);
		variance /= static_cast<double>(iter->size());

		if (variance > maximumMeanVariance)
			removeCluster = true;

		// Remove cluster if necessary
		if (removeCluster)
			iter = clusters.erase(iter);
		else
			iter++;
	}

	std::cout << "Feature clustering finished" << std::endl;
	
	// Find the convex hull of all remaining clusters and generate a label image
	Polygon<double> polygon;
	Ga::Image labelImage(typeid(int), inputImage.sizeX(), inputImage.sizeY());
	
	for (size_t i=0; i<clusters.size(); i++)
	{
		std::cout << "Cluster " << i << ": size(" << clusters[i].size() << "), label(" << (i+1) << ")" << std::endl;
		polygon.computeConvexHull(clusters[i]);
		polygon.render(labelImage, (int)i + 1);
	}

	labelImage.write(outputFilename);

	std::cout << "Output image saved" << std::endl;

	return 0;
}
