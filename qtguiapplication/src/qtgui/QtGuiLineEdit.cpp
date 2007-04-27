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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiLineEdit.cpp,v $
 * $Revision: 1.2 $
 * $Date: 2002/02/22 14:34:21 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiLineEdit.h"
#include "QtGuiDialogItemPrivate.h"
#include <qlist.h>
#include <qlineedit.h>
#include <qvalidator.h>

class QtGuiLineEditPrivate : public QtGuiDialogItemPrivate {
public:
  enum {
    STRING=1,
    INT,
    DOUBLE,
    PASSWORD
  };
  QtGuiLineEditPrivate() {
    type_=STRING;
    lineedit_=0;
    variable_=0;
  }
  void setVars(QString classname) {
    QtGuiDialogItemPrivate::setVars(classname);
    QString options[]={"STRING","INT","DOUBLE","PASSWORD",""};
    setOption(type_,argdict_,"type",options);
    switch (type_) {
    case STRING:
    case PASSWORD:
      format_="%s";
      break;
    case INT:
      format_="%d";
      iMin_=1;
      iMax_=-1;
      setInt(iMin_,argdict_,"min");
      setInt(iMax_,argdict_,"max");
      break;
    case DOUBLE:
      format_="%lf";
      dMin_=1.0;
      dMax_=-1.0;
      decimals_=-1;
      setDouble(dMin_,argdict_,"min");
      setDouble(dMax_,argdict_,"max");
      setInt(decimals_,argdict_,"decimals");
      break;
    }
    setString(format_,argdict_,"format");
  }

  //  void setAttribs() {
  //    QtGuiPrivate::setAttribs();
  //  }
  void create(QWidget* parent, QLayout* layout);
  QLineEdit *lineedit_;
  QValidator *validator_;
  // Attribs
  int type_;
  QString format_;
  union {
	  int iMin_;
    double dMin_;
  };
  union {
  	double dMax_;
  	int iMax_;
  };
  int decimals_;
};

/*****************************************************************
CLASS: QtGuiLineEdit
*****************************************************************/
QtGuiLineEdit *newQtGuiLineEdit(MLParser *parser, ArgDict* argdict)
{
  return new QtGuiLineEdit(parser,argdict);
}

QtGuiLineEdit::QtGuiLineEdit(MLParser *parser, ArgDict* argdict)
{
  qDebug("QtGuiLineEdit::QtGuiLineEdit\n");

  gdata_=idata_=data_=new QtGuiLineEditPrivate;
  if (!parser) return;


  // Evaluate arguments
  data_->argdict_=parser->args(argdict);
  data_->setVars("INPUT");
  data_->regObj(this);
}

QtGuiLineEdit::~QtGuiLineEdit()
{
}

QWidget *QtGuiLineEdit::create(QWidget* parent, QLayout* layout)
{
  qDebug("QtGuiLineEdit::create\n");
  data_->create(parent,layout);
  variableGet();
  if (data_->isSet(*(data_->argdict_),"val")) 
    valueSet(data_->getString(*(data_->argdict_),"val"));
  else
  	switch (data_->type_) {
  	case QtGuiLineEditPrivate::DOUBLE:
  		valueSet("0.0");
  		break;
    case QtGuiLineEditPrivate::INT:
    	valueSet("0");
    	break;
  	}
  connect(data_->lineedit_,SIGNAL(returnPressed()),
	  this,SLOT(stateChanged()));
  if (data_->getBool(*(data_->argdict_),"update",false)) {
    connect(data_->lineedit_,SIGNAL(textChanged(const QString&)),
    	  this,SLOT(stateChanged()));
  }
  delete data_->argdict_;
  data_->argdict_=0;
  return data_->widget_;
}

int QtGuiLineEdit::valueGetInt()
{
  int x;
  QString val=valueGetStr();
  QTextIStream(&val) >> x;
  return x;
}

double QtGuiLineEdit::valueGetDouble()
{ 
  double x;
  QString val=valueGetStr();
  QTextIStream(&val) >> x;
  return x;
}

QString QtGuiLineEdit::valueGetStr()
{
  QString val=data_->lineedit_->text();
  if (val.isEmpty()) val="";
  return val;
}

