/***************************************************************************
                          mapviewer.cpp  -  description
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
 * $Source: /data/cvs/gda/gda/semnetgui/mapviewer.cpp,v $
 * $Revision: 1.23 $
 * $Date: 2004/02/27 10:32:31 $
 * $Author: mueller $
 * $Locker:  $
 */

#include "mapviewer.h"
#include <qimage.h>
#include <qpainter.h>
#include <assert.h>
#include <qapplication.h>

MapViewer::MapViewer(QWidget * parent, const char *name):QWidget(parent, name)
{
  colorTable_ = 0;
  iNodeTable_ = 0;
  image_ = 0;
  root_ = 0;
  map_ = 0;
  dataImage_ = 0;
  alpha_ = 0.5;
  zoom_ = 1.0;
  randomColor_=0;
}

MapViewer::~MapViewer()
{
}

/** set the root node */
void MapViewer::set(INode * node, GeoImage * map)
{
  qDebug("MapViewer::set");
  root_ = node;
  map_ = map;
  if (!root_ || !map_)
    return;
  if (image_)
    delete image_;
  if (colorTable_)
    delete colorTable_;
  if (iNodeTable_)
    delete iNodeTable_;
  colorTableSize_ = node->attributeInt("IDEnd") + 1;
  colorTable_ = new uint[colorTableSize_];
  iNodeTable_ = new INode * [colorTableSize_];
  for (int i = 0; i < colorTableSize_; i++) {
    iNodeTable_[i] = 0;
    colorTable_[i] = 0;
  }
  image_ = new QImage(map->cols(), map->rows(), 32);
  if (image_->height() > QApplication::desktop()->height())
    zoom_ = QApplication::desktop()->height() / float (image_->height());
  generateMapImage();
}

/** Debug Labelimage view */
void MapViewer::set(int id, GeoImage * map)
{
  qDebug("MapViewer::set(int,map) Debug");
  root_ = 0;
  map_ = map;
  if (!map_)
    return;
  if (image_)
    delete image_;
  if (colorTable_)
    delete colorTable_;
  if (iNodeTable_)
    delete iNodeTable_;
  image_ = new QImage(map->cols(), map->rows(), 32);
  if (image_->height() > QApplication::desktop()->height())
    zoom_ = QApplication::desktop()->height() / float (image_->height());

  int x, y;
  for (y = 0; y < map_->rows(); y++)
    for (x = 0; x < map_->cols(); x++) {
      int mapId = map_->getId(x, y);
      if (mapId == 0)
        image_->setPixel(x, y, black.rgb());
      else if (mapId != id)
        image_->setPixel(x, y, green.rgb());
      else
        image_->setPixel(x, y, red.rgb());
    }
  displayImage_ = *image_;
  zoom(zoom_);
  //resize(image_->size());
}

/** Generate an image using the stored map and instance tree */
void MapViewer::generateMapImage()
{
  qDebug("MapViewer::generateMapImage");
  if (!root_)
    return;
  if (!map_)
    return;
  setColor(root_);
  int x, y;
  if (image_->width() != map_->cols() || image_->height() != map_->rows()) {
    image_->create(map_->cols(), map_->rows(), 32);
  }
  if (dataImage_) {
    if (dataImage_->cols() != map_->cols()
        || dataImage_->rows() != map_->rows()) {
      float invalpha = 1 - alpha_;
      double xscale = double (dataImage_->cols()) / map_->cols();
      double yscale = double (dataImage_->rows()) / map_->rows();
      switch (dataImage_->dataType()) {
      case GeoImage::PPM:
      case GeoImage::PFM_3BYTE:
        for (y = 0; y < map_->rows(); y++)
          for (x = 0; x < map_->cols(); x++) {
            uint c = colorTable_[map_->getId(x, y)];
            uint v = dataImage_->getId(int (x * xscale), int (y * yscale));
            c = qRgb(int (qRed(c) * alpha_ + qRed(v) * invalpha),
                     int (qGreen(c) * alpha_ + qGreen(v) * invalpha),
                     int (qBlue(c) * alpha_ + qBlue(v) * invalpha));
            image_->setPixel(x, y, c);
          }
        break;
      default:
        for (y = 0; y < map_->rows(); y++)
          for (x = 0; x < map_->cols(); x++) {
            uint c = colorTable_[map_->getId(x, y)];
            uint v = dataImage_->getId(int (x * xscale), int (y * yscale));
            c = qRgb(int (qRed(c) * alpha_ + v * invalpha),
                     int (qGreen(c) * alpha_ + v * invalpha),
                     int (qBlue(c) * alpha_ + v * invalpha));
            image_->setPixel(x, y, c);
          }
        break;
      }
    }
    else {
      float invalpha = 1 - alpha_;
      switch (dataImage_->dataType()) {
      case GeoImage::PPM:
      case GeoImage::PFM_3BYTE:
        for (y = 0; y < map_->rows(); y++)
          for (x = 0; x < map_->cols(); x++) {
            uint c = colorTable_[map_->getId(x, y)];
            uint v = dataImage_->getId(x, y);
            c = qRgb(int (qRed(c) * alpha_ + qRed(v) * invalpha),
                     int (qGreen(c) * alpha_ + qGreen(v) * invalpha),
                     int (qBlue(c) * alpha_ + qBlue(v) * invalpha));
            image_->setPixel(x, y, c);
          }
        break;
      default:
        for (y = 0; y < map_->rows(); y++)
          for (x = 0; x < map_->cols(); x++) {
            uint c = colorTable_[map_->getId(x, y)];
            uint v = dataImage_->getId(x, y);
            c = qRgb(int (qRed(c) * alpha_ + v * invalpha),
                     int (qGreen(c) * alpha_ + v * invalpha),
                     int (qBlue(c) * alpha_ + v * invalpha));
            image_->setPixel(x, y, c);
          }
        break;
      }
    }
  }
  else {
    for (y = 0; y < map_->rows(); y++)
      for (x = 0; x < map_->cols(); x++)
        image_->setPixel(x, y, colorTable_[map_->getId(x, y)]);
  }
  zoom(zoom_);
}

