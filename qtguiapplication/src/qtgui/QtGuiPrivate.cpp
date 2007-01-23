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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiPrivate.cpp,v $
 * $Revision: 1.2 $
 * $Date: 2001/12/20 13:40:20 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiPrivate.h"
#include <qapp.h>
QtGuiPrivate::QtGuiPrivate() {
#if 0
  xpos_=ypos_=0;
  minwidth_=minheight_=0;
  maxwidth_=maxheight_=0;
#endif
  name_="";
  label_="";
  tip_="";
  pixmap_=0;
  connectionList_=0;
  widget_=0;
  sending_=false;
  immediately_=false;
  argdict_=0;
}

void QtGuiPrivate::setString(QString &str, ArgDict *dict, QString name) 
{
  MLParser::setString(str,dict,name);
}


void QtGuiPrivate::setBool(bool &var, ArgDict *dict, QString name) 
{
  MLParser::setBool(var,dict,name);
}

void QtGuiPrivate::setInt(int &var, ArgDict *dict, QString name) 
{
  MLParser::setInt(var,dict,name);
}

void QtGuiPrivate::setDouble(double &var, ArgDict *dict, QString name) 
{
  MLParser::setDouble(var,dict,name);
}

void QtGuiPrivate::setSignal(QtGuiSignal*& s, ArgDict *dict, 
				    QString name) 
{
      if (s) s->set(dict,name);
      else s=new QtGuiSignal(dict,name);
}

void QtGuiPrivate::setOption(int &var, ArgDict *dict, 
				    QString name, QString *options)
{
  MLParser::setOption(var,dict,name,options);
}

void QtGuiPrivate::setAccel(int &var, ArgDict *dict, QString name)
{
  QString *s=(*dict)[name];
  if (s) {
    var=QAccel::stringToKey(*s);
    qDebug("setAccel: %s=%08x(%d)\n",(const char*)*s,var,var);
  }
}

bool QtGuiPrivate::isSet(const ArgDict& dict,const QString& name)
{
  return (dict[name]!=0);
}

bool QtGuiPrivate::getBool(ArgDict& dict, const QString& name, 
			  const bool default_val)
{
  bool var=default_val;
  MLParser::setBool(var,&dict,name);
  return var;
}

int QtGuiPrivate::getInt(ArgDict& dict, const QString& name, 
			 int default_val)
{
  int var=default_val;
  MLParser::setInt(var,&dict,name);
  return var;
}

QString QtGuiPrivate::getString(ArgDict& dict, const QString& name, 
				QString default_val)
{
  QString var=default_val;
  MLParser::setString(var,&dict,name);
  return var;
}

int QtGuiPrivate::getOption(ArgDict& dict, QString name, QString *options, 
			    int default_val)
{
  int var=default_val;
  MLParser::setOption(var,&dict,name,options);
  return var;
}

int QtGuiPrivate::getBitMask(ArgDict& dict, QString name, QString *options, 
			     int *optionvals, int default_val)
{
  int var=default_val;
  MLParser::setBitMask(var,&dict,name,options,optionvals);
  return var;
}

void QtGuiPrivate::setVars(QString classname)
{
  setString(name_,argdict_,"name");
  if (!name_.isEmpty()) classname=name_;
  setString(label_,argdict_,"label");
  if (!label_.isEmpty())label_=qApp->translate(classname,label_);
  setString(tip_,argdict_,"tip");
  if (!tip_.isEmpty()) tip_=qApp->translate(classname,tip_);
  {
    QString s="";
    setString(s,argdict_,"pixmap");
    if (!s.isEmpty()) {
      s=_qtguiappl->absFilename(s);
      pixmap_=new QPixmap(s,0,0);
    }
  }
#if 0
  setInt(xpos_,argdict_,"x");
  setInt(ypos_,argdict_,"y");
  setInt(minwidth_,argdict_,"minwidth");
  setInt(minheight_,argdict_,"minheight");
  setInt(maxwidth_,argdict_,"maxwidth");
  setInt(maxheight_,argdict_,"maxheight");
#endif
  if ((*argdict_)["connect"]) {
    connectionList_=new QList<QtGuiConnect>();
    connectionList_->setAutoDelete(TRUE);
    QString *s;
    while ((s=(*argdict_)["connect"])) {
      connectionList_->append(new QtGuiConnect(*s));
      argdict_->remove("connect");
    }
  }
  setBool(immediately_,argdict_,"immediately");
}

