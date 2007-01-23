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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiLabel.cpp,v $
 * $Revision: 1.2 $
 * $Date: 2001/12/20 13:40:20 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiLabel.h"
#include "QtGuiDialogItemPrivate.h"
#include <qlist.h>
#include <qlabel.h>
#include <qapplication.h>
class QtGuiLabelPrivate : public QtGuiDialogItemPrivate {
public:
  QtGuiLabelPrivate() {
    type_="TEXT";
    wlabel_=0;
  }
  void setVars(QString classname) {
    QtGuiDialogItemPrivate::setVars(classname);
    setString(type_,argdict_,"type");
  }

  void setAttribs() {
    QtGuiPrivate::setAttribs();
    if (pixmap_) {
    	wlabel_->setPixmap(*pixmap_);
    }
    else {
	    wlabel_->setText(label_);
    }
//! This breaks the layoutmanagement. Don't know why
//    wlabel_->setAlignment(wlabel_->alignment()|QLabel::WordBreak);
    wlabel_->setAlignment(wlabel_->alignment());
  }
  void create(QWidget* parent, QLayout* layout);
  QLabel *wlabel_;
  // Attribs
  QString type_;
};

/*****************************************************************
CLASS: QtGuiLabel
*****************************************************************/
QtGuiLabel *newQtGuiLabel(MLParser *parser, ArgDict* argdict)
{
  return new QtGuiLabel(parser,argdict);
}

QtGuiLabel::QtGuiLabel(MLParser *parser, ArgDict* argdict)
{
  qDebug("QtGuiLabel::QtGuiLabel\n");

  gdata_=idata_=data_=new QtGuiLabelPrivate;
  if (!parser) return;


  // Evaluate arguments
  data_->argdict_=parser->args(argdict);
  data_->setVars("LABEL");
  data_->regObj(this);
}

QtGuiLabel::~QtGuiLabel()
{
}

QWidget *QtGuiLabel::create(QWidget* parent, QLayout* layout)
{
  qDebug("QtGuiLabel::create\n");
  data_->create(parent,layout);
  delete data_->argdict_;
  data_->argdict_=0;
  return data_->widget_;
}

void QtGuiLabel::labelSet(QString val)
{
  data_->wlabel_->setText(val);
}

void QtGuiLabel::valueSet(QString val)
{
  labelSet(val);
}


/*

void QtGuiLabel::show()
{
  qDebug("QtGuiLabel::show %s\n",(const char*)data_->name_);
  data_->widget_->show();
  qDebug("QtGuiLabel::show %s done\n",(const char*)data_->name_);
}

void QtGuiLabel::hide()
{
}
*/

/*****************************************************************
CLASS: QtGuiLabelPrivate
*****************************************************************/
void QtGuiLabelPrivate::create(QWidget *parent, QLayout *layout)
{
  widget_=wlabel_=new QLabel(parent,name_);
  //  widget_=wlabel_=new QLabel(label_,parent,name_);
  setAttribs();
  addItem(layout,parent);

  { //! Debug
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
}

#ifndef TMAKECONF
#include "QtGuiLabel.moc"
#endif
