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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiWidget.cpp,v $
 * $Revision: 1.1 $
 * $Date: 2001/07/31 11:57:04 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiWidget.h"
#include "QtGuiDialogItemPrivate.h"
#include <qlist.h>
#include <qlabel.h>

class QtGuiWidgetPrivate : public QtGuiDialogItemPrivate {
public:
  QtGuiWidgetPrivate() {
  }
  void setVars(QString classname) {
    QtGuiDialogItemPrivate::setVars(classname);
  }

  //  void setAttribs() {
  //    QtGuiPrivate::setAttribs();
  //  }
  void create(QWidget* parent, QLayout* layout);
};

/*****************************************************************
CLASS: QtGuiWidget
*****************************************************************/
QtGuiWidget *newQtGuiWidget(MLParser *parser, ArgDict* argdict)
{
  return new QtGuiWidget(parser,argdict);
}

QtGuiWidget::QtGuiWidget(MLParser *parser, ArgDict* argdict)
{
  qDebug("QtGuiWidget::QtGuiWidget\n");

  gdata_=idata_=data_=new QtGuiWidgetPrivate;
  if (!parser) return;


  // Evaluate arguments
  data_->argdict_=parser->args(argdict);
  data_->setVars("WIDGET");
  data_->regObj(this);
}

QtGuiWidget::~QtGuiWidget()
{
}

QWidget *QtGuiWidget::create(QWidget* parent, QLayout* layout)
{
  qDebug("QtGuiWidget::create\n");
  data_->create(parent,layout);
  delete data_->argdict_;
  data_->argdict_=0;
  return data_->widget_;
}

void QtGuiWidget::connectObject()
{
  data_->connect(data_->widget_,"valueChanged",data_->widget_,"valueSet");
}

/*
void QtGuiWidget::show()
{
  qDebug("QtGuiWidget::show %s\n",(const char*)data_->name_);
  data_->widget_->show();
  qDebug("QtGuiWidget::show %s done\n",(const char*)data_->name_);
}

void QtGuiWidget::hide()
{
}
*/

/*****************************************************************
CLASS: QtGuiWidgetPrivate
*****************************************************************/
void QtGuiWidgetPrivate::create(QWidget *parent, QLayout *layout)
{
  QObject *object;
  object=(*(_qtguiappl->classDict()))[name_];
  if (object && object->inherits("QWidget"))
    {
      qDebug("QtGuiWidget::create() grabbing %s\n",(const char*)name_);
      widget_=(QWidget*)object;
      widget_->reparent(parent,0,QPoint(0,0));
      setAttribs();
      addItem(layout,parent);
    }
  else
    qDebug("QtGuiWidget::create() not found %s\n",(const char*)name_);
}
