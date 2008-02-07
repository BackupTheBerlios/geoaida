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

double calculateMean(Ga::Image &image, int channel, int left, int right, int top, int bottom)
{
	double pixel = (right-left+1)*(bottom-top+1);
	double mean = 0.0;

	for (int y=top; y<bottom; y++)
		for (int x=left; x<right; x++)
		{
			int x1 = std::min(image.sizeX()-1, std::max(0, x));
			int y1 = std::min(image.sizeY()-1, std::max(0, y));
			
			mean += double(image.getInt(x1, y1, channel));
		}

	return mean/pixel;
}

double calculateNCC(Ga::Image &image1, int offsetx1, int offsety1, double mean1, Ga::Image &image2, int offsetx2, int offsety2, double mean2, int width, int height, int channel)
{
	double ncorr1 = 0.0;
	double ncorr21 = 0.0, ncorr22 = 0.0;
	for (int y=0; y<height; y++)
	{
		for (int x=0; x<width; x++)
		{
			double color1 = double(image1.getInt(x+offsetx1, y+offsety1, channel));
			double color2 = double(image2.getInt(x+offsetx2, y+offsety2, channel));

			ncorr1 += (color1 - mean1) * (color2 - mean2);
			ncorr21 += (color1 - mean1) * (color1 - mean1);
			ncorr22 += (color2 - mean2) * (color2 - mean2);
		}
	}
	
	return ncorr1 / sqrt(ncorr21 * ncorr22);
}

void fillCutout(Ga::Image &image, int left, int top, int width, int height)
{
	for (int y=0; y<height; y++)
		for (int x=0; x<width; x++)
			image.set(x+left, y+top, 255);
}

bool checkCutout(Ga::Image &image, int left, int top, int width, int height)
{
	for (int y=0; y<height; y++)
		for (int x=0; x<width; x++)
			if (image.getInt(x+left, y+top))
				return false;

	return true;
}

void segmentImageNCC(Ga::Image &image, Ga::Image &match, double threshold, std::vector<Segment> &segments, Ga::Image &cutoutImage)
{
	double tMean[] = {calculateMean(match, 0, 0, match.sizeX(), 0, match.sizeY()), calculateMean(match, 1, 0, match.sizeX(), 0, match.sizeY()), calculateMean(match, 2, 0, match.sizeX(), 0, match.sizeY())};

	for (int y=0; y<=(image.sizeY()-match.sizeY()*2); y++)
	{
		for (int x=0; x<=(image.sizeX()-match.sizeX()*2); x++)
		{
			if (!checkCutout(cutoutImage, x, y, match.sizeX(), match.sizeY()))
				continue;
			
			double iMean[] = {calculateMean(image, 0, x, x+match.sizeX(), y, y+match.sizeY()), calculateMean(image, 1, x, x+match.sizeX(), y, y+match.sizeY()), calculateMean(image, 2, x, x+match.sizeX(), y, y+match.sizeY())};

			double ncc0 = calculateNCC(image, x, y, iMean[0], match, 0, 0, tMean[0], match.sizeX(), match.sizeY(), 0);
			double ncc1 = calculateNCC(image, x, y, iMean[1], match, 0, 0, tMean[1], match.sizeX(), match.sizeY(), 1);
			double ncc2 = calculateNCC(image, x, y, iMean[2], match, 0, 0, tMean[2], match.sizeX(), match.sizeY(), 2);
			double ncc = ncc0 * ncc1 * ncc2;
			
			if (ncc > threshold)
			{
				// Find block with maximum ncc in the neighbourhood
				int maxX = x;
				int maxY = y;
				double maxNCC = ncc;
				
				for (int y1=0; y1<match.sizeY()/2; y1++)
				{
					for (int x1=0; x1<match.sizeX()/2; x1++)
					{
						double lncc0 = calculateNCC(image, x+x1, y+y1, iMean[0], match, 0, 0, tMean[0], match.sizeX(), match.sizeY(), 0);
						double lncc1 = calculateNCC(image, x+x1, y+y1, iMean[1], match, 0, 0, tMean[1], match.sizeX(), match.sizeY(), 1);
						double lncc2 = calculateNCC(image, x+x1, y+y1, iMean[2], match, 0, 0, tMean[2], match.sizeX(), match.sizeY(), 2);
						double lncc = lncc0 * lncc1 * lncc2;
						
						if (lncc > maxNCC)
						{
							maxX = x + x1;
							maxY = y + y1;
							maxNCC = lncc;
						}
					}
				}
				
				// Add segment
				Segment segment;
				Vertex<int> center;
				center[0] = maxX + match.sizeX()/2;
				center[1] = maxY + match.sizeY()/2;
				
				segment.setCenter(center);
				segment.setDiameter(match.sizeX());
				segments.push_back(segment);

				fillCutout(cutoutImage, maxX+1, maxY+1, match.sizeX()-2, match.sizeY()-2);
			}
		}
	}
}
