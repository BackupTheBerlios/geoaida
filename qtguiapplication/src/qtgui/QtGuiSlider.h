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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiSlider.h,v $
 * $Revision: 1.2 $
 * $Date: 2002/06/10 05:49:56 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef _QtGuiSlider_h
#define _QtGuiSlider_h
#include "QtGui.h"
#include "QtGuiDialogItem.h"

class QtGuiSliderPrivate;

class QtGuiSlider : public QtGuiDialogItem {
  Q_OBJECT
 public:
  QtGuiSlider(MLParser *parser=0, ArgDict* argdict=0);
  ~QtGuiSlider();
  virtual QWidget *create(QWidget *parent=0, QLayout *layout=0);
//  virtual void connectObject();
  virtual void variableSet();
  virtual void variableGet();
  virtual int valueGetInt();
  virtual double valueGetDouble();
  virtual QString valueGetStr();
 public slots:
  virtual void valueSet(int);
  virtual void valueSet(double);
  virtual void valueSet(QString);
 protected slots:
  virtual void stateChanged();
 private:  
  QtGuiSliderPrivate* data_;
public slots: // Public slots
  /** set the min and max value for this slider */
  virtual void setMinMax(int min, int max);
};

QtGuiSlider *newQtGuiSlider(MLParser *parser, ArgDict* argdict=0);

#endif
