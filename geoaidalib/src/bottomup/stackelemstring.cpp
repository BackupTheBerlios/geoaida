/***************************************************************************
                          stackelemstring.cpp  -  description
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

#include "stackelemstring.h"

StackElemString::StackElemString(QString s)
{
  data_=s;
}

StackElemString::StackElemString(const StackElemString& elem)
{
  data_=elem.data_;
}

StackElemString::~StackElemString()
{
}

/** returns the type (STRING) of this stack element  */
int StackElemString::type()
{
  return STRING;
}

const char* StackElemString::typeName()
{
  return "string";
}

/** Returns a copy of this stack element */
StackElem* StackElemString::copy()
{
  QString val=((StackElemString*)this)->data(); //liste holen
  return (new StackElemString(val));
}

/** Returns the contained string from this stack element */
QString StackElemString::data()
{
  return data_;
}
