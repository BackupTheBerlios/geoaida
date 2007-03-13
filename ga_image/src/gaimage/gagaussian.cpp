/* -*- mode:c++ -*- */
//*******************************************************************
//  Filename:   gagaussian.cpp
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

#include "gagaussian.h"
#include <vector>
#include <cmath>
#include <algorithm>

/*

Implementation rationale: 

* Does not reuse existing convolution because the Gaussian blur can
be implemented as a separable convolution, which scales linearly,
no quadratically with the sigma parameter.

* Does not introduce a separate function for separable convolutions
as the Gaussian blur is the only filter which can losslessly be
separated. If the need arises (e.g. for fast median filter approx.),
convolution can be outsourced.

* Assumes the border pixels to repeat infinitely outside of the image
bounds, i.e. they have more weight than other pixels. I deemed this
a less problematic choice than to assume a constant colour outside.

Possible improvements:

* Speed up by working on ImageT instead of Image?

*/

Ga::Image Ga::gaussianBlur(const Image &source, double sigma)
{    
    // No need to blur?
    if (sigma <= 0)
        return source;

    // Size of relevant Gauss mask.
    // 3x3 is the minimum (1x1 would only marginally darken the input).
    int mirroredPortion = static_cast<int>(std::ceil(sigma * 2));
    mirroredPortion = std::max(mirroredPortion, 1);
    int size = 2 * mirroredPortion + 1;

    // Compute Gauss mask.
    std::vector<double> mask(size);
    for (int i = -mirroredPortion; i < mirroredPortion; ++i)
        mask[i + mirroredPortion] =
            std::exp(-(i * i)/(2 * sigma * sigma)) / (std::sqrt(2 * M_PI) * sigma);

    int sizeX = source.sizeX(), sizeY = source.sizeY(), channels = source.noChannels();

    // Horizontal convolution.
    Image intermediate = source;
    for (int ch = 0; ch < channels; ++ch)
        for (int x = 0; x < sizeX; ++x)
            for (int y = 0; y < sizeY; ++y)
            {
                double accum = 0;
                for (int i = -mirroredPortion; i < mirroredPortion; ++i)
                {
                    // Capture scanner inside image bounds.
                    int curX = std::max(std::min(x + i, sizeX - 1), 0);
                    accum += source.getFloat(curX, y, ch) * mask[i + mirroredPortion];
                }
                intermediate.set(x, y, accum, ch);
            }
            
    // Vertical convolution.
    Image result = intermediate;
    for (int ch = 0; ch < channels; ++ch)
        for (int x = 0; x < sizeX; ++x)
            for (int y = 0; y < sizeY; ++y)
            {
                double accum = 0;
                for (int i = -mirroredPortion; i < mirroredPortion; ++i)
                {
                    // Capture scanner inside image bounds.
                    int curY = std::max(std::min(y + i, sizeY - 1), 0);
                    accum += intermediate.getFloat(x, curY, ch) * mask[i + mirroredPortion];
                }
                result.set(x, y, accum, ch);
            }
            
    return result;
}
