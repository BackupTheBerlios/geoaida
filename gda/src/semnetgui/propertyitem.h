/***************************************************************************
                          propertyitem.h  -  description
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
 * $Source: /data/cvs/gda/gda/semnetgui/propertyitem.h,v $
 * $Revision: 1.7 $
 * $Date: 2001/11/27 16:35:30 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef PROPERTYITEM_H
#define PROPERTYITEM_H

#include <qlistview.h>
#include <qwidget.h>
#include "gnode.h"
#include <qlineedit.h>
/**Class to view an edit a node-attribute (property)
  *@author Martin Pahl
  */

class PropertyEditor;

class PropertyItem:public QObject, public QListViewItem
{
Q_OBJECT public:
  PropertyItem(GNode * node, PropertyEditor * parent, QString name,
               QString val, bool readOnly = false, Attribute * attrib = 0);
  PropertyItem(GNode * node, PropertyItem * parent, QString name, QString val,
               bool readOnly = false, Attribute * attrib = 0);
  ~PropertyItem();
  /** Edit the PropertyName */
  void editName();
  /** Deletes this attribute from nodes attributeList. Returns true, if
      the attribute was successfully removed
  */
  bool attributeRemove();
  /** add a tooltip to this item */
  void addToolTip();
  public slots:                 // Public slots
  /** Edit the attribute value */
  void edit();
  /** Finish editing process */
  void editDone();
protected:                     // Protected attributes
  /** Name of the property (attribute) to edit */
    QString name_;
  /** node_ to edit */
  GNode *node_;
  /** Editor for the attribute name */
  QLineEdit *nameEditor_;
  /** attributeeditor */
  QWidget *editor_;
  /** value of  the attribute name_ */
  QString value_;
  /** parent of this item */
  PropertyEditor *parent_;
  /** if true this is a readonly item */
  bool readOnly_;
  /** Description for this attribute */
  Attribute *attrib_;
    signals:                    // Signals
  /** Request an update of the attribute list */
  void requestUpdate();
};

#endif