void QtGuiPrivate::setAttribs()
{
  if (!widget_) return;
  if (isSet(*argdict_,"MINWIDTH"))
    widget_->setMinimumWidth(getInt(*argdict_,"MINWIDTH",0));
  if (isSet(*argdict_,"MINHEIGHT"))
    widget_->setMinimumHeight(getInt(*argdict_,"MINHEIGHT",0));
  if (isSet(*argdict_,"MAXWIDTH"))
    widget_->setMaximumWidth(getInt(*argdict_,"MAXWIDTH",0));
  if (isSet(*argdict_,"MAXHEIGHT"))
    widget_->setMaximumHeight(getInt(*argdict_,"MAXHEIGHT",0));
  if (!tip_.isEmpty()) QToolTip::add(widget_,tip_);
  if (getBool(*argdict_,"FIXEDSIZE"))
     widget_->setFixedSize(widget_->size()); 
  if (isSet(*argdict_,"FIXEDWIDTH")) {
    if ((getInt(*argdict_,"FIXEDWIDTH",-1)) != -1)
     widget_->setFixedWidth(getInt(*argdict_,"FIXEDWIDTH",-1));
    else      
     widget_->setFixedWidth(widget_->width()); 
  }
  if (isSet(*argdict_,"FIXEDHEIGHT")) {
    if ((getInt(*argdict_,"FIXEDHEIGHT",-1)) != -1)
     widget_->setFixedHeight(getInt(*argdict_,"FIXEDHEIGHT",-1));
    else      
     widget_->setFixedHeight(widget_->height()); 
  }
  { // Size Policy
    int sizePolicyX=0;
    int sizePolicyY=0;
    QString options[]={"FIXED","MINIMUM","MAXIMUM","PREFERRED","MINIMUMEXPANDING","EXPANDING","IGNORED",""};
    QSizePolicy::SizeType sizeType[]={QSizePolicy::Preferred,
                                      QSizePolicy::Fixed,
                                      QSizePolicy::Minimum,
                                      QSizePolicy::Maximum,
                                      QSizePolicy::Preferred,
                                      QSizePolicy::MinimumExpanding,
                                      QSizePolicy::Expanding,
                                      QSizePolicy::Ignored};
    setOption(sizePolicyX,argdict_,"sizepolicyx",options);
    setOption(sizePolicyY,argdict_,"sizepolicyy",options);
    if (sizePolicyX||sizePolicyY) {
      widget_->setSizePolicy(QSizePolicy(sizeType[sizePolicyX],sizeType[sizePolicyY]));
    }

  }
}

void QtGuiPrivate::regObj(QObject *obj)
{
  if (name_.isEmpty()) return;
  _qtguiappl->classDict()->insert(name_,obj);
  qDebug("register %s\n",(const char*)name_);
}


void QtGuiPrivate::connectSignal(QObject *sender, 
					const char *signal, 
					QtGuiSignal *action,
					bool autotype)
{
  if (!action) return;
  if (!sender) return;
  action->connectSignal(sender,signal,autotype);
}

void QtGuiPrivate::connectSlot(QObject *receiver, 
				      const char *slot, 
				      QtGuiSignal *action, 
				      bool autotype)
{
  if (!action) return;
  if (!receiver) return;
  action->connectSlot(receiver,slot,autotype);
}

void QtGuiPrivate::connect(QObject *sender, const char* signal, 
	       QObject *receiver, const char* slot)
{
  if (!connectionList_) return;
  QtGuiConnect *c;
  for (c=connectionList_->first(); c!=0; c=connectionList_->next())
    {
      c->connect(sender,signal,receiver,slot);
    }
  
}
