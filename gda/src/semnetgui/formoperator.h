/***************************************************************************
                          formoperator.h  -  description
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

#ifndef OPERATOR_H
#define OPERATOR_H

#include <formbasenode.h>

/**Diese Klasse ist zur Darstellung der FormOperatoren in einer Baumstrukur 
  *@author Ulrike
  */

class FormOperator : public FormBaseNode  {
public: 
	FormOperator();
	~FormOperator();
	
  /** print fkt. */
  virtual void print();
  virtual void printChilds();
	virtual void setOpen();
	virtual void setClose();
	
	/** setzt Separator */
  void separator(QString s);

  /** gibt Separator zurueck */
  QString separator (void);

 /** setzt Praefix */
  void praefix (QString p);

  /** gibt Praefix zurück */
  QString praefix (void);

private: // Private attributes
  /** enthält das Separator-Zeichen */
  QString separator_;

	/** enthält den Praefix */
  QString praefix_;

};

#endif
