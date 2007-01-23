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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiButton.h,v $
 * $Revision: 1.2 $
 * $Date: 2002/02/22 14:34:21 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef _QtGuiButton_h
#define _QtGuiButton_h
#include "QtGui.h"
#include "QtGuiDialogItem.h"

class QtGuiButtonPrivate;

class QtGuiButton : public QtGuiDialogItem {
  Q_OBJECT
 public:
  QtGuiButton(MLParser *parser=0, ArgDict* argdict=0);
  ~QtGuiButton();
  virtual QWidget *create(QWidget *parent=0, QLayout *layout=0);
//  virtual void connectObject();
//  virtual void open();
//  virtual void close();
  virtual bool valueGetBool();
  virtual int valueGetInt();
 signals:
  void valueChanged();
  void valueChanged(bool);
  void valueChanged(int);
  void valueChanged(int,bool);
 public slots:
  virtual void valueSet(bool);
  virtual void valueSet(int);
  virtual void valueSet(QString);
  void valueSetBitMask(int);
  void toggle();
  void down();
  void up();
 protected slots:
   virtual void stateChanged();
 private:  
   QtGuiButtonPrivate* data_;
};

QtGuiButton *newQtGuiButton(MLParser *parser, ArgDict* argdict=0);

#endif
