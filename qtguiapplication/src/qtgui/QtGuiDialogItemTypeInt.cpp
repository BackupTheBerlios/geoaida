// QtGuiDialogItemTypeInt - Library 
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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiDialogItemTypeInt.cpp,v $
 * $Revision: 1.1 $
 * $Date: 2001/07/31 11:57:04 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiDialogItemTypeInt.h"
#include "QtGuiDialogItemPrivate.h"

QtGuiDialogItemTypeInt::QtGuiDialogItemTypeInt()
{
  gdata_=0;
  idata_=0;
  //  gdata_=new QtGuiDialogItemTypeIntPrivate();
}

QtGuiDialogItemTypeInt::~QtGuiDialogItemTypeInt()
{
  qDebug("QtGuiDialogItemTypeInt::~QtGuiDialogItemTypeInt should be overloaded\n");
}


void QtGuiDialogItemTypeInt::valueSet(bool val)
{
  if (val) valueSet(int(1));
  else valueSet(int(0));
}

void QtGuiDialogItemTypeInt::valueSet(int val)
{
  qDebug("QtGuiDialogItemTypeInt::valueSet(int val=%d) should be overloaded\n",
	 val);
}

void QtGuiDialogItemTypeInt::valueSet(double val)
{
  valueSet(int(val));
}

void QtGuiDialogItemTypeInt::valueSet(QString val)
{
  int x;
  QTextIStream(&val) >> x;
  valueSet(x);
}

bool QtGuiDialogItemTypeInt::valueGetBool()
{
  return (valueGetInt()!=0);
}

int QtGuiDialogItemTypeInt::valueGetInt()
{
  qDebug("QtGuiDialogItemTypeIntDialogItem::valueGetInt should be overloaded\n");
  return 0;
}

double QtGuiDialogItemTypeInt::valueGetDouble()
{
  return valueGetInt();
}

QString QtGuiDialogItemTypeInt::valueGetStr()
{
  return QString("").setNum(valueGetInt());
}

void QtGuiDialogItemTypeInt::stateChanged()
{
  if (idata_->sending_) {
    qDebug("QtGuiDialogItemTypeInt::stateChanged name=%s recursion detected\n",
	 (const char*)idata_->name_);
    return;
  }
  idata_->sending_=true;
  int val=valueGetInt();
  qDebug("QtGuiDialogItemTypeInt::stateChanged() name=%s\n",
	 (const char*)idata_->name_);
  if (idata_->variable_ && idata_->immediately_) {
    idata_->variable_->valueSetInt(val);
  }
  emit valueChanged(val);
  emit valueChanged(val!=0);
  emit valueChanged();
  idata_->sending_=false;
}


#ifndef TMAKECONF
#include "QtGuiDialogItemTypeInt.moc"
#endif
