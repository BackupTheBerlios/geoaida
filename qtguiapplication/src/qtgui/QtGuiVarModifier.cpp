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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiVarModifier.cpp,v $
 * $Revision: 1.1 $
 * $Date: 2001/07/31 11:57:04 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiVarModifier.h"
#include <qtextstream.h>


void QtGuiVarModifier::valueSetDouble(double val)
{
  qDebug("QtGuiVarModifier::ValueSetDouble(%f)\n",val);
  if (!var_void_) return;
  switch (type_) {
  case DOUBLE:
    *var_double_=val;
    break;
  case INT:
    *var_int_=int(val);
    break;
  case STRING:
    var_string_->sprintf("%f",val);
    break;
  }
}

void QtGuiVarModifier::valueSetInt(int val)
{
  qDebug("QtGuiVarModifier::ValueSetInt(%d)\n",val);
  if (!var_void_) return;
  switch (type_) {
  case DOUBLE:
    *var_double_=double(val);
    break;
  case INT:
    *var_int_=val;
    break;
  case STRING:
    var_string_->sprintf("%d",val);
    break;
  }
}

void QtGuiVarModifier::valueSetString(QString val)
{
  qDebug("QtGuiVarModifier::ValueSetString(%s)\n",
	 (const char*)val);
  if (!var_void_) return;
  switch (type_) {
  case DOUBLE:
    QTextIStream(&val) >> *var_double_;
    break;
  case INT:
    QTextIStream(&val) >> *var_int_;
    break;
  case STRING:
    *var_string_=val;
    break;
  }
}

QString QtGuiVarModifier::valueGetString()
{
  qDebug("QtGuiVarModifier::ValueGetString\n");
  QString str="";
  if (!var_void_) return str;
  switch (type_) {
  case DOUBLE:
    str.setNum(*var_double_,'f',frac_);
    break;
  case INT:
    str.sprintf("%d",*var_int_);
    break;
  case STRING:
    str=*var_string_;
    break;
  default:
    str="";
  }
  return str;
}

int QtGuiVarModifier::valueGetInt()
{
  qDebug("QtGuiVarModifier::ValueGetInt\n");
  int val=0;
  if (!var_void_) return val;
  switch (type_) {
  case DOUBLE:
    val=int(*var_double_);
    break;
  case INT:
    val=*var_int_;
    break;
  case STRING:
    QTextIStream(var_string_) >> val;
    break;
  default:
    val=0;
  }
  return val;
}

double QtGuiVarModifier::valueGetDouble()
{
  qDebug("QtGuiVarModifier::ValueGetDouble\n");
  double val=0;
  if (!var_void_) return val;
  switch (type_) {
  case DOUBLE:
    val=*var_double_;
    break;
  case INT:
    val=double(*var_int_);
    break;
  case STRING:
    QTextIStream(var_string_) >> val;
    break;
  default:
    val=0;
  }
  return val;
}


