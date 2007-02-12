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
 * $Source: /data/cvs/geoaidalib/geoaida/plm_view/mapviewer.cpp,v $
 * $Revision: 1.16 $
 * $Date: 2003/01/09 15:37:35 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "mapviewer.h"
#include "gnode.h"
#include <qimage.h>
#include <qpainter.h>
#include <assert.h>
#include <qapplication.h>

using namespace Ga;

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
  colorTableUpdate_=true;
  maxDepth_=0;
}

MapViewer::~MapViewer()
{
}

static int getMaxId(GNode* node, int maxId)
{
  if (node->id()>maxId) maxId=node->id();
  if (node->idStart()>maxId) maxId=node->idStart();
  if (node->idEnd()>maxId) maxId=node->idEnd();
  QListIterator < GNode > it = QListIterator < GNode > (node->children());
  for (; it.current(); ++it) {
    maxId=getMaxId(it.current(),maxId);
  }
  return maxId;
}

/** set the root node */
void MapViewer::set(GNode * node, Image * map)
{
  qDebug("MapViewer::set");
  root_ = node;
  map_ = map;
  if (!map_ || map_->isEmpty() )
    return;
  if (image_)
    delete image_;
  if (colorTable_)
    delete colorTable_;
  if (iNodeTable_)
    delete iNodeTable_;
  if (root_) calcMaxDepth(root_);
  int maxId=0;
  minId_=0;
  {
    const void* it=map->constBegin();
    int size=map->sizeImage();
    for (int i=0; i<size; i++) {
      int id=map->getInt(it);
      if (id>maxId) maxId=id;
      if (id<minId_) minId_=id;
      map->nextCol(it);
    }
  }
  if (root_) colorTableSize_ = getMaxId(root_,maxId)+1-minId_;
  else colorTableSize_=maxId+1-minId_;
  colorTable_ = new uint[colorTableSize_];
  qDebug("colortablesize=%d",colorTableSize_);
  iNodeTable_ = new GNode*[colorTableSize_];
  for (int i = 0; i < colorTableSize_; i++) {
    iNodeTable_[i] = 0;
    colorTable_[i] = 0;
  }
  image_ = new QImage(map->sizeX(), map->sizeY(), 32);
  if (image_->height() > QApplication::desktop()->height())
    zoom_ = QApplication::desktop()->height() / float (image_->height());
  generateMapImage();
}

