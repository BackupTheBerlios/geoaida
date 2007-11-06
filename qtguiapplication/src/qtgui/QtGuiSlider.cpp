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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiSlider.cpp,v $
 * $Revision: 1.2 $
 * $Date: 2002/06/10 05:49:56 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiSlider.h"
#include "QtGuiDialogItemPrivate.h"
#include <qlist.h>
#include <qslider.h>
class QtGuiSliderPrivate : public QtGuiDialogItemPrivate {
public:
  enum {
    INT=1,
    DOUBLE
  };
  QtGuiSliderPrivate() {
    slider_=0;
    minval_=0;
    maxval_=100;
    step_=1;
    variable_=0;
    type_=INT;
    tickinterval_=1;
  }
  void setVars(QString classname) {
    QtGuiDialogItemPrivate::setVars(classname);
    setDouble(minval_,argdict_,"minval");
    setDouble(maxval_,argdict_,"maxval");
    setDouble(step_,argdict_,"step");
    {
      QString options[]={"VERTICAL","HORIZONTAL",""};
      int val;
      setOption(val,argdict_,"dir",options);
      direction_= ((val==1) ? QSlider::Vertical : QSlider::Horizontal);
    }
    {
      QString options[]={"INT","DOUBLE",""};
      setOption(type_,argdict_,"type",options);
    }
    {
      QString options[]={"NOMARKS","ABOVE","LEFT","BELOW","RIGHT","BOTH",""};
      int val=0;
      setOption(val,argdict_,"tickmarks",options);
      switch (val) {
      case 2:
	ticksetting_=QSlider::Above;
	break;
      case 3:
	ticksetting_=QSlider::Left;
	break;
      case 4:
	ticksetting_=QSlider::Below;
	break;
      case 5:
	ticksetting_=QSlider::Right;
	break;
      case 6:
	ticksetting_=QSlider::Both;
	break;
      default:
	ticksetting_=QSlider::NoMarks;
	break;
      }
    setInt(tickinterval_,argdict_,"tickinterval");
    }
  }

  void setAttribs() {
    QtGuiDialogItemPrivate::setAttribs();
    slider_->setOrientation(direction_);
    slider_->setTickmarks(ticksetting_);
    slider_->setTickInterval(int(tickinterval_/step_));
    int minval=int(minval_/step_+0.5);
    int maxval=int(maxval_/step_+0.5);
    qDebug("QtGuiSlider::setAttribs minval=%d, maxval=%d\n",minval,maxval);
    if (minval!=maxval) {
      slider_->setRange(minval,maxval);
    }
  }
  void create(QWidget* parent, QLayout* layout);
  QSlider *slider_;
  // Attribs
  Qt::Orientation direction_;
  double minval_;
  double maxval_;
  double step_;
  QSlider::TickSetting ticksetting_;
  int type_;
  int tickinterval_;
};

/*****************************************************************
CLASS: QtGuiSlider
*****************************************************************/
QtGuiSlider *newQtGuiSlider(MLParser *parser, ArgDict* argdict)
{
  return new QtGuiSlider(parser,argdict);
}

QtGuiSlider::QtGuiSlider(MLParser *parser, ArgDict* argdict)
{
  qDebug("QtGuiSlider::QtGuiSlider\n");

  gdata_=idata_=data_=new QtGuiSliderPrivate;
  if (!parser) return;


  // Evaluate arguments
  data_->argdict_=parser->args(argdict);
  data_->setVars("SLIDER");
  data_->regObj(this);
}

QtGuiSlider::~QtGuiSlider()
{
}

QWidget *QtGuiSlider::create(QWidget* parent, QLayout* layout)
{
  qDebug("QtGuiSlider::create\n");
  data_->create(parent,layout);
  variableGet();
  if (data_->isSet(*(data_->argdict_),"val")) 
    valueSet(data_->getString(*(data_->argdict_),"val"));

  connect(data_->slider_,SIGNAL(valueChanged(int)),
	  this,SLOT(stateChanged()));
  delete data_->argdict_;
  data_->argdict_=0;
  return data_->widget_;
}

int QtGuiSlider::valueGetInt()
{
  return (int)valueGetDouble();
}

double QtGuiSlider::valueGetDouble()
{ 
  return data_->slider_->value()*data_->step_;
}

QString QtGuiSlider::valueGetStr()
{
  return QString("").setNum(valueGetDouble());
}

void QtGuiSlider::valueSet(int val)
{
  data_->slider_->setValue(int(val/data_->step_+0.5));
}

void QtGuiSlider::valueSet(double val)
{
  data_->slider_->setValue(int(val/data_->step_+0.5));
}

void QtGuiSlider::valueSet(QString val)
{
  double x;
  QTextIStream(&val) >> x;
  valueSet(x);
}


void QtGuiSlider::variableGet()
{
  qDebug("QtGuiSlider::variableGet\n");
  if (!data_->variable_) return;
  valueSet(data_->variable_->valueGetDouble());
}

void QtGuiSlider::variableSet()
{
  qDebug("QtGuiSlider::variableSet\n");
  if (!data_->variable_) return;
  data_->variable_->valueSetDouble(valueGetDouble());
}

void QtGuiSlider::stateChanged()
{
  if (idata_->sending_) {
    qDebug("QtGuiSlider::stateChanged name=%s recursion detected\n",
	 (const char*)idata_->name_);
    return;
  }
  idata_->sending_=true;
  qDebug("QtGuiSlider::stateChanged() name=%s (%d)\n",
	 (const char*)idata_->name_,
	 valueGetInt());
  if (idata_->variable_ && idata_->immediately_) {
    idata_->variable_->valueSetDouble(valueGetDouble());
  }
  switch (data_->type_) {
  case QtGuiSliderPrivate::INT:
    emit valueChanged(valueGetInt());
    break;
  case QtGuiSliderPrivate::DOUBLE:
    emit valueChanged(valueGetDouble());
    break;
  }
  idata_->sending_=false;
}

/** set the min and max value for this slider */
void QtGuiSlider::setMinMax(int min, int max)
{
  qDebug("QtGuiSlider::setMinMax(%d,%d)",min,max);
  data_->slider_->setMinValue(min);
  data_->slider_->setMaxValue(max);
}



/*****************************************************************
CLASS: QtGuiSliderPrivate
*****************************************************************/
void QtGuiSliderPrivate::create(QWidget *parent, QLayout *layout)
{
  variable_=(*(_qtguiappl->varDict()))[varname_];
  widget_=slider_=new QSlider(parent,name_);
  setAttribs();
  addItem(layout,parent);

}

#ifndef TMAKECONF
#include "QtGuiSlider.moc"
#endif
