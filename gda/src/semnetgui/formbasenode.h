/***************************************************************************
                          formbasenode.h  -  description
                             -------------------
    begin                : Tue Sep 4 2001
    copyright            : (C) 2001 by Ulrike
    email                : waschkow@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef BASENODE_H
#define BASENODE_H

#include "treegnode.h"
#include "treenode.h"
#include <iostream>

using namespace std;

/**Basisklasse für einen Knoten im Baum
  *@author Ulrike
  */

class FormBaseNode : public TreeNode<FormBaseNode,TreeGNode>  {
public:
  /** Basic Constructor */
	FormBaseNode();
  /** Constructor - set the name of the node*/
	FormBaseNode(QString name);
	/** Destructor */
	~FormBaseNode();

  /** Set the name_ */
  void name(QString name);

  /** returns the name of this node */
  virtual const QString& name() const;

public:
  /** Virtuelle Print-Funktion. Gibt Infos zu den Knoten aus  */
  virtual void print();
  virtual void printChilds();

protected: // Protected attributes
	/** name of the node */
	QString name_;

};

#endif
