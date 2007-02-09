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
 * $Source: /data/cvs/gad/gad/main.cpp,v $
 * $Revision: 1.4 $
 * $Date: 2002/07/19 04:52:52 $
 * $Author: pahl $
 * $Locker:  $
 */

#include <qtgui.h>
#include "MainGui.h"
#include <qmainwindow.h>
#include <qwindowsstyle.h>
#include <qapp.h>
#include <qslider.h>
#include <qlabel.h>
#include <qstatusbar.h>
#include "GadIO.h"
#include "ClassDict.h"
#include "GadObject.h"
#include "GadWorkspace.h"
#include <stdlib.h>
#include <qfileinfo.h>
#include <qdir.h>
#include <unistd.h>

int main(int argc, char **argv)
{
#ifdef MUELL
  double testdouble = 100;
  int testdouble1 = 50;
  int sender = 2;
  int option = 2;
#endif

  // Read application file
  QString homedir(getenv("GAD"));
  qDebug("homedir=%s\n", (const char *) homedir);
  if (homedir.isEmpty())
    homedir = PRGDIR;
  qDebug("homedir=%s\n", (const char *) homedir);
  // Qt-Frame definieren
  QApplication app(argc, argv);
  QString guifilename;
#ifdef WIN32
  QFile f(homedir + "share/data/application/gad_win32.app");
  if (f.exists())
    guifilename = "share/data/application/gad_win32.app";
  else
#endif
    guifilename = "share/data/application/gad.app";
  QtGuiApplication qtguiapplication(guifilename, PRGDIR);
  QApplication::setStyle(new QWindowsStyle);

  MainGui mainGui;
  mainGui.readInit();
  mainGui.readModules();
  qDebug("Modules read\n");


  // Register classes
  qtguiapplication.insertClass("qapp", &app);
  qtguiapplication.insertClass("centralWidget", mainGui.centralWidget());
  qtguiapplication.insertClass("gad", &mainGui);



  // Create Application
  QMainWindow *mainwidget = (QMainWindow *) qtguiapplication.create();

  mainGui.test();
  mainwidget->show();
  app.setMainWidget(mainwidget);
#ifdef TEST
  if (gadgui) {
    gadgui->create(mainwidget);
    gadgui->show();
  }
#endif

  if (argc>1) {
    mainGui.loadWorkspace(argv[1]);
  }

  app.exec();

  mainGui.dumpModules();

  qDebug("%s\n", TMPDIR);

  QString s(QString().sprintf("rm -rf %s", GadGetTmpDirPID().latin1()));

  qDebug("%s\n", (const char *) s);
  system((const char *) s);

  return 0;
}
