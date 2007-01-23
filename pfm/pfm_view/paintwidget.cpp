/***************************************************************************
                          paintwidget.cpp  -  description
                             -------------------
    begin                : Wed Feb 14 2001
    copyright            : (C) 2001 by jrgen bckner
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

#include "paintwidget.h"

PaintWidget::PaintWidget(QWidget *parent, const char *name )
: QLabel(parent,name) {
}

PaintWidget::~PaintWidget(){
}

void PaintWidget::mousePressEvent( QMouseEvent *e )
{
	emit mousePressed(e);
}

void PaintWidget::mouseReleaseEvent( QMouseEvent *e )
{
	emit mouseReleased(e);
}

/*
  Record the pixel position of interest.
*/
void PaintWidget::mouseMoveEvent( QMouseEvent *e )
{
	emit mouseMoved(e);
}

void PaintWidget::resizeEvent( QResizeEvent *e )
{
	emit widgetResized(e);
}

void PaintWidget::paintEvent( QPaintEvent *e )
{
	emit sigPaint(e);
}
