/***************************************************************************
                          mapviewer.h  -  description
                             -------------------
    begin                : Wed Jun 13 2001
    copyright            : (C) 2001 by Martin pahl
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
 * $Source: /data/cvs/gda/gda/semnetgui/mapviewer.h,v $
 * $Revision: 1.7 $
 * $Date: 2002/12/06 06:33:17 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef MAPVIEWER_H
#define MAPVIEWER_H

#include <qwidget.h>
#include <qpainter.h>
#include <qimage.h>
#include "inode.h"
#include <qscrollview.h>

/**Viewer to show a map taking in to a acount a given instance tree
  *@author Martin pahl
  */

class MapViewer : public QWidget
{
Q_OBJECT public:
  MapViewer(QWidget * parent = 0, const char *name = 0);
   ~MapViewer();
  /** zoom the map view */
  void zoom(float zoom);
public slots:
  /** set the root node */
  void set(INode * node, GeoImage * map);
  /** Debug Labelimage view */
  void set(int id, GeoImage * map);
  /** Generate an image using the stored map and instance tree */
  void generateMapImage();
  /** set the color table to random colors */
  void randomColor(bool state);
  /** set data image to be merged with the map */
  void setDataImage(GeoImage * img);
  /** No descriptions */
  void setAlpha(int val);
  /** unzoom the mapview */
  void zoomOut();
  /** zoom the mapview */
  void zoomIn();
  /** Save the display map to a file */
  void save(QString filename, const char* fmt);
protected:                     // Protected attributes
  /** Table to map node ids to image colors */
    INode ** iNodeTable_;
  uint *colorTable_;
  int colorTableSize_;
  INode *root_;
  GeoImage *map_;
  GeoImage *dataImage_;
  QImage *image_;
  QImage displayImage_;
  bool randomColor_;
  INode *currentNode_;
  float zoom_;
protected:                     // Protected methods
  /** No descriptions */
  void setColor(INode * node);
  /** No descriptions */
  void paintEvent(QPaintEvent *);
protected slots:              // Protected slots
  /** No descriptions */
  virtual void mouseMoveEvent(QMouseEvent * ev);
  /** No descriptions */
  virtual void mousePressEvent(QMouseEvent * ev);
  /** No descriptions */
  virtual void mouseReleaseEvent(QMouseEvent * ev);
    signals:                    // Signals
  /** No descriptions */
  void selected(GNode *);
  /** No descriptions */
  void info(QString);
public:                        // Public attributes
  /** Describes the visibility of the data image */
  float alpha_;
};

#endif
