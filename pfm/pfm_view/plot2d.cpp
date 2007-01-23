/***************************************************************************
                          plot2d.cpp  -  description
                             -------------------
    begin                : Thu May 16 2002
    copyright            : (C) 2002 by jrgen bckner
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

#include "plot2d.h"
#include <qpainter.h>

Plot2D::Plot2D(QWidget *parent, const char *name ) : QWidget(parent,name)
{
}

Plot2D::~Plot2D()
{
}
/** set the picture to be drawn */
void Plot2D::set(const QPicture& p)
{
  picture_=p;
}

/** No descriptions */
void Plot2D::paintEvent(QPaintEvent*)
{
	qDebug("Plot2D::paintEvent");
  QPainter p;
  p.begin(this);
#if 0
  QRect r=p.window();
  qDebug("Plot2D::paintEvent(): %d %d %d %d",
          r.left(),
          r.top(),
          r.right(),
          r.bottom());
#endif
  p.drawPicture(picture_);
	p.end();
}
