/***************************************************************************
                          semnet.h  -  description
                             -------------------
    begin                : Mon Sep 4 2000
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
 * $Source: /data/cvs/gda/gda/core/semnet.h,v $
 * $Revision: 1.3 $
 * $Date: 2001/11/27 16:35:30 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef SEMNET_H
#define SEMNET_H

#include "MLParser.h"
#include "snode.h"

/**Semantic Net of type SNode
  *@author Martin Pahl
  */

class SemNet
{
public:
  SemNet();
  ~SemNet();

  /** Read a semantic net by using a filename */
  void read(const QString & fname);
  /** Read a semantic net  */
  void read(QIODevice & fp);
  /** Get the rootNode_ of the semantic net */
  SNode *rootNode(void);
  /** write the semantic net to the file fp */
  void write(QTextStream & fp);
  /** Write the semantic net to the file fname */
  void write(const QString & fname);
protected:                     // Protected attributes
  /** root node of the semantic net */
    SNode * rootNode_;
};
#endif
