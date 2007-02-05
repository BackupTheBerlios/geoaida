/***************************************************************************
                          operator.h  -  description
                             -------------------
    begin                : Thu Oct 5 2000
    copyright            : (C) 2000 by Martin Pahl
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

/*
 * $Source: /data/cvs/gda/gda/core/operator.h,v $
 * $Revision: 1.7 $
 * $Date: 2001/11/27 16:35:30 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef OPERATOR_H
#define OPERATOR_H

#include <MLParser.h>
#include "gnode.h"
#include "attribute.h"

/**An Operator starts an external executable to perform a certain task e.g. topDown, bottomUp for a node
  *@author Martin Pahl
  */

class Operator
{
public:
  Operator();
  /** Generate a new operator using the provided parser */
  Operator(MLParser & parser);
  ~Operator();
  /** execute the operator returning the pid */
  QString command(AttribList & attribList);
  /** returns the name of the operator */
  QString name();
  /** read attributes for this operator through parser */
  void read(MLParser & parser);
  /** Returns the description of the attributes */
    QDict < Attribute > *attributeDesc();
  /** returns the type of operator (topdown, bottomup ...)   */
  QString type();
        /** returns a tip for this operator */
  QString tip();
  /** Returns true if operator works best on whole image, otherwise returns false */
  bool runGlobal();
protected:                     // Protected attributes
  /** classname of the operator */
    QString class_;
  /** command to execute */
  QString cmd_;
  /** name of the operator */
  QString name_;
  /** dictionary of attributes defined for this operator */
    QDict < Attribute > attributeDict_;
  /** type of operator (topdown, bottomup ...)    */
  QString type_;
  /** tip for this operator */
  QString tip_;
public:                        // Public attributes
  /** true if operator works best on whole image, default is false */
    bool runGlobal_;
};

#endif
