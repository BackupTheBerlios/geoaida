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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiSignal.cpp,v $
 * $Revision: 1.3 $
 * $Date: 2002/02/22 14:34:21 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiSignal.h"
#include "QtGuiApplication.h"
#include "qregexp.h"

#include "QtGui.h"

QtGuiSignal::QtGuiSignal(ArgDict *dict, QString& name)
{
  siglist_.setAutoDelete(TRUE);
  set(dict,name);
}

QtGuiSignal::~QtGuiSignal()
{
}

void QtGuiSignal::set(ArgDict *dict, QString &name)
{
  QString *s;
  do {
    s=(*dict)[name];
    if (s) {
      qDebug("QtGuiSignal::set %s=%s\n",(const char*)name,(const char*)*s);
      siglist_.append(new QString(*s));
      dict->remove(name);
    }
  } while (s);
}


void QtGuiSignal::connectSignalString(QObject *sender, 
				      const char *signal, 
				      QString action)
{
  if (action.isEmpty()) return;
  if (sender==0) return;
  qDebug("Connecting %s to action %s\n",
	 (const char*)signal,
	 (const char*)action);
  QString receiverstr=SignalObject(action);
  QString slot=SignalSlot(action);
  if ((receiverstr.isEmpty()) || (slot.isEmpty())) return;
  QtGuiClassDict *classDict =_qtguiappl->classDict();
  QObject *receiver=(*classDict)[receiverstr];
  qDebug("Connecting %s\n",(const char*)receiverstr);
  if (!receiver) {
    qDebug("%s not found\n",(const char*)receiverstr);
    return; 
  }
  slot="1"+slot;
  qDebug("Connecting %s to %s.%s\n",
	 (const char*)signal,
	 (const char*)receiverstr,
	 (const char*)slot);
  
  QObject::connect(sender,signal,receiver,slot);
}

void QtGuiSignal::connectSignal(QObject *sender, 
				const char *signal, 
				bool autotype)
{
  QString *action;
  QString s;
  for (action=siglist_.first();action!=0; action=siglist_.next()) {
    s=signal;
    if (autotype) s+=signalType(*action);
    connectSignalString(sender,(const char*)s,*action);
  }
}

void QtGuiSignal::connectSlotString(QObject *receiver, 
				    const char *slot, 
				    QString action)
{
  if (action.isEmpty()) return;
  if (receiver==0) return;
  qDebug("Connecting action %s to %s\n",
	 (const char*)slot,
	 (const char*)action);
  QString senderstr=SignalObject(action);
  QString signal=SignalSlot(action);
  if ((senderstr.isEmpty()) || (signal.isEmpty())) return;
  QtGuiClassDict *classDict =_qtguiappl->classDict();
  QObject *sender=(*classDict)[senderstr];
  qDebug("Connecting %s\n",(const char*)senderstr);
  if (!sender) return;
  signal="2"+signal;
  qDebug("Connecting %s.%s to %s\n",
	 (const char*)senderstr,
	 (const char*)signal,
	 (const char*)slot);
  
  QObject::connect(sender,signal,receiver,slot);
}


void QtGuiSignal::connectSlot(QObject *receiver, 
			      const char *slot, 
			      bool autotype)
{
  QString *action;
  QString s;
  for (action=siglist_.first();action!=0; action=siglist_.next()) {
    s=slot;
    if (autotype) s+=signalType(*action);
    connectSlotString(receiver,(const char*)s,*action);
  }
  
}

QString QtGuiSignal::signalType(QString& signal)
{
#if QT_VERSION >= 300
  return signal.right(signal.length()-signal.findRev(QRegExp("\\(.*\\)")));
#else
  return signal.right(signal.length()-signal.findRev(QRegExp("(.*)")));
#endif
}

