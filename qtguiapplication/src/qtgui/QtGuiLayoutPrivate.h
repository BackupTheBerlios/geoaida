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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiLayoutPrivate.h,v $
 * $Revision: 1.1 $
 * $Date: 2001/07/31 11:57:04 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef _QtGuiLayoutPrivate_h
#define _QtGuiLayoutPrivate_h

#include "QtGuiDialogItemPrivate.h"
#include <qlayout.h>

class QtGuiLayoutPrivate : public QtGuiDialogItemPrivate {
public:
  QtGuiLayoutPrivate() {
    layoutstyle_=0;
    rows_=1;
    cols_=1;
    layout_=0;
  }
  ~QtGuiLayoutPrivate(){};
  enum {
    DEFAULT,
    VBOX,
    HBOX,
    GRID,
    TABLE
  };
  void setVars(QString classname) {
    QtGuiDialogItemPrivate::setVars(classname);
    QString options[]={"VBOX","HBOX","GRID","TABLE",""};
    setOption(layoutstyle_,argdict_,"layout",options);
    setInt(rows_,argdict_,"rows");
    setInt(cols_,argdict_,"cols");
  }
  void createLayout(QWidget *parent, QLayout *parentlayout=0);
  QLayout *layout_;
  // Attributes
  int layoutstyle_;
  int rows_, cols_;
};

#endif
