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
 * $Source: /data/cvs/geoaidalib/geoaida/plm_view/mapviewer.h,v $
 * $Revision: 1.5 $
 * $Date: 2002/09/26 15:20:57 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef MAPVIEWER_H
#define MAPVIEWER_H

#include <qwidget.h>
#include <qpainter.h>
#include <qimage.h>
#include <qscrollview.h>
#include <gaimage.h>

class GNode;

/**Viewer to show a map taking in to a acount a given instance tree
  *@author Martin pahl
  */

class MapViewer : public QWidget
{
Q_OBJECT
public:
  MapViewer(QWidget * parent = 0, const char *name = 0);
   ~MapViewer();
  /** zoom the map view */
  void zoom(float zoom);
public slots:
  /** set the root node */
  void set(GNode * node, Ga::Image* map);
  /** Generate an image using the stored map and instance tree */
  void generateMapImage();
  /** set the color table to the node colors */
  void nodeColor();
  /** set the color table to the inode colors */
  void iNodeColor();
  /** set the color table to random colors */
  void randomColor();
  /** color intensity proportional to node depth */
  void depthColor();
  /** set data image to be merged with the map */
  void setDataImage(Ga::Image* img);
  /** No descriptions */
  void setAlpha(int val);
  /** unzoom the mapview */
  void zoomOut();
  /** zoom the mapview */
  void zoomIn();
  /** scale the map to the dimension of the dataimage */
  void scaleMap();
  /** stretch the colortable to maximum contrast */
  void stretchContrast();
  /** Set the maximum depth to show */
  void maxDepth(int depth);
protected:                     // Protected attributes
  /** Table to map node ids to image colors */
  GNode ** iNodeTable_;
  uint *colorTable_;
  int colorTableSize_;
  GNode *root_;
  Ga::Image *map_;
  Ga::Image *dataImage_;
  QImage *image_;
  QImage displayImage_;
  enum { COL_NODE, COL_INODE, COL_RANDOM, COL_DEPTH } colorMode_;
  GNode *currentNode_;
  float zoom_;
  /** if true, create a new colortable. This flag is needed for the stretch contrast method */
  bool colorTableUpdate_;
  /** maximumdepth of the net */
  int maxDepth_;
  int minId_;
protected:                     // Protected methods
  /** No descriptions */
  void setColor(GNode * node);
  /** No descriptions */
  void paintEvent(QPaintEvent *);
  /** calculate the maximum depth of the net by scanning the net */
  void calcMaxDepth(GNode* node);
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
