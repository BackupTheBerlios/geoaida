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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiBrowser.h,v $
 * $Revision: 1.1 $
 * $Date: 2001/07/31 11:57:04 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef _QtGuiBrowser_h
#define _QtGuiBrowser_h
#include "QtGui.h"
#include "QtGuiDialogItem.h"

class QtGuiBrowserPrivate;

class QtGuiBrowser : public QtGuiDialogItem {
  Q_OBJECT
 public:
  QtGuiBrowser(MLParser *parser=0, ArgDict* argdict=0);
  ~QtGuiBrowser();
  virtual QWidget *create(QWidget *parent=0, QLayout *layout=0);
//  virtual void connectObject();
  virtual void variableSet();
  virtual void variableGet();
  virtual QString valueGetStr();
 public slots:
  virtual void valueSet(QString);
  virtual void backward();
  virtual void forward();
  virtual void home();
 protected slots:
  virtual void stateChanged();
  
 private:  
  QtGuiBrowserPrivate* data_;
};

QtGuiBrowser *newQtGuiBrowser(MLParser *parser, ArgDict* argdict=0);

#endif
