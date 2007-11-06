/***************************************************************************
                          treeeditor.cpp  -  description
                             -------------------
    begin                : Wed Sep 6 2000
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
 * $Source: /data/cvs/gda/gda/semnetgui/treeeditor.cpp,v $
 * $Revision: 1.19 $
 * $Date: 2004/02/27 10:30:17 $
 * $Author: mueller $
 * $Locker:  $
 */

#include "treeeditor.h"
#include <qpopupmenu.h>
#include "treeitem.h"
#include "inode.h"

#ifdef DEBUG
  #define DEBUGMSG
#endif

TreeEditor::TreeEditor(QWidget * parent, const char *name)
:QListView(parent, name)
{
  
  rootNode_ = 0;
  clipboardNode_ = 0;
  noUpdate_ = false;
  setRootIsDecorated(true);
  addColumn("Node");
//  addColumn("2. Spalte");
  connect(this,
          SIGNAL(rightButtonPressed(QListViewItem *, const QPoint &, int)),
          this, SLOT(popupMenu(QListViewItem *, const QPoint &, int)));
  connect(this, SIGNAL(currentChanged(QListViewItem *)), this,
          SLOT(itemSelected(QListViewItem *)));
}

TreeEditor::~TreeEditor()
{
//  if (!rootNode_) delete rootNode_;
}


/** Set the rootNode_ of the semantic net */
void TreeEditor::rootNode(GNode * node)
{
  noUpdate_=true;
  clear();
  rootNode_ = node;
  if (!node) {
    noUpdate_=false;
    emit itemChanged(0, 0);
    return;
  }
  QString name = rootNode_->name();
#ifdef DEBUGMSG
  qDebug("treeeditor:73:Root=%s\n", (const char *) rootNode_->name());
  qDebug("Root=%s\n", name.ascii());
#endif
//      TreeItem *item=new TreeItem(this,"Root");
  TreeItem *item = new TreeItem(this, rootNode_->name());
#if 0
  if (rootNode_->isCompound())
    item->setPixmap(0, *pixmap_compound);
  else
    item->setPixmap(0, *pixmap_generalize);
#endif
  item->insertNode(rootNode_);
  noUpdate_=false;
//hier Absturz mit EDX =0xFDFDFDFD (Stacküberschreitung)
  emit itemChanged(item, rootNode_);
}

void TreeEditor::connectSlots()
{
  if (!rootNode_)
    return;
  connect(this, SIGNAL(expanded(QListViewItem *)),
          SLOT(showDetails(QListViewItem *)));
  connect(this, SIGNAL(collapsed(QListViewItem *)),
          SLOT(hideDetails(QListViewItem *)));
}

void TreeEditor::disconnectSlots()
{
  if (!rootNode_)
    return;
  if (rootNode_->isA("INode")) {
    disconnect(SIGNAL(expanded(QListViewItem *)), this,
               SLOT(showDetails(QListViewItem *)));
    disconnect(SIGNAL(collapsed(QListViewItem *)), this,
               SLOT(hideDetails(QListViewItem *)));
  }
}

