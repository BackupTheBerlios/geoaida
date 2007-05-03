/***************************************************************************
                          ga_draw.h  -  drawing and painting functions
                                        for geoAIDA template image class
                             -------------------
    begin                : Wed Jan 24 2001
    copyright            : (C) 2001 by Oliver Stahlhut
    email                : stahlhut@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __GA_DRAW_H
#define __GA_DRAW_H

#include "gaimaget.h"
#include "gaimage.h"

namespace Ga {

/** draws a line with the Bresenham-algorithm using pointers (fast!), with range-check
  \param x1 x coordinate of the starting point
  \param y1 y coordinate of the starting point
  \param x2 x coordinate of the end point
  \param y2 y cooridnate of the end point
  \param c value of the line points */
void drawLine(Image& img, int x1, int y1, int x2, int y2, double c);

/** draws a line with antialiasing and variable line-width (with range-check)
  \param gx1 x geo-coordinate of point 1
  \param gy2 y geo-coordinate of point 1
  \param gx2 x geo-coordinate of point 2
  \param gy2 y geo-coordinate of point 2
  \param c value of the line points
  \param antialias toggle antialiasing */
void drawGeoLine(Image& img, double gx1, double gy1, double gx2, double gy2, double width, double c, bool antialias);

} // namespace Ga
#endif
