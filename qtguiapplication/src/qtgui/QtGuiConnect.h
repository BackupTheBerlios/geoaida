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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiConnect.h,v $
 * $Revision: 1.1 $
 * $Date: 2001/07/31 11:57:04 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef _QtGuiConnect_h
#define _QtGuiConnect_h
 
#include <qstring.h>
#include <qobject.h>
#include "MLParser.h"

class QtGuiConnect {
 public:
  QtGuiConnect(const QString& connection,
	       const QString& senderstr="",
	       QObject *sender=0,
	       const QString& receiverstr="",
	       QObject *receiver=0);
  ~QtGuiConnect();
  void set(const QString& connection,
	   const QString& senderstr="",
	   QObject *sender=0,
	   const QString& receiverstr="",
	   QObject *receiver=0);
  void connect(QObject* sender,
	       const char *signal,
	       QObject* receiver,
	       const char *slot);
  static QString methodType(const QString& signal);
 protected:
  static QString objectStr(const QString& s);
  static QString methodStr(const QString& s);
 private:
   QString senderObjectStr_;
   QString senderMethod_;
   QString receiverObjectStr_;
   QString receiverMethod_;
   QObject *sender_;
   QObject *receiver_;
};

#endif
