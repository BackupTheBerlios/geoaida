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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiApplication.h,v $
 * $Revision: 1.2 $
 * $Date: 2002/02/22 14:34:21 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef _QtGuiApplication_h
#define _QtGuiApplication_h
#include "QtGui.h"

class QtGuiApplPrivate;

class QtGuiApplication : public QtGui {
 public:
  QtGuiApplication(const QString& filename, QString defaultPrefix);
  QtGuiApplication(const QString& filename);
  QtGuiApplication(MLParser *parser=0, ArgDict* argdict=0);
  ~QtGuiApplication();
  void init();
  void read(const QString& filename, ArgDict* argdict=0);
  void read(MLParser *parser, ArgDict* argdict=0);
  virtual QWidget *widget();
  virtual QWidget *create(QWidget *parent=0, QLayout *layout=0);
  void showDialog(QString name);
  void hideDialog(QString name);
  void insertClass(const QString &name,QObject *obj) 
    {classDict_->insert(name,obj);}
  void insertVar(const QString &name, 
	         void *var_addr, int type, int len=0, int frac=-1) 
    {
      varDict_->insert(name,var_addr,type,len,frac);
    }
  void insertVarString(const QString &name, QString *var_addr, int len=0) 
    {
      varDict_->insert(name,var_addr,QtGuiVarModifier::STRING,len);
    }
  void insertVarInt(const QString &name, int *var_addr, int len=0) 
    {
      varDict_->insert(name,var_addr,QtGuiVarModifier::INT,len);
    }
  void insertVarDouble(const QString &name, double *var_addr, 
		       int len=0, int frac=-1) 
    {
      varDict_->insert(name,var_addr,QtGuiVarModifier::DOUBLE,len,frac);
    }
  QString absFilename(const QString& filename);
  QObject *getClass(const QString& name) {return (*classDict_)[name];}
  QtGuiClassDict *classDict() {return classDict_;}
  QtGuiVarDict *varDict() {return varDict_;}
  /** returns the prefix to the programm installation */
  QString prefix() {return prefix_;}
 private:
  QtGuiApplPrivate* data_;
  QtGuiClassDict *classDict_;
  QtGuiVarDict *varDict_;
  MLTagTable *tagTable_;
  QString directory_;
  QString prefix_;
};

extern QtGuiApplication *_qtguiappl;
#endif
