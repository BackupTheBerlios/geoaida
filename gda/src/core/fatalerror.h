/***************************************************************************
                          fatalerror.h  -  description
                             -------------------
    begin                : Tue Nov 27 2001
    copyright            : (C) 2001 by Martin pahl
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
 * $Source: /data/cvs/gda/gda/core/fatalerror.h,v $
 * $Revision: 1.1 $
 * $Date: 2002/02/02 12:26:40 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef FATALERROR_H
#define FATALERROR_H

#include <qstring.h>


/**Error class is thrown whenever there is a fatal error
  *@author Martin pahl
  */

class FatalError {
public: 
	FatalError(QString msg);
	~FatalError();
  /** returns the error message */
  QString message();
protected: // Protected attributes
  /** error message */
  QString message_;
};

#endif
