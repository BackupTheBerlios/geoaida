// GAD - Library 
// Copyright (C) 2000 Martin Pahl, Universität Hannover
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
 * $Source: /data/cvs/gad/gad/GadEnum.cpp,v $
 * $Revision: 1.3 $
 * $Date: 2002/07/05 04:43:27 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "GadEnum.h"
#include <qhbox.h>
#include <qtooltip.h>
#include <qpushbutton.h>
#include <qfiledialog.h>

GadEnum *newGadEnum(MLParser & parser, ArgDict * argdict)
{
  return new GadEnum(parser, argdict);
}

GadEnum::GadEnum()
{
  init();
}

GadEnum::GadEnum(MLParser & parser, ArgDict * argdict)
{
  init();
  parse(parser, argdict);
}

void GadEnum::init()
{
  cmd_ = "";
  value_ = "";
}

GadEnum::~GadEnum()
{
  if (widget_)
    delete widget_;
}

void GadEnum::parse(MLParser & parser, ArgDict * argdict)
{
  qDebug("GadEnum::parse\n");
  argdict_ = parser.args(argdict);
  GadGui::parse(parser, argdict_);
  parser.setString(value_, argdict_, "value");
  parser.setString(cmd_, argdict_, "cmd");
  QString val;
  MLParser::setString(val,argdict_,"options");
  options_ = QStringList::split(",", val);
  MLParser::setString(val,argdict_,"optionslabel");
  optionsLabel_ = QStringList::split(",", val);
}

QWidget *GadEnum::create(QWidget * parent, QLayout * layout)
{
  qDebug("GadEnum::create\n");

  widget_ = new QHBox(parent);
  ((QHBox *) widget_)->setSpacing(5);
  if (label_.isEmpty())
    label_ = name_;
  qDebug("        label=%s\n", (const char *) label_);
  new QLabel(label_, widget_);
  editor_ = new QComboBox(widget_);
  editor_->insertStringList(optionsLabel_);
#if 0
  lineEdit_ = new QLineEdit(widget_);
  lineEdit_->setMinimumWidth(100);
#endif
  QToolTip::add(widget_, tip_);
  return widget_;
}

void GadEnum::setValue(const QString * value)
{
  if (value)
    value_ = *value;
  else
    value_ = "";
  if (widget_)
    editor_->setCurrentItem(options_.findIndex(value_));
}

QString GadEnum::getValue()
{
  qDebug("GadGui::getValue not implemented\n");
  if (widget_) {
    QString val=options_[optionsLabel_.findIndex(editor_->currentText())];
    return val;
  }
  return QString("");
}

void GadEnum::show()
{
  qDebug("GadEnum::show\n");
  widget_->show();
}

void GadEnum::hide()
{
  widget_->hide();
  qDebug("GadEnum::hide\n");
}

void GadEnum::apply(void)
{
  qDebug("GadEnum::apply not implemented\n");
}

void GadEnum::cancel(void)
{
  qDebug("GadEnum::cancel not implemented\n");
}


