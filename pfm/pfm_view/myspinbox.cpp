/***************************************************************************
                          myspinbox.cpp  -  description
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

#include "myspinbox.h"

mySpinBox::mySpinBox(float min, float max, float step,
		     QWidget* parent, const char* name):
  QSpinBox(int(min*100), int(max*100), int(step*100), parent, name)
{
}
QString mySpinBox::mapValueToText(int value)
{
  double newValue = value;
  newValue = newValue/ 100;
  QString bla;
  bla.setNum(newValue);
  return bla;
}
int mySpinBox::mapTextToValue(bool* ok)
{
  QString bla = text();
  float blubb = bla.toFloat(ok);
  blubb = blubb*100;
  int blubber = int(blubb);
  return blubber;
}
