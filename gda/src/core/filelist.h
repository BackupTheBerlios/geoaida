/***************************************************************************
                          file.h  -  description
                             -------------------
    begin                : Thu Oct 19 2000
    copyright            : (C) 2000 by Jürgen Bückner
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

#ifndef FILE_LIST_H
#define FILE_LIST_H

#include <qstring.h>
#include <qstringlist.h>
#include <MLParser.h>
/**class to handel a list of file informations
  *@author Martin Pahl
  */

class FileList:public QDict< ArgDict >
{
public:
  /** default constructor */
  FileList();

  /** constructor resd from the provided filename*/
  FileList(QString filename);

  /** denstructor */
  ~FileList();

  /** clears the filelist */
  void clear();
  /** read a list of File and the attributes through parser */
  void read(MLParser & parser);
  /** read a list of File and the attributes from the provided filename */
  void read(QString filename);
  /** read a list of File and the attributes from the provided filepointer */
  void read(QIODevice & fp);

  /** return a list of included files*/
  QStringList list(QString type = "");

  /** return the filename corresponding to the given key */
  QString filename(QString fileKey);
  /** return Info about the file*/
  QString info(QString fileKey);

protected:                     // Protected attributes
  QStringList list_;

};

extern FileList fileList_;
#endif
