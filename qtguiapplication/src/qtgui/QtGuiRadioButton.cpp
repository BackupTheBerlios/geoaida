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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiRadioButton.cpp,v $
 * $Revision: 1.1 $
 * $Date: 2001/07/31 11:57:04 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiRadioButton.h"
#include "QtGuiDialogItemPrivate.h"
#include <qlist.h>
#include <qradiobutton.h>

class QtGuiRadioButtonPrivate : public QtGuiDialogItemPrivate {
public:
  QtGuiRadioButtonPrivate() {
    button_=0;
    id_=1;
  }
  void setVars(QString classname) {
    QtGuiDialogItemPrivate::setVars(classname);
    setInt(id_,argdict_,"id");
  }

  void setAttribs() {
    QtGuiDialogItemPrivate::setAttribs();
    button_->setText(label_);
  }
  void create(QWidget* parent, QLayout* layout);
  QRadioButton *button_;
  // Attribs
  int id_;
};

/*****************************************************************
CLASS: QtGuiRadioButton
*****************************************************************/
QtGuiRadioButton *newQtGuiRadioButton(MLParser *parser, ArgDict* argdict)
{
  return new QtGuiRadioButton(parser,argdict);
}

QtGuiRadioButton::QtGuiRadioButton(MLParser *parser, ArgDict* argdict)
{
  qDebug("QtGuiRadioButton::QtGuiRadioButton\n");

  gdata_=idata_=data_=new QtGuiRadioButtonPrivate;
  if (!parser) return;


  // Evaluate arguments
  data_->argdict_=parser->args(argdict);
  data_->setVars("RADIOBUTTON");
  if (data_->varname_.isEmpty()) data_->varname_=data_->name_;
  data_->regObj(this);
}

QtGuiRadioButton::~QtGuiRadioButton()
{
}

QWidget *QtGuiRadioButton::create(QWidget* parent, QLayout* layout)
{
  qDebug("QtGuiRadioButton::create\n");
  data_->create(parent,layout);
  if (data_->isSet(*(data_->argdict_),"val")) 
    valueSet(data_->getInt(*(data_->argdict_),"val"));
  connect(data_->button_,SIGNAL(toggled(bool)),this,SLOT(stateChanged()));
  delete data_->argdict_;
  data_->argdict_=0;
  return data_->widget_;
}


int QtGuiRadioButton::valueGetInt()
{
  if (data_->button_->isChecked()) return data_->id_;
  else return 0;
}

void QtGuiRadioButton::valueSet(int val)
{
  if (val & data_->id_) {
    qDebug("QtGuiRadioButton::valueSet(%d) (name=%s,id=%d) true\n",
	   val,(const char*)data_->name_,data_->id_);
    data_->button_->setChecked(TRUE);
  }
  else {
    qDebug("QtGuiRadioButton::valueSet(%d) (name=%s,id=%d) false\n",
	   val,(const char*)data_->name_,data_->id_);
    data_->button_->setChecked(FALSE);
  }
}

void QtGuiRadioButton::enable(int val)
{
  qDebug("QtGuiRadioButton::enable(%d)\n",val);
  QtGui::enable((val & data_->id_)!=0);
}

/*****************************************************************
CLASS: QtGuiRadioButtonPrivate
*****************************************************************/
void QtGuiRadioButtonPrivate::create(QWidget *parent, QLayout *layout)
{
  variable_=(*(_qtguiappl->varDict()))[varname_];
  widget_=button_=new QRadioButton(label_,parent,name_);
  setAttribs();
  addItem(layout,parent);
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
#include "QtGuiRadioButton.moc"
#endif
