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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiButtonGroup.cpp,v $
 * $Revision: 1.1 $
 * $Date: 2001/07/31 11:57:04 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiButtonGroup.h"
#include "QtGuiDialogItem.h"
#include "QtGuiLayoutPrivate.h"
#include <qlist.h>
#include <qvbuttongroup.h>
#include <qhbuttongroup.h>

class QtGuiButtonGroupPrivate : public QtGuiLayoutPrivate {
public:
  QtGuiButtonGroupPrivate() {
    item_=new QList<QtGuiDialogItem>;
    item_->setAutoDelete(true);
    name_="";
    layoutstyle_=VBOX;
    buttongroup_=0;
  }
    
  void setAttribs() {
    QtGuiPrivate::setAttribs();
    buttongroup_->setTitle(label_);
  }
    
  void create(QtGuiButtonGroup*,QWidget *parent, QLayout *layout);
  QList<QtGuiDialogItem> *item_;
  QButtonGroup *buttongroup_;
};

/*****************************************************************
CLASS: QtGuiButtonGroup
*****************************************************************/
QtGuiButtonGroup *newQtGuiButtonGroup(MLParser *parser, ArgDict* argdict)
{
  return new QtGuiButtonGroup(parser,argdict);
}

QtGuiButtonGroup::QtGuiButtonGroup(MLParser *parser, ArgDict* argdict)
{
  qDebug("QtGuiButtonGroup::QtGuiButtonGroup\n");
  int tag;
  QtGuiDialogItem *item;

  gdata_=idata_=data_=new QtGuiButtonGroupPrivate;
  if (!parser) return;


  // Evaluate arguments
  data_->argdict_=parser->args(argdict);
  data_->setVars("BUTTONGROUP");
  if (data_->varname_.isEmpty()) {
    qDebug("QtGuiButtonGroup(%s): no varname given\n",(const char*)data_->name_);
    data_->varname_=data_->name_;
  }
  qDebug("register object\n");
  data_->regObj(this);

  // Read Tags
  do {
    tag=parser->tag(*_qtguitagtable);
    switch (tag) {
    case -TAG_BUTTONGROUP:
      break;
    case MLParser::END_OF_FILE:
      break;
    default:
      item=(QtGuiDialogItem*)QtGuiNewObjectId(tag,parser);
      data_->item_->append(item);
      break;
    }
      
  } while (tag!=-TAG_BUTTONGROUP);
}

QtGuiButtonGroup::~QtGuiButtonGroup()
{
}

QWidget *QtGuiButtonGroup::create(QWidget* parent, QLayout* layout)
{
  qDebug("QtGuiButtonGroup::create %s\n",(const char*)data_->name_);
  data_->create(this,parent,layout);
  variableGet();
  if (data_->isSet(*(data_->argdict_),"val")) 
    valueSet(data_->getInt(*(data_->argdict_),"val"));
  qDebug("QtGuiButtonGroup::create %s done\n",(const char*)data_->name_);
  delete data_->argdict_;
  data_->argdict_=0;

  return data_->widget_;
}

void QtGuiButtonGroup::connectObject()
{
  QtGui::connectObject();
  QTGUI_DO_ITEMS(QtGuiDialogItem,data_->item_,connectObject())
}



void QtGuiButtonGroup::variableSet()
{
  qDebug("QtGuiButtonGroup::variableSet");
  QtGuiDialogItem *item;
  item=data_->item_->first();
  while (item) {
    item->variableSet();
    item=data_->item_->next();
  }
  QtGuiDialogItemTypeInt::variableSet();
}

void QtGuiButtonGroup::variableGet()
{
  int val=0;
  QtGuiDialogItem *item;
  item=data_->item_->first();
  while (item) {
    item->variableGet();
    item=data_->item_->next();
  }
  QtGuiDialogItemTypeInt::variableGet();
}

void QtGuiButtonGroup::valueSet(int val)
{
  qDebug("QtGuiButtonGroup::valueSet val=%d\n",val);
  QtGuiDialogItem *item;
  item=data_->item_->first();
  while (item) {
    item->valueSet(val);
    item=data_->item_->next();
  }
  qDebug("QtGuiButtonGroup::valueSet val=%d Done\n",val);
  //  emit valueChanged(val);
}

int QtGuiButtonGroup::valueGetInt()
{
  int val=0;
  QtGuiDialogItem *item;
  item=data_->item_->first();
  while (item) {
    val+=item->valueGetInt();
    item=data_->item_->next();
  }
  return val;
}

void QtGuiButtonGroup::enable()
{
  QTGUI_DO_ITEMS(QtGuiDialogItem,data_->item_,QtGui::enable())
}

void QtGuiButtonGroup::enable(int val)
{
  qDebug("QtGuiButtonGroup::enable(%d)\n",val);
  QTGUI_DO_ITEMS(QtGuiDialogItem,data_->item_,enable(val))
}

void QtGuiButtonGroup::disable()
{
  QTGUI_DO_ITEMS(QtGuiDialogItem,data_->item_,disable())
}


/*****************************************************************
CLASS: QtGuiButtonGroupPrivate
*****************************************************************/
void QtGuiButtonGroupPrivate::create(QtGuiButtonGroup *pub_data,
				     QWidget *parent,QLayout *layout)
{
  variable_=(*(_qtguiappl->varDict()))[varname_];
  if (!variable_) {
    qDebug("QtGuiButtonGroup::create varname=%s not found\n",
	   (const char*)varname_);
    }
  widget_=buttongroup_=new QButtonGroup(parent,name_);
  setAttribs();
  switch (layoutstyle_) {
  case HBOX:
    buttongroup_->setColumnLayout(1,QGroupBox::Vertical);
    break;
  default:
    buttongroup_->setColumnLayout(1,QGroupBox::Horizontal);
  }
  //  createLayout(widget_);
  layout_=buttongroup_->layout();
  addItem(layout,parent);
//  QTGUI_DO_ITEMS(QtGuiDialogItem,item_,create(widget_,layout_))
  QTGUI_DO_ITEMS(QtGuiDialogItem,item_,create(widget_,0))
  pub_data->connect(buttongroup_,SIGNAL(clicked(int)),
		    pub_data,SLOT(stateChanged()));
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

#ifndef TMAKECONF
#include "QtGuiButtonGroup.moc"
#endif