void QtGuiLineEdit::valueSet(int val)
{
  switch (data_->type_) {
  case QtGuiLineEditPrivate::INT:
    valueSet(QString("").sprintf(data_->format_,val));
    break;
  default:
    valueSet(QString("").sprintf("%d",val));
    break;
  }
}

void QtGuiLineEdit::valueSet(double val)
{
  switch (data_->type_) {
  case QtGuiLineEditPrivate::DOUBLE:
    valueSet(QString("").sprintf(data_->format_,val));
    break;
  default:
    valueSet(QString("").sprintf("%f",val));
    break;
  }
}

void QtGuiLineEdit::valueSet(QString val)
{
  QString s;
  switch (data_->type_) {
  case QtGuiLineEditPrivate::STRING:
  case QtGuiLineEditPrivate::PASSWORD:
    s.sprintf(data_->format_,(const char*)val);
    data_->lineedit_->setText(s);
    break;
  default:
    data_->lineedit_->setText(val);
    break;
  }
}


void QtGuiLineEdit::variableGet()
{
  qDebug("QtGuiLineEdit::variableGet\n");
  if (!data_->variable_) return;
  qDebug("QtGuiLineEdit::variableGet (%s=%s)\n",
	 (const char*)data_->varname_,
	 (const char*)data_->variable_->valueGetString());
  valueSet(data_->variable_->valueGetString());
}

void QtGuiLineEdit::variableSet()
{
  qDebug("QtGuiLineEdit::variableSet\n");
  if (!data_->variable_) {
    stateChanged();
		return;
	}
  data_->variable_->valueSetString(valueGetStr());
}

void QtGuiLineEdit::stateChanged()
{
  if (data_->sending_) {
    qDebug("QtGuiLineEdit::stateChanged name=%s recursion detected\n",
	 (const char*)data_->name_);
    return;
  }
  data_->sending_=true;
  qDebug("QtGuiLineEdit::stateChanged(%d) name=%s val=%s\n",
	 data_->immediately_,
	 (const char*)data_->name_,(const char*)valueGetStr());
  if (data_->variable_ && data_->immediately_) {
    variableSet();
  }
  switch (data_->type_) {
  case QtGuiLineEditPrivate::STRING:
  case QtGuiLineEditPrivate::PASSWORD:
    emit valueChanged(valueGetStr());
    break;
  case QtGuiLineEditPrivate::INT:
    emit valueChanged(valueGetInt());
    emit valueChanged(valueGetStr());
    break;
  case QtGuiLineEditPrivate::DOUBLE:
    emit valueChanged(valueGetDouble());
    emit valueChanged(valueGetStr());
    break;
  }
  emit valueChanged();
  data_->sending_=false;
}

/*****************************************************************
CLASS: QtGuiLineEditPrivate
*****************************************************************/
void QtGuiLineEditPrivate::create(QWidget *parent, QLayout *layout)
{
  variable_=(*(_qtguiappl->varDict()))[varname_];
  widget_=lineedit_=new QLineEdit(parent,name_);
  switch (type_) {
  case DOUBLE:
  	qDebug("QtGuiLineEditPrivate::create doubleValidator (%f - %f , %d)",dMin_, dMax_, decimals_);
  	validator_=new QDoubleValidator(lineedit_,"lineedit");
   	if (dMin_<=dMax_) {
   		((QDoubleValidator*)validator_)->setRange(dMin_,dMax_);
   		qDebug("setRange %f %f",dMin_,dMax_);
   	}
   	if (decimals_>=0) ((QDoubleValidator*)validator_)->setDecimals(decimals_);
  	lineedit_->setValidator(validator_);
  	break;
  case INT:
  	qDebug("QtGuiLineEditPrivate::create intValidator");
  	validator_=new QIntValidator(lineedit_,"lineedit");
   	if (iMin_<=iMax_) ((QIntValidator*)validator_)->setRange(iMin_,iMax_);
  	lineedit_->setValidator(validator_);
  	break;
  default:
  	qDebug("QtGuiLineEditPrivate::create no Validator");
  	validator_=0;
  }
  setAttribs();
  addItem(layout,parent);
}

#ifndef TMAKECONF
#include "QtGuiLineEdit.moc"
#endif
