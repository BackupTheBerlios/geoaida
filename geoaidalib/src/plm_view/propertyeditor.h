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
 * $Source: /data/cvs/geoaidalib/geoaida/plm_view/propertyeditor.h,v $
 * $Revision: 1.1 $
 * $Date: 2002/06/05 09:22:21 $
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
Q_OBJECT
public:
  PropertyEditor(QWidget * parent = 0, const char *name = 0);
   ~PropertyEditor();
  public slots:                 // Public slots
  /** Set the node to be edited */
  void setNode(TreeItem * listViewItem, GNode * node);
  void updateNode();
  /** Slot is called whenever an item is selected */
  void itemSelected(QListViewItem *);

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
