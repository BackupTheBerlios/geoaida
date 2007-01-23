/***************************************************************************
                          doubleslider.h  -  description
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

#ifndef DOUBLESLIDER_H
#define DOUBLESLIDER_H

#include <qwidget.h>
#include <qpainter.h>
#include <qpixmap.h>

/** 
Die Klasse Doubleslider stellt das Display für das CombiSlider-Widget zur Verfügung.
*/

class doubleslider: public QWidget
{
  Q_OBJECT
 public:
  doubleslider(float min__, float max__,
	       QWidget* parent = 0, const char* name =0);
  
 public slots:
    
 ///Setzt den ersten Wert.
  void setWert1(float val1);
 ///Setzt den zweiten Wert.
  void setWert2(float val2);
  ///Setzt den ersten Wert (Übergabe als String).
  void setWert1(const QString& val1);
  ///Setzt den zweiten Wert (Übergabe als String).
  void setWert2(const QString& val2);

 signals:
  ///Dieses Signal wird gesendet, wenn sich der erste Wert geändert hat.
  void value1Changed(float);
  ///Dieses Signal wird gesendet, wenn sich der zweite Wert geändert hat.
  void value2Changed(float);

 protected:
  virtual void resizeEvent(QResizeEvent* event);
  virtual void paintEvent(QPaintEvent*);

 private:
  float min_, max_, diff_, wert1, wert2;
  QPixmap* buffer;
  bool bewegeLinks;
  bool bewegeRechts;
  void paintAll();

 protected:
  virtual void mouseMoveEvent(QMouseEvent* event);
  virtual void mousePressEvent(QMouseEvent* event);
  virtual void mouseReleaseEvent(QMouseEvent* event);


};
#endif
