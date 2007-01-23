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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiSignal.h,v $
 * $Revision: 1.1 $
 * $Date: 2001/07/31 11:57:04 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef _QtGuiSignal_h
#define _QtGuiSignal_h
 
#include <qstring.h>
#include <qobject.h>
#include <qlist.h>
#include "MLParser.h"

class QtGuiSignal {
 public:
  QtGuiSignal(ArgDict *dict, QString& name);
  ~QtGuiSignal();
  void set(ArgDict *dict, QString &name);
  void connectSignal(QObject *sender, const char *signal, bool autotype=false);
  void connectSlot(QObject *receiver, const char *slot, bool autotype=false);
  static QString signalType(QString& signal);
 protected:
  static QString SignalObject(QString signal) {
    return signal.left(signal.findRev('.'));
  }
  static QString SignalSlot(QString signal) {
    return signal.right(signal.length()-signal.findRev('.')-1);
  }
  void connectSignalString(QObject *sender, const char *signal, 
			   QString action);
  void connectSlotString(QObject *receiver,
			 const char *slot, 
			 QString action);
 private:
   QList<QString> siglist_;
};

#endif
