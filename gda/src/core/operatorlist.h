/***************************************************************************
                          operatorlist.h  -  description
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
 * $Source: /data/cvs/gda/gda/core/operatorlist.h,v $
 * $Revision: 1.8 $
 * $Date: 2002/06/28 07:27:25 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef OPERATORLIST_H
#define OPERATORLIST_H

#include "operator.h"
#ifdef MUELL
#include "resource.h"
#endif
#include "qdict.h"
#include "qfile.h"

/**List of all available Operators
  *@author Martin Pahl
  */

class OperatorList:public QDict < Operator >
{
public:
  OperatorList();
  /** Read the operatorlist from the provided filename */
  OperatorList(QString filename);
  ~OperatorList();
  /** Read all operator files in the given directory */
  void readDir(QString dir);
  /** Read the operatorlist from the provided filename */
  void read(QString filename);
  /** Read the operatorlist from the provided filepointer */
  void read(QIODevice & fp);
  /** Returns a list of the keys contained in this dictionary */
  QStringList *keys(QString type);
protected:                     // Protected attributes
  /** Contains the list of keys in this dictionary */
    QDict < QStringList > keys_;
};

extern OperatorList operatorList_;
#endif
