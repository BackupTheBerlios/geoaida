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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGui.h,v $
 * $Revision: 1.4 $
 * $Date: 2002/02/22 14:34:21 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef _QtGui_h
#define _QtGui_h

#include "MLParser.h"
#include "QtGuiVarModifier.h"
#include <qwidget.h>

class QtGuiClassDict : public QDict<QObject> {
 public:
  QtGuiClassDict() : QDict<QObject>(17,TRUE){}
};

/**
  * QtGuiVarDict
  * @short Dictionary for the variables of the programm used in the GUI.
  * @author Martin Pahl
  * @version $Id:
  */
class QtGuiVarDict : public QDict<QtGuiVarModifier> {
 public:
  QtGuiVarDict() : QDict<QtGuiVarModifier>(17,TRUE){}
  void insert(const QString &name, QtGuiVarModifier *item)
    {
      QDict<QtGuiVarModifier>::insert(name, item);
    }
  void insert(const QString &name,
	      void *var_addr, int type, int len=0, int frac=-1)
    {
      insert(name, new QtGuiVarModifier(var_addr,type,len,frac));
    }
};

class QtGuiPrivate;
/**
  * @short Baseclass of all GUI-elements
  * @author Martin Pahl
  * @version $Id:
  */
class QtGui : public QObject {
 Q_OBJECT
 public:
  enum TAGS {
    TAG_APPLICATION=1,
    TAG_MENU,
      TAG_SUBMENU,
        TAG_MENUITEM,
        TAG_ITEM=TAG_MENUITEM,
        TAG_SEPARATOR,
    TAG_TOOLBAR,
//      TAG_TOOLBUTTON,
    TAG_CENTRALWIDGET,
    TAG_DOCKWINDOW,
    TAG_STATUSBAR,
    TAG_DIALOG,
      TAG_SCROLLVIEW,
      TAG_TABVIEW,
        TAG_LAYOUT,
          TAG_INPUT,
          TAG_SLIDER,
          TAG_LABEL,
          TAG_WIDGET,
          TAG_BUTTON,
          TAG_BUTTONGROUP,
            TAG_CHECKBOX,
            TAG_RADIOBUTTON,
          TAG_COMBOBOX,
          TAG_BROWSER,
  };
 /** 
   * Constructor
   */
  QtGui();
  QtGui(MLParser*, ArgDict* argdict=0);
 /** 
   * Destructor
   */
  virtual ~QtGui();
  virtual QWidget *widget(QWidget* w=0);
  virtual const QString& name();
  virtual QWidget *create(QWidget *parent=0, QLayout *layout=0);
  virtual void connectObject();
  virtual void variableSet();
  virtual void variableGet();
  virtual bool valueGetBool();
  virtual int valueGetInt();
  virtual double valueGetDouble();
  virtual QString valueGetStr();
 signals:
  void valueChanged();
  void valueChanged(bool);
  void valueChanged(int);
  void valueChanged(double);
  void valueChanged(QString);
 public slots:
  virtual void show();
  virtual void show(bool state);
  virtual void hide();
  virtual void hide(bool state);
  virtual void enable(void);
  virtual void enable(bool state);
  virtual void disable();
  virtual void disable(bool state);
  virtual void valueSet();
  virtual void valueSet(bool);
  virtual void valueSet(int);
  virtual void valueSet(double);
  virtual void valueSet(QString);
 protected slots:
  virtual void stateChanged();
 protected:
  QtGuiPrivate *gdata_;
};

typedef QtGui* QtGuiNewObj(MLParser*,ArgDict*);

extern MLTagTable *_qtguitagtable;
void QtGuiInitObjectTable();
QtGui* QtGuiNewObject(QString &name, MLParser *parser, ArgDict* argdict=0);
QtGui* QtGuiNewObjectId(int tagid, MLParser *parser, ArgDict* argdict=0);

#endif
