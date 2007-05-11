/* -*- mode:c++ -*- */
//*******************************************************************
//  Filename:   gacanny.cpp
//  Ort:        TNT, Uni. - Hannover, Germany
//
//  Info:	Gaussian Image Blur
//
// Copyright (C) 2007 Julian Raschke
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//           --------      
//          /     ^ /      
//         /  ^^   /--     
//        / ^  ^^/  /
//        --------  /--
//         /    * /+ /
//         --------  /
//          /   ~~  /
//          --------
//
//*******************************************************************

#include "gacanny.h"
#include <vector>
#include <cmath>
#include <algorithm>

// Local helpers.

namespace {
    using namespace Ga;

    Image normalizeImage(Image &source) {
        int sizeX = source.sizeX(), sizeY = source.sizeY();
        Image result(typeid(float), sizeX, sizeY);

    	float min = source.minValue();
    	float range = source.maxValue() - min;

        for (int x = 0; x < sizeX; ++x)
            for (int y = 0; y < sizeY; ++y) {
    			float pix = source.getFloat(x, y, 0);
    			pix -= min;
    			pix /= range;
    			result.set(x, y, pix, 0);
    		}

    	return result;
    }

    const int SOBEL_X[9]   = {  1,  0, -1,    2,  0, -2,    1,  0, -1 };
    const int SOBEL_Y[9]   = {  1,  2,  1,    0,  0,  0,   -1, -2, -1 };

    // Helper for convolution:
    // Adjust x/y when going over edge
    float getPixel(const Image &source, int x, int y) {
        if (x < 0)
            x = 0;
        else if (x >= source.sizeX())
            x = source.sizeX() - 1;
        if (y < 0)
            y = 0;
        else if (y >= source.sizeY())
            y = source.sizeY() - 1;
        return source.getFloat(x, y, 0);
    }

    // Helper for convolution:
    // Adjust val when outside 0..1
    void setPixel(Image &dest, int x, int y, float val) {
        if (val < 0)
            val = 0;
        else if (val > 1)
            val = 1;
        dest.set(x, y, val, 0);
    }

    Image convolve(const Image &source, const int (&coefficients)[9], double factor, double base) {
        int sizeX = source.sizeX(), sizeY = source.sizeY();
        Image result(typeid(float), sizeX, sizeY);

        for (int x = 0; x < sizeX; ++x)
            for (int y = 0; y < sizeY; ++y) {
                float accum = base;
                for (int i = 0; i < 9; ++i) {
                    int relX = (i % 3) - 1;
                    int relY = (i / 3) - 1;
                    accum += getPixel(source, x + relX, y + relY) * coefficients[i] * factor;
                }

                setPixel(result, x, y, accum);
            }

        return result;
    }

    Image combineIntensities(const Image &sobelX, const Image &sobelY) {
        int sizeX = sobelX.sizeX(), sizeY = sobelX.sizeY();
        Image result(typeid(float), sizeX, sizeY);

        for (int x = 0; x < sizeX; ++x)
            for (int y = 0; y < sizeY; ++y) {
                float gx = sobelX.getFloat(x, y, 0);
                float gy = sobelY.getFloat(x, y, 0);
                result.set(x, y, fabs(gx - 0.5) + fabs(gy - 0.5), 0);
            }

        return result;
    }

    const float VERTICAL = 1.0/2;
    const float HORIZONTAL = 1.0/4;
    const float SLASH = 1.0/8;
    const float BACKSLASH = 1.0/16;

    Image calcAngles(const Image &sobelX, const Image &sobelY) {
        int sizeX = sobelX.sizeX(), sizeY = sobelX.sizeY();
        Image result(typeid(float), sizeX, sizeY);

        for (int x = 0; x < sizeX; ++x)
            for (int y = 0; y < sizeY; ++y) {
                double gx = sobelX.getFloat(x, y, 0) - 0.5;
                double gy = sobelY.getFloat(x, y, 0) - 0.5;
                double res;
                if (gx == 0)
                    if (gy == 0)
                        res = 0;
                    else
                        res = M_PI / 2;
                else
                    res = atan(gy / gx);

                if (res < M_PI / 8 && res > -M_PI / 8)
                    result.set(x, y, VERTICAL, 0);
                else if (res > 3 * M_PI / 8 || res < -3 * M_PI / 8)
                    result.set(x, y, HORIZONTAL, 0);
                else if (res < 0)
                    result.set(x, y, BACKSLASH, 0);
                else
                    result.set(x, y, SLASH, 0);
            }

        return result;
    }

