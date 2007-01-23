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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiConnect.cpp,v $
 * $Revision: 1.2 $
 * $Date: 2001/11/06 13:23:14 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiConnect.h"
#include "QtGuiApplication.h"
#include "qregexp.h"

QtGuiConnect::QtGuiConnect(const QString& connection,
			   const QString& senderstr,
			   QObject *sender,
			   const QString& receiverstr,
			   QObject *receiver)
{
  senderObjectStr_="";
  senderMethod_="";
  receiverObjectStr_="";
  receiverMethod_="";
  sender_=0;
  receiver_=0;
  set(connection,senderstr,sender,receiverstr,receiver);
}

QtGuiConnect::~QtGuiConnect()
{
}

void QtGuiConnect::set(const QString& connection,
		       const QString& senderstr,
		       QObject *sender,
		       const QString& receiverstr,
		       QObject *receiver)
{
  qDebug("QtGuiConnect::set\n");
  int pos=connection.findRev('-');
  QString s,r;
  if (pos==-1) {
    s=connection;
    r="";
  }
  else {
    s=connection.left(pos);
    r=connection.right(connection.length()-pos-1);
  }
  qDebug("send=%s\nreceive=%s\n",(const char*)s,(const char*)r);
  QString so=objectStr(s);
  QString sm=methodStr(s);
  QString st=methodType(sm);
  QString ro=objectStr(r);
  QString rm=methodStr(r);
  QString rt=methodType(rm);
  qDebug("(%s, %s)->(%s, %s)\n",
	 (const char*)so, (const char*)sm,
	 (const char*)ro, (const char*)rm);
  if (so.isEmpty()) {
    so=senderstr;
    sender_=sender;
  }
  if (ro.isEmpty()) {
    ro=receiverstr;
    receiver_=receiver;
  }
  senderObjectStr_=so;
  senderMethod_=sm;
  receiverObjectStr_=ro;
  receiverMethod_=rm;
  qDebug("(%s, %s)->(%s, %s)\n",
	 (const char*)so, (const char*)sm,
	 (const char*)ro, (const char*)rm);
}

QString QtGuiConnect::methodType(const QString& s)
{
#if QT_VERSION >= 300
  int pos=s.findRev(QRegExp("\\(.*\\)"));
#else
  int pos=s.findRev(QRegExp("(.*)"));
#endif
  if (pos==-1) return QString("");
  return s.right(s.length()-pos);
}

QString QtGuiConnect::objectStr(const QString& s) 
{
  int pos=s.findRev('.');
  if (pos==-1) return QString("");
  return s.left(pos);
}

QString QtGuiConnect::methodStr(const QString& s) 
{
  int pos=s.findRev('.');
  if (pos==-1) return QString(s);
  return s.right(s.length()-pos-1);
}

void QtGuiConnect::connect(QObject* sender,
			   const char *signal,
			   QObject* receiver,
			   const char *slot)
{
  qDebug("QtGuiConnect::connect\n");
  QtGuiClassDict *classDict =_qtguiappl->classDict();
  if ((!sender_) && (!senderObjectStr_.isEmpty())) 
      sender_=(*classDict)[senderObjectStr_];
  if ((!receiver_) && (!receiverObjectStr_.isEmpty()))
    receiver_=(*classDict)[receiverObjectStr_];
  if ((!receiver_) && (!sender_) 
      && (receiverMethod_.isEmpty())
      && (senderMethod_.isEmpty())) return;
  if (!sender_) sender_=sender;
  if (!receiver_) receiver_=receiver;
  if (senderMethod_.isEmpty()) senderMethod_=signal;
  if (receiverMethod_.isEmpty()) receiverMethod_=slot;
  QString st=methodType(senderMethod_);
  QString rt=methodType(receiverMethod_);
  if (st.isEmpty() && rt.isEmpty()) {
    senderMethod_+="()";
    receiverMethod_+="()";
  } 
  else {
    if (st.isEmpty()) senderMethod_+=rt;
    if (rt.isEmpty()) receiverMethod_+=st;
  }
  qDebug("(%s(0x%08x).%s)->(%s(0x%08x).%s))\n",
	 (const char*)senderObjectStr_,
	 sender_,
	 (const char*)senderMethod_,
	 (const char*)receiverObjectStr_,
	 receiver_,
	 (const char*)receiverMethod_);
  if ((!sender_) || (!receiver_)) return;
  QObject::connect(sender_,"2"+senderMethod_,
		   receiver_,"1"+receiverMethod_);

}

