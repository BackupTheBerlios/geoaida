/***************************************************************************
                          formbasenode.cpp  -  description
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

#include "formbasenode.h"
using namespace std;

FormBaseNode::FormBaseNode(){
}

FormBaseNode::FormBaseNode(QString name){
  name_ = name;
}

FormBaseNode::~FormBaseNode(){
}

/** Set the name_ */
void FormBaseNode::name(QString name){
  name_ = name;
}

/** returns the name of this node */
const QString& FormBaseNode::name() const
{
	return name_;
}

/** Virtuelle Print-Funktion fuer Testzwecke. Gibt Infos zu den Knoten aus  */
void FormBaseNode::print(){
  cout << "call: FormBaseNode::print() - "<<name_<<endl;
}

/** Virtuelle Print-Funktion fuer Testzwecke. Gibt Infos zu den Knoten aus  */
void FormBaseNode::printChilds(){
  //Liste der Kinder holen
  QList<FormBaseNode> nodelist = this->children();
  cout << ">>call: FormBaseNode::printChilds() - "<<name_<<endl;

  //Fuer alle Kinder die 'printChilds' Fkt. aufrufen
  FormBaseNode *node;
  for ( node = nodelist.first(); node != 0; node = nodelist.next() )
    node->printChilds();

  cout << "<<call: FormBaseNode::printChilds() - "<<name_<<endl;
}
