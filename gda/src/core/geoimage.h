/***************************************************************************
                          geoimage.h  -  description
                             -------------------
    begin                : Thu Oct 19 2000
    copyright            : (C) 2000 by Jürgen Bückner
    email                : bueckner@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/



#ifndef GEO_IMAGE_H
#define GEO_IMAGE_H

#ifdef WIN32
#include <float.h>
//#define	isnan(v)	_isnan(v)
#ifndef __USE_ISOC99
#define __USE_ISOC99
#endif
#include <stdio.h>
#undef __USE_ISOC99
#endif

#include <qstring.h>
#include <qshared.h>
#include <qpoint.h>
#include <qfile.h>
#include "MLParser.h"
#include "geoimagecache.h"

#ifdef WIN32
#include <pbm.h>
#include <pgm.h>
#include <ppm.h>
#include <pnm.h>
#include "pfm.h"
//#include <linuxmath.h>
#else
extern "C"
{
#include <pbm.h>
#include <pgm.h>
#include <ppm.h>
#include <pnm.h>
#include "pfm.h"
}
#include <math.h>
#endif

#ifdef linux
extern "C"
{
  int __isnanf(float);
}
#endif
#ifndef NAN
#define NAN sqrt(-1)
#endif
/**class to handel the infos for one image
  *@author Jürgen Bückner
  */

class GeoImage:public ArgDict, QShared
{
public:
  /** default constructor */
  GeoImage();

  /** constructor read attributes for this GeoImage through parser*/
  GeoImage(MLParser & parser);

  /** constructor read attributes for this GeoImage through dictionary*/
  GeoImage(ArgDict & dict);

  /** label-picture constructor */
  GeoImage(QString fname,
           QString key,
           float north = 0.0,
           float south = 0.0, float west = 0.0, float east = 0.0);

  /** label-picture constructor */
    GeoImage(QString fname,
             QString key,
             int xsize,
             int ysize, float north, float south, float west, float east);

  /** denstructor */
   ~GeoImage();

  /** init routine */
  void init();

  /** read attributes for this GeoImage through parser */
  void read(MLParser & parser);

  /** configure attributes for this GeoImage through dictionary */
  void configure(ArgDict & dict);

  /** load image info - not the data */
  void load();

  /** return the image filename */
  QString filename();

  /** return data */
  const void *data();
  /** write a scrap of the data.
  * return the filename.
  * if the file exist do nothing .
  * argument fname is optional .
  * the coordinates of the image part are geodata e.g. Gauss Krueger */
  QString part(float west, float north, float east, float south,
               QString fname = "");
  /** write the image to disk */
  void write();
  /** return data typ */
  int dataType()
  {
    return type_;
  };
  /** return cols */
  int cols()
  {
    return cols_;
  };
  /** return rows */
  int rows()
  {
    return rows_;
  };
  /** return  geoNorth*/
  float geoNorth(float v = NAN)
  {
#ifdef WIN32
    if (!_isnan(v))
#else
    if (!isnan(v))
#endif
      geoNorth_ = v;
    return geoNorth_;
  };
  /** return  geoSouth*/
  float geoSouth(float v = NAN)
  {
#ifdef WIN32
    if (!_isnan(v))
#else
    if (!isnan(v))
#endif
      geoSouth_ = v;
    return geoSouth_;
  };
  /** return  geoEast*/
  float geoEast(float v = NAN)
  {
#ifdef WIN32
    if (!_isnan(v))
#else
    if (!isnan(v))
#endif
      geoEast_ = v;
    return geoEast_;
  };
  /** return  geoWest*/
  float geoWest(float v = NAN)
  {
#ifdef WIN32
    if (!_isnan(v))
#else
    if (!isnan(v))
#endif
      geoWest_ = v;
    return geoWest_;
  };
        /** returns the type of image (Laserscan, VIS, SAR ...) */
  float pixelSizeX();
  float pixelSizeY();
  float resolutionX(float res = 0)
  {
    if (res)
      resolutionX_ = res;
    return resolutionX_;
  }
  float resolutionY(float res = 0)
  {
    if (res)
      resolutionY_ = res;
    return resolutionY_;
  }
  QString type()
  {
    return *(find("type"));
  }
  /** return info about this geoimage*/
  QString info()
  {
    return QString().
      sprintf("%s: %s, type:%s, key:%s, x_resolution:%s, y_resolution:%s, ",
              find("key")->latin1(), find("file")->latin1(),
              find("type")->latin1(), find("key")->latin1(),
              find("x_res")->latin1(), find("y_res")->latin1());
  };
  /** Generate a mask image (pbm)  */
  QString mask(float west, float north, float east, float south, int id,
               QString prefixDir="", QString fname = "");
  /** decrements the reference counter from this image and returns a NULL-pointer. If the 
		reference counter is 0, this image is deleted. */
  GeoImage *unlink();
  /** returns a pointer to this image and increments the reference counter */
  GeoImage *shallowCopy();
  /** Merge the region with the id from labelimage img into this image with the newId, if the previous id in this image is compareId */
  bool mergeInto(GeoImage & img, int compareId, int id, int newId);
  /** gets the id at the picture coordinatex gx, gy */
  int getId(int x, int y);
  /** gets the id at the geo coordinatex gx, gy */
  int getId(float gx, float gy);
  /** Convert geo coordinates to picture coordinates */
  float geo2picX(float x);
  float geo2picY(float y);
  /** Convert picture coordinates to geo coordinates */
  float pic2geoX(float x);
  float pic2geoY(float y);
  /** Calculate the bounding box in pixeln from given bounding box in geocoordinates */
  void picBBox(float gW, float gN, float gE, float gS, int &llx, int &lly,
               int &urx, int &ury);
  /** Calculate the bounding box in geocoordinates from given bounding box in pixeln */
  void geoBBox(int llx, int lly, int urx, int ury, float &gW, float &gN,
               float &gE, float &gS);
  /** size of data */
  int dataSize();
  /** frees the data */
  void freeData();

  enum IMGTYPE
  {
    PFMStorageTypeDefine,
    PBM = PFM_LAST,
    PGM,
    PPM,
    UNKNOWN
  };

protected:                     // Protected attributes
  void *data_;
  int dataSize_;
  IMGTYPE type_;
  int cols_, rows_;
  float geoNorth_, geoSouth_, geoEast_, geoWest_;
  float minval_, maxval_;
  float resolutionX_, resolutionY_;
  static GeoImageCache cache_;
private:
  /** test the consistent of the image date */
  bool testSize(int cols, int rows, IMGTYPE type);

};

#endif

// geoSouth geoNorth  geoEast geoWest   float geoNorth, geoSouth, geoEast, geoWest;
