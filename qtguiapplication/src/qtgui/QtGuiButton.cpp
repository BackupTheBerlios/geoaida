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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiButton.cpp,v $
 * $Revision: 1.2 $
 * $Date: 2002/02/22 14:34:21 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiButton.h"
#include "QtGuiDialogItemPrivate.h"
#include <qlist.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qtooltip.h>

// #define DEBUG_MSG

class QtGuiButtonPrivate : public QtGuiDialogItemPrivate {
public:
  QtGuiButtonPrivate() {
    togglemode_=false;
    button_=0;
    id_=0;
    offid_=0;
  }
  void setVars(QString classname) {
    QtGuiDialogItemPrivate::setVars(classname);
    setBool(togglemode_,argdict_,"toggle");
    setInt(id_,argdict_,"id");
    setInt(offid_,argdict_,"offid");
    if (!togglemode_) offid_=id_;
  }

  void setAttribs() {
    QtGuiDialogItemPrivate::setAttribs();
    button_->setAutoResize(true);
    if (toolbutton_)
      ((QToolButton*)button_)->setToggleButton(togglemode_);
    else
      ((QPushButton*)button_)->setToggleButton(togglemode_);
    if (pixmap_) {
#ifdef DEBUG_MSG
      qDebug("QtGuiButton::setAttribs(%s) pixmap\n",(const char*)name_);
#endif
      button_->setPixmap(*pixmap_);
    }
    else {
#ifdef DEBUG_MSG
      qDebug("QtGuiButton::setAttribs(%s) label=%s\n",
	     (const char*)name_,(const char*)label_);
#endif	
      button_->setText(label_);
    }
  }
  void create(QWidget* parent, QLayout* layout);
  QButton *button_;

  // Attribs
  bool togglemode_;
  bool toolbutton_;
  int id_;
  int offid_;
};

/*****************************************************************
CLASS: QtGuiButton
*****************************************************************/
QtGuiButton *newQtGuiButton(MLParser *parser, ArgDict* argdict)
{
  return new QtGuiButton(parser,argdict);
}

QtGuiButton::QtGuiButton(MLParser *parser, ArgDict* argdict)
{
#ifdef DEBUG_MSG
  qDebug("QtGuiButton::QtGuiButton\n");
#endif

  gdata_=idata_=data_=new QtGuiButtonPrivate;
  if (!parser) return;


  // Evaluate arguments
  data_->argdict_=parser->args(argdict);
  data_->setVars("BUTTON");
  data_->regObj(this);
}

QtGuiButton::~QtGuiButton()
{
}

QWidget *QtGuiButton::create(QWidget* parent, QLayout* layout)
{
#ifdef DEBUG_MSG
  qDebug("QtGuiButton::create\n");
#endif
  data_->create(parent,layout);
  if (data_->isSet(*(data_->argdict_),"val")) 
    valueSet(data_->getBool(*(data_->argdict_),"val"));
  connect(data_->button_,SIGNAL(clicked()),this,SLOT(stateChanged()));
  delete data_->argdict_;
  data_->argdict_=0;

  return data_->widget_;
}

void QtGuiButton::valueSet(bool val)
{
#ifdef DEBUG_MSG
  qDebug("QtGuiButton::valueSet(bool val=%d)\n", val);
#endif
  if (data_->toolbutton_)
    ((QToolButton*)data_->button_)->setOn(val);
  else
    ((QPushButton*)data_->button_)->setOn(val);
}

void QtGuiButton::valueSet(int val)
{
  data_->id_=val;
}

void QtGuiButton::valueSet(QString val)
{
  data_->label_=val;
  data_->button_->setText(val);
}

void QtGuiButton::valueSetBitMask(int mask)
{
#ifdef DEBUG_MSG
  qDebug("QtGuiButton::valueSetBitMask(%d)\n",mask);
#endif
  if ((data_->id_&mask)==data_->id_) valueSet(true);
  else valueSet(false);
}


bool QtGuiButton::valueGetBool()
{
  return data_->button_->isOn();
}

int QtGuiButton::valueGetInt()
{
  return (valueGetBool() ? data_->id_ : data_->offid_);
}

void QtGuiButton::toggle()
{
  valueSet(!valueGetBool());
}

void QtGuiButton::up()
{
  valueSet(false);
}

void QtGuiButton::down()
{
  valueSet(true);
}

void QtGuiButton::stateChanged()
{
  if (idata_->sending_) {
#ifdef DEBUG_MSG
    qDebug("QtGuiButton::stateChanged name=%s recursion detected\n",
	 (const char*)idata_->name_);
#endif	
    return;
  }
  idata_->sending_=true;
  bool val=valueGetBool();
#ifdef DEBUG_MSG
  qDebug("QtGuiButton::stateChanged() name=%s id=%d\n",
	 (const char*)idata_->name_,data_->id_);
#endif	
  if (idata_->variable_ && idata_->immediately_) {
    idata_->variable_->valueSetInt(val);
  }
  emit valueChanged(valueGetInt());
  emit valueChanged(valueGetBool());
  emit valueChanged(valueGetInt(),valueGetBool());
  emit valueChanged();
  idata_->sending_=false;
}

/*
void QtGuiButton::show()
{
  qDebug("QtGuiButton::show %s\n",(const char*)data_->name_);
  data_->widget_->show();
  qDebug("QtGuiButton::show %s done\n",(const char*)data_->name_);
}

void QtGuiButton::hide()
{
}
*/

/*****************************************************************
CLASS: QtGuiButtonPrivate
*****************************************************************/
void QtGuiButtonPrivate::create(QWidget *parent, QLayout *layout)
{
  if (parent->inherits("QToolBar")) {
    toolbutton_=true;
#ifdef DEBUG_MSG
    qDebug("QtGuiButton::create: %s is toolbutton\n",(const char*)name_);
#endif
    widget_=button_=new QToolButton(parent,name_);
  } 
  else {
    toolbutton_=false;
#ifdef DEBUG_MSG
    qDebug("QtGuiButton::create: %s is pushbutton\n",(const char*)name_);
#endif
    widget_=button_=new QPushButton(parent,name_);
  }
    
  setAttribs();
  addItem(layout,parent);
}

#ifndef TMAKECONF
#include "QtGuiButton.moc"
#endif
