/***************************************************************************
                          treenode.cpp  -  description
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
 * $Source: /data/cvs/ga_image/ga_image/gaimage/treegnode.hpp,v $
 * $Revision: 1.2 $
 * $Date: 2002/06/05 12:09:12 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "treegnode.h"

inline TreeGNode::TreeGNode()
{
  parent_=0;
}

inline TreeGNode::~TreeGNode(){
}

/** Get the parent of this node */
inline TreeGNode* TreeGNode::parent()
{
	return parent_;
}

/** set the parent */
inline void TreeGNode::parent(TreeGNode* node)
{
	parent_=node;
}

/** Get the List of children */
inline QList<TreeGNode>& TreeGNode::children()
{
	return children_;
}

/** add a child to the tree */
inline void TreeGNode::childLink(TreeGNode *node)
{
	if (!node) return;
	if (node->parent()) node->parent()->childUnlink(node);
	node->parent(this);
	children_.append(node);
}

/** Remove a child from the list of children */
inline void TreeGNode::childUnlink(TreeGNode *node)
{
	if (children_.findRef(node)>=0) {
		children_.take();
		node->parent(0);
	}
}
