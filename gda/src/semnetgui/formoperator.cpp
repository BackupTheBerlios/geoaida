/***************************************************************************
                          formoperator.cpp  -  description
                             -------------------
    begin                : Wed Sep 5 2001
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

#include "formoperator.h"

FormOperator::FormOperator(){
}
FormOperator::~FormOperator(){
}


/** print fkt. */
void FormOperator::printChilds()  {
  //Liste der Kinder holen
  QList<FormBaseNode> nodelist = this->children();
	cout <<name_;
		
	setOpen();
		
  //Fuer alle Kinder die 'printChilds' Fkt. aufrufen
  FormBaseNode *node;
  for ( node = nodelist.first(); node != 0; node = nodelist.next() ){
    //if(node != nodelist.getFirst()) cout <<name_;                               // da stimmt was nicht
    node->printChilds();
	 }
	setClose();
}



/** Virtuelle Print-Funktion fuer Testzwecke. Gibt Infos zu den Knoten aus  */
void FormOperator::print(){
	cout << "call: FormOperator::print() - "<<name_<<endl;
}



/** Set-Funktion setzt "( "  */
void FormOperator::setOpen(){
  cout <<"(";
}

/** Set-Funktion setzt ") "  */
void FormOperator::setClose(){
  cout <<")";
}

/** setzt Separator */
void FormOperator::separator(QString s){
	separator_ = s;	
}

/** gibt Separator zurueck */
QString FormOperator::separator(void){
	cout <<separator_<<endl;
	cout <<"geschafft"<<endl;
	return separator_;
}

/** setzt Praefix */
void FormOperator::praefix(QString p){
	praefix_ = p;
}

/** gibt Praefix zurück */
QString FormOperator::praefix(void){
	cout <<praefix_;
	cout <<"klappt"<<endl;
	return praefix_;
}
