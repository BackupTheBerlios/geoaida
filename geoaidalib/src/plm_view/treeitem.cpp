/***************************************************************************
                          treeitem.cpp  -  description
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
 * $Source: /data/cvs/geoaidalib/geoaida/plm_view/treeitem.cpp,v $
 * $Revision: 1.2 $
 * $Date: 2002/06/06 04:13:01 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "treeitem.h"
#include "gnode.h"
#include <qpainter.h>

// #define DEBUG_MSG

QPixmap *pixmap_generalize = 0;
QPixmap *pixmap_compound = 0;

TreeItem::TreeItem(QListViewItem * parent, QString label1, QString label2,
                   QString label3, QString label4, QString label5,
                   QString label6, QString label7, QString label8)
:
QListViewItem(parent, label1, label2, label3, label4, label5, label6, label7,
              label8)
{
  nodeObj_ = 0;
}

TreeItem::TreeItem(QListView * parent, QString label1, QString label2,
                   QString label3, QString label4, QString label5,
                   QString label6, QString label7, QString label8)
:
QListViewItem(parent, label1, label2, label3, label4, label5, label6, label7,
              label8)
{
  nodeObj_ = 0;
}

TreeItem::~TreeItem()
{
#ifdef WIN32
//Access Violation.
//  if (nodeObj_) nodeObj_->setGuiPtr(0);
  if (nodeObj_!=0) delete nodeObj_;
#else
  if (nodeObj_) nodeObj_->setGuiPtr(0);
#endif
//  if (nodeObj_!=0) delete nodeObj_;
}

void TreeItem::insertNode(GNode * node)
{
  nodeObj_ = node;
  node->setGuiPtr(this);
  QListIterator < GNode > it = QListIterator < GNode > (node->children());
  while (it.current()) {
    GNode *n = it.current();
#ifdef DEBUG_MSG
    qDebug("TreeItem: add %s\n", (const char *) n->name());
#endif
    TreeItem *item = new TreeItem(this, n->name());
    if (n->isA("SNode")) {
      pixmap_.resize(20, 10);
      pixmap_.fill(n->color());
      item->setPixmap(0, pixmap_);
    }
#if 0
    if (nodeObj_->isCompound())
      item->setPixmap(0, *pixmap_compound);
    else
      item->setPixmap(0, *pixmap_generalize);
#endif
#ifdef DEBUG_MSG
    qDebug("TreeItem: done %s\n", (const char *) n->name());
#endif
    item->insertNode(n);
    ++it;
  }
  setOpen(TRUE);
}

/** Show popupmenu */
void TreeItem::popupMenu(void)
{
}

/** delete associated node */
void TreeItem::deleteNode()
{
  if (nodeObj_)
    delete nodeObj_;
}

/** return the associated nodeObj_ */
GNode *TreeItem::node()
{
  return nodeObj_;
}

void TreeItem::paintCell(QPainter * p, const QColorGroup & cg,
                         int column, int width, int align)
{
  if (!p)
    return;

  QColorGroup tmp_cg(cg);
#if 0
  if (nodeObj_->isA("INode")) {
    INode *iNode = (INode *) nodeObj_;
    QFont f = p->font();
    if (iNode->attribute("status") == "deleted") {
      f.setItalic(true);
      f.setWeight(QFont::Light);
    }
    else if (iNode->status()==TD_ERROR
            || iNode->status()==BU_ERROR) {
      tmp_cg.setColor(QColorGroup::Text,red);
    }
    else if (iNode->status()==TD_ABORTED
            || iNode->status()==BU_ABORTED) {
      tmp_cg.setColor(QColorGroup::Text,yellow);
    }
    else {
      f.setWeight(QFont::DemiBold);
    }
    p->setFont(f);
  }
#endif
  QListViewItem::paintCell(p, tmp_cg, column, width, align);
}

void TreeItem::redrawSNode()
{
  setText(0, nodeObj_->name());
  repaint();
}

void stateChanged(TreeItem * ti)
{
  if (ti)
    ti->redrawSNode();
}
