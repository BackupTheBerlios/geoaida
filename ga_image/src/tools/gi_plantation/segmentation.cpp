#include "segmentation.h"
#include <list>

#define MARKING			128
#define IS_BGCOLOR(x)	((x) > MARKING)
#define IS_FGCOLOR(x)	((x) < MARKING)

Segment::Segment() : _center(Vertex<int>()), _diameter(0.0f)
{
}

Vertex<int> Segment::center() const
{
	return _center;
}

void Segment::setCenter(const Vertex<int> &center)
{
	_center = center;
}

float Segment::diameter() const
{
	return _diameter;
}

void Segment::setDiameter(float diameter)
{
	_diameter = diameter;
}

void markandfind(Ga::Image &image, int x, int y, std::list<Vertex<int> > &regionpixels)
{
	if ((x < 0) || (y < 0) || (x >= image.sizeX()) || (y >= image.sizeY()) || (image.getInt(x, y) == MARKING))
		return;

	Vertex<int> vertex;
	vertex.at(X) = x;
	vertex.at(Y) = y;
	regionpixels.push_back(vertex);
	image.set(x, y, MARKING);

	markandfind(image, x-1, y, regionpixels);
	markandfind(image, x+1, y, regionpixels);
	markandfind(image, x, y-1, regionpixels);
	markandfind(image, x, y+1, regionpixels);
}

void segmentImage(Ga::Image &image, std::vector<Segment> &segments)
{	
	segments.clear();

	// Discard all background pixels
	for (int y=0; y<image.sizeY(); y++)
		for (int x=0; x<image.sizeX(); x++)
			if (IS_BGCOLOR(image.getInt(x, y)))
				image.set(x, y, MARKING);

	// Find all regions with foreground pixels
	while (true)
	{
		std::list<Vertex<int> > regionpixels;
		
		// Find next unmarked pixel
		bool foundNextRegion = false;
		int startX=0, startY=0;
		for (int y=0; y<image.sizeY(); y++)
		{
			for (int x=0; x<image.sizeX(); x++)
			{
				if (IS_FGCOLOR(image.getInt(x, y)))
				{
					foundNextRegion = true;
					startX = x;
					startY = y;
				}
			}
		}

		if (!foundNextRegion)
			break;

		// Find all region pixels using flood-fill and mark them
		markandfind(image, startX, startY, regionpixels);

		// Create new segments from region pixels
		Segment segment;
		Vertex<int> center;
		float diameter;

		for (std::list<Vertex<int> >::iterator iter=regionpixels.begin(); iter!=regionpixels.end(); iter++)
		{
			center = center + *iter;
		}
		center = center / regionpixels.size();

		for (std::list<Vertex<int> >::iterator iter=regionpixels.begin(); iter!=regionpixels.end(); iter++)
		{
		}

		segment.setCenter(center);
		segment.setDiameter(diameter);
		segments.push_back(segment);
	}
}
