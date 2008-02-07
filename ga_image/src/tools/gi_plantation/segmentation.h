#ifndef _SEGMENTATION_H_
#define _SEGMENTATION_H_

#include <gaimage.h>
#include <vector>

#include "vertex.h"

class Segment
{
	public:
		Segment();
		
		Vertex<int> center() const;
		void setCenter(const Vertex<int> &center);
		
		float diameter() const;
		void setDiameter(float diameter);
		
	private:
		Vertex<int> _center;
		float _diameter;
};

void segmentImage(Ga::Image &image, std::vector<Segment> &segments);
void segmentImageNCC(Ga::Image &image, Ga::Image &match, double threshold, std::vector<Segment> &segments, Ga::Image &cutoutImage);

#endif
