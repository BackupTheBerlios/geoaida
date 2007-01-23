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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiDockWindow.cpp,v $
 * $Revision: 1.1 $
 * $Date: 2001/12/04 16:25:51 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiDockWindow.h"
#include "QtGuiDialogItem.h"
#include "QtGuiLayoutPrivate.h"
#include <qlist.h>
#include <qtabdialog.h>
#include <qdockwindow.h>
#include <qmainwindow.h>

class QtGuiDockWindowPrivate : public QtGuiLayoutPrivate {
public:
  QtGuiDockWindowPrivate() {
    item_=new QList<QtGuiDialogItem>;
    item_->setAutoDelete(true);
    name_="centralwidget";
    layoutstyle_=VBOX;
    dockwindow_=0;
    edge_=Qt::DockBottom;
    closeMode_=QDockWindow::Never;
    index_=-1;
    newLine_=false;
    extraOffset_=-1;
    parent_=0;
  }
    
  void setAttribs() {
    QtGuiLayoutPrivate::setAttribs();
    if (parent_ && (parent_->isA("QMainWindow"))) {
      QMainWindow* mw=(QMainWindow*)parent_;
      mw->addDockWindow(dockwindow_,edge_);
      if (index_==-1)
        mw->moveDockWindow(dockwindow_,edge_);
      else
        mw->moveDockWindow(dockwindow_,edge_,index_,newLine_,extraOffset_);
      dockwindow_->setCloseMode(closeMode_);
    }
  }
	void setVars(QString classname) {
    QtGuiLayoutPrivate::setVars(classname);
    {
      QString options[]={"Top","Bottom","Left","Right","Minimized","TornOff","Unmanaged",""};
  		int val=0;
      setOption(val,argdict_,"edge",options);
      Qt::Dock result[]={Qt::DockBottom,Qt::DockTop,Qt::DockBottom,Qt::DockLeft,Qt::DockRight,
                         Qt::DockMinimized,Qt::DockTornOff,Qt::DockUnmanaged};
      edge_=result[val];
    }
    {
      QString options[]={"Never","Docked","Undocked","Always",""};
  		int val=0;
      setOption(val,argdict_,"closemode",options);
      QDockWindow::CloseMode result[]={QDockWindow::Never,
                                       QDockWindow::Never,
                                       QDockWindow::Docked,
                                       QDockWindow::Undocked,
                                       QDockWindow::Always};

      closeMode_=result[val];
    }
    setInt(index_,argdict_,"index");
    setInt(extraOffset_,argdict_,"extraOffset");
    setBool(newLine_,argdict_,"newLine");
	}
    
  void create(QWidget *parent, QLayout *layout);
  QList<QtGuiDialogItem> *item_;
  QDockWindow *dockwindow_;
	Qt::Dock edge_;
	int index_;
	bool newLine_;
	int extraOffset_;
	QDockWindow::CloseMode closeMode_;
	QWidget* parent_;
};

/*****************************************************************
CLASS: QtGuiDockWindow
*****************************************************************/
QtGuiDockWindow *newQtGuiDockWindow(MLParser *parser, ArgDict* argdict)
{
  return new QtGuiDockWindow(parser,argdict);
}

QtGuiDockWindow::QtGuiDockWindow(MLParser *parser, ArgDict* argdict)
{
  qDebug("QtGuiDockWindow::QtGuiDockWindow\n");
  int tag;
  QtGuiDialogItem *item;

  gdata_=idata_=data_=new QtGuiDockWindowPrivate;
  if (!parser) return;


  // Evaluate arguments
  data_->argdict_=(argdict?argdict:new ArgDict());
  data_->argdict_->insert("PARENT",new QString("LAYOUT"));
  data_->argdict_=parser->args(data_->argdict_);
  data_->setVars("DOCKWINDOW");
  qDebug("register object\n");
  data_->regObj(this);

  // Read Tags
  do {
    tag=parser->tag(*_qtguitagtable);
    switch (tag) {
    case -TAG_DOCKWINDOW:
      break;
    case MLParser::END_OF_FILE:
      break;
    default:
      item=(QtGuiDialogItem*)QtGuiNewObjectId(tag,parser);
      data_->item_->append(item);
      break;
    }
      
  } while ((tag!=-TAG_DOCKWINDOW) && (tag!=MLParser::END_OF_FILE));
}

QtGuiDockWindow::~QtGuiDockWindow()
{
}

QWidget *QtGuiDockWindow::create(QWidget* parent, QLayout* layout)
{
  qDebug("QtGuiDockWindow::create %s\n",(const char*)data_->name_);
  data_->create(parent,layout);
  qDebug("QtGuiDockWindow::create %s done\n",(const char*)data_->name_);
  delete data_->argdict_;
  data_->argdict_=0;
  return data_->widget_;
}

void QtGuiDockWindow::connectObject()
{
  QTGUI_DO_ITEMS(QtGuiDialogItem,data_->item_,connectObject())
}

void QtGuiDockWindow::show()
{
  qDebug("QtGuiDockWindow::show %s\n",(const char*)data_->name_);
  data_->widget_->show();
  qDebug("QtGuiDockWindow::show %s done\n",(const char*)data_->name_);
}

void QtGuiDockWindow::hide()
{
  qDebug("QtGuiDockWindow::hide %s\n",(const char*)data_->name_);
  data_->widget_->hide();
  qDebug("QtGuiDockWindow::hide %s done\n",(const char*)data_->name_);
}

/*****************************************************************
CLASS: QtGuiDockWindowPrivate
*****************************************************************/
void QtGuiDockWindowPrivate::create(QWidget *parent,QLayout *layout)
{
  parent_=parent;
  widget_=dockwindow_=new QDockWindow(QDockWindow::InDock,parent,name_);
#if 0
  widget_=new QWidget(parent);
  dockwindow_->setWidget(widget_);
#endif
  dockwindow_->setResizeEnabled(true);
  setAttribs();
  createLayout(widget_);
  addItem(layout,parent);
  QtGuiDialogItem *item;
  item=item_->first();
  while (item) {
#if 0
    item->create(widget_,layout_);
#endif
    item->create(widget_,dockwindow_->boxLayout());
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
