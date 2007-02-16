/***************************************************************************
                          doubleslider.cpp  -  description
                             -------------------
    begin                : Thu Sep 7 2000
    copyright            : (C) 2000 by jürgen bückner
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

#include "doubleslider.h"
#include <iostream.h>

doubleslider::doubleslider(float val1, float val2 ,
			   QWidget* parent, const char* name):
  QWidget(parent, name)
{
  bewegeLinks = false;
  bewegeRechts = false;
  min_ = val1; max_ = val2; diff_ = max_ - min_;
  buffer = new QPixmap(width(), height());
  setMinimumWidth (100);
  paintAll();
}

void doubleslider::setWert1(float val1) {
  wert1 = (val1-min_)/diff_;
  paintAll();
}

void doubleslider::setWert1(const QString& val1) {
  wert1 = (val1.toFloat()-min_)/diff_;
  paintAll();
}

void doubleslider::setWert2(float val2) {
  wert2 = (val2-min_)/diff_;
  //cout <<"#f#:"<< wert2<<" = ("<<val2<<" - "<<min_<<") / "<<diff_<<endl;
  paintAll();
}

void doubleslider::setWert2(const QString& val2) {
 wert2 = (val2.toFloat()-min_)/diff_;
 //cout <<"#s#:"<< wert2<<" = ("<<val2.toFloat()<<" - "<<min_<<") / "<<diff_<<endl;
 paintAll();
}

void doubleslider::paintAll() {
  
  QPainter windowpainter;
  QPainter bufferpainter;
  windowpainter.begin(this);
  bufferpainter.begin(buffer);

  windowpainter.fillRect(0, 0, width(), height(), white);
  bufferpainter.fillRect(0, 0, width(), height(), white);

  windowpainter.setPen( QPen( black, 3) );
  bufferpainter.setPen( QPen( black, 3) );

  windowpainter.drawRect(0, 0, width(), height());
  bufferpainter.drawRect(0, 0, width(), height());

  windowpainter.setPen( QPen( green, 3) );
  bufferpainter.setPen( QPen( green, 3) );

  windowpainter.drawLine(int(width()*wert1), 0, int(width()*wert1), height());
  bufferpainter.drawLine(int(width()*wert1), 0, int(width()*wert1), height());

  windowpainter.setPen( QPen( blue, 3) );
  bufferpainter.setPen( QPen( blue, 3) );

  windowpainter.drawLine(int(width()*wert2), 0, int(width()*wert2), height());
  bufferpainter.drawLine(int(width()*wert2), 0, int(width()*wert2), height());

  windowpainter.end();
  bufferpainter.end();
}

void doubleslider::paintEvent(QPaintEvent*){
  bitBlt(this, 0, 0, buffer);
}

void doubleslider::resizeEvent(QResizeEvent* event){
  buffer->resize(event->size().width(), event->size().height());
  paintAll();
}

void doubleslider::mousePressEvent(QMouseEvent* event){
  if ( (event->x() > wert1*width() - 3) &&
       (event->x() < wert1*width() + 3) )
    bewegeLinks = true;
  if ( (event->x() > wert2*width() - 3) &&
       (event->x() < wert2*width() + 3) )
    bewegeRechts = true;
}

void doubleslider::mouseReleaseEvent(QMouseEvent*){
  bewegeLinks = false;
  bewegeRechts = false;
}

void doubleslider::mouseMoveEvent(QMouseEvent* event) {
  if (bewegeLinks) {
    float temp  = (event->x()*1.0) / width();
    if (temp < wert2) {
  	  wert1 = temp;
	    paintAll();
	    emit value1Changed(wert1);
  	}
  }
  if (bewegeRechts) {
    float temp  = (event->x()*1.0) / width();
    if (temp > wert1) {
	    wert2 = temp;
	    paintAll();
	    emit value2Changed(wert2);
	  }
  }
}
