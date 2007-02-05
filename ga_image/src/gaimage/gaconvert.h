/***************************************************************************
                          ga_convert.h  -  conversion utilities for the
                                           geoAIDA template image class

                             -------------------
    begin                : Thu Feb 1 2001
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

#ifndef __GA_CONVERT_H
#define __GA_CONVERT_H

#include "gaimaget.h"

namespace Ga {
/**  converts three single band images to a rgb-color image */
ImageT<unsigned long> * convert2rgb(ImageT<unsigned char> &red,
				    ImageT<unsigned char> &green,
				    ImageT<unsigned char> &blue  )
{
  assert(red.sizeX() == green.sizeX());
  assert(green.sizeX() == blue.sizeX());
  assert(red.sizeY() == green.sizeY());
  assert(green.sizeY() == blue.sizeY());
  
  ImageT<unsigned long> *pic = new ImageT<unsigned long>(red.sizeX(), red.sizeY());
  pic->typeImage(_PPM);
  pic->setGeoCoordinates(red.getGeoWest(), red.getGeoNorth(), red.getGeoEast(), red.getGeoSouth());
  pic->setGeoRes(red.getGeoRes());
  
  unsigned long *elem = pic->data();
  unsigned char *elem_red = red.data();
  unsigned char *elem_green = green.data();
  unsigned char *elem_blue = blue.data();
  
  for (unsigned int i = 0; i < red.sizeImage(); ++i, ++elem, ++elem_red, ++elem_green, ++elem_blue) {
    *elem = (unsigned long)( *elem_red + *elem_green * 0x0100 + *elem_blue * 0x010000 );
  }
  return pic;
}
 
} // namespace Ga

#endif
