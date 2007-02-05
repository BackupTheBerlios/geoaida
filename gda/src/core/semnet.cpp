/***************************************************************************
                          semnet.cpp  -  description
                             -------------------
    begin                : Mon Sep 4 2000
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
 * $Source: /data/cvs/gda/gda/core/semnet.cpp,v $
 * $Revision: 1.8 $
 * $Date: 2002/06/28 07:27:25 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "semnet.h"
#include <qfile.h>
#ifdef WIN32
#include <stdlib.h> // für exit
#endif

SemNet::SemNet()
{
  rootNode_ = new SNode();
#ifdef WIN32
        if (rootNode_ == 0){
          cout << "Out of Memory..14";
          exit(1);
        }
#endif
  rootNode_->name("Szene");
//  rootNode_->xxxmin(1);
//  rootNode_->xxxmax(1);
}

SemNet::~SemNet()
{
  if (rootNode_)
    delete rootNode_;
}

/** Read a semantic net  */
void SemNet::read(QIODevice & fp)
{
  if (rootNode_)
    delete rootNode_;
  MLParser parser(&fp);
  int tag;
  do {
    tag = parser.tag(nodeTagTable);
  } while ((tag != TOK_NODE) && (tag != MLParser::END_OF_FILE));
  if (tag == TOK_NODE)
    rootNode_ = new SNode(parser);
  if (rootNode_)
    qDebug("SemNet: Root=%s\n", (const char *) rootNode_->name());
#ifdef WIN32
#warning cout ist gar nicht definiert!!!!!
  else {
    cout << "Out of Memory..14";
    exit(1);
  }
#endif
}

/** Read a semantic net by using a filename */
void SemNet::read(const QString & fname)
{
  QFile fp(fname);
  if (!fp.open(IO_ReadOnly)) {
    qDebug("SemNet::read(%s): file not found", (const char *) fname);
    return;
  }
  read(fp);
  fp.close();
}

/** write the semantic net to the file fp */
void SemNet::write(QTextStream & fp)
{
  if (rootNode_)
    rootNode_->write(fp, QString(""));
}

/** Write the semantic net to the file fname */
void SemNet::write(const QString & fname)
{
  QFile fp(fname);
  if (!fp.open(IO_WriteOnly)) {
    qDebug("SemNet::write(%s): file not accessable", (const char *) fname);
    return;
  }
  QTextStream str(&fp);
  write(str);
  fp.close();
}

/** Get the rootNode_ of the semantic net */
SNode *SemNet::rootNode(void)
{
  return rootNode_;
}
