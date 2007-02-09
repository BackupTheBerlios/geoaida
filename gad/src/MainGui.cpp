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
 * $Source: /data/cvs/gad/gad/MainGui.cpp,v $
 * $Revision: 1.4 $
 * $Date: 2002/07/19 04:52:52 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiApplication.h"
#include "MainGui.h"
#include <qdir.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <stdlib.h>

MainGui::MainGui()
{
  GadGuiInitObjectTable();
  //  centralWidget_.enableClipper(TRUE);
  ws_ = new GadWorkspace(centralWidget_.viewport());
  wsName_="default.wsp";
  centralWidget_.addChild(ws_);
}

MainGui::~MainGui()
{
}

void MainGui::readInit()
{
  QString configFile(getenv("HOME"));
  configFile += "/.gadrc";
  QFile fp(configFile);
  if (!fp.open(IO_ReadOnly)) {
    config_.insert("dir", new QString(PRGDIR "share/data/modules/"));
    return;
  }
  MLParser parser(&fp);
  QString keywords[] = { "MODULES", "" };
  MLTagTable tagtable(keywords);

  int tag;
  while ((tag = parser.tag(tagtable)) != MLParser::END_OF_FILE) {
    qDebug("tag=%d\n", tag);
    switch (tag) {
    case 1:
      parser.args(&config_);
      break;
    default:
      break;
    }
  }
  fp.close();

}

void MainGui::dumpModules()
{
  qDebug("DumpModules\n");
  QDictIterator < ModuleDict > cit(classDict_);
  for (; cit.current(); ++cit) {
    qDebug("Class %s\n", (const char *) cit.currentKey());
    ModuleDict *moduleDict = cit.current();
    QDictIterator < GadModule > mit(*moduleDict);
    for (; mit.current(); ++mit) {
      qDebug("  Module %s\n", (const char *) mit.currentKey());
    }
  }
}

void MainGui::readModule(const QString filename)
{
  qDebug("ReadModule %s\n", (const char *) filename);
  int tag;
  QFile fp(filename);
  if (!fp.open(IO_ReadOnly))
    return;
  MLParser parser(&fp);
  GadModule *gadgui = 0;
  qDebug("Modules reading\n");
  while ((tag = parser.tag(*_gadguitagtable)) != MLParser::END_OF_FILE) {
    qDebug("tag=%d (%s)\n", tag, (const char *) parser.lasttagstr());
    gadgui = (GadModule *) GadGuiNewObject(tag, &parser);
    if (!gadgui)
      continue;
    if (classDict_[gadgui->className()] == 0) {
      ModuleDict *moduleDict = new ModuleDict();
      classDict_.insert(gadgui->className(), moduleDict);
    }
    ModuleDict *moduleDict = classDict_[gadgui->className()];
    if ((*moduleDict)[gadgui->name()] != 0) {
      qDebug("Warning: Module %s redefined in %s\n",
             (const char *) gadgui->name(), (const char *) filename);
    }
    moduleDict->insert(gadgui->name(), gadgui);
  }
}

void MainGui::readModules()
{
  QDictIterator < QString > it(config_);
  for (; it.current(); ++it) {
    if (it.currentKey() == "dir") {
      qDebug("ReadModules %s\n", (const char *) *(it.current()));
      QDir d(*(it.current()), "*.mod");
      int i;
      for (i = 0; i < d.count(); i++) {
        readModule(*(it.current()) + "/" + d[i]);
      }
    }
  }
}

void MainGui::test()
{
  ws_->newWS(classDict_);
  return;
  GadObject *object;
  GadModule *module = GetModule(classDict_, "misc", "readpfm");
  if (module) {
    qDebug("Generating Module widget\n");
    object = new GadObject(module, ws_);
    object = new GadObject(module, ws_);
    object->move(150, 0);
    /*
       object=new GadObject(module,ws_);
       object->move(300,0);
       object=new GadObject(module,ws_);
       object->move(450,0);
       object=new GadObject(module,ws_);
       object->move(600,0);
       object=new GadObject(module,ws_);
       object->move(150,200);
     */
  }
  ws_->resize();
  ws_->save("test.ws");
}

void MainGui::newWorkspace()
{
  qDebug("MainGui::nmewWorkspace\n");
  delete ws_;
  ws_ = new GadWorkspace(centralWidget_.viewport());
  ws_->show();
  centralWidget_.setResizePolicy(QScrollView::Default);
  centralWidget_.addChild(ws_);
  ws_->newWS(classDict_);
  //  ws_->show();
  //  centralWidget_.setHScrollBarMode(QScrollView::AlwaysOn);
  //  centralWidget_.setVScrollBarMode(QScrollView::AlwaysOn);
  //  centralWidget_.setResizePolicy(QScrollView::AutoOne);
  //  centralWidget_.updateGeometry();
  //  centralWidget_.show();

}

void MainGui::loadWorkspace(QString filename)
{
  qDebug("MainGui::loadWorkspace\n");
  if (filename.isEmpty()) {
    filename = QFileDialog::getOpenFileName("", "*.wsp");
    if (filename.isEmpty())
      return;
  }
  delete ws_;
  wsName_=filename;
  ws_ = new GadWorkspace(centralWidget_.viewport());
  centralWidget_.setResizePolicy(QScrollView::Default);
  centralWidget_.addChild(ws_);
  ws_->load(classDict_, wsName_);
  _qtguiappl->widget()->setCaption("GAD ("+QFileInfo(wsName_).baseName()+")");
  qDebug("MainGui::loadWorkspace %s done\n",wsName_.latin1());
  ws_->show();

}

void MainGui::saveWorkspace()
{
  QString s = QFileDialog::getSaveFileName(wsName_, "*.wsp");
  if (!s.isEmpty())
    ws_->save(s);
}



