/***************************************************************************
                          tree.cpp  -  description
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
 * $Source: /data/cvs/ga_image/ga_image/gaimage/treenode.hpp,v $
 * $Revision: 1.1 $
 * $Date: 2001/12/03 13:00:07 $
 * $Author: stahlhut $
 * $Locker:  $
 */

template <class T, class Base>
TreeNode<T, Base>::TreeNode(){
}

template <class T, class Base>
TreeNode<T, Base>::~TreeNode(){
}

/** Get the parent of this node */
template <class T, class Base>
T* TreeNode<T, Base>::parent()
{
	return (T*)parent_;
}


/** Get the List of children */
template <class T, class Base>
QList<T>& TreeNode<T, Base>::children()
{
	return (QList<T>&)children_;
}

