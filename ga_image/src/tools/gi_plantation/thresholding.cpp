#include "thresholding.h"

using namespace Ga;

void thresholdImage(Image &image, int threshold)
{
	for (int channel = 0; channel < image.noChannels(); channel++)
	{
		for (int y = 0; y < image.sizeY(); y++)
		{
			for (int x = 0; x < image.sizeX(); x++)
			{
				int pixelValue = image.getInt(x, y, channel);
				pixelValue = (pixelValue <= threshold) ? 0 : 255;
				image.set(x, y, pixelValue, channel);
			}
		}
	}
}
