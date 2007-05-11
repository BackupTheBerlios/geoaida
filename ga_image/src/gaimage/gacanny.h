/* -*- mode:c++ -*- */
//*******************************************************************
//  Filename:   gacanny.h
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
//        / ^ ° ^^/  /
//        --------  /--
//         /  °  * /+ /
//         --------  /
//          /   ~~  /
//          --------
//
//*******************************************************************


#ifndef __GA_CANNY_H
#define __GA_CANNY_H

#include "gaimage.h"

namespace Ga {
    /*
    Implementation of the Canny edge detection algorithm.
    
    Creates a same-sized image from the supplied one, where detected edges are
    drawn in white on a black background. The high threshold controls how
    strong (0..1) an edge must be to be detected; it will then be traced for as
    long as it is stronger than the low threshold.
    
    The implementation consists of separate phases, all of them O(pixels).
    
    Application note: Because the algorithm cannot trace directly adjacent,
    parallel lines, it might be of advantage to enlargen the image if the
    resolution is too low.
    
    Notes: This includes the sobel operator to approximate the gradient. This
    could probably be merged into gagradient.
    */
    
    Image canny(const Image& img, double lowThreshold, double highThreshold);
}

#endif
