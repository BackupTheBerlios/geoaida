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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiStatusBar.cpp,v $
 * $Revision: 1.1 $
 * $Date: 2001/07/31 11:57:04 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiStatusBar.h"
#include "QtGuiPrivate.h"
#include "QtGuiDialogItem.h"
#include <qapp.h>
#include <qlist.h>
#include <qstatusbar.h>

class QtGuiStatusBarPrivate : public QtGuiPrivate {
public:
  QtGuiStatusBarPrivate() {
    item_=new QList<QtGuiDialogItem>;
    item_->setAutoDelete(true);
    name_="statusbar";
  }
  void create(QWidget *parent);
  QList<QtGuiDialogItem> *item_;
  QStatusBar *statusbar_;
};

/*****************************************************************
CLASS: QtGuiStatusBar
*****************************************************************/
QtGuiStatusBar *newQtGuiStatusBar(MLParser *parser, ArgDict* argdict)
{
  return new QtGuiStatusBar(parser,argdict);
}

QtGuiStatusBar::QtGuiStatusBar(MLParser *parser, ArgDict* argdict)
{
  qDebug("QtGuiStatusBar::QtGuiStatusBar\n");
  int tag;
  QtGuiDialogItem *item;

  data_=new QtGuiStatusBarPrivate;
  gdata_=data_;
  if (!parser) return;


  // Evaluate arguments
  data_->argdict_=(argdict?argdict:new ArgDict());
  data_->argdict_->insert("PARENT",new QString("STATUSBAR"));
  data_->argdict_=parser->args(data_->argdict_);
  data_->setVars("STATUSBAR");
  data_->regObj(this);

  // Read Tags
  do {
    tag=parser->tag(*_qtguitagtable);
    switch (tag) {
    case -TAG_STATUSBAR:
      break;
    case MLParser::END_OF_FILE:
      break;
    default:
      item=(QtGuiDialogItem*)QtGuiNewObjectId(tag,parser);
      data_->item_->append(item);
      break;
    }
      
  } while ((tag!=-TAG_STATUSBAR) && (tag !=MLParser::END_OF_FILE));;
}

QtGuiStatusBar::~QtGuiStatusBar()
{
}

QWidget *QtGuiStatusBar::create(QWidget* parent, QLayout*)
{
  qDebug("QtGuiStatusBar::create\n");
  data_->create(parent);
  delete data_->argdict_;
  data_->argdict_=0;

  return data_->widget_;
}

void QtGuiStatusBar::connectObject()
{
  QTGUI_DO_ITEMS(QtGuiDialogItem,data_->item_,connectObject())
}

void QtGuiStatusBar::show()
{
  qDebug("QtGuiStatusBar::show %s\n",(const char*)data_->name_);
  QtGuiDialogItem *item;
  item=data_->item_->first();
  while (item) {
    item->variableGet();
    item=data_->item_->next();
  }
  
  data_->widget_->show();
  qDebug("QtGuiStatusBar::show %s done\n",(const char*)data_->name_);
}

void QtGuiStatusBar::hide()
{
}

void QtGuiStatusBar::message(QString msg)
{
  data_->statusbar_->message(msg);
  qApp->syncX();
  qApp->processEvents();
}

void QtGuiStatusBar::message(QString msg,int ms)
{
  data_->statusbar_->message(msg,ms);
  qApp->syncX();
  qApp->processEvents();
}

void QtGuiStatusBar::clear()
{
  data_->statusbar_->clear();
  qApp->syncX();
  qApp->processEvents();
}

/*****************************************************************
CLASS: QtGuiStatusBarPrivate
*****************************************************************/
void QtGuiStatusBarPrivate::create(QWidget *parent)
{
  widget_=statusbar_=new QStatusBar(parent,name_);
  setAttribs();
  QTGUI_DO_ITEMS(QtGuiDialogItem,item_,create(widget_))
}

#ifndef TMAKECONF
#include "QtGuiStatusBar.moc"
#endif
