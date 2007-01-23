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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiCentralWidget.cpp,v $
 * $Revision: 1.1 $
 * $Date: 2001/07/31 11:57:04 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiCentralWidget.h"
#include "QtGuiDialogItem.h"
#include "QtGuiLayoutPrivate.h"
#include <qlist.h>
#include <qtabdialog.h>

class QtGuiCentralWidgetPrivate : public QtGuiLayoutPrivate {
public:
  QtGuiCentralWidgetPrivate() {
    item_=new QList<QtGuiDialogItem>;
    item_->setAutoDelete(true);
    name_="centralwidget";
    layoutstyle_=VBOX;
    centralwidget_=0;
  }
    
  void setAttribs() {
    QtGuiLayoutPrivate::setAttribs();
  }
    
  void create(QWidget *parent, QLayout *layout);
  QList<QtGuiDialogItem> *item_;
  QWidget *centralwidget_;
};

/*****************************************************************
CLASS: QtGuiCentralWidget
*****************************************************************/
QtGuiCentralWidget *newQtGuiCentralWidget(MLParser *parser, ArgDict* argdict)
{
  return new QtGuiCentralWidget(parser,argdict);
}

QtGuiCentralWidget::QtGuiCentralWidget(MLParser *parser, ArgDict* argdict)
{
  qDebug("QtGuiCentralWidget::QtGuiCentralWidget\n");
  int tag;
  QtGuiDialogItem *item;

  gdata_=idata_=data_=new QtGuiCentralWidgetPrivate;
  if (!parser) return;


  // Evaluate arguments
  data_->argdict_=(argdict?argdict:new ArgDict());
  data_->argdict_->insert("PARENT",new QString("LAYOUT"));
  data_->argdict_=parser->args(data_->argdict_);
  data_->setVars("LAYOUT");
  qDebug("register object\n");
  data_->regObj(this);

  // Read Tags
  do {
    tag=parser->tag(*_qtguitagtable);
    switch (tag) {
    case -TAG_CENTRALWIDGET:
      break;
    case MLParser::END_OF_FILE:
      break;
    default:
      item=(QtGuiDialogItem*)QtGuiNewObjectId(tag,parser);
      data_->item_->append(item);
      break;
    }
      
  } while ((tag!=-TAG_CENTRALWIDGET) && (tag!=MLParser::END_OF_FILE));
}

QtGuiCentralWidget::~QtGuiCentralWidget()
{
}

QWidget *QtGuiCentralWidget::create(QWidget* parent, QLayout* layout)
{
  qDebug("QtGuiCentralWidget::create %s\n",(const char*)data_->name_);
  data_->create(parent,layout);
  qDebug("QtGuiCentralWidget::create %s done\n",(const char*)data_->name_);
  delete data_->argdict_;
  data_->argdict_=0;
  return data_->widget_;
}

void QtGuiCentralWidget::connectObject()
{
  QTGUI_DO_ITEMS(QtGuiDialogItem,data_->item_,connectObject())
}

void QtGuiCentralWidget::show()
{
  qDebug("QtGuiCentralWidget::show %s\n",(const char*)data_->name_);
  data_->widget_->show();
  qDebug("QtGuiCentralWidget::show %s done\n",(const char*)data_->name_);
}

void QtGuiCentralWidget::hide()
{
}

/*****************************************************************
CLASS: QtGuiCentralWidgetPrivate
*****************************************************************/
void QtGuiCentralWidgetPrivate::create(QWidget *parent,QLayout *layout)
{
  widget_=centralwidget_=new QWidget(parent,name_);
  setAttribs();
  createLayout(widget_);
  addItem(layout,parent);
  QtGuiDialogItem *item;
  item=item_->first();
  while (item) {
    item->create(widget_,layout_);
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
