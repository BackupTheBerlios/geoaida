/***************************************************************************
                          gasensor.cpp  -  description
                             -------------------
    begin                : Mon Jan 22 2001
    copyright            : (C) 2001 by jürgen bückner
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


#include "gasensor.h"

//GASensor::GASensor(){
//}
//GASensor::~GASensor(){
//}

/** return label   */
unsigned long int GASensor::id(void) {
	return id_;
}

/** return node pointer   */
Node* GASensor::node(void) {
	return node_;
}

/** set node pointer   */
void GASensor::node(Node* n){
	node_=n;
	id_ =((*node_)["id"])->toInt(); //get region ID
}


