/***************************************************************************
                          fatalerror.cpp  -  description
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
 * $Source: /data/cvs/gda/gda/core/fatalerror.cpp,v $
 * $Revision: 1.1 $
 * $Date: 2002/02/02 12:26:40 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "fatalerror.h"

FatalError::FatalError(QString msg)
{
  message_=msg;
}

FatalError::~FatalError()
{
}
/** returns the error message */
QString FatalError::message()
{
  return "Error: "+message_;
}
