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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiObjectTable.cpp,v $
 * $Revision: 1.3 $
 * $Date: 2001/12/20 13:40:20 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGui.h"
#include "QtGuiMenu.h"
#include "QtGuiSubMenu.h"
#include "QtGuiMenuItem.h"
#include "QtGuiMenuSeparator.h"
#include "QtGuiToolBar.h"
// #include "QtGuiToolButton.h"
#include "QtGuiCentralWidget.h"
#include "QtGuiDockWindow.h"
#include "QtGuiStatusBar.h"
#include "QtGuiDialog.h"
#include "QtGuiScrollView.h"
#include "QtGuiTabView.h"
#include "QtGuiLayout.h"
#include "QtGuiLineEdit.h"
#include "QtGuiSlider.h"
#include "QtGuiLabel.h"
#include "QtGuiWidget.h"
#include "QtGuiButton.h"
#include "QtGuiButtonGroup.h"
#include "QtGuiCheckBox.h"
#include "QtGuiRadioButton.h"
#include "QtGuiComboBox.h"
#include "QtGuiBrowser.h"

static QDict<void> _qtguitable(17,false);

static struct QtGuiFuncTable {
  QtGuiFuncTable(const QString &n, QtGuiNewObj* f) {name=n; func=f;}
  QString name;
  QtGuiNewObj* func;
} _qtguifunctable[] =
{
  QtGuiFuncTable("APPLICATION", (QtGuiNewObj*)0),
  QtGuiFuncTable("MENU",         (QtGuiNewObj*)newQtGuiMenu),
    QtGuiFuncTable("SUBMENU",    (QtGuiNewObj*)newQtGuiSubMenu),
      QtGuiFuncTable("ITEM",     (QtGuiNewObj*)newQtGuiMenuItem),
      QtGuiFuncTable("SEPARATOR",(QtGuiNewObj*)newQtGuiMenuSeparator),
  QtGuiFuncTable("TOOLBAR",(QtGuiNewObj*)newQtGuiToolBar),
  //    QtGuiFuncTable("TOOLBUTTON",(QtGuiNewObj*)newQtGuiToolButton),
  QtGuiFuncTable("CENTRALWIDGET",(QtGuiNewObj*)newQtGuiCentralWidget),
  QtGuiFuncTable("DOCKWINDOW",(QtGuiNewObj*)newQtGuiDockWindow),
  QtGuiFuncTable("STATUSBAR",(QtGuiNewObj*)newQtGuiStatusBar),
  QtGuiFuncTable("DIALOG",(QtGuiNewObj*)newQtGuiDialog),
    QtGuiFuncTable("SCROLLVIEW",(QtGuiNewObj*)newQtGuiScrollView),
    QtGuiFuncTable("TABVIEW",(QtGuiNewObj*)newQtGuiTabView),
      QtGuiFuncTable("LAYOUT",(QtGuiNewObj*)newQtGuiLayout),
        QtGuiFuncTable("INPUT",(QtGuiNewObj*)newQtGuiLineEdit),
        QtGuiFuncTable("SLIDER",(QtGuiNewObj*)newQtGuiSlider),
        QtGuiFuncTable("LABEL",(QtGuiNewObj*)newQtGuiLabel),
        QtGuiFuncTable("WIDGET",(QtGuiNewObj*)newQtGuiWidget),
        QtGuiFuncTable("BUTTON",(QtGuiNewObj*)newQtGuiButton),
        QtGuiFuncTable("BUTTONGROUP",(QtGuiNewObj*)newQtGuiButtonGroup),
          QtGuiFuncTable("CHECKBOX",(QtGuiNewObj*)newQtGuiCheckBox),
          QtGuiFuncTable("RADIOBUTTON",(QtGuiNewObj*)newQtGuiRadioButton),
        QtGuiFuncTable("COMBOBOX",(QtGuiNewObj*)newQtGuiComboBox),
        QtGuiFuncTable("BROWSER",(QtGuiNewObj*)newQtGuiBrowser),
  QtGuiFuncTable("",(QtGuiNewObj*)0)
};


MLTagTable *_qtguitagtable;

void QtGuiInitObjectTable()
{
  int size;
  int i;
  for (size=0; !_qtguifunctable[size].name.isEmpty(); size++);
  QString *keyword=new QString[size+1];
  for (i=0;i<size; i++) {
    keyword[i]=_qtguifunctable[i].name;
    qDebug("QtGuiInitObjectTable %s\n",(const char*)_qtguifunctable[i].name);
    if ((void*)_qtguifunctable[i].func)
      _qtguitable.insert(_qtguifunctable[i].name,
			 (void*)_qtguifunctable[i].func);
  }
  keyword[size]="";
  
  _qtguitagtable=new MLTagTable(keyword);
}

QtGui* QtGuiNewObject(QString &name, MLParser *parser, ArgDict* argdict)
{
  QtGuiNewObj *func;
  func=(QtGuiNewObj*)_qtguitable[name];
  if (func) return (QtGui*)(*func)(parser,argdict);
  delete parser->args();
  return 0;
}

QtGui* QtGuiNewObjectId(int tagid, MLParser *parser, ArgDict* argdict)
{
  QtGuiNewObj *func;
  if (tagid>0) {
    qDebug("QtGuiNewObjectId %d %s\n",
	   tagid,(const char*)_qtguifunctable[tagid-1].name);
    func=(QtGuiNewObj*)_qtguifunctable[tagid-1].func;
    if (func) return (QtGui*)(*func)(parser,argdict);
  }
  delete parser->args();
  return 0;
}

