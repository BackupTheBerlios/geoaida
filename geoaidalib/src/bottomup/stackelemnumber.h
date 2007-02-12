/***************************************************************************
                          stackelemnumber.h  -  description
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

#ifndef STACKELEMNUMBER_H
#define STACKELEMNUMBER_H

#ifdef WIN32
#include "stackelem.h"
#else
#include <stackelem.h>
#endif

/**Stack element holding a number
  *@author Martin Pahl
  */

class StackElemNumber : public StackElem  {
public: 
	StackElemNumber(double v);
	StackElemNumber(const StackElemNumber& elem);
	~StackElemNumber();
  /** returns the type (NUMBER) of this stack element  */
  int type();
  const char* typeName();
  /** Returns a copy of this stack element */
  StackElem* copy();
  /** Returns the contained numeric value from this stack element */
  double data();

private: // Private attributes
  /** Data */
  double data_;
};

#endif
