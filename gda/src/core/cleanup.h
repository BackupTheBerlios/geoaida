/***************************************************************************
                          cleanup.h  -  description
                             -------------------
    begin                : Tue Jul 24 2001
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
 * $Source: /data/cvs/gda/gda/core/cleanup.h,v $
 * $Revision: 1.4 $
 * $Date: 2002/02/22 14:54:51 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef CLEANUP_H
#define CLEANUP_H

#include <qmap.h>
#include <qstring.h>

/** Names of temporary files are stored in this class for later cleanup
  *@author Martin pahl
  */

class CleanUp:public QMap < QString, char >
{
public:
  CleanUp();
  ~CleanUp();
  /** Append filename to the list of temporary files */
  void append(QString filename);
  /** Delete the stored files */
  void deleteFiles();
  static QString getTmpDir();
  static QString getTmpDirPID();
  /** creates the relDir in absDir and returns the new absDir */
  static QString mkdir(QString absDir, QString relDir);
  static QString& prefix(QString prefix="");
  static QString prefix_;
};

extern CleanUp cleanUp_;
#endif
