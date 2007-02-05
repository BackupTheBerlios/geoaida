/***************************************************************************
                          operatorlist.cpp  -  description
                             -------------------
    begin                : Thu Oct 5 2000
    copyright            : (C) 2000 by Martin Pahl
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
 * $Source: /data/cvs/gda/gda/core/operatorlist.cpp,v $
 * $Revision: 1.9 $
 * $Date: 2002/06/15 09:36:51 $
 * $Author: gerhards $
 * $Locker:  $
 */

#include "operatorlist.h"
#include <MLParser.h>
#include <qdir.h>
#ifdef WIN32
#include <stdlib.h> // für exit
#endif

OperatorList::OperatorList():QDict < Operator > (17, false), keys_(17, false)
{
}

/** Read the operatorlist from the provided filename */
OperatorList::OperatorList(QString filename)
{
  read(filename);
}

OperatorList::~OperatorList()
{
}

/** Read all operator files in the given directory */
void
OperatorList::readDir(QString dir)
{
  qDebug("OperatorList::readDir %s\n", dir.latin1());
#ifdef WIN32
  QDir d(dir, stopsuffix);
#else
  QDir d(dir, "*.op");
#endif
  unsigned int i;
  for (i = 0; i < d.count(); i++) {
#ifdef WIN32
    read(dir + "\\" + d[i]);
#else
    read(dir + "/" + d[i]);
#endif
  }
}

/** Read the operatorlist from the provided filename */
void
OperatorList::read(QString filename)
{
  QFile fp(filename);
  qDebug("OperatorList::read(%s)", filename.latin1());
  if (!fp.open(IO_ReadOnly)) {
    qDebug("SmeNet::read(%s): file not found\n", (const char *) filename);
    return;
  }
  read(fp);
  fp.close();

}

/** read the operatorlist from the provided filepointer */
void
OperatorList::read(QIODevice & fp)
{
  QString keywords[] = { "OPERATOR", "" };
  const MLTagTable nodeTagTable(keywords);
  const int TOK_OPERATOR = 1;
  MLParser parser(&fp);
  int tag;
  do {
    tag = parser.tag(nodeTagTable);
    switch (tag) {
    case TOK_OPERATOR:
      {
        Operator *op = new Operator(parser);
#ifdef WIN32
        if (op == 0){
          cout << "Out of Memory..13";
          exit(1);
        }
#endif
        insert(op->name(), op);
        QString type = op->type();
        if (!keys_[type]) {
          QStringList *l = new QStringList();
          keys_.insert(type, l);
          (*l) += "*** No Operator ***";
        }
        QStringList *l = keys_[type];

        (*l) += op->name();
        break;
      }
    default:
      {
        ArgDict *args = parser.args();
        delete args;
        break;
      }
    }
  } while ((tag != MLParser::END_OF_FILE));
}

/** Returns a list of the keys contained in this dictionary */
QStringList *
OperatorList::keys(QString type)
{
  return keys_[type];
}

OperatorList operatorList_;
