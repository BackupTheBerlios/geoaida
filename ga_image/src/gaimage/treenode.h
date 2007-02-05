/***************************************************************************
                          tree.h  -  description
                             -------------------
    begin                : Wed Sep 27 2000
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
 * $Source: /data/cvs/ga_image/ga_image/gaimage/treenode.h,v $
 * $Revision: 1.1 $
 * $Date: 2001/12/03 13:00:07 $
 * $Author: stahlhut $
 * $Locker:  $
 */

#ifndef TREENODE_H
#define TREENODE_H

#include <qstring.h>
#include <qlist.h>
/**Template class for a tree
  *@author Martin Pahl
  */

template <class T, class Base>
class TreeNode : public Base
{
public: 
	TreeNode();
	virtual ~TreeNode();
  /** Get the parent of this node */
  T* parent();
  /** Get the List of children */
  QList<T>& children();
};

#include "treenode.hpp"
#endif
