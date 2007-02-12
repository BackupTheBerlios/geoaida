/***************************************************************************
                          stackelemstring.h  -  description
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

#ifndef STACKELEMSTRING_H
#define STACKELEMSTRING_H

#include <qstring.h>
#ifdef WIN32
#include "stackelem.h"
#else
#include <stackelem.h>
#endif

/**Stack element holding a string
  *@author Martin Pahl
  */

class StackElemString : public StackElem  {
public: 
	StackElemString(QString s);
	StackElemString(const StackElemString& elem);
	~StackElemString();
  /** returns the type (STRING) of this stack element  */
  int type();
  const char* typeName();
  /** Returns a copy of this stack element */
  StackElem* copy();
  /** Returns the contained string from this stack element */
  QString data();
private: // Private attributes
  /** Data */
  QString data_;
};

#endif
