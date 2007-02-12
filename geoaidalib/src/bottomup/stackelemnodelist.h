/***************************************************************************
                          stackelemnodelist.h  -  description
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

#ifndef STACKELEMNODELIST_H
#define STACKELEMNODELIST_H

#ifdef WIN32
#include "stackelem.h"
#else
#include <stackelem.h>
#endif
#include "nodelist.h"
//#include "regionsensor.h"

/**Stack element holding a node list
  *@author Martin Pahl
  */

class StackElemNodeList : public StackElem  {
public: 
  /** constructs a stack element containing the given nodelist */
  StackElemNodeList(const NodeList& list);
  /** Copy constructor */
  StackElemNodeList(const StackElemNodeList& list);
	~StackElemNodeList();
  /** returns the type (NODELIST) of this stack element  */
  int type();
  const char* typeName();
  /** Returns a copy of this stack element */
  StackElem* copy();
  /** return the data in the stack element */
  NodeList& data();
private: // Private attributes
  /**  Data */
  NodeList data_;
};

#endif
