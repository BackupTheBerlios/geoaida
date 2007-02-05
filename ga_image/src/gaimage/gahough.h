/* -*- mode:c++ -*- */
//*******************************************************************
//  Filename:   hough.h
//  Datum:      1997
//  Autoren:    J"urgen B"uckner
//  Ort:        TNT, Uni. - Hannover, Germany
//  Projekt:    Bilddatenstruktur - TROPIC
//
//  Info:	Provides the Hough transformation
//		for all pixel having a value less than
//		a certain threshold t
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

#ifndef __GA_HOUGH_H
#define __GA_HOUGH_H

#include "gaimaget.h"

namespace Ga {

/** returns a hough map containing t in x-direction, angle in y-direction
  ->                       ->
        ( cos alpha * t)
  p =                    + p0
        ( sin alpha * t)
*/

Image houghMapLines(const Image& imgIn,
                    double angleMin, double angleMax, double angleStep);
} // namespace Ga

#endif
