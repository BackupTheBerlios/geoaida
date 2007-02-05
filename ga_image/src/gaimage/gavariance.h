/***************************************************************************
                          gavariance.h  -  variance of image
                             -------------------
    begin                : Thu Jan 11 2001
    copyright            : (C) 2001 TNT, Uni Hannover
    authors              : Martin Pahl, Jürgen Bückner, Oliver Stahlhut
    email                : pahl@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 * $Source: /data/cvs/ga_image/ga_image/gaimage/gavariance.h,v $
 * $Revision: 1.5 $
 * $Date: 2002/09/13 05:29:00 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef __GA_VARIANCE_H
#define __GA_VARIANCE_H

#include "gaimage.h"

namespace Ga {

Image variance(Image& imgIn, int winSize);

Image variance(Image& imgIn, int winSize, double scaleSize);

double sqrError(const Image& img1, int channel1,
                 int offset1X, int offset1Y,
                 const Image& img2, int channel2,
                 int offset2X, int offset2Y,
                 int winSizeX, int winSizeY);
double covariance(const Image& img1, int channel1,
                 int offset1X, int offset1Y,
                 const Image& img2, int channel2,
                 int offset2X, int offset2Y,
                 int winSizeX, int winSizeY);
double correlation(const Image& img1, int channel1,
                 int offset1X, int offset1Y,
                 const Image& img2, int channel2,
                 int offset2X, int offset2Y,
                 int winSizeX, int winSizeY);

} // namespace Ga
#endif
