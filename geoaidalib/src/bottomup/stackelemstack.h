/***************************************************************************
                          stackelemstack.h  -  description
                             -------------------
    begin                : Tue Nov 13 2001
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

#ifndef STACKELEMSTACK_H
#define STACKELEMSTACK_H

#ifdef WIN32
#include "stack.h"
#include "stackelem.h"
#else
#include <stack.h>
#include <stackelem.h>
#endif

/**This stack element holds a stack
  *@author Martin Pahl
  */

class StackElemStack : public StackElem  {
public: 
	StackElemStack(const Stack& stack);
	StackElemStack(const StackElemStack& elem);
	~StackElemStack();
	StackElem* copy();
	Stack& data();
	int type();
  const char* typeName();
protected: // Protected attributes
  /** Data */
  Stack stack_;
};

#endif
