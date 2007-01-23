/***************************************************************************
                          myspinbox.h  -  description
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

#ifndef MYSPINBOX_H
#define MYSPINBOX_H

#include <cmath>

#include <qspinbox.h>

///Die Klasse mySpinBox ist von QSpinBox abgeleitet und ermöglicht das Eingeben
// von Fließkommazahlen zwischen Null und Eins mit zwei Nachkommastellen.
class mySpinBox: public QSpinBox
{
 public:
  mySpinBox(float min, float max, float step,
	    QWidget* parent =0, const char* name = 0);

 protected:
  virtual QString mapValueToText(int value);
  virtual int mapTextToValue(bool* ok);
};


#endif
