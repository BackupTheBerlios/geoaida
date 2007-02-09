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
 * $Source: /data/cvs/gad/gad/GadText.cpp,v $
 * $Revision: 1.3 $
 * $Date: 2002/07/05 04:43:27 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "GadText.h"
#include <qhbox.h>
#include <qtooltip.h>
#include <qpushbutton.h>
#include <qfiledialog.h>

GadText *newGadText(MLParser & parser, ArgDict * argdict)
{
  return new GadText(parser, argdict);
}

GadText::GadText()
{
  init();
}

GadText::GadText(MLParser & parser, ArgDict * argdict)
{
  init();
  parse(parser, argdict);
}

void GadText::init()
{
  cmd_ = "";
  value_ = "";
}

GadText::~GadText()
{
  if (widget_)
    delete widget_;
}

void GadText::parse(MLParser & parser, ArgDict * argdict)
{
  qDebug("GadText::parse\n");
  argdict_ = parser.args(argdict);
  GadGui::parse(parser, argdict_);
  parser.setString(value_, argdict_, "value");
  parser.setString(cmd_, argdict_, "cmd");
}

QWidget *GadText::create(QWidget * parent, QLayout * layout)
{
  qDebug("GadText::create\n");

  widget_ = new QHBox(parent);
  ((QHBox *) widget_)->setSpacing(5);
  if (label_.isEmpty())
    label_ = name_;
  qDebug("        label=%s\n", (const char *) label_);
  new QLabel(label_, widget_);
  lineEdit_ = new QLineEdit(widget_);
  lineEdit_->setMinimumWidth(100);
  QToolTip::add(widget_, tip_);
  return widget_;
}

void GadText::setValue(const QString * value)
{
  if (widget_) {
    if (value)
      lineEdit_->setText(*value);
    else
      lineEdit_->setText(value_);
  }
}

QString GadText::getValue()
{
  qDebug("GadText::getValue\n");
  if (widget_)
    return lineEdit_->text();
  return QString("");
}

void GadText::show()
{
  qDebug("GadText::show\n");
  widget_->show();
}

void GadText::hide()
{
  widget_->hide();
  qDebug("GadText::hide\n");
}

void GadText::apply(void)
{
  qDebug("GadText::apply not implemented\n");
}

void GadText::cancel(void)
{
  qDebug("GadText::cancel not implemented\n");
}


