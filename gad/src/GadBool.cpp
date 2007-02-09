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
 * $Source: /data/cvs/gad/gad/GadBool.cpp,v $
 * $Revision: 1.1 $
 * $Date: 2002/07/05 05:14:54 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "GadBool.h"
#include <qhbox.h>
#include <qtooltip.h>
#include <qpushbutton.h>
#include <qfiledialog.h>

GadBool *newGadBool(MLParser & parser, ArgDict * argdict)
{
  return new GadBool(parser, argdict);
}

GadBool::GadBool()
{
  init();
}

GadBool::GadBool(MLParser & parser, ArgDict * argdict)
{
  init();
  parse(parser, argdict);
}

void GadBool::init()
{
  cmd_ = "";
  value_ = "";
}

GadBool::~GadBool()
{
  if (widget_)
    delete widget_;
}

void GadBool::parse(MLParser & parser, ArgDict * argdict)
{
  qDebug("GadBool::parse\n");
  argdict_ = parser.args(argdict);
  GadGui::parse(parser, argdict_);
  parser.setString(value_, argdict_, "value");
  parser.setString(cmd_, argdict_, "cmd");
  MLParser::setString(on_,argdict_,"on");
  MLParser::setString(off_,argdict_,"off");
}

QWidget *GadBool::create(QWidget * parent, QLayout * layout)
{
  qDebug("GadBool::create\n");

  widget_ = new QHBox(parent);
  ((QHBox *) widget_)->setSpacing(5);
  if (label_.isEmpty())
    label_ = name_;
  qDebug("        label=%s\n", (const char *) label_);
  new QLabel(label_, widget_);
  editor_ = new QCheckBox(widget_);
  QToolTip::add(widget_, tip_);
  return widget_;
}

void GadBool::setValue(const QString * value)
{
  if (value)
    value_ = *value;
  else
    value_ = "";
  if (widget_)
    editor_->setChecked(value_==on_);
}

QString GadBool::getValue()
{
  qDebug("GadGui::getValue not implemented\n");
  if (widget_) {
    if (editor_->isChecked()) return on_;
    else return off_;
  }
  return QString("");
}

void GadBool::show()
{
  qDebug("GadBool::show\n");
  widget_->show();
}

void GadBool::hide()
{
  widget_->hide();
  qDebug("GadBool::hide\n");
}

void GadBool::apply(void)
{
  qDebug("GadBool::apply not implemented\n");
}

void GadBool::cancel(void)
{
  qDebug("GadBool::cancel not implemented\n");
}

