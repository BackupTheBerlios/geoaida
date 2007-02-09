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
 * $Source: /data/cvs/gad/gad/GadIO.cpp,v $
 * $Revision: 1.4 $
 * $Date: 2002/08/07 14:22:48 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "GadIO.h"
#include <qhbox.h>
#include <qtooltip.h>
#include <qpushbutton.h>
#include <qfiledialog.h>

GadIO *newGadIO(MLParser & parser, ArgDict * argdict)
{
  return new GadIO(parser, argdict);
}

GadIO::GadIO()
{
  init();
}

GadIO::GadIO(MLParser & parser, ArgDict * argdict)
{
  init();
  parse(parser, argdict);
}

void GadIO::init()
{
  cmd_ = "";
  value_ = "";
  optional_ = false;
  nocheck_ = false;
}

GadIO::~GadIO()
{
  if (widget_)
    delete widget_;
}

void GadIO::parse(MLParser & parser, ArgDict * argdict)
{
  qDebug("GadIO::parse\n");
  argdict_ = parser.args(argdict);
  GadGui::parse(parser, argdict_);
  if (parser.lasttagstr().upper() == "INPUT")
    mode_ = INPUT;
  else
    mode_ = OUTPUT;
  parser.setString(value_, argdict_, "value");
  parser.setBool(optional_, argdict_, "optional");
  if (optional_)
    nocheck_ = true;
  parser.setBool(nocheck_, argdict_, "nocheck");
  parser.setString(depend_, argdict_, "depend");
}

QWidget *GadIO::create(QWidget * parent, QLayout * layout)
{
  qDebug("GadIO::create\n");

  widget_ = new QHBox(parent);
  ((QHBox *) widget_)->setSpacing(5);
  if (label_.isEmpty())
    label_ = name_;
  qDebug("        label=%s\n", (const char *) label_);
  QPushButton *button = new QPushButton(label_, widget_);
  connect(button, SIGNAL(clicked()), this, SLOT(browse()));
  lineEdit_ = new QLineEdit(widget_);
  lineEdit_->setMinimumWidth(100);
  QToolTip::add(widget_, tip_);
  return widget_;
}

void GadIO::browse()
{
  qDebug("GadIO::browse\n");

  switch (mode_) {
  case INPUT:
    qDebug("      INPUT\n");
    lineEdit_->setText(QFileDialog::getOpenFileName(lineEdit_->text()));
    break;
  case OUTPUT:
    qDebug("      OUTPUT\n");
    lineEdit_->setText(QFileDialog::getSaveFileName(lineEdit_->text()));
    break;
  }
}

void GadIO::setValue(const QString * value)
{
  if (widget_) {
    if (value)
      lineEdit_->setText(*value);
    else
      lineEdit_->setText(value_);
  }
}

QString GadIO::getValue()
{
  qDebug("GadIO::getValue\n");
  if (widget_)
    return lineEdit_->text();
  return "";
}

void GadIO::show()
{
  qDebug("GadIO::show\n");
  widget_->show();
}

void GadIO::hide()
{
  widget_->hide();
  qDebug("GadIO::hide\n");
}

void GadIO::apply(void)
{
  qDebug("GadIO::apply not implemented\n");
}

void GadIO::cancel(void)
{
  qDebug("GadIO::cancel not implemented\n");
}

