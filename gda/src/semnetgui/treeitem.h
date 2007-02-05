/***************************************************************************
                          listviewitem.h  -  description
                             -------------------
    begin                : Fri Sep 1 2000
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
 * $Source: /data/cvs/gda/gda/semnetgui/treeitem.h,v $
 * $Revision: 1.5 $
 * $Date: 2002/01/23 09:46:05 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef TREEITEM_H
#define TREEITEM_H

#include <qlistview.h>
#include <qpixmap.h>

/**
  *@author Martin Pahl
  */
class GNode;

class TreeItem:public QListViewItem
{
public:
  TreeItem(QListViewItem * parent, QString label1, QString label2 =
           QString::null, QString label3 = QString::null, QString label4 =
           QString::null, QString label5 = QString::null, QString label6 =
           QString::null, QString label7 = QString::null, QString label8 =
           QString::null);
  TreeItem(QListView * parent, QString label1, QString label2 =
           QString::null, QString label3 = QString::null, QString label4 =
           QString::null, QString label5 = QString::null, QString label6 =
           QString::null, QString label7 = QString::null, QString label8 =
           QString::null);
  virtual ~ TreeItem();
  virtual void insertNode(GNode * node);
  /** Show popupmenu */
  void popupMenu(void);
  /** delete associated node */
  void deleteNode();
  /** return the associated nodeObj_ */
  GNode *node();
  void paintCell(QPainter * p, const QColorGroup & cg,
                 int column, int width, int align);
  void redrawSNode();
private:
    GNode *nodeObj_;
    QPixmap pixmap_;
};

extern QPixmap *pixmap_generalize;
extern QPixmap *pixmap_compound;

#endif
