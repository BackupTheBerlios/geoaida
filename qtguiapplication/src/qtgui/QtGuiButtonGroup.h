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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiButtonGroup.h,v $
 * $Revision: 1.1 $
 * $Date: 2001/07/31 11:57:04 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef _QtGuiButtonGroup_h
#define _QtGuiButtonGroup_h
#include "QtGuiDialogItemTypeInt.h"

class QtGuiButtonGroupPrivate;

class QtGuiButtonGroup : public QtGuiDialogItemTypeInt {
  Q_OBJECT
 public:
  QtGuiButtonGroup(MLParser *parser=0, ArgDict* argdict=0);
  ~QtGuiButtonGroup();
  virtual QWidget *create(QWidget *parent=0, QLayout *layout=0);
  virtual void connectObject();
//  virtual void show();
//  virtual void hide();
//  virtual QString &name();
  virtual void variableSet();
  virtual void variableGet();
  virtual int valueGetInt();
 public slots:
  virtual void valueSet(int);
  virtual void enable();
  virtual void enable(int);
  virtual void disable();
 private:  
  QtGuiButtonGroupPrivate* data_;
};

QtGuiButtonGroup *newQtGuiButtonGroup(MLParser *parser, ArgDict* argdict=0);

#endif
