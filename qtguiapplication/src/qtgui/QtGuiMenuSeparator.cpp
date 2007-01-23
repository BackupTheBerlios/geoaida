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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiMenuSeparator.cpp,v $
 * $Revision: 1.1 $
 * $Date: 2001/07/31 11:57:04 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiMenuSeparator.h"
#include "QtGuiPrivate.h"
#include <qlist.h>
#include <qmenubar.h>
#include <qtoolbar.h>

class QtGuiMenuSeparatorPrivate : public QtGuiPrivate {
public:
  QtGuiMenuSeparatorPrivate() {
    name_="menuseparator";
  }
    
  void create(QtGuiMenuSeparator *pub_data, QWidget *parent);
  QWidget *widget_;
};

/*****************************************************************
CLASS: QtGuiMenuSeparator
*****************************************************************/
QtGuiMenuSeparator *newQtGuiMenuSeparator(MLParser *parser, ArgDict* argdict)
{
  return new QtGuiMenuSeparator(parser,argdict);
}

QtGuiMenuSeparator::QtGuiMenuSeparator(MLParser *parser, ArgDict* argdict)
{
  qDebug("QtGuiMenu::QtGuiMenuSeparator\n");

  gdata_=data_=new QtGuiMenuSeparatorPrivate;
  if (!parser) return;

  // Evaluate arguments
  data_->argdict_=parser->args(argdict);
}

QtGuiMenuSeparator::~QtGuiMenuSeparator()
{
}

QWidget *QtGuiMenuSeparator::create(QWidget *parent, QLayout*)
{
  qDebug("QtGuiMenuSeparator::create %s\n",(const char*)data_->label_);;
  data_->create(this,parent);
  delete data_->argdict_;
  data_->argdict_=0;
  return data_->widget_;
}

void QtGuiMenuSeparator::open()
{
}

void QtGuiMenuSeparator::close()
{
}


/*****************************************************************
CLASS: QtGuiMenuSeparatorPrivate
*****************************************************************/
void QtGuiMenuSeparatorPrivate::create(QtGuiMenuSeparator *pub_data,
				       QWidget *parent)
{
  widget_=parent;
  
  if (parent->inherits("QPopupMenu")) {
    qDebug("QtGuiMenuSeparator::create: parent is QPopupMenu\n");
    ((QPopupMenu*)parent)->insertSeparator();
  }
  
  if (parent->inherits("QMenuBar")) {
    qDebug("QtGuiMenuSeparator::create: parent is QMenuBar\n");
    ((QMenuBar*)parent)->insertSeparator();
  }
  
  if (parent->inherits("QToolBar")) {
    qDebug("QtGuiMenuSeparator::create: parent is QToolBar\n");
    ((QToolBar*)parent)->addSeparator();
  }

}

#ifndef TMAKECONF
#include "QtGuiMenuSeparator.moc"
#endif
