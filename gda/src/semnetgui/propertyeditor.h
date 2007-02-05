/***************************************************************************
                          propertyeditor.h  -  description
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
 * $Source: /data/cvs/gda/gda/semnetgui/propertyeditor.h,v $
 * $Revision: 1.5 $
 * $Date: 2001/11/27 16:35:30 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef PROPERTYEDITOR_H
#define PROPERTYEDITOR_H

#include <qlistview.h>
#include "gnode.h"

/**Widget to edit the property (attribute) list of a node
  *@author Martin Pahl
  */

class PropertyItem;
class TreeItem;

class PropertyEditor:public QListView
{
Q_OBJECT public:
  PropertyEditor(QWidget * parent = 0, const char *name = 0);
   ~PropertyEditor();
  public slots:                 // Public slots
  /** Set the node to be edited */
  void setNode(TreeItem * listViewItem, GNode * node);
  void updateNode();
  /** Slot is called whenever an item is selected */
  void itemSelected(QListViewItem *);
  /** Show popupmenu */
  void popupMenu(QListViewItem * item, const QPoint & point, int column);
  /** set type field of new attribute */
  void setType(int type);
  /** set min field of new INT attribute */
  void setMin(int min);
  /** set max field of new INT attribute */
  void setMax(int max);
  /** set min field of new INT attribute */
  void setMin(double min);
  /** set max field of new INT attribute */
  void setMax(double max);
  /** set options field of new ENUM attribute */
  void setOptions(QString options);
  /** set default field of new attribute */
  void setDefault(QString val);

protected:                     // Protected attributes
  /** Pointer to the node, which will currently be edited */
    GNode * node_;
  /** AttributeList of node_ */
  AttribList *attribList_;
  /** Currently selected item */
  PropertyItem *selectedItem_;
  /** pointer to the TreeItem */
  TreeItem *listViewItem_;
  /** Parameters for new attribute */
  int type_;
  int minInt_;
  int maxInt_;
  double minDouble_;
  double maxDouble_;
  QString options_;
  QString defaultValue_;
  /** update the item list */
  bool update_;
  /** stores the currently selected item if an update is requestet to prevent this item from
      being deleted with the signal processing */
  PropertyItem *toBeDeleted_;
    signals:                    // Signals
  /** Emitted when type of new attribute is set to STRING */
  void typeSTRING();
  /** Emitted when type of new attribute is set to INT */
  void typeINT();
  /** Emitted when type of new attribute is set to DOUBLE */
  void typeDOUBLE();
  /** Emitted when type of new attribute is set to ENUM */
  void typeENUM();
  /** Emitted when type of new attribute is set to OPERATOR */
  void typeOPERATOR();
  /** Emitted when type of new attribute is changed */
  void typeNO();
};

#endif
