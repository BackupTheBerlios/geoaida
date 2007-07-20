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
 * $Source: /data/cvs/gda/gda/semnetgui/propertyeditor.cpp,v $
 * $Revision: 1.21 $
 * $Date: 2004/02/27 10:30:16 $
 * $Author: mueller $
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
  setSorting(0);
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
  const QStringList & sections = node->attributeSections();
  {
    QStringList::ConstIterator it;
    for (it = sections.begin(); it != sections.end(); ++it) {
#ifdef DEBUGMSG
      qDebug("PropertyEditor::setNode: section=%s\n", (*it).latin1());
#endif
      PropertyItem *parent = new PropertyItem(node_, this, *it, "", true);
      setOpen(parent, true);
      parent->setOpen(true);
      QDict < Attribute > *attribDict = node_->attributeDesc(*it);
      if (attribDict) {
        QDictIterator < Attribute > ait(*attribDict);
        for (; ait.current(); ++ait) {
          QString name = (*ait.current()).fullname();
#ifdef DEBUGMSG
          qDebug("PropertyEditor::setNode attrib=%s\n", name.latin1());
#endif
            new PropertyItem(node_, parent, name, node_->attribute(name),
                             false, ait.current());

        }
      }
      else {
        if (node->isA("INode")) {
          QDictIterator < QString > it(*attribList_);
#ifdef DEBUGMSG
          qDebug("PropertyEditor::setNode: Using default attriblist\n");
#endif
          for (; it.current(); ++it) {
              new PropertyItem(node_, parent, it.currentKey(),
                               *(it.current()), true);
          }
        }
      }
    }
#ifdef DEBUGMSG
    qDebug("Done");
#endif
  }
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
      qDebug("add Attribute %s", (const char *) (*it));
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
void
PropertyEditor::itemSelected(QListViewItem * item)
{
  if (update_){
      return;
  }
  if (selectedItem_) {
    selectedItem_->editDone();
    if (node_ && listViewItem_){
      listViewItem_->setText(0, node_->name());
      listViewItem_->repaint();
    }
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
}

/** Show popupmenu */
void
PropertyEditor::popupMenu(QListViewItem * item, const QPoint & point,
                          int column)
{
  itemSelected(0);
#ifdef DEBUGMSG
  qDebug("PropertyEditor: right button pressed");
#endif
  if (!item)
    return;                     // ((TreeItem*)item)->popupMenu();
  selectedItem_ = (PropertyItem *) item;
  setSelected(item, true);
  repaint();
  QPopupMenu *popup;
  popup = new QPopupMenu;
  popup->insertItem("New Attribute", 0);
  popup->insertItem("Delete Attribute", 1);
  popup->insertItem("Rename Attribute", 2);
  int ret = popup->exec(point);
  delete popup;
  switch (ret) {
  case 0:
    selectedItem_ = new PropertyItem(node_, this, "New Attribute", "");
    repaint();
    selectedItem_->editName();
    break;
  case 1:
    if (selectedItem_->attributeRemove()) {
      delete selectedItem_;
      selectedItem_ = 0;
      repaint();
    }
    break;
  case 2:
    selectedItem_->editName();
    break;
  }
}

/** set type attribute of new TYPE attribute */
void
PropertyEditor::setType(int type)
{
#ifdef DEBUGMSG
  qDebug("PropertyEditor::setType(%d)", type);
#endif
  emit typeNO();
  type_ = type;
  switch (type_) {
  case Attribute::STRING:
    emit typeSTRING();
    break;
  case Attribute::INT:
    emit typeINT();
    break;
  case Attribute::DOUBLE:
    emit typeDOUBLE();
    break;
  case Attribute::ENUM:
    emit typeENUM();
    break;
  case Attribute::OPERATOR:
    emit typeOPERATOR();
    break;
  }
}

/** set min attribute of new INT attribute */
void
PropertyEditor::setMin(int min)
{
  minInt_ = min;
}

/** set max attribute of new INT attribute */
void
PropertyEditor::setMax(int max)
{
  maxInt_ = max;
}

/** set min attribute of new DOUBLE attribute */
void
PropertyEditor::setMin(double min)
{
  minDouble_ = min;
}

/** set max attribute of new DOUBLE attribute */
void
PropertyEditor::setMax(double max)
{
  maxDouble_ = max;
}

/** set options attribute of new ENUM attribute */
void
PropertyEditor::setOptions(QString options)
{
  options_ = options;
}

/** set default attribute of new attribute */
void
PropertyEditor::setDefault(QString val)
{
  defaultValue_ = val;
}
