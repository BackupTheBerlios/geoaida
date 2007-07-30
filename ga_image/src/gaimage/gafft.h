/* -*- mode:c++ -*- */
//*******************************************************************
//  Filename:   gafft.h
//  Ort:        TNT, Uni. - Hannover, Germany
//
//  Info:	Fast Fourier Transform
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


#ifndef __GA_FFT_H
#define __GA_FFT_H

#include "gaimage.h"

namespace Ga {
    /*
      These methods calcuate the fft and inverse fft.
      They are using the open source library fftw3.
      
    */

    int fft_real2compl(const Image& real, const bool polar, Image& realout, Image& imagout);
    int fft_compl2compl(const Image& real, const Image& imag, const bool polar, Image& realout, Image& imagout);
    int ifft_compl2compl(const Image& real, const Image& imag, const bool polar, Image& realout, Image& imagout);
}

#endif
