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
 * $Source: /data/cvs/geoaidalib/geoaida/plm_view/treeeditor.cpp,v $
 * $Revision: 1.4 $
 * $Date: 2002/06/10 05:49:28 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "treeeditor.h"
#include <qpopupmenu.h>
#include "treeitem.h"

// #define DEBUGMSG

TreeEditor::TreeEditor(QWidget * parent, const char *name)
:QListView(parent, name)
{
  rootNode_ = 0;
  clipboardNode_ = 0;
  noUpdate_ = false;
  maxDepth_=-1;
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
}

/** Set the rootNode_ of the semantic net */
void TreeEditor::rootNode(GNode * node)
{
  noUpdate_=true;
#ifdef WIN32
//  clear();
#else
  clear();
#endif
  rootNode_ = node;
  if (!node) {
    noUpdate_=false;
    emit itemChanged(0, 0);
    return;
  }
  QString name = rootNode_->name();
#ifdef DEBUGMSG
  qDebug("Root=%s\n", (const char *) rootNode_->name());
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
void TreeEditor::popupMenu(QListViewItem * item, const QPoint & point,
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
  QPopupMenu popup;
  popup.insertItem("Hide depth", 0);
  popup.insertItem("Show depth", 1);
  int ret = popup.exec(point);
  int depth=lItem->node()->depth();
  switch (ret) {
  case 0:
    noUpdate_=true;
    setMaxDepth((TreeItem*)firstChild(),depth,false);
    noUpdate_=false;
    emit treeChanged();
    break;
  case 1:
    noUpdate_=true;
    setMaxDepth((TreeItem*)firstChild(),depth,true);
    noUpdate_=false;
    emit treeChanged();
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
#if 0
    if (node->isA("INode")) {
      qDebug("TreeEditor::itemSelected: Debug");
      INode *iNode = (INode *) node;
      GeoImage *map = iNode->labelImage();
      if (map)
        emit itemChanged(iNode->attributeInt("id"), map);
      else
        qDebug("No map");
    }
#endif
  }
}



/** No descriptions */
void TreeEditor::setCurrentNode(GNode * searchItem)
{
  if (!searchItem) {
    TreeItem *it = (TreeItem *) currentItem();
    setCurrentItem(it);
    ensureItemVisible(it);;
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

#if 0
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
#endif

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


/** Recursivly set the maximum depth to show */
void TreeEditor::setMaxDepth(TreeItem* item, int depth, bool set)
{
  GNode* node=item->node();
  qDebug("TreeEditr::setMaxDepth: %s(%d) %d %d",
         node->name().latin1(),node->depth(),depth,set);
  if (node->depth()==depth) {
    node->setShowDetails(set);
    item->setOpen(set);
    return;
  }
  QListViewItem* it=item->firstChild();
  while (it) {
    setMaxDepth((TreeItem*)it,depth,set);
    it=it->nextSibling();
  }

}

