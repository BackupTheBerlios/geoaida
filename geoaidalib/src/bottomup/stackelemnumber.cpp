/***************************************************************************
                          stackelemnumber.cpp  -  description
                             -------------------
    begin                : Wed Aug 1 2001
    copyright            : (C) 2001 by Martin Pahl
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

#include "stackelemnumber.h"

StackElemNumber::StackElemNumber(double v) {
  data_=v;
}

StackElemNumber::StackElemNumber(const StackElemNumber& elem) {
  data_=elem.data_;
}

StackElemNumber::~StackElemNumber() {
}

/** returns the type (NUMBER) of this stack element  */
int StackElemNumber::type() {
  return NUMBER;
}

const char* StackElemNumber::typeName()
{
  return "number";
}

/** Returns a copy of this stack element */
StackElem* StackElemNumber::copy() {
  double val=((StackElemNumber*)this)->data(); //liste holen
  return (new StackElemNumber(val));
}

/** Returns the contained numeric value from this stack element */
double StackElemNumber::data() {
  return data_;
}