/** Generate an image using the stored map and instance tree */
void MapViewer::generateMapImage()
{
  qDebug("MapViewer::generateMapImage");
  if (!map_)
    return;
  if (colorTableUpdate_) setColor(root_);
  int x, y;
  if (image_->width() != map_->sizeX() || image_->height() != map_->sizeY()) {
    image_->create(map_->sizeX(), map_->sizeY(), 32);
  }
  if (dataImage_) {
    if (dataImage_->sizeX() != map_->sizeX()
        || dataImage_->sizeY() != map_->sizeY()) {
      float invalpha = 1 - alpha_;
      double xscale = double (dataImage_->sizeX()) / map_->sizeX();
      double yscale = double (dataImage_->sizeY()) / map_->sizeY();
      switch (dataImage_->noChannels()) {
      case 3:
        for (y = 0; y < map_->sizeY(); y++)
          for (x = 0; x < map_->sizeX(); x++) {
            uint c = colorTable_[map_->getInt(x, y)-minId_];
            int r = dataImage_->getInt(int (x * xscale), int (y * yscale),0);
            int g = dataImage_->getInt(int (x * xscale), int (y * yscale),1);
            int b = dataImage_->getInt(int (x * xscale), int (y * yscale),2);
            c = qRgb(int (qRed(c) * alpha_ + r * invalpha),
                     int (qGreen(c) * alpha_ + g * invalpha),
                     int (qBlue(c) * alpha_ + b* invalpha));
            image_->setPixel(x, y, c);
          }
        break;
      default:
        for (y = 0; y < map_->sizeY(); y++)
          for (x = 0; x < map_->sizeX(); x++) {
            uint c = colorTable_[map_->getInt(x, y)-minId_];
            uint v = dataImage_->getInt(int (x * xscale), int (y * yscale));
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
      switch (dataImage_->noChannels()) {
      case 3:
        for (y = 0; y < map_->sizeY(); y++)
          for (x = 0; x < map_->sizeX(); x++) {
            uint c = colorTable_[map_->getInt(x, y)-minId_];
            int r = dataImage_->getInt(x,y,0);
            int g = dataImage_->getInt(x,y,1);
            int b = dataImage_->getInt(x,y,2);
            c = qRgb(int (qRed(c) * alpha_ + r * invalpha),
                     int (qGreen(c) * alpha_ + g * invalpha),
                     int (qBlue(c) * alpha_ + b* invalpha));
            image_->setPixel(x, y, c);
          }
        break;
      default:
        for (y = 0; y < map_->sizeY(); y++)
          for (x = 0; x < map_->sizeX(); x++) {
            uint c = colorTable_[map_->getInt(x, y)-minId_];
            uint v = dataImage_->getInt(x,y);
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
    for (y = 0; y < map_->sizeY(); y++)
      for (x = 0; x < map_->sizeX(); x++) {
        image_->setPixel(x, y, colorTable_[map_->getInt(x, y)-minId_]);
      }
  }
  zoom(zoom_);
}


/** No descriptions */
void MapViewer::setColor(GNode * node)
{
  if (!node) {
//    qDebug("MapViewer::setColor: mode=%d tableSize=%d",colorMode_,colorTableSize_);
	  switch (colorMode_) {
	  case COL_NODE:
	  case COL_INODE:
      colorTable_[0]=qRgb(0,0,0);
      if (colorTableSize_<360) {
			    for (int i=1; i<colorTableSize_; i++) {
          colorTable_[i]=QColor(int(float(i)/colorTableSize_*360),255,255,QColor::Hsv).rgb();
			  }
      }
      else {
			    for (int i=1; i<colorTableSize_; i++) {
          colorTable_[i]=QColor(i%360,255,255,QColor::Hsv).rgb();
			  }
      }
	    break;
	  case COL_RANDOM:
      colorTable_[0]=qRgb(0,0,0);
	    for (int i=1; i<colorTableSize_; i++) {
	  	  colorTable_[i] = qRgb(random()%256,random()%256,random()%256);
			}
	    break;
	  case COL_DEPTH:
      for (int i=0; i<colorTableSize_; i++) {
        int v=int(float(i)/colorTableSize_*255);
			  colorTable_[i] = qRgb(v,v,v);
//      colorTable_[i]=QColor(240,255,f*255,QColor::Hsv).rgb();
			}
	    break;
  	}
    return;
  }
  int start_id = node->idStart();
  int end_id = node->idEnd();
  assert(start_id-minId_<colorTableSize_);
  assert(end_id-minId_<colorTableSize_);
  uint color;
  switch (colorMode_) {
  case COL_NODE:
    color = node->classNode()->color();
    break;
  case COL_INODE:
    color = node->color();
    break;
  case COL_RANDOM:
    color=qRgb(random()%256,random()%256,random()%256);
    break;
  case COL_DEPTH:
    {
      float f=float(node->depth())/maxDepth_;
      color=QColor(240,255,int(f*255),QColor::Hsv).rgb();
    }
    break;
  }
  colorTable_[start_id-minId_] = color;
  iNodeTable_[start_id-minId_] = node;
  if (node->showDetails() && node->depth()<maxDepth_) {
    QListIterator < GNode > it = QListIterator < GNode > (node->children());
    for (; it.current(); ++it) {
      setColor(it.current());
    }
  }
  else {
    for (int i = start_id+1-minId_; i <= end_id-minId_; i++) {
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
  int x = ev->x();
  int y = ev->y();
  int mx=int(x/zoom_);
  int my=int(y/zoom_);
  if (mx >= 0 && mx < map_->sizeX() && my >= 0 && my < map_->sizeY()) {
    float gX=map_->image2GeoX(mx);
    float gY=map_->image2GeoY(my);
    if (!root_) {
			QString msg;
   		msg.sprintf("pic: (%d,%d) val: %f",mx,my,map_->getFloat(mx,my));
	    emit info(msg);
      return;
    }
    int id = map_->getInt(mx, my);
    if (id-minId_<0 || id-minId_>=colorTableSize_) currentNode_=0;
    else currentNode_ = iNodeTable_[id-minId_];
    QString msg;
    if (currentNode_) {
      msg.sprintf("geo: (%f,%f) pic: (%d,%d) class: %s id: %d name: %s depth: %d",
                  gX,gY,
                  mx, my,
                  (const char *) currentNode_->attribute("class").utf8(),
                  currentNode_->id(),
                  (const char *) currentNode_->name().utf8(),
                  currentNode_->depth());
    }
    else {
      msg.sprintf("geo: (%f,%f) pic: (%d,%d) id: %d not classified %d",
                  gX,gY,mx,my, id,colorTableSize_);
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
  mouseMoveEvent(ev);
  if (!root_)
    return;
  emit selected(currentNode_);
}

/** No descriptions */
void MapViewer::mouseReleaseEvent(QMouseEvent * ev)
{
  mousePressEvent(ev);
}

/** set the color table to the node colors */
void MapViewer::nodeColor()
{
  colorMode_=COL_NODE;
  generateMapImage();
}

/** set the color table to the inode colors */
void MapViewer::iNodeColor()
{
  colorMode_=COL_INODE;
  generateMapImage();
}

/** set the color table to random colors */
void MapViewer::randomColor()
{
  colorMode_=COL_RANDOM;
  generateMapImage();
}

/** color intensity proportional to node depth */
void MapViewer::depthColor()
{
  colorMode_=COL_DEPTH;
  generateMapImage();
}

/** set data image to be merged with the map */
void MapViewer::setDataImage(Image * img)
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
  if (zoom_ > 1.0)
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
#ifdef WIN32
    int h = int (image_->height() * zoom);
    if (h == 0)
      h = 1;
    displayImage_ = image_->smoothScale (w, h) ;
#else
    displayImage_ = image_->scaleWidth(w);
#endif
  }
  else
    displayImage_ = *image_;
  setFixedSize(displayImage_.size());
  repaint();
}
/** scale the map to the dimension of the dataimage */
void MapViewer::scaleMap()
{
  if (!map_) return;
  if (!dataImage_) return;
  *map_=map_->resampleNN(dataImage_->sizeX(),dataImage_->sizeY());
}

/** stretch the colortable to maximum contrast */
void MapViewer::stretchContrast()
{
  qDebug("MapViewer::stretchContrast\n");
  int maxRgb=0;
  for (int i=0; i<colorTableSize_; i++) {
    uint c=colorTable_[i];
    int r=qRed(c);
    int g=qGreen(c);
    int b=qBlue(c);
    if (r>maxRgb) maxRgb=r;
    if (g>maxRgb) maxRgb=g;
    if (b>maxRgb) maxRgb=b;
  }
  qDebug("MapViewer::stretchContrast %d\n",maxRgb);
  if (maxRgb<255) {
  	float f=255.0/maxRgb;
	  for (int i=0; i<colorTableSize_; i++) {
	    uint c=colorTable_[i];
	    int r=qRed(c);
	    int g=qGreen(c);
	    int b=qBlue(c);
	    colorTable_[i]=qRgb(int(r*f),int(g*f),int(b*f));
	  }
	  colorTableUpdate_=false;
	  generateMapImage();
	  colorTableUpdate_=true;
  }

}

/** calculate the maximum depth of the net by scanning the net */
void MapViewer::calcMaxDepth(GNode* node)
{
  int d=node->depth();
  if (d>maxDepth_) maxDepth_=d;
  QListIterator < GNode > it = QListIterator < GNode > (node->children());
  for (; it.current(); ++it) {
    calcMaxDepth(it.current());
  }
}

/** Set the maximum depth to show */
void MapViewer::maxDepth(int depth)
{
  maxDepth_=depth;
}