/** No descriptions */
void MapViewer::setColor(INode * node)
{
  int start_id = node->attributeInt("IDStart");
  int end_id = node->attributeInt("IDEnd");
  uint color;
  if (randomColor_) {
    color=qRgb(random()%256,random()%256,random()%256);
  }
  else {
    color = node->sNode()->color();
  }
  colorTable_[start_id] = color;
  iNodeTable_[start_id] = node;
#if 0
  if (node->showDetails() && node->sNode()->showDetails()) {
#endif
    if (node->showDetails()) {
      QListIterator < INode > it = QListIterator < INode > (node->children());
      for (; it.current(); ++it) {
        setColor(it.current());
      }
    }
    else {
      for (int i = start_id + 1; i <= end_id; i++) {
        colorTable_[i] = color;
        iNodeTable_[i] = node;
      }
    }
  }

/** No descriptions */
void MapViewer::paintEvent(QPaintEvent *)
{
  if (!image_)
    return;
  QPainter paint(this);
  paint.setPen(Qt::blue);
  paint.drawImage(0, 0, displayImage_);
}

/** No descriptions */
void MapViewer::mouseMoveEvent(QMouseEvent * ev)
{
  if (!map_)
    return;
  if (!root_)
    return;
  int x = ev->x();
  int y = ev->y();
  int mx=int(x/zoom_);
  int my=int(y/zoom_);
  if (mx >= 0 && mx < map_->cols() && my >= 0 && my < map_->rows()) {
    int id = map_->getId(mx, my);
    assert(id >= 0);
    assert(id < colorTableSize_);
    currentNode_ = iNodeTable_[id];
    QString msg;
//!    float gx,gy;
    if (currentNode_) {
      msg.sprintf("geo: (%f,%f) pic: (%d,%d) class: %s id: %s name: %s",
                  map_->pic2geoX(mx),
                  map_->pic2geoY(my),
                  mx, my,
                  (const char *) currentNode_->attribute("class").utf8(),
                  (const char *) currentNode_->attribute("id").utf8(),
                  (const char *) currentNode_->name().utf8());
    }
    else {
      msg.sprintf("geo: (%f,%f) pic: (%d,%d) id: %d not classified",
                  map_->pic2geoX(mx), map_->pic2geoY(my), mx,
                  my, id);
    }
    emit info(msg);
    qDebug("MapViewer::mouseMoveEvent: (%d,%d) %d", x, y, id);
  }
  else {
    qDebug("MapViewer::mouseMoveEvent: (%d,%d)", x, y);
  }
}

/** No descriptions */
void MapViewer::mousePressEvent(QMouseEvent * ev)
{
  if (!root_)
    return;
  mouseMoveEvent(ev);
  emit selected(currentNode_);
}

/** No descriptions */
void MapViewer::mouseReleaseEvent(QMouseEvent * ev)
{
  if (!root_)
    return;
  mousePressEvent(ev);
}

/** set the color table to random colors */
void MapViewer::randomColor(bool state)
{
  randomColor_ = state;
  qDebug("MapViewer::randomColor(%d)", state);
}

/** set data image to be merged with the map */
void MapViewer::setDataImage(GeoImage * img)
{
  dataImage_ = img;
}

/** No descriptions */
void MapViewer::setAlpha(int val)
{
  alpha_ = val / 100.0;
//  generateMapImage();
}

/** zoom the mapview */
void MapViewer::zoomIn()
{
  zoom_ *= 2;
  if (zoom_ > 1.0 && zoom_<1.99 )
    zoom_ = 1.0;
  qDebug("MapViewer::zoomIn: %f", zoom_);
  zoom(zoom_);
}

/** unzoom the mapview */
void MapViewer::zoomOut()
{
  zoom_ /= 2;
  qDebug("MapViewer::zoomIn: %f", zoom_);
  zoom(zoom_);
}

/** zoom the map view */
void MapViewer::zoom(float zoom)
{
  if (!image_)
    return;
  if (zoom != 1.0) {

    int w = int (image_->width() * zoom);
    if (w == 0)
      w = 1;
    displayImage_ = image_->scaleWidth(w);
  }
  else
    displayImage_ = *image_;
  setFixedSize(displayImage_.size());
  repaint();
}

/** Save the display map to a file */
void MapViewer::save(QString filename, const char* fmt)
{
  displayImage_.save(filename,fmt,-1);
}
