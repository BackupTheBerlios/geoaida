/* -*- mode:c++ -*- */
//*******************************************************************
//  Filename:   gaconvolution.h
//  Ort:        TNT, Uni. - Hannover, Germany
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
//           --------      
//          /     ^ /      
//         /  ^^   /--     
//        / ^ ° ^^/  /
//        --------  /--
//         /  °  * /+ /
//         --------  /
//          /   ~~  /
//          --------
//
//*******************************************************************


#ifndef __GA_CONVOLUTION_H
#define __GA_CONVOLUTION_H

#include "gaimage.h"

namespace Ga {
    /*
      This method convolves the image with the kernel
      by using fft. This ist faster then convolution
      without fft when using unless only very small
      kernels are used.      
    */ 
    Image convolve(Image const& image, Image const& kernel);
}

#endif
