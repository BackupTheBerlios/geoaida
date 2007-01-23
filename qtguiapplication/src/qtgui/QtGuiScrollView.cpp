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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiScrollView.cpp,v $
 * $Revision: 1.1 $
 * $Date: 2001/12/20 13:40:20 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiScrollView.h"
#include "QtGuiDialogItem.h"
#include "QtGuiLayoutPrivate.h"
#include <qlist.h>
#include <qscrollview.h>

class QtGuiScrollViewPrivate : public QtGuiLayoutPrivate {
public:
  QtGuiScrollViewPrivate() {
    item_=new QList<QtGuiDialogItem>;
    item_->setAutoDelete(true);
    name_="";
    layoutstyle_=VBOX;
    scrollview_=0;
  }
    
  void setAttribs() {
    QtGuiPrivate::setAttribs();
  }
    
  void create(QWidget *parent, QLayout *layout);
  QList<QtGuiDialogItem> *item_;
  QScrollView *scrollview_;
};

/*****************************************************************
CLASS: QtGuiScrollView
*****************************************************************/
QtGuiScrollView *newQtGuiScrollView(MLParser *parser, ArgDict* argdict)
{
  return new QtGuiScrollView(parser,argdict);
}

QtGuiScrollView::QtGuiScrollView(MLParser *parser, ArgDict* argdict)
{
  qDebug("QtGuiScrollView::QtGuiScrollView\n");
  int tag;
  QtGuiDialogItem *item;

  gdata_=idata_=data_=new QtGuiScrollViewPrivate;
  if (!parser) return;


  // Evaluate arguments
  data_->argdict_=(argdict?argdict:new ArgDict());
  data_->argdict_->insert("PARENT",new QString("SCROLLVIEW"));
  data_->argdict_=parser->args(data_->argdict_);
  data_->setVars("SCROLLVIEW");
  qDebug("register object\n");
  data_->regObj(this);

  // Read Tags
  do {
    tag=parser->tag(*_qtguitagtable);
    switch (tag) {
    case -TAG_SCROLLVIEW:
      break;
    case MLParser::END_OF_FILE:
      break;
    default:
      item=(QtGuiDialogItem*)QtGuiNewObjectId(tag,parser);
      data_->item_->append(item);
      break;
    }
      
  } while (tag!=-TAG_SCROLLVIEW);
}

QtGuiScrollView::~QtGuiScrollView()
{
}

QWidget *QtGuiScrollView::create(QWidget* parent, QLayout* layout)
{
  qDebug("QtGuiScrollView::create %s\n",(const char*)data_->name_);
  data_->create(parent,layout);
  qDebug("QtGuiScrollView::create %s done\n",(const char*)data_->name_);
  delete data_->argdict_;
  data_->argdict_=0;

  return data_->widget_;
}

void QtGuiScrollView::variableSet()
{
  QtGuiDialogItem *item;
  item=data_->item_->first();
  while (item) {
    item->variableSet();
    item=data_->item_->next();
  }
}

void QtGuiScrollView::variableGet()
{
  qDebug("QtScrollView::variableGet\n");
  QtGuiDialogItem *item;
  item=data_->item_->first();
  while (item) {
    item->variableGet();
    item=data_->item_->next();
  }
}

void QtGuiScrollView::connectObject()
{
  QTGUI_DO_ITEMS(QtGuiDialogItem,data_->item_,connectObject())
}

void QtGuiScrollView::show()
{
  qDebug("QtGuiScrollView::show %s\n",(const char*)data_->name_);
  data_->widget_->show();
  qDebug("QtGuiScrollView::show %s done\n",(const char*)data_->name_);
}

void QtGuiScrollView::hide()
{
}

/*****************************************************************
CLASS: QtGuiScrollViewPrivate
*****************************************************************/
void QtGuiScrollViewPrivate::create(QWidget *parent,QLayout *layout)
{
  widget_=scrollview_=new QScrollView(parent,name_);
  setAttribs();
//  createLayout(widget_);
  addItem(layout,parent);
  QtGuiDialogItem *item;
  item=item_->first();
  while (item) {
//    item->create(widget_,layout_);
    item->create(scrollview_->viewport());
    scrollview_->addChild(item->widget());
    item=item_->next();
  }
//  ((QTabDialog*)parent)->addTab(widget_,label_);

  QSize size=widget_->minimumSizeHint();
  if (size.isValid())
    qDebug("MinimumSizeHint(%s) minwidth=%d minheight=%d\n",
	   (const char*)name_,size.width(),size.height());
  else
    qDebug("MinimumSizeHint(%s) no Valid sizehint\n",(const char*)name_);
  size=widget_->sizeHint();
  if (size.isValid())
    qDebug("SizeHint(%s) width=%d height=%d\n",
	   (const char*)name_,size.width(),size.height());
  else
    qDebug("SizeHint(%s) no Valid sizehint\n",(const char*)name_);
}
