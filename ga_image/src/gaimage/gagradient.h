/* -*- mode:c++ -*- */
//*******************************************************************
//  Filename:   gradient.h
//  Datum:      1997
//  Autoren:    J"urgen B"uckner
//  Ort:        TNT, Uni. - Hannover, Germany
//  Projekt:    Bilddatenstruktur - TROPIC
//
//  Info:	calculate the gradient of the image [3x3] Sobel
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

#ifndef __GA_GRADIENT_H
#define __GA_GRADIENT_H

#include "gaimage.h"

namespace Ga {

Image gradient(const Image& imgIn);

/** Calculates the gradient using 4 points. The result
  image contains the absolute gradient in channel 0
  and the angle in channel 1 */
Image gradient2D(const Image &pic, int channel=0);

/** Calculates the gradient using 4 points of a 3 channel
  image e.g. a color image. The result
  image contains the absolute gradient in channel 0
  and the angle in channel 1 */
Image gradient2D3Channels(const Image &pic);

/** Calculates the gradient using 4 points of a 3 channel
  hsv-image taking the maximum of the 3 gradients. The result
  image contains the absolute gradient in channel 0
  and the angle in channel 1 */
Image gradient2DhsvMax(const Image &pic);

} // namespace Ga
#endif
