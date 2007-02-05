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



#ifndef GEO_IMAGE_LIST_H
#define GEO_IMAGE_LIST_H

#include <qstring.h>
#include <qstringlist.h>
#include <qpoint.h>
#include <qcache.h>
#include "qfile.h"
#include "geoimage.h"
#include "geoimagecache.h"
#ifdef WIN32
//#define	_isnan(v)	isnan(v)
#endif

/**class to handel a list of image informations
  *@author Jürgen Bückner
  */

class GeoImageList:public QDict< GeoImage >
{
public:
  /** default constructor */
  GeoImageList();

  /** constructor resd from the provided filename*/
  GeoImageList(QString filename);

  /** denstructor */
  ~GeoImageList();

  /** clears the imagelist */
  void clear();
  /** Remove given image from list. The image is deleted. */
  void remove(GeoImage * img);
  /** read a list of GeoImage and the attributes through parser */
  void read(MLParser & parser);
  /** read a list of GeoImage and the attributes from the provided filename */
  void read(QString filename);
  /** read a list of GeoImage and the attributes from the provided filepointer */
  void read(QIODevice & fp);

  /** return a list of included images*/
  QStringList list(QString type = "");

  /** return void pointer to the GeoImage class */
  GeoImage *geoImage(QString imgKey);
  /** return void pointer to the image data */
  const void *data(QString imgKey);
  /** return the cols numbers of the image */
  int cols(QString imgKey);
  /** return the rows numbers of the image */
  int rows(QString imgKey);
  /** return geoNorth value of the image*/
  float geoNorth(QString imgKey = "");
  /** return geoSouth value of the image*/
  float geoSouth(QString imgKey = "");
  /** return geoEast value of the image*/
  float geoEast(QString imgKey = "");
  /** return geoWest  valueof the image*/
  float geoWest(QString imgKey = "");
  /** return Info about the image*/
  QString info(QString imgKey);
  /** write a scrap of the data to a file*/
  QString part(QString imgKey, float west, float north, float east,
               float south, QString fname = "");
  /** loads the given labelimage fname and returns a pointer to the new image. If the image
	was already requested, only a pointer to the image is returned and the reference
	counter of the image is incremented. */
  GeoImage *loadLabelImage(QString fname, QString key, float west =
                           0, float north = 0, float east = 0, float south =
                           0);
  /** return the maximum resolution in the list of images */
  float maxResolution();
  /** return the minimum resolution in the list of images */
  float minResolution();

protected:                     // Protected attributes
  QStringList list_;

  /** Geocoordinates of the boundingbox of the scene */
  float geoNorth_, geoWest_, geoSouth_, geoEast_;

private:                       // Private attributes
  /** minimum resolution of the images in the list */
  float minRes_;
  /** maximum resolution of the images in the list */
  float maxRes_;
  /** flag to indicate if the minRes and maxRes vars are uptodate */
  bool minMaxResUptodate_;
};

extern GeoImageList geoImageList_;
extern GeoImageList labelImageList_;
#endif
