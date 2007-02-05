/***************************************************************************
                          geoimagecache.cpp  -  description
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
 * $Source: /data/cvs/gda/gda/core/geoimagecache.cpp,v $
 * $Revision: 1.1 $
 * $Date: 2003/06/05 08:21:23 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "geoimagecache.h"
#include <geoimage.h>


GeoImageCache::GeoImageCache()
{
  maxCosts_=1000;
}

GeoImageCache::~GeoImageCache(){
}

/** set the maximum costs (no of bytes for all images in the list) */
void GeoImageCache::setMaxCosts(int maxCosts)
{
  maxCosts_=maxCosts;
}

/** No descriptions */
void GeoImageCache::useImage(GeoImage* img)
{
  qDebug("GeoImageCache::useImage(: costs %d maxcosts %d\n",costs_,maxCosts_);
  if (findRef(img)!=-1) {
    take();
    costs_-=img->dataSize();
  }
  prepend(img);
  qDebug("GeoImageCache::useImage: %s\n",
      (const char*)img->filename());

  img->data();
  costs_+=img->dataSize();
  if (img->dataSize()*2>maxCosts_) maxCosts_=img->dataSize()*2;
  GeoImage* im=last();
  while (costs_>maxCosts_ && im!=img) {
    qDebug("GeoImageCache::useImage: Removing image data %s\n",
      (const char*)im->filename());
    costs_-=im->dataSize();
    im->freeData();
    take();
    im=last();
  }
  qDebug("GeoImageCache::useImage): costs %d maxcosts %d\n",costs_,maxCosts_);
}

/** Remove image from the cache */
void GeoImageCache::removeImage(GeoImage* img)
{
  if (findRef(img)!=-1) {
    take();
    costs_-=img->dataSize();
  }
  qDebug("GeoImageCache::removeImage: costs %d maxcosts %d\n",costs_,maxCosts_);
}
