/***************************************************************************
                          paintwidget.h  -  description
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

#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <qwidget.h>
#include <qlabel.h>

/**Widget for painting a pixmap
  *@author jrgen bckner
  */

class PaintWidget : public QLabel  {
   Q_OBJECT
public: 
	PaintWidget(QWidget *parent=0, const char *name=0);
	~PaintWidget();
protected:
  void	resizeEvent( QResizeEvent * );
  void	mousePressEvent( QMouseEvent * );
  void	mouseReleaseEvent( QMouseEvent * );
  void	mouseMoveEvent( QMouseEvent * );
  void	paintEvent( QPaintEvent * );
signals: // Signals
  /** emitted if mouse is moved in paintwidget */
  void mouseMoved(QMouseEvent*);
  void mousePressed(QMouseEvent*);
  void mouseReleased(QMouseEvent*);
  void widgetResized(QResizeEvent*);
  void sigPaint(QPaintEvent*);
};

#endif
