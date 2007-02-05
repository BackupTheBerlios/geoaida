/***************************************************************************
                          geoimagecache.h  -  description
                             -------------------
    begin                : Tue Jun 3 2003
    copyright            : (C) 2003 by Martin pahl
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
 * $Source: /data/cvs/gda/gda/core/geoimagecache.h,v $
 * $Revision: 1.1 $
 * $Date: 2003/06/05 08:21:23 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef GEOIMAGECACHE_H
#define GEOIMAGECACHE_H

#include <qlist.h>
class GeoImage;

/**image cache
  *@author Martin pahl
  */

class GeoImageCache : public QPtrList<GeoImage>  {
public: 
  GeoImageCache();
  ~GeoImageCache();
  /** No descriptions */
  void useImage(GeoImage* img);
  /** set the maximum costs (no of bytes for all images in the list) */
  void setMaxCosts(int maxCosts);
  /** Remove image from the cache */
  void removeImage(GeoImage* img);
protected:
  int costs_;
  int maxCosts_;
};

#endif
