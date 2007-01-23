// QtGui - Library 
// Copyright (C) 1999 Martin Pahl, Universität Hannover
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free
// Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

/*
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiMenuItem.cpp,v $
 * $Revision: 1.1 $
 * $Date: 2001/07/31 11:57:04 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiMenuItem.h"
#include "QtGuiMenuItem.h"
#include "QtGuiPrivate.h"
#include <qlist.h>
#include <qmenubar.h>

class QtGuiMenuItemPrivate : public QtGuiPrivate {
public:
  QtGuiMenuItemPrivate() {
    menuitem_=new QList<QtGui>;
    name_="menuitem";
    label_="menuitem";
    accel_=0;
    id_=-1;
    offid_=-1;
    popup_=0;
    //    menubar_=0;
  }
  void setVars(QString classname) {
    QtGuiPrivate::setVars(classname);
    setAccel(accel_,argdict_,"accel");
    qDebug("setAccel: %08x(%d)\n",accel_,accel_);
    setInt(id_,argdict_,"id");
    offid_=id_;
    setInt(offid_,argdict_,"offid");
  }
  void setAttribs() {
    if (accel_) {
      qDebug("QMenuItem(%d): accel=%08x(%d)\n",id_,accel_,accel_);
      ((QPopupMenu*)widget_)->setAccel(accel_,id_);
    }
    if (!tip_.isEmpty())
      ((QPopupMenu*)widget_)->setWhatsThis(id_,tip_);
  }
    
  void create(QtGuiMenuItem *pub_data, QPopupMenu *parent);
  void connectItem(QtGuiMenuItem *pub_data, 
		   QPopupMenu *popup);
  QList<QtGui> *menuitem_;
  QWidget *widget_;
  // Attribute
  int id_;
  int offid_;
  int accel_;
  QPopupMenu *popup_;
};

/*****************************************************************
CLASS: QtGuiMenuItem
*****************************************************************/
QtGuiMenuItem *newQtGuiMenuItem(MLParser *parser, ArgDict* argdict)
{
  return new QtGuiMenuItem(parser,argdict);
}

QtGuiMenuItem::QtGuiMenuItem(MLParser *parser, ArgDict* argdict)
{
  qDebug("QtGuiMenu::QtGuiMenuItem\n");

  gdata_=data_=new QtGuiMenuItemPrivate;
  if (!parser) return;

  // Evaluate arguments
  data_->argdict_=parser->args(argdict);
  data_->setVars("ITEM");
  qDebug("register object\n");
  data_->regObj(this);
}

QtGuiMenuItem::~QtGuiMenuItem()
{
}

QWidget *QtGuiMenuItem::create(QWidget *parent, QLayout*)
{
  qDebug("QtGuiMenuItem::create %s\n",(const char*)data_->label_);;
  data_->create(this,(QPopupMenu*)parent);
  if (data_->isSet(*(data_->argdict_),"val")) 
    valueSet(data_->getBool(*(data_->argdict_),"val"));
  delete data_->argdict_;
  data_->argdict_=0;
  return data_->widget_;
}

void QtGuiMenuItem::connectObject()
{
}

void QtGuiMenuItem::open()
{
}

void QtGuiMenuItem::close()
{
}

void QtGuiMenuItem::enable()
{
  qDebug("QtGuiMenuItem::enable\n");
  data_->popup_->setItemEnabled(data_->id_,true);
}

void QtGuiMenuItem::disable()
{
  qDebug("QtGuiMenuItem::disable\n");
  data_->popup_->setItemEnabled(data_->id_,false);
}

bool QtGuiMenuItem::valueGetBool()
{
  return data_->popup_->isItemChecked(data_->id_);
}

int QtGuiMenuItem::valueGetInt()
{
  return (valueGetBool() ? data_->id_ : data_->offid_);
}

void QtGuiMenuItem::valueSet(bool state)
{
  qDebug("QtGuiMenuItem::valueSet (%d)\n",state);
  data_->popup_->setItemChecked(data_->id_,state);
}

void QtGuiMenuItem::valueSet(int val)
{
  valueSet((val&data_->id_)!=0);
}

void QtGuiMenuItem::valueSet(int id, bool state)
{
  qDebug("QtGuiMenuItem::valueSet (%d,%d)\n",id,state);
  if (data_->id_ != id) valueSet(false);
  else valueSet(state);
}

void QtGuiMenuItem::valueSetBitMask(int mask)
{
  qDebug("QtGuiMenuItem::valueSetBitMask(%d)\n",mask);
  if ((data_->id_&mask)==data_->id_) valueSet(true);
  else valueSet(false);
}


void QtGuiMenuItem::stateChanged()
{
  qDebug("QtGuiMenuItem::stateChanged name=%s\n",
	 (const char*)data_->name_);
  emit valueChanged(data_->id_,!valueGetBool());
  emit valueChanged(valueGetInt());
  emit valueChanged(!valueGetBool());
  emit valueChanged();
}

/*****************************************************************
CLASS: QtGuiMenuItemPrivate
*****************************************************************/
void QtGuiMenuItemPrivate::create(QtGuiMenuItem *pub_data, QPopupMenu *parent)
{
  widget_=popup_=parent;
  id_= parent->insertItem(label_,id_);
  setAttribs();
  connectItem(pub_data,parent);
}

void QtGuiMenuItemPrivate::connectItem(QtGuiMenuItem *pub_data,
				       QPopupMenu *popup) 
{
  popup->connectItem(id_,pub_data,SLOT(stateChanged()));
  connect(pub_data,"valueChanged",pub_data,"valueSet");
}

#ifndef TMAKECONF
#include "QtGuiMenuItem.moc"
#endif
