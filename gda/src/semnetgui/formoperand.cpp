/***************************************************************************
                          formoperand.cpp  -  description
                             -------------------
    begin                : Thu Sep 6 2001
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

#include "formoperand.h"

Operand::Operand(){
}
Operand::~Operand(){
}


/** print fkt.
void Operand::printChilds()  {
  //Liste der Kinder holen
  QList<FormBaseNode> nodelist = this->children();
		

  //Fuer alle Kinder die 'printChilds' Fkt. aufrufen
  FormBaseNode *node;
  for ( node = nodelist.first(); node != 0; node = nodelist.next() ){
    if(node != nodelist.getFirst()) cout <<name_;
    node->printChilds();
  }
}

void Operand::childLink(FormBaseNode *node){
  cout << "call: Operand::childLink() - ERROR"<<endl;
}
**/
