/***************************************************************************
                          propertyeditor.cpp  -  description
                             -------------------
    begin                : Mon Sep 11 2000
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
 * $Source: /data/cvs/geoaidalib/geoaida/plm_view/propertyeditor.cpp,v $
 * $Revision: 1.1 $
 * $Date: 2002/06/05 09:22:21 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "propertyeditor.h"
#include "propertyitem.h"
#include "treeitem.h"
#include "qpopupmenu.h"


// #define NO_CLASSES

// #define DEBUGMSG

PropertyEditor::PropertyEditor(QWidget * parent, const char *name)
:QListView(parent, name)
{
  attribList_ = 0;
  node_ = 0;
  listViewItem_ = 0;
  selectedItem_ = 0;
  update_ = false;
  toBeDeleted_ = 0;
  addColumn("Name:");
  addColumn("Value:");
  setSorting(-1);
//      setAllColumnsShowFocus(true).
  connect(this,
          SIGNAL(rightButtonPressed(QListViewItem *, const QPoint &, int)),
          this, SLOT(popupMenu(QListViewItem *, const QPoint &, int)));
  connect(this, SIGNAL(currentChanged(QListViewItem *)), this,
          SLOT(itemSelected(QListViewItem *)));
}

PropertyEditor::~PropertyEditor()
{
}

void
PropertyEditor::updateNode()
{
  qDebug("PropertyEditor::updateNode");
  update_ = true;
}

/** Set the node to be edited */
void
PropertyEditor::setNode(TreeItem * listViewItem, GNode * node)
{
#ifdef DEBUGMSG
  qDebug("PropertyEditor::setNode: Start\n");
#endif
  if (toBeDeleted_) {
    delete toBeDeleted_;
    toBeDeleted_ = 0;
  }
  if (update_) {
    toBeDeleted_ = selectedItem_;
    if (toBeDeleted_) {
      QListViewItem *parent = toBeDeleted_->QListViewItem::parent();
      if (!parent)
        this->takeItem(toBeDeleted_);
      else
        parent->takeItem(toBeDeleted_);
    }
    selectedItem_ = 0;
  }
  itemSelected(0);
  node_ = node;
  listViewItem_ = listViewItem;
  selectedItem_ = 0;
  clear();
  if (!node_ || !listViewItem_)
    return;
  attribList_ = &(node->attribList());
  QDictIterator < QString > it(*attribList_);
#ifdef DEBUGMSG
  qDebug("PropertyEditor::setNode: Using default attriblist\n");
#endif
  for (; it.current(); ++it) {
    new PropertyItem(node_, this, it.currentKey(),
                     *(it.current()), true);
  }
#ifdef DEBUGMSG
    qDebug("Done\n");
#endif
  {
    QListViewItemIterator it(this);
    for (; it.current(); ++it) {
      PropertyItem *p = (PropertyItem *) (it.current());
      p->addToolTip();
    }
  }
#if 0
  PropertyItem *parent =
    new PropertyItem(node_, this, "default Attribs", "", true);
  parent->setSelectable(false);
  parent->setOpen(true);
  {
    QStringList::Iterator it;
    for (it = node_->predefinedAttribs().begin();
         it != node_->predefinedAttribs().end(); ++it) {
#ifdef DEBUGMSG
      qDebug("add Attribute %s\n", (const char *) (*it));
#endif
      new PropertyItem(node_, parent, (*it), node_->attribute(*it));
    }
  }
  parent = new PropertyItem(node_, this, "add Attributes", "", true);
  {
    QDictIterator < QString > it(*attribList_);
    for (; it.current(); ++it) {
      if (!node_->predefinedAttribs().contains(it.currentKey()))
        new PropertyItem(node_, parent, it.currentKey(), *(it.current()));
    }
  }
#endif
}

/** Slot is called whenever an item is selected */
void PropertyEditor::itemSelected(QListViewItem * item)
{
#if 0
  if (update_)
    return;
  if (selectedItem_) {
    listViewItem_->setText(0, node_->name());
    listViewItem_->repaint();
  }
  if (update_) {
    selectedItem_ = (PropertyItem *) item;
    setNode(listViewItem_, node_);
    update_ = false;
  }
  else {
    selectedItem_ = (PropertyItem *) item;
    if (selectedItem_ && selectedItem_ != toBeDeleted_)
      selectedItem_->edit();
  }
#endif
}