    Image findLocalMaxs(const Image &intensities, const Image &angles) {
        int sizeX = intensities.sizeX(), sizeY = intensities.sizeY();
        Image result(typeid(float), sizeX, sizeY);

        for (int x = 0; x < sizeX; ++x)
            for (int y = 0; y < sizeY; ++y) {
                double localIntensity = intensities.getFloat(x, y, 0);

                if (intensities.getFloat(x-1, y-1) > localIntensity && angles.getFloat(x, y, 0) == SLASH ||
                    intensities.getFloat(x+0, y-1) > localIntensity && angles.getFloat(x, y, 0) == HORIZONTAL ||
                    intensities.getFloat(x+1, y-1) > localIntensity && angles.getFloat(x, y, 0) == BACKSLASH ||

                    intensities.getFloat(x-1, y+0) > localIntensity && angles.getFloat(x, y, 0) == VERTICAL ||
                    intensities.getFloat(x+1, y+0) > localIntensity && angles.getFloat(x, y, 0) == VERTICAL ||

                    intensities.getFloat(x-1, y+1) > localIntensity && angles.getFloat(x, y, 0) == BACKSLASH ||
                    intensities.getFloat(x+0, y+1) > localIntensity && angles.getFloat(x, y, 0) == HORIZONTAL ||
                    intensities.getFloat(x+1, y+1) > localIntensity && angles.getFloat(x, y, 0) == SLASH)
                        localIntensity = 0;

                result.set(x, y, localIntensity, 0);
            }

        return result;
    }

    bool inImage(const Image &img, int x, int y) {
        return !(x < 0 || x >= img.sizeX() || y < 0 || y >= img.sizeY());
    }

    static const float REQUIRED_DIR[9] = {
        BACKSLASH, VERTICAL, SLASH,
        HORIZONTAL, -1, HORIZONTAL,
        SLASH, VERTICAL, BACKSLASH
    };

    bool trace(Image &result, const Image &intensities, const Image &angles, int x, int y, double threshold) {
        result.set(x, y, 255, 0);

        for (int i = 0; i < 9; ++i) {
            if (i == 4)
                continue;
            int scanX = x + i % 3 - 1;
            int scanY = y + i / 3 - 1;
            if (inImage(result, scanX, scanY) && 
                intensities.getFloat(scanX, scanY, 0) >= threshold &&
                result.getFloat(scanX, scanY) != 255 &&
                angles.getFloat(x, y, 0) == REQUIRED_DIR[i])
                trace(result, intensities, angles, scanX, scanY, threshold);
        }
    }

    Image applyHysteresis(const Image &intensities, const Image &angles,
                         double low, double high) {
        int sizeX = intensities.sizeX(), sizeY = intensities.sizeY();
        Image result(typeid(unsigned char), sizeX, sizeY);

        for (int x = 0; x < sizeX; ++x)
            for (int y = 0; y < sizeY; ++y)
                result.set(x, y, 0, 0);

        for (int x = 0; x < sizeX; ++x)
            for (int y = 0; y < sizeY; ++y)
                if (intensities.getFloat(x, y, 0) >= high)
                    trace(result, intensities, angles, x, y, low);

        return result;
    }
    
}

Ga::Image Ga::canny(const Image& img, double lowThreshold, double highThreshold) {
    Image input = img;
    
	// The Canny algorithm is designed to work on grayscale images, so make sure we have one.
	if (input.noChannels() != 1) {
		input = input.convert2Luminance();
	}
	
	// Normalize the image so every pixel lies in 0..1.
	input = normalizeImage(input);
    
    Image sobelX = convolve(input, SOBEL_X, 1.0/4, 0.5);
    Image sobelY = convolve(input, SOBEL_Y, 1.0/4, 0.5);
    
    Image intensities = combineIntensities(sobelX, sobelY);
    Image angles = calcAngles(sobelX, sobelY);

    Image localMaxs = findLocalMaxs(intensities, angles);
    Image canny = applyHysteresis(localMaxs, angles, lowThreshold, highThreshold);

    return canny;
}
