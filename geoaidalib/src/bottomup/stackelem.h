/***************************************************************************
                          stackelem.h  -  description
                             -------------------
    begin                : Tue Jul 31 2001
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

#ifndef STACKELEM_H
#define STACKELEM_H

/**Stack Element
  *@author Martin Pahl
  */

class StackElem {
public:
	StackElem();
	virtual ~StackElem();
  /** Type of stack element */
  virtual int type()=0;
  virtual const char* typeName()=0;
  /** Returns a copy of this stack element */
  virtual StackElem* copy() = 0;
  enum {
    UNKNOWN,
    ERROR,
    NODELIST,
    STRING,
    NUMBER,
    STACK
  };
};

#endif
