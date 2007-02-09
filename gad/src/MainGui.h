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
 * $Source: /data/cvs/gad/gad/MainGui.h,v $
 * $Revision: 1.3 $
 * $Date: 2002/07/19 04:52:52 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef maingui_h
#define maingui_h
#include "GadWorkspace.h"
#include <qscrollview.h>
#include <qarray.h>
#include <qpopupmenu.h>

class MainGui:public QObject
{
Q_OBJECT public:
  MainGui();
  ~MainGui();
  void readInit();
  void readModule(const QString filename);
  void readModules();
  void dumpModules();
  void test();
  QWidget *centralWidget();
public slots:
  void newWorkspace();
  void loadWorkspace(QString filename="");
  void saveWorkspace();
private:
    ClassDict classDict_;
  QScrollView centralWidget_;
  GadWorkspace *ws_;
  ArgDict config_;
  QString wsName_;
};

inline QWidget *MainGui::centralWidget()
{
  return &centralWidget_;
}

#endif
