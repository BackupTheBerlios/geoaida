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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiCheckBox.cpp,v $
 * $Revision: 1.1 $
 * $Date: 2001/07/31 11:57:04 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiCheckBox.h"
#include "QtGuiDialogItemPrivate.h"
#include <qlist.h>
#include <qcheckbox.h>

class QtGuiCheckBoxPrivate : public QtGuiDialogItemPrivate {
public:
  QtGuiCheckBoxPrivate() {
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
  QCheckBox *button_;
  QtGuiVarModifier *variable_;
  // Attribs
  int id_;
};

/*****************************************************************
CLASS: QtGuiCheckBox
*****************************************************************/
QtGuiCheckBox *newQtGuiCheckBox(MLParser *parser, ArgDict* argdict)
{
  return new QtGuiCheckBox(parser,argdict);
}

QtGuiCheckBox::QtGuiCheckBox(MLParser *parser, ArgDict* argdict)
{
  qDebug("QtGuiCheckBox::QtGuiCheckBox\n");

  gdata_=idata_=data_=new QtGuiCheckBoxPrivate;
  if (!parser) return;


  // Evaluate arguments
  data_->argdict_=parser->args(argdict);
  data_->setVars("CHECKBOX");
  if (data_->varname_.isEmpty()) data_->varname_=data_->name_;
  data_->regObj(this);
}

QtGuiCheckBox::~QtGuiCheckBox()
{
}

QWidget *QtGuiCheckBox::create(QWidget* parent, QLayout* layout)
{
  qDebug("QtGuiCheckBox::create\n");
  data_->create(parent,layout);
  if (data_->isSet(*(data_->argdict_),"val")) 
    valueSet(data_->getInt(*(data_->argdict_),"val"));
  connect(data_->button_,SIGNAL(toggled(bool)),this,SLOT(stateChanged()));
  delete data_->argdict_;
  data_->argdict_=0;
  return data_->widget_;
}



int QtGuiCheckBox::valueGetInt()
{
  if (data_->button_->isChecked()) {
    qDebug("QtGuiCheckBox::valueGetInt() true id=%d\n",
	   data_->id_);
    return data_->id_;
  }
  else {
    qDebug("QtGuiCheckBox::valueGetInt() false id=%d\n",
	   data_->id_);
    return 0;
  }
}


void QtGuiCheckBox::valueSet(int val)
{
  if (val & data_->id_) {
    qDebug("QtGuiRadioButton::valueSetInt(%d) (id=%d) true\n",
	   val,data_->id_);
    data_->button_->setChecked(TRUE);
  }
  else {
    qDebug("QtGuiRadioButton::valueSetInt(%d) (id=%d) false\n",
	   val,data_->id_);
    data_->button_->setChecked(FALSE);
  }
}

void QtGuiCheckBox::enable(int val)
{
  QtGui::enable((val & data_->id_)!=0); 
}

/*****************************************************************
CLASS: QtGuiCheckBoxPrivate
*****************************************************************/
void QtGuiCheckBoxPrivate::create(QWidget *parent, QLayout *layout)
{
  variable_=(*(_qtguiappl->varDict()))[varname_];
  widget_=button_=new QCheckBox(label_,parent,name_);
  setAttribs();
  addItem(layout,parent);
}

#ifndef TMAKECONF
#include "QtGuiCheckBox.moc"
#endif
