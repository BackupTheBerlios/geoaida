/***************************************************************************
                          combislider.h  -  description
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

#ifndef COMBISLIDER_H
#define COMBISLIDER_H

//#include <cmath.h>

#include <qframe.h>
#include "myspinbox.h"
#include <qvalidator.h>
#include <qlayout.h>
#include <qlabel.h>

#include "doubleslider.h"

/**
Die Klasse combiSlider besteht aus zwei Spinboxen und einer Anzeige mit zwei Zeigern. Über die Pfeiltasten können die Werte in 0.05 - er Schritten verändert werden. Die Werte können auch direkt eingegeben oder per Maus eingestellt werden.
*/
class combiSlider: public QFrame
{
  Q_OBJECT
public:
  ///Der uebliche Konstruktor fuer QT-Widgets. 
  combiSlider(QString labelText, QWidget* parent = 0, const char* name = 0,
              float min__ = 0.0, float max__ = 0.0);
public slots:
  ///Mit setValue1 wird der erste Wert gesetzt und das Display aktualisiert.
  void setValue1(float val1);
 /// Mit setValue2 wird der zweite Wert gesetzt und das Display aktualisiert. 
  void setValue2(float val2);
public:
  /// Liefert den ersten Wert.
  float value1();
  /// Liefert den zweiten Wert.
  float value2();

protected:
  
private:
  doubleslider* slider;
  mySpinBox* box1;
  mySpinBox* box2;
  ///Kontrolliert die Eingabe (2 Nachkommastellen, zw. Null und Eins).
  QDoubleValidator* vali1;
  QDoubleValidator* vali2;
  float min_, max_, diff_;
private slots:
  void valiAnpassen1(int wert);
  void valiAnpassen2(int wert);

};

#endif
