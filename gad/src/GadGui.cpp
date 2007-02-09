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
 * $Source: /data/cvs/gad/gad/GadGui.cpp,v $
 * $Revision: 1.5 $
 * $Date: 2002/11/06 06:49:42 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "GadGui.h"
#include <qregexp.h>

GadGui::GadGui()
{
  init();
}

GadGui::GadGui(MLParser & parser, ArgDict * argdict)
{
  init();
  parse(parser, argdict);
}

void GadGui::init()
{
  name_ = "";
  label_ = "";
  tip_ = "";
  name_ = "";
  x_ = 0;
  y_ = 0;
  widget_ = 0;
  argdict_ = 0;
}

GadGui::~GadGui()
{
  if (widget_)
    delete widget_;
  if (argdict_)
    delete argdict_;
}

void GadGui::parse(MLParser & parser, ArgDict * argdict)
{
  argdict_ = parser.args(argdict);
  parser.setString(name_, argdict, "name");
  parser.setString(label_, argdict, "label");
  parser.setString(tip_, argdict, "tip");
  parser.setInt(x_, argdict, "x");
  parser.setInt(y_, argdict, "x");
  cmd_ = "@" + name_ + "@";
  parser.setString(cmd_, argdict, "cmd");
}

QWidget *GadGui::create(QWidget * parent, QLayout * layout)
{
  qDebug("GadGui::create not implemented\n");
  return 0;
}

const QString GadGui::command(ArgDict & argdict)
{
  qDebug("GadGui::command %s %s\n",name_.latin1(),cmd_.latin1());
  QString cmd = cmd_;
  if (!argdict[name_] || *argdict[name_]=="")
    return "";
  QDictIterator < QString > it(argdict);
  for (; it.current(); ++it) {
    cmd.replace(QRegExp("@" + it.currentKey() + "@", false), *it.current());
  }
  qDebug("GadGui::command %s\n", (const char *) cmd);
  return cmd;
}

void GadGui::setValue(const QString *)
{
  qDebug("GadGui::setValue not implemented\n");
}

QString GadGui::getValue()
{
  qDebug("GadGui::getValue is virtual\n");
  return "";
}

void GadGui::show()
{
  qDebug("GadGui::show\n");
  widget_->show();
}

void GadGui::hide()
{

  widget_->hide();
  qDebug("GadGui::hide\n");
}

void GadGui::apply(void)
{
  qDebug("GadGui::apply not implemented\n");
}

void GadGui::cancel(void)
{
  qDebug("GadGui::cancel not implemented\n");
}


