#include "meanDistance.h"

#include <algorithm>
#include <utility>

using namespace std;

void calculateMeanDistances(const vector<Segment> &segments, vector<double> &meanDistances, int nnCount)
{
	nnCount = std::min(nnCount, (int)segments.size()-1);
	meanDistances.resize(segments.size());

	for (size_t i=0; i<segments.size(); i++)
	{
		std::vector<std::pair<size_t, double> > nearestNeighbours;
		nearestNeighbours.reserve(segments.size()-1);
		
		for (size_t j=0; j<segments.size(); j++)
		{
			if (i == j)
				continue;

			double distance = (segments.at(i).center() - segments.at(j).center()).euclideanLength();
			nearestNeighbours.push_back(std::pair<size_t, double>(j, distance));
		}
		
		std::sort(nearestNeighbours.begin(), nearestNeighbours.end(), NNSort());
		
		double meanDistance = 0;
		for (size_t k=0; k<nnCount; k++)
			meanDistance += nearestNeighbours.at(k).second;
		meanDistances.at(i) = meanDistance / (double)nnCount;
	}
}
