/***************************************************************************
                          bottomup.cpp  -  description
                             -------------------
    begin                : Tue Dec 12 2000
    copyright            : (C) 2000 by jürgen bückner
    email                : bueckner@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "bottomup.h"

BottomUp::BottomUp(){
  nodelist_=0;
  init();
}

BottomUp::BottomUp(QString filename){
  nodelist_=0;
  init();
  read(filename);
}

BottomUp::~BottomUp(){
  if (nodelist_)
    delete nodelist_;
}

void BottomUp::init() {
  if (nodelist_)
    delete nodelist_;
  nodelist_ = 0;
}

/** Read the infos from the provided file */
void BottomUp::read(QString filename)
{
	if (nodelist_) delete nodelist_;
	nodelist_ = new NodeList(filename);

}

/** Read the infos from the provided filepointer */
void BottomUp::read(QIODevice& fp){
	if (nodelist_) delete nodelist_;
	nodelist_ = new NodeList();
	nodelist_->read(fp);
}
/** return the number of classes read into 'nodelist'  */
int BottomUp::number_of_classes(){
	return nodelist_->size();
}
/** return a list of pointer to the objects of specified class type.
	* To select all say "ALL" */
NodeList* BottomUp::selectClass (QString classname){
  if (nodelist_->isEmpty()) return nodelist_;
	if (classname.compare("ALL") == 0) return nodelist_;
	return nodelist_->selectClass(classname);
}
