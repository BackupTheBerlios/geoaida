/* -*- mode:c++ -*- */
//*******************************************************************
//  Filename:   gaconvolution.cpp
//  Ort:    TNT, Uni. - Hannover, Germany
//
//  Info:	Convolution by DFT Multiplikation
//
// Copyright (C) 2007 Christian Becker
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
//*******************************************************************


#include <math.h>


#include <string>
#include <iostream>
#include <cstdlib>

#include <gaimage.h>
#include <garegion.h>
#include <unistd.h>

#include <gafft.h>

#include "gaconvolution.h"

using namespace std;
using namespace Ga;

const int verbose=0;


Image Ga::convolve(Image const& imageIn, Image const& kernelIn){
    Image image = imageIn;
    Image kernel = kernelIn;
    Image real_image(typeid(float), image.sizeX(), image.sizeY());
    Image imag_image(typeid(float), image.sizeX(), image.sizeY());
    Image real_paddedKernel(typeid(float), image.sizeX(), image.sizeY());
    Image imag_paddedKernel(typeid(float), image.sizeX(), image.sizeY());
    
    // Rotate kernel 180 degree and pad with zero
    const int sizeX=kernel.sizeX();
    const int sizeY=kernel.sizeY();

    for (int x=0; x < sizeX; x++)
        for (int y=0; y < sizeY; y++){
            real_paddedKernel.set(x, y, kernel.getFloat(sizeX-x, sizeY-y));
        }
        
    
    for(int x=0; x<image.sizeX(); x++)
        for(int y=0; y<image.sizeY(); y++)
            real_image.set(x, y, image.getFloat(x, y));
    
    
    fft_compl2compl(real_image, imag_image, false, real_image, imag_image);
    fft_compl2compl(real_paddedKernel, 
                    imag_paddedKernel, 
                    false, 
                    real_paddedKernel, 
                    imag_paddedKernel);
    
    for(int x=0; x<image.sizeX(); x++)
        for(int y=0; y<image.sizeY(); y++){
            real_image.set(x, y, real_image.getFloat(x, y)*real_paddedKernel.getFloat(x, y));
            imag_image.set(x, y, imag_image.getFloat(x, y)*imag_paddedKernel.getFloat(x, y));
        }

    ifft_compl2compl(real_image, imag_image, false, real_image, imag_image);
    
    return real_image;

}    

