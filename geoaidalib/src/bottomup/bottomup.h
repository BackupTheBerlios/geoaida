/***************************************************************************
                          bottomup.h  -  description
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

#ifndef BUTTONUP_H
#define BUTTONUP_H

#include "qdict.h"
#include "qfile.h"
#include <MLParser.h>
#include "nodelist.h"

/**
  *@author jürgen bückner
  */
/** node list class */
class BottomUp {
public: 
	/** default constructor */
	BottomUp();
  /** Read the infos from the provided filename */
	BottomUp(QString filename);
	/** default destructor */
	~BottomUp();
	
	/** init funktion */
	void init();
  /** Read the infos from the provided filename */
	void read(QString filename);
  /** Read the infos from the provided filepointer */
  void read(QIODevice& fp);
  /** return the number of classes read into 'arglist'  */
  int number_of_classes();
	/** show info */
	void node_info() {nodelist_->info();};
  /** return a list of pointer to the objects of specified class type */
  NodeList* selectClass (QString classname);
  /** return a list of pointer to the objects of specified class type */
  int calcForSelect (QString classname);

protected: // Protected attributes
  /** list of class description */
  NodeList* nodelist_;
};

#endif

