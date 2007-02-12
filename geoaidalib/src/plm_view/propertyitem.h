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
 * $Source: /data/cvs/geoaidalib/geoaida/plm_view/propertyitem.h,v $
 * $Revision: 1.1 $
 * $Date: 2002/06/05 09:22:21 $
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
Q_OBJECT
public:
  PropertyItem(GNode * node, PropertyEditor * parent, QString name,
               QString val, bool readOnly = false);
  PropertyItem(GNode * node, PropertyItem * parent, QString name, QString val,
               bool readOnly = false);
  ~PropertyItem();
  /** add a tooltip to this item */
  void addToolTip();
public slots:                 // Public slots
protected:                     // Protected attributes
  /** Name of the property (attribute) to edit */
    QString name_;
  /** node_ to edit */
  GNode *node_;
  /** value of  the attribute name_ */
  QString value_;
  /** parent of this item */
  PropertyEditor *parent_;
signals:                    // Signals
  /** Request an update of the attribute list */
  void requestUpdate();
};

#endif
