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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiPrivate.h,v $
 * $Revision: 1.1 $
 * $Date: 2001/07/31 11:57:04 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef _QtGuiPrivate_h
#define _QtGuiPribate_h

#include <qwidget.h>
#include <qstring.h>
#include <qpixmap.h>
#include <qtextstream.h>
#include <qtooltip.h>
#include <qaccel.h>
#include "MLParser.h"
#include "QtGuiApplication.h"
#include "QtGuiSignal.h"
#include "QtGuiConnect.h"

class QtGuiPrivate {
public:
  QtGuiPrivate();
  ~QtGuiPrivate(){};

  // function to set attributes
  void setString(QString &str, ArgDict *dict, QString name);
  void setBool(bool &var, ArgDict *dict, QString name);
  void setInt(int &var, ArgDict *dict, QString name);
  void setDouble(double &var, ArgDict *dict, QString name);
  void setSignal(QtGuiSignal*& s, ArgDict *dict, QString name);
  void setOption(int &var, ArgDict *dict, QString name, QString *options);
  void setAccel(int &var, ArgDict *dict, QString name);
  bool isSet(const ArgDict& dict,const QString& name);
  bool getBool(ArgDict& dict, const QString& name, 
	      const bool default_val=false);
  int getInt(ArgDict& dict, const QString& name, 
	     const int default_val=0);
  QString getString(ArgDict& dict, const QString& name, 
		    const QString default_val="");
  int getOption(ArgDict& dict, QString name, QString *options, int default_option=0);
  int getBitMask(ArgDict& dict, QString name, QString *options, 
		 int *optionvals=0, int default_val=0);
  void setVars(QString classname);
  void setAttribs();
  void regObj(QObject *obj);
  void connect(QObject *sender=0, const char* signal=0, 
	       QObject *receiver=0, const char* slot=0);
  void connectSignal(QObject *sender, const char *signal, 
		     QtGuiSignal *action, bool autotype=false);
  void connectSlot(QObject *receiver, const char *slot, 
		   QtGuiSignal *action, bool autotype=false);
  QWidget *widget_;
  bool sending_;

  // Attribute
#if 0
  int xpos_,ypos_;
  int minwidth_,minheight_;
  int maxwidth_,maxheight_;
#endif
  QString name_;
  QString label_;
  QString tip_;
  QPixmap *pixmap_;
  QList<QtGuiConnect> *connectionList_;
  ArgDict* argdict_;
  bool immediately_;
};



#define QTGUI_DO_ITEMS(type,list,function) \
 { \
  type *item; \
  item=list->first(); \
  while (item) { \
    item->function; \
    item=list->next(); \
  } \
 }

#endif
