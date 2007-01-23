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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiMenu.cpp,v $
 * $Revision: 1.1 $
 * $Date: 2001/07/31 11:57:04 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiMenu.h"
#include "QtGuiSubMenu.h"
#include "QtGuiPrivate.h"
#include <qlist.h>
#include <qmenubar.h>

class QtGuiMenuPrivate : public QtGuiPrivate {
public:
  QtGuiMenuPrivate() {
    submenu_=new QList<QtGui>;
    submenu_->setAutoDelete(true);
    name_="mainmenu";
  }
  QList<QtGui> *submenu_;
};

/*****************************************************************
CLASS: QtGuiMenu
*****************************************************************/
QtGuiMenu *newQtGuiMenu(MLParser *parser, ArgDict* argdict)
{
  return new QtGuiMenu(parser,argdict);
}

QtGuiMenu::QtGuiMenu(MLParser *parser, ArgDict* argdict)
{
  qDebug("QtGuiMenu::QtGuiMenu\n");
  int tag;
  QtGui *submenu;
 
  data_=new QtGuiMenuPrivate;
  gdata_=data_;
  if (!parser) return;


  // Evaluate arguments
  data_->argdict_=parser->args(argdict);
  data_->setVars("MENU");
  data_->regObj(this);

  // Read Tags
  do {
    tag=parser->tag(*_qtguitagtable);
    switch (tag) {
    case -TAG_MENU:
      break;
    case MLParser::END_OF_FILE:
      break;
    default:
      submenu=(QtGui*)QtGuiNewObjectId(tag,parser);
      data_->submenu_->append(submenu);
      break;
    }
      
  } while (tag!=-TAG_MENU);
}

QtGuiMenu::~QtGuiMenu()
{
}

QWidget *QtGuiMenu::create(QWidget* parent, QLayout*)
{
  qDebug("QtGuiMenu::create\n");
  data_->widget_=new QMenuBar(parent,data_->name_);
  ((QMenuBar*)data_->widget_)->setSeparator( QMenuBar::InWindowsStyle );
  QTGUI_DO_ITEMS(QtGui,data_->submenu_,create(data_->widget_))
  delete data_->argdict_;
   data_->argdict_=0;

  return data_->widget_;
}

void QtGuiMenu::connectObject()
{
  QTGUI_DO_ITEMS(QtGui,data_->submenu_,connectObject())
  
}
/*
void QtGuiMenu::open()
{
}

void QtGuiMenu::close()
{
}
*/