/** slot to popup a menu for the selected item */
void
  TreeEditor::popupMenu(QListViewItem * item, const QPoint & point,
                        int column)
{
#ifdef DEBUGMSG
  qDebug("TreeEditor: right button pressed\n");
#endif
  if (!item)
    return;                     // ((TreeItem*)item)->popupMenu();
  TreeItem *lItem = (TreeItem *) item;
  setSelected(item, true);
  repaint();  
  QPopupMenu *popup;
  popup = new QPopupMenu;
  popup->insertItem("Copy Node", 0);
  popup->insertItem("Cut Node", 1);
  popup->insertItem("Paste Node", 2);
  popup->insertItem("New Node", 3);
  int ret = popup->exec(point);
  delete popup;
  
  switch (ret) {
  case 0:
    clipboardNode_ = lItem->node()->copy();
    break;
  case 1:
    clipboardNode_ = lItem->node();
#ifdef DEBUGMSG
    qDebug("treeeditor:%s\n", (const char *) clipboardNode_->name());
    qDebug("treeeditor:%s\n", (const char *) clipboardNode_->parent()->name());
#endif
    clipboardNode_->parent()->childUnlink(clipboardNode_);
    delete lItem;
    repaint();
    break;
  case 2:
    if (clipboardNode_) {
      TreeItem *item = new TreeItem(lItem, clipboardNode_->name());
      item->insertNode(clipboardNode_);
      lItem->node()->childLink(clipboardNode_);
      clipboardNode_ = 0;
    }
    break;
  case 3:
    {
      GNode *node = lItem->node()->newNode();
      node->name("New Node");
#ifdef DEBUGMSG
      qDebug("treeeditor:New Node = %s\n", (const char *) node->name());
#endif
      TreeItem *item = new TreeItem(lItem, node->name());
      lItem->setOpen(true);
      item->insertNode(node);
      lItem->node()->childLink(node);
    }
    break;
  default:
    break;
  }
}

/** Slot should be called, whenever the selected item changes */
void TreeEditor::itemSelected(QListViewItem * item)
{
  if (!item)
    return;
  TreeItem *lItem = (TreeItem *) item;
  GNode *node = lItem->node();
  if (node) {
    emit itemChanged(lItem, node);
    if (node->isA("INode")) {
      qDebug("TreeEditor::itemSelected: Debug");
      INode *iNode = (INode *) node;
      GeoImage *map = iNode->labelImage();
      if (map)
        emit itemChanged(iNode->attributeInt("id"), map);
      else
        qDebug("No map");
    }
  }
}



/** No descriptions */
void TreeEditor::setCurrentNode(GNode * searchItem)
{
  if (!searchItem) {
    TreeItem *it = (TreeItem *) currentItem();
    setCurrentItem(it);
    ensureItemVisible(it);
  }
  else {
    itemFound_ = false;
    TreeItem *it = (TreeItem *) firstChild();
    searchNode(it, searchItem);
  }
}

/** No descriptions */
void TreeEditor::searchNode(TreeItem * startItem, GNode * searchItem)
{
  if (!startItem)
    return;
  GNode *node = startItem->node();
  if (node == searchItem) {
    itemFound_ = true;
    setCurrentItem(startItem);
    ensureItemVisible(startItem);
    return;
  }

  for (QListViewItem * it = startItem->firstChild(); it != 0;
       it = it->nextSibling()) {
    searchNode((TreeItem *) it, searchItem);
  }
}

/** adapt the view of INodes to the view of SNodes. This function is only used for INode trees. */
void TreeEditor::adaptViewToSNodeTree()
{
  if (noUpdate_)
    return;
  if (!rootNode_ || !rootNode_->isA("INode"))
    return;
  QListViewItemIterator it(this);
  qDebug("TreeEditor::adaptViewToSNodeTree");
  noUpdate_ = true;
  for (; it.current(); ++it) {
    TreeItem *item = (TreeItem *) it.current();
    INode *inode = (INode *) item->node();
    SNode *snode = inode->sNode();
    bool state = snode->showDetails();
    inode->setShowDetails(state);
    item->setOpen(state);
  }
  noUpdate_ = false;
  emit treeChanged();
}

/** No descriptions */
void TreeEditor::showDetails(QListViewItem * item)
{
  if (noUpdate_)
    return;
  qDebug("TreeEditor::showDetails");
  TreeItem *it = (TreeItem *) item;
  GNode *node = (GNode *) it->node();
  node->setShowDetails(true);
  emit treeChanged();
}

/** No descriptions */
void TreeEditor::hideDetails(QListViewItem * item)
{
  if (noUpdate_)
    return;
  qDebug("TreeEditor::hideDetails");
  TreeItem *it = (TreeItem *) item;
  GNode *node = (GNode *) it->node();
  node->setShowDetails(false);
  emit treeChanged();
}
