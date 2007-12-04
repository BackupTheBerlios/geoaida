#ifndef _MEANDISTANCE_H_
#define _MEANDISTANCE_H_

#include <vector>
#include "segmentation.h"

struct NNSort
{
	bool operator ()(const std::pair<size_t, double> &vec1, const std::pair<size_t, double> &vec2)
	{
		return vec1.second < vec2.second;
	}
};

void calculateMeanDistances(const std::vector<Segment> &segments, std::vector<double> &meanDistances, int nnCount);

#endif
