/***************************************************************************
                          stackelemnodelist.cpp  -  description
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

#include "stackelemnodelist.h"

/** constructs a stack element containing the given nodelist */
StackElemNodeList::StackElemNodeList(const NodeList& list)
{
  data_=list;
}

StackElemNodeList::StackElemNodeList(const StackElemNodeList& elem)
{
 data_=elem.data_;
}


StackElemNodeList::~StackElemNodeList()
{
}

/** returns the type (NODELIST) of this stack element  */
int StackElemNodeList::type()
{
  return NODELIST;
}

const char* StackElemNodeList::typeName()
{
  return "nodelist";
}

/** Returns a copy of this stack element */
StackElem* StackElemNodeList::copy()
{
  NodeList nl=((StackElemNodeList*)this)->data(); //liste holen
  return (new StackElemNodeList(nl));
}

/** return the data in the stack element */
NodeList& StackElemNodeList::data()
{
  return data_;
}
