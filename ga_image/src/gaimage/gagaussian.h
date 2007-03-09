/* -*- mode:c++ -*- */
//*******************************************************************
//  Filename:   gagaussian.h
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


#ifndef __GA_GAUSSIAN_H
#define __GA_GAUSSIAN_H

#include "gaimage.h"

namespace Ga {
    Image gaussianBlur(const Image& img, double sigma);
}

#endif
