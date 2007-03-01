/***************************************************************************
                          pfmcanny.cpp  -  description
                             -------------------
    begin                : Thu Feb 28 2007
    copyright            : (C) 2007 by Julian Raschke
    email                : raschke@tnt.uni-hannover.de
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
#include <gagradient.h>
using namespace Ga;

void Usage(int argc, char **argv)
{
	printf("Usage:\n"
         "  %s <input-file> <canny-file>\n",
         argv[0]);
	exit(0);
}

#ifdef linux
extern "C" {
int __isnanf(float);
	   }
#ifndef NAN
#define NAN sqrt(-1)
#endif
#endif

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
    return source.getFloat(x, y, 0) / 255;
}

// Adjust val when outside 0..1
void setPixel(Image &dest, int x, int y, float val) {
    if (val < 0)
        val = 0;
    else if (val > 1)
        val = 1;
    dest.set(x, y, val * 255, 0);
}

const int GAUSS[9]     = {  1,  2,  1,    2,  4,  2,    1,  2,  1 };
const int SOBEL_X[9]   = {  1,  0, -1,    2,  0, -2,    1,  0, -1 };
const int SOBEL_Y[9]   = {  1,  2,  1,    0,  0,  0,   -1, -2, -1 };

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
            float gx = getPixel(sobelX, x, y);
            float gy = getPixel(sobelY, x, y);
            setPixel(result, x, y, fabs(gx - 0.5) + fabs(gy - 0.5));
        }

    return result;
}

const float VERTICAL = 1.0/2;
const float HORIZONTAL = 1.0/4;
const float SLASH = 1.0/8;
const float BACKSLASH = 1.0/16;

// TODO: Does NOT agree with Wikipedia example image :(

Image calcAngles(const Image &sobelX, const Image &sobelY) {
    int sizeX = sobelX.sizeX(), sizeY = sobelX.sizeY();
    Image result(typeid(float), sizeX, sizeY);

    for (int x = 0; x < sizeX; ++x)
        for (int y = 0; y < sizeY; ++y) {
            double gx = getPixel(sobelX, x, y) - 0.5;
            double gy = getPixel(sobelY, x, y) - 0.5;
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
            double localIntensity = getPixel(intensities, x, y);
            
            if (getPixel(intensities, x-1, y-1) > localIntensity && angles.getFloat(x, y, 0) != BACKSLASH ||
                getPixel(intensities, x+0, y-1) > localIntensity && angles.getFloat(x, y, 0) != VERTICAL ||
                getPixel(intensities, x+1, y-1) > localIntensity && angles.getFloat(x, y, 0) != SLASH ||
                
                getPixel(intensities, x-1, y+0) > localIntensity && angles.getFloat(x, y, 0) != HORIZONTAL ||
                getPixel(intensities, x+1, y+0) > localIntensity && angles.getFloat(x, y, 0) != HORIZONTAL ||
                
                getPixel(intensities, x-1, y+1) > localIntensity && angles.getFloat(x, y, 0) != SLASH ||
                getPixel(intensities, x+0, y+1) > localIntensity && angles.getFloat(x, y, 0) != VERTICAL ||
                getPixel(intensities, x+1, y+1) > localIntensity && angles.getFloat(x, y, 0) != BACKSLASH)
                    localIntensity = 0;
            
            setPixel(result, x, y, localIntensity);
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
    setPixel(result, x, y, 1.0);

    for (int i = 0; i < 9; ++i) {
        if (i == 4)
            continue;
        int scanX = x + i % 3 - 1;
        int scanY = y + i / 3 - 1;
        if (inImage(result, scanX, scanY) && 
            getPixel(intensities, scanX, scanY) >= threshold &&
            getPixel(result, scanX, scanY) != 1.0 &&
            angles.getFloat(x, y, 0) == REQUIRED_DIR[i])
            trace(result, intensities, angles, scanX, scanY, threshold);
    }
}

Image applyHysteresis(const Image &intensities, const Image &angles,
                     double low, double high) {
    int sizeX = intensities.sizeX(), sizeY = intensities.sizeY();
    Image result(typeid(float), sizeX, sizeY);

    for (int x = 0; x < sizeX; ++x)
        for (int y = 0; y < sizeY; ++y)
            setPixel(result, x, y, 0);

    for (int x = 0; x < sizeX; ++x)
        for (int y = 0; y < sizeY; ++y)
            if (getPixel(intensities, x, y) >= high)
                trace(result, intensities, angles, x, y, low);

    return result;
}

int main(int argc, char **argv)
{
  if (argc < 3)
    Usage(argc,argv);

  const char* inputfile=argv[1];
  const char* cannyfile=argv[2];

  Image input;
  input.read(inputfile);
  if (input.isEmpty()) {
    fprintf(stderr,"Can't open inputfile %s\n", inputfile);
    return -1;
  }

  Image blurred;
  blurred = convolve(input, GAUSS, 1.0/16, 0);

  Image sobelX = convolve(blurred, SOBEL_X, 1.0/4, 0.5);
  Image sobelY = convolve(blurred, SOBEL_Y, 1.0/4, 0.5);

  Image intensities = combineIntensities(sobelX, sobelY);
  Image angles = calcAngles(sobelX, sobelY);

  Image localMaxs = findLocalMaxs(intensities, angles);
  Image canny = applyHysteresis(intensities, angles, 0.1, 0.2);
  canny.write(cannyfile, 0);
}
