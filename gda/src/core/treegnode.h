/***************************************************************************
                          treenode.h  -  description
                             -------------------
    begin                : Thu Sep 28 2000
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
 * $Source: /data/cvs/ga_image/ga_image/gaimage/treegnode.h,v $
 * $Revision: 1.1 $
 * $Date: 2001/12/03 13:00:07 $
 * $Author: stahlhut $
 * $Locker:  $
 */

#ifndef TREEGNODE_H
#define TREEGNODE_H

#include <qlist.h>
#include <qstring.h>
/**TreeGNode for trees
  *@author Martin Pahl
  */

class TreeGNode {
public:
	TreeGNode();
	virtual ~TreeGNode();
	  /** Get the parent of this node */
  TreeGNode* parent();
  /** Get the List of children */
  QList<TreeGNode>& children();
  /** set the parent */
  void parent(TreeGNode*);
  /** Remove a child from the list of children */
  void childUnlink(TreeGNode *node);
  /** add a child to the tree */
  void childLink(TreeGNode *node);
  /** returns the name of this node */
  virtual const QString& name() const = 0;
protected: // Protected attributes
  /** List of children */
  QList<TreeGNode> children_;
  /** parent of this node */
  TreeGNode* parent_;
	};

#include "treegnode.hpp"

#endif
