/***************************************************************************
                          formoperand.h  -  description
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

#ifndef OPERAND_H
#define OPERAND_H

#include <formbasenode.h>

/**Klasse zur Darstellung von Operanden in einer Baumstruktur
  *@author Ulrike
  */

class Operand : public FormBaseNode {
public: 
	Operand();
	~Operand();
	
  /** print fkt.
  virtual void printChilds();
  virtual void childLink(FormBaseNode *node);**/

};

#endif
