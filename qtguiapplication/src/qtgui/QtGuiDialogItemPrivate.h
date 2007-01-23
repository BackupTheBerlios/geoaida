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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiDialogItemPrivate.h,v $
 * $Revision: 1.1 $
 * $Date: 2001/07/31 11:57:04 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef _QtGuiDialogItemPrivate_h
#define _QtGuiDialogItemPrivate_h

#include "QtGuiPrivate.h"
#include <qlayout.h>

class QtGuiDialogItemPrivate : public QtGuiPrivate {
public:
  QtGuiDialogItemPrivate() {
    varname_="";
    variable_=0;
  }
  void setVars(QString classname) {
    QtGuiPrivate::setVars(classname);
    setString(varname_,argdict_,"varname");
    if (varname_.isEmpty()) varname_=name_;
  }
  ~QtGuiDialogItemPrivate(){};
  void addItem(QLayout *layout, QWidget* parent=0);
  QString varname_;
  QtGuiVarModifier *variable_;
};


#endif

