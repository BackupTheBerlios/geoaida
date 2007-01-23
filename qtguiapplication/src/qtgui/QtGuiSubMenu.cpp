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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiSubMenu.cpp,v $
 * $Revision: 1.1 $
 * $Date: 2001/07/31 11:57:04 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiSubMenu.h"
#include "QtGuiMenuItem.h"
#include "QtGuiMenuSeparator.h"
#include "QtGuiPrivate.h"
#include <qlist.h>
#include <qmenubar.h>
#include <qpopupmenu.h>

class QtGuiSubMenuPrivate : public QtGuiPrivate {
public:
  QtGuiSubMenuPrivate() {
    menuitem_=new QList<QtGui>;
    name_="submenu";
    label_="submenu";
    id_=0;
    accel_=0;
  }
  void setVars(QString classname) {
    QtGuiPrivate::setVars(classname);
    setAccel(accel_,argdict_,"accel");
  }
  void setAttribs(QMenuData*) {
    /* Does not work on SubMenus
    if (accel_)
      ((QPopupMenu*)parent)->setAccel(accel_,id_);
    */
  }
  QWidget *create(QMenuData *parent);
  QList<QtGui> *menuitem_;
  QPopupMenu *popup_;
  int id_;
  int accel_;
};

/*****************************************************************
CLASS: QtGuiSubMenu
*****************************************************************/
QtGuiSubMenu *newQtGuiSubMenu(MLParser *parser, ArgDict* argdict)
{
  return new QtGuiSubMenu(parser,argdict);
}


QtGuiSubMenu::QtGuiSubMenu(MLParser *parser, ArgDict* argdict)
{
  qDebug("QtGuiMenu::QtGuiSubMenu\n");
  int tag;
  QtGuiSubMenu *submenu;
  QtGuiMenuItem *menuitem;
  QtGuiMenuSeparator *separator;

  enum {
    TAG_SUBMENU=1,
    TAG_MENUITEM,
    TAG_SEPARATOR
  };
  QString keyword[]={"SUBMENU","ITEM","SEPARATOR",""};
  MLTagTable tagtable(keyword);

  data_=new QtGuiSubMenuPrivate;
  if (!parser) return;

  // Evaluate arguments
  data_->argdict_=parser->args(argdict);
  data_->setVars("SUBMENU");

  // Read Tags
  do {
    tag=parser->tag(tagtable);
    switch (tag) {
    case TAG_SUBMENU:
      submenu=new QtGuiSubMenu(parser);
      data_->menuitem_->append(submenu);
      break;
    case -TAG_SUBMENU:
      break;
    case TAG_MENUITEM:
      menuitem=new QtGuiMenuItem(parser);
      data_->menuitem_->append(menuitem);
      break;
    case TAG_SEPARATOR:
      separator=new QtGuiMenuSeparator(parser);
      data_->menuitem_->append(separator);
      break;
    case -TAG_MENUITEM:
      break;
    case MLParser::END_OF_FILE:
      break;
    default:
      qDebug("QtGuiSubMenu: tag %s unknown",
	     (tag==0 ? "UNKNOWN" : (const char*)keyword[tag-1]));
      delete parser->args();
    }
      
  } while ((tag!=-TAG_SUBMENU) &&(tag!=MLParser::END_OF_FILE));
}

QtGuiSubMenu::~QtGuiSubMenu()
{
}

QWidget *QtGuiSubMenu::create(QWidget* parent, QLayout*)
{
  qDebug("QtGuiSubMenu::create %d %d %d\n",
	 parent->isA("QPopupMenu"),
	 parent->isA("QMenuBar"),
	 parent->isA("QWidget"));
  if (parent->isA("QPopupMenu"))
    data_->create((QPopupMenu*)parent);
  else
    data_->create((QMenuBar*)parent);
  delete data_->argdict_;
  data_->argdict_=0;

  return data_->popup_;
}

void QtGuiSubMenu::connectObject()
{
  QTGUI_DO_ITEMS(QtGui,data_->menuitem_,connectObject())
  
}

void QtGuiSubMenu::open()
{
}

void QtGuiSubMenu::close()
{
}

/*****************************************************************
CLASS: QtGuiSubMenuPrivate
*****************************************************************/
QWidget *QtGuiSubMenuPrivate::create(QMenuData* parent)
{
  qDebug("QtGuiSubMenu::create\n");
  widget_=popup_=new QPopupMenu;
  id_= parent->insertItem(label_,popup_);
  setAttribs(parent);
  QtGui *menuitem;
  menuitem=menuitem_->first();
  while (menuitem) {
    menuitem->create(popup_);
    menuitem=menuitem_->next();
  }
  return popup_;
}
