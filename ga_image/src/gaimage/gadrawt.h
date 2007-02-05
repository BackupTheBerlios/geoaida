/***************************************************************************
                          ga_drawt.h  -  drawing and painting functions
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

#ifndef __GA_DRAWT_H
#define __GA_DRAWT_H

#include "gaimaget.h"

namespace Ga {

/** draws a line with the Bresenham-algorithm using pointers (fast!), with range-check
  \param x1 x coordinate of the starting point
  \param y1 y coordinate of the starting point
  \param x2 x coordinate of the end point
  \param y2 y cooridnate of the end point
  \param value of the line points */
template<class PixTyp> void drawLine(ImageBase &im, int x1, int y1, int x2, int y2, double value)
{
  PixTyp c=PixTyp(value);
  ImageT<PixTyp>& img=(ImageT<PixTyp>&)im;
  int size_x = img.sizeX(), size_y = img.sizeY();

#if 0
 	assert(x1 >= 0);
  assert(x1 < size_x);
  assert(y1 >= 0);
  assert(y1 < size_y);
  assert(x2 >= 0);
  assert(x2 < size_x);
  assert(y2 >= 0);
  assert(y2 < size_y);
#endif

  // Bresham is optimized/works only for the 1st octant of the
  // mathematical coordinate system. We have to transfer all lines
  // virtually into that 1st octant!
  int tmp, negative = 0, mirror = 0;

  // calculate the gradient
  int dx = abs(x2 - x1), dy = abs(y2 - y1);

  // first we mirror the lines with slope > 1 (45 degree symmetry of each quadrant, swap x & y)
  if (dy > dx)
  {
    mirror = 1;
    tmp = dy; dy = dx; dx = tmp;
    tmp = x2; x2 = y2; y2 = tmp;
    tmp = x1; x1 = y1; y1 = tmp;
  }

  // second we mirror all lines with of quadrant 2 & 3 to quadrant 1 & 4 (symm: x = 0)
  if (x1 > x2)
  {
    tmp = x2; x2 = x1; x1 = tmp;
    tmp = y2; y2 = y1; y1 = tmp;
  }

  // at last we (virtually) mirror all lines in the 4th quadrant to q1 (symm: y = 0)
  if (y1 > y2)
    negative = 1;

  int incE = 2 * dy, incNE = 2 * (dy - dx);
  int d = 2 * dy - dx;
  int x = x1, y = y1;

  PixTyp *pix = img.data();
  if (!mirror)
    pix += y * size_x + x;
  else
    pix += x * size_x + y;

  // now we draw the line. The mirror operations have to be reversed for the actual drawing
  if (!mirror) {
    if (!negative) {
      while ( x <= x2 ) {
        if (x >= 0 && y >= 0 && x < size_x && y < size_y) *pix = c;
        if (d < 0) d += incE; else { d += incNE; ++y; pix += size_x; }
        ++x; ++pix;
      }
    } else {
      while ( x <= x2 ) {
        if (x >= 0 && y >= 0 && x < size_x && y < size_y) *pix = c;
        if (d < 0) d += incE; else { d += incNE; --y; pix -= size_x; }
        ++x; ++pix;
      }
    }
  } else { // caution x and y are reversed now!
    if (!negative) {
      while ( x <= x2 ) {
        if (x >= 0 && y >= 0 && x < size_y && y < size_x) *pix=c;
        if (d < 0) d += incE; else { d += incNE; ++y; ++pix; }
        ++x; pix += size_x;
      }
    } else {
      while ( x <= x2 ) {
        if (x >= 0 && y >= 0 && x < size_y && y < size_x) *pix = c;
        if (d < 0) d += incE; else { d += incNE; --y; --pix; }
        ++x; pix += size_x;
      }
    }
  }
}

/** draws a line with antialiasing and variable line-width (with range-check)
  \param gx1 x geo-coordinate of point 1
  \param gy2 y geo-coordinate of point 1
  \param gx2 x geo-coordinate of point 2
  \param gy2 y geo-coordinate of point 2
  \param value of the line points
  \param antialias toggle antialiasing */
template<class PixTyp> void drawGeoLine(ImageBase &im, double gx1, double gy1, double gx2, double gy2, double width, double value, bool antialias)
{
  PixTyp c=PixTyp(value);
  ImageT<PixTyp>& img=(ImageT<PixTyp>&)im;
  double tmp, wmax = (width - 1.0) / 2.0;

  // always run from west to east ...
  if (gx2 < gx1)
  {
    tmp = gx2; gx2 = gx1; gx1 = tmp;
    tmp = gy2; gy2 = gy1; gy1 = tmp;
  }

  double x = gx1, y = gy1, dx = gx2 - gx1, dy = gy2 - gy1;
  tmp = sqrt(dx * dx + dy * dy);

  // "unit" vector in line direction: dx*dx+dy*dy= 1 imagepixel
  dx *= img.pixel2geodist(1) / tmp;
  dy *= img.pixel2geodist(1) / tmp;

  // vector perpendicular to line direction
  double dxs = -dy, dys = dx;

  // do a half-pel fill ...
  dx *= 0.5; dy *= 0.5;

  if (!antialias)
  {
    while ( x <= gx2 && ((dy > 0 && y <= gy2) || (dy < 0 && y >= gy2) || dy == 0.0) )
    {
      for (double w = -wmax; w <= wmax; w += 0.5)
      {
        img.set( (int)(img.geo2ImageX_double(x + dxs * w) + 0.5),
                 (int)(img.geo2ImageY_double(y + dys * w) + 0.5), c, 0, 1 );
      }
      x += dx; y += dy;
    }
  }
  else
  {
    double retx, rety, xfract, yfract;
    int col;

    while ( x <= gx2 && ((dy > 0 && y <= gy2) || (dy < 0 && y >= gy2) || dy == 0.0) )
    {
      for (double w = -wmax; w <= wmax; w += 0.5)
      {
        retx = img.geo2ImageX_double(x + dxs * w);
        rety = img.geo2ImageY_double(y + dys * w);

       if (fabs(w) != wmax)
          img.set((int)(retx + 0.5), (int)(rety + 0.5), c);
        else
        {
          xfract = retx - floor(retx); retx = floor(retx);
          yfract = rety - floor(rety); rety = floor(rety);

          col = (int)((1.0 - xfract) * (1.0 - yfract) * (double)c);
          if (img.getInt(int(retx), int(rety)) < col)
            img.set(int(retx), int(rety), col);

          ++retx;
          col = (int)((xfract) * (1.0 - yfract) * (double)c);
          if (img.getInt(int(retx), int(rety)) < col)
            img.set(int(retx), int(rety), col);

          --retx; ++rety;
          col = (int)((1.0 - xfract) * (yfract) * (double)c);
          if (img.getInt(int(retx), int(rety)) < col)
            img.set(int(retx), int(rety), col);

          ++retx;
          col = (int)((xfract) * (yfract) * (double)c);
          if (img.getInt(int(retx), int(rety)) < col)
            img.set(int(retx), int(rety), col);
        }
      }
      x += dx; y += dy;
    }
  }
}

} // namespace Ga
#endif
