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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiToolBar.cpp,v $
 * $Revision: 1.1 $
 * $Date: 2001/07/31 11:57:04 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiToolBar.h"
#include "QtGuiPrivate.h"
#include "QtGuiDialogItem.h"
#include <qlist.h>
#include <qtoolbar.h>

class QtGuiToolBarPrivate : public QtGuiPrivate {
public:
  QtGuiToolBarPrivate() {
    item_=new QList<QtGuiDialogItem>;
    item_->setAutoDelete(true);
    name_="toolbar";
  }
  void create(QWidget *parent);
  QList<QtGuiDialogItem> *item_;
  QToolBar *toolbar_;
};

/*****************************************************************
CLASS: QtGuiToolBar
*****************************************************************/
QtGuiToolBar *newQtGuiToolBar(MLParser *parser, ArgDict* argdict)
{
  return new QtGuiToolBar(parser,argdict);
}

QtGuiToolBar::QtGuiToolBar(MLParser *parser, ArgDict* argdict)
{
  qDebug("QtGuiToolBar::QtGuiToolBar\n");
  int tag;
  QtGuiDialogItem *item;

  data_=new QtGuiToolBarPrivate;
  gdata_=data_;
  if (!parser) return;


  // Evaluate arguments
  data_->argdict_=(argdict?argdict:new ArgDict());
  data_->argdict_->insert("PARENT",new QString("TOOLBAR"));
  data_->argdict_=parser->args(data_->argdict_);
  data_->setVars("TOOLBAR");
  data_->regObj(this);

  // Read Tags
  do {
    tag=parser->tag(*_qtguitagtable);
    switch (tag) {
    case -TAG_TOOLBAR:
      break;
    case MLParser::END_OF_FILE:
      break;
    default:
      item=(QtGuiDialogItem*)QtGuiNewObjectId(tag,parser);
      data_->item_->append(item);
      break;
    }
      
  } while (tag!=-TAG_TOOLBAR);
}

QtGuiToolBar::~QtGuiToolBar()
{
}

QWidget *QtGuiToolBar::create(QWidget* parent, QLayout*)
{
  qDebug("QtGuiToolBar::create\n");
  data_->create(parent);
  delete data_->argdict_;
  data_->argdict_=0;
  return data_->widget_;
}

void QtGuiToolBar::connectObject()
{
  QTGUI_DO_ITEMS(QtGuiDialogItem,data_->item_,connectObject())
}

void QtGuiToolBar::show()
{
  qDebug("QtGuiToolBar::show %s\n",(const char*)data_->name_);
  QtGuiDialogItem *item;
  item=data_->item_->first();
  while (item) {
    item->variableGet();
    item=data_->item_->next();
  }
  
  data_->widget_->show();
  qDebug("QtGuiToolBar::show %s done\n",(const char*)data_->name_);
}

void QtGuiToolBar::hide()
{
}


/*****************************************************************
CLASS: QtGuiToolBarPrivate
*****************************************************************/
void QtGuiToolBarPrivate::create(QWidget *parent)
{
  if (parent->inherits("QMainWindow"))
    widget_=toolbar_=new QToolBar((QMainWindow*)parent,name_);
  else
    widget_=new QWidget();
  setAttribs();
  QTGUI_DO_ITEMS(QtGuiDialogItem,item_,create(widget_))
}
