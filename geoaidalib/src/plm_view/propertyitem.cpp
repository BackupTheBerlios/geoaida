/***************************************************************************
                          propertyitem.cpp  -  description
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
 * $Source: /data/cvs/geoaidalib/geoaida/plm_view/propertyitem.cpp,v $
 * $Revision: 1.1 $
 * $Date: 2002/06/05 09:22:21 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "propertyitem.h"
#include "propertyeditor.h"
#include "qheader.h"
#include "qcombobox.h"
#include "qcheckbox.h"
#include "qlineedit.h"
#include "qtooltip.h"
#include "qcolordialog.h"
#include <qtextedit.h>
#include <qregexp.h>
#include "QtGuiApplication.h"
// #define DEBUGMSG

PropertyItem::PropertyItem(GNode * node, PropertyEditor * parent,
                           QString name, QString val, bool readOnly)
:QListViewItem(parent, name, val)
{
  node_ = node;
  name_ = name;
  value_ = val;
  parent_ = parent;
}

PropertyItem::PropertyItem(GNode * node, PropertyItem * parent, QString name,
                           QString val, bool readOnly)
:QListViewItem(parent, name, val)
{
#ifdef DEBUGMSG
  qDebug("PropertyItem::PropertyItem(name=%s)", name.latin1());
#endif
  node_ = node;
  name_ = name;
  value_ = val;
  parent_ = (PropertyEditor *) (this->listView());
}

PropertyItem::~PropertyItem()
{
}


/** add a tooltip to this item */
void
PropertyItem::addToolTip()
{
#if 0
  QRect r = parent_->itemRect(this);
  int height = r.height();
  r.setY(r.y() + height);       //parent_->columnWidth(0));
  r.setHeight(height);
#ifdef DEBUGMSG
  qDebug("Rectangle=(%d %d / %d %d )\n", r.x(), r.y(), r.width(), r.height());
#endif
      QToolTip::add(parent_, r, attrib_->tip());
//      else
//              QToolTip::add(parent_,r,"Keine <b>Hilfe</b>");
#endif
}
