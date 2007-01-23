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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGui.cpp,v $
 * $Revision: 1.4 $
 * $Date: 2002/11/06 06:49:27 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGui.h"
#include "QtGuiPrivate.h"
#include <stdlib.h>

QtGui::QtGui()
{
  gdata_=0;
  //  gdata_=new QtGuiPrivate();
}

QtGui::QtGui(MLParser*, ArgDict*)
{
  QtGui();
}

QtGui::~QtGui()
{
  qDebug("QtGui::~QtGui should be overloaded\n");
}

QWidget* QtGui::widget(QWidget*)
{
  qDebug("QtGui::widget should be overloaded\n");
  return gdata_->widget_;
}

QWidget *QtGui::create(QWidget*,QLayout*)
{
  qDebug("QtGui::create should be overloaded\n");
  return 0;
}

void QtGui::connectObject()
{
  if (!gdata_) {
    qDebug("QtGui::show called uninitialized\n");
    return;
  }
  qDebug("QtGui::connectObject\n");
  gdata_->connect(this,"valueChanged",this,"valueSet");
#if 0
  if (gdata_->widget_) {
    gdata_->connectSlot(this,SLOT(enable(bool)),
			gdata_->enableslot_);
    gdata_->connectSlot(this,SLOT(disable(bool)),
			gdata_->disableslot_);
    gdata_->connectSlot(this,SLOT(hide(bool)),
			gdata_->hideslot_);
    gdata_->connectSlot(this,SLOT(show(bool)),
			gdata_->showslot_);
  }
#endif
}


void QtGui::show(bool state)
{
  if (state) show();
  else hide();
}

void QtGui::hide(bool state)
{
  if (state) hide();
  else show();
}

void QtGui::show()
{
  qDebug("QtGui::show\n");
  if (gdata_)
    if (gdata_->widget_) gdata_->widget_->show();
    else qDebug("QtGui::show (%s) called uninitialized\n",
		(const char*)gdata_->name_);
  else qDebug("QtGui::show called uninitialized\n");
}

void QtGui::hide()
{
  if (gdata_) {
    if (gdata_->widget_) gdata_->widget_->hide();
  }
  else
    qDebug("QtGui::hide called uninitialized\n");
}

void QtGui::enable(bool state)
{
  qDebug("QtGui::enable(%d)\n",state);
  if (state) enable();
  else disable();
}

void QtGui::disable(bool state)
{
  qDebug("QtGui::disable(%d)\n",state);
  if (state) disable();
  else enable();
}

void QtGui::enable()
{
  qDebug("QtGui::enable\n");
  if (gdata_) {
    if (gdata_->widget_) gdata_->widget_->setEnabled(true);
  }
  else
    qDebug("QtGui::enable called uninitialized\n");
}

void QtGui::disable()
{
  qDebug("QtGui::disable\n");
  if (gdata_) {
    if (gdata_->widget_) gdata_->widget_->setEnabled(false);
  }
  else
    qDebug("QtGui::enable called uninitialized\n");
}

const QString& QtGui::name()
{
  if (gdata_) return gdata_->name_;
  qDebug("QtGui::name class not initialized\n");
#ifndef WIN32
  exit(0);
#endif
  
}

void QtGui::valueSet(bool val)
{
  valueSet((val ? "true" : "false"));
  qDebug("QtGui::valueSet(int val=%d) should be overloaded\n",val);
}

void QtGui::valueSet(int val)
{
  valueSet(QString().sprintf("%d",val));
  qDebug("QtGui::valueSet(int val=%d) should be overloaded\n",val);
}

void QtGui::valueSet(double val)
{
  valueSet(QString().sprintf("%f",val));
  qDebug("QtGui::valueSet(double val=%f) should be overloaded\n",val);
}

void QtGui::valueSet(QString val)
{
  qDebug("QtGui::valueSet(QString val=%s) should be overloaded\n",
	 (const char*)val);
}

void QtGui::valueSet()
{
  qDebug("QtGui::valueSet()\n");
  valueSet(QString(""));
}

bool QtGui::valueGetBool()
{
  qDebug("QtGuiDialogItem::valueGetBool should be overloaded\n");
  return false;
}

int QtGui::valueGetInt()
{
  qDebug("QtGuiDialogItem::valueGetInt should be overloaded\n");
  return 0;
}

double QtGui::valueGetDouble()
{
  qDebug("QtGuiDialogItem::valueGetInt should be overloaded\n");
  return 0;
}

QString QtGui::valueGetStr()
{
  qDebug("QtGuiDialogItem::valueGetInt should be overloaded\n");
  return QString("");
}


void QtGui::variableSet()
{
  qDebug("QtGui::variableSet() should be overloaded\n");
}

void QtGui::variableGet()
{
  qDebug("QtGui::variableGet() should be overloaded\n");
}

void QtGui::stateChanged()
{
  qDebug("QtGui:stateChanged should be overloaded\n");
}


#ifndef TMAKECONF
#include "QtGui.moc"
#endif
