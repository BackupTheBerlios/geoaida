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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiApplication.cpp,v $
 * $Revision: 1.4 $
 * $Date: 2002/11/06 06:49:27 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiApplication.h"
#include "QtGuiMenu.h"
#include "QtGuiToolBar.h"
#include "QtGuiCentralWidget.h"
#include "QtGuiDockWindow.h"
#include "QtGuiStatusBar.h"
#include "QtGuiDialog.h"
#include "QtGuiPrivate.h"
#include <qapplication.h>
#include <qwindowsstyle.h>
#include <qmotifstyle.h>
#include <qptrlist.h>
#include <qmenubar.h>
#include <qtoolbar.h>
#include <qmainwindow.h>
#include <qstring.h>
#include <qregexp.h>
#include <qtextstream.h>
#include <qfile.h>
#include <qdir.h>
#include <qfileinfo.h>

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

// #define DEBUG_MSG

QtGuiApplication *_qtguiappl = 0;

static QString getPrefix(QString testFile, QString defaultPrefix)
{
  QString procPath;
  procPath.sprintf("/proc/%d/exe",getpid());
  QFileInfo li(procPath);
  if (!li.isSymLink()) return "";
  QFileInfo fi(li.readLink());
  QDir dir=fi.dir();
  dir.cdUp();
  QString prefix=dir.absPath();
  qDebug("prefix=%s",prefix.latin1());
  if (!QFile::exists(prefix+"/"+testFile)) {
    if (QFile::exists(defaultPrefix+"/"+testFile))
      prefix=defaultPrefix;
    else {
      qFatal("Can't find application file\n"
             "  %s/%s\n"
             "or\n"
             "  %s/%s\n",
             prefix.latin1(),testFile.latin1(),
             defaultPrefix.latin1(),testFile.latin1());
      exit(-1);
    }
  }
  return prefix;
}

class QtGuiApplPrivate : public QtGuiPrivate {
public:
  QtGuiApplPrivate() {
    menu_=new QDict<QtGuiMenu>(17,false);
    menu_->setAutoDelete(true);
    toolbar_=new QDict<QtGuiToolBar>(17,false);
    statusbar_=new QDict<QtGuiStatusBar>(17,false);
    centralwidget_=new QDict<QtGuiCentralWidget>(17,false);
    toolbar_->setAutoDelete(true);
    dialog_=new QDict<QtGuiDialog>(17,false);
    dialog_->setAutoDelete(true);
		dockwindow_=new QPtrList<QtGuiDockWindow>();
	  dockwindow_->setAutoDelete(true);
    name_="qtApplication";
    label_="Qt-Application";
    mainwidget_="";
    style_=0;
  }
  void setVars(QString classname) {
    QtGuiPrivate::setVars(classname);
    QString options[]={"Windows","Motif",""};
    setOption(style_,argdict_,"style",options);
    setString(mainwidget_,argdict_,"mainwidget");
  }
  void setAttribs() {
    //    QtGuiPrivate::setAttribs();
    widget_->setCaption(label_);
    widget_->setIconText(label_);
    if (pixmap_) widget_->setIcon(*pixmap_);
#if 0	
    {
      QObject *widget=(*_qtguiappl->classDict())[mainwidget_];
      if (widget) {
	if (widget->inherits("QWidget")) {
#ifdef DEBUG_MSG	
	  qDebug("QtGuiApplication::setAttribs()\n");
#endif
	  ((QWidget*)widget)->reparent(widget_,0,QPoint(0,0));
	  ((QMainWindow*)widget_)->setCentralWidget((QWidget*)widget);
	}
      }
       
    }
#endif
  }
  void createMenu(QString name, QWidget *parent);
  void connectMenu(QString name);
  void createToolBar(QString name, QWidget *parent);
  void connectToolBar(QString name);
  void createDialog(QString name, QWidget *parent);
  void connectDialog(QString name);
  void createCentralWidget(QString name, QWidget *parent);
  void connectCentralWidget(QString name);
  void createStatusBar(QString name, QWidget *parent);
  void connectStatusBar(QString name);
  QDict<QtGuiMenu> *menu_;
  QDict<QtGuiDialog> *dialog_;
  QDict<QtGuiToolBar> *toolbar_;
  QDict<QtGuiCentralWidget> *centralwidget_;
  QDict<QtGuiStatusBar> *statusbar_;
  QPtrList<QtGuiDockWindow> *dockwindow_;
  // Attribs
  int style_;
  QString mainwidget_;
};

/*****************************************************************
CLASS: QtGuiAppl
*****************************************************************/
void QtGuiApplication::init()
{
#ifdef DEBUG_MSG
  qDebug("QtGuiApplication::init %s\n",SLOT($));
#endif
  if (!_qtguiappl) {
    _qtguiappl=this;
    QtGuiInitObjectTable();
  }
  classDict_=new QtGuiClassDict();
  varDict_=new QtGuiVarDict();
  
  gdata_=data_=new QtGuiApplPrivate;
  directory_=QDir::currentDirPath();
  prefix_="";
#ifdef DEBUG_MSG
  qDebug("QtGuiApplication::init() currentdir=%s\n",
	 (const char*)directory_);
#endif
}

QtGuiApplication::QtGuiApplication(MLParser* parser, ArgDict* argdict)
{
  init();
  if (!parser) return;
  read(parser,argdict);
}

QtGuiApplication::QtGuiApplication(const QString& filename, QString defaultPrefix)
{
  init();
  prefix_=getPrefix(filename,defaultPrefix);
  read(prefix_+"/"+filename);
}

QtGuiApplication::QtGuiApplication(const QString& filename)
{
  init();
  read(filename);
}

void QtGuiApplication::read(const QString& filename, ArgDict* argdict) 
{
#ifdef WIN32
  QFileInfo fileinfo(filename);
  directory_=fileinfo.dirPath(TRUE);
#else
  directory_=(QFileInfo(filename)).dirPath(TRUE);
#endif
#ifdef DEBUG_MSG
  qDebug("QtGuiApplication::read() currentdir=%s\n",
	 (const char*)directory_);
#endif
  QFile fp(filename);
  if (!fp.open(IO_ReadOnly)) return;
  MLParser parser(&fp);
  read(&parser);
}

void QtGuiApplication::read(MLParser *parser, ArgDict* argdict)
{
  int tag;
  QtGuiMenu *menu;
  QtGuiToolBar *toolbar;
  QtGuiCentralWidget *centralwidget;
  QtGuiStatusBar *statusbar;
  QtGuiDialog *dialog;
  QtGuiDockWindow *dockwindow;
  tag=parser->tag(*_qtguitagtable);
  if (tag!=TAG_APPLICATION) {
    qWarning("QtGuiApplication::QtGuiApplication: First tag should be <APPLICATION>\n");
#ifndef WIN32
    exit(0);
#endif
  }
    
  data_->argdict_=parser->args(argdict);
  data_->setVars("APPLICATION");
  
  do {
    tag=parser->tag(*_qtguitagtable);
#ifdef DEBUG_MSG
    qDebug("LastTag=%s\n",(const char*)parser->lasttagstr());
#endif

    switch (tag) {
    case TAG_MENU:
      menu=(QtGuiMenu*)QtGuiNewObjectId(tag,parser);
      data_->menu_->insert(menu->name(),menu);
      break;
    case TAG_TOOLBAR:
      toolbar=(QtGuiToolBar*)QtGuiNewObjectId(tag,parser);
      data_->toolbar_->insert(toolbar->name(),toolbar);
      break;
    case TAG_CENTRALWIDGET:
      centralwidget=(QtGuiCentralWidget*)QtGuiNewObjectId(tag,parser);
      data_->centralwidget_->insert(centralwidget->name(),centralwidget);
      break;
    case TAG_STATUSBAR:
      statusbar=(QtGuiStatusBar*)QtGuiNewObjectId(tag,parser);
      data_->statusbar_->insert(statusbar->name(),statusbar);
      break;
    case TAG_DIALOG:
      dialog=(QtGuiDialog*)QtGuiNewObjectId(tag,parser);
      data_->dialog_->insert(dialog->name(),dialog);
      break;
    case TAG_DOCKWINDOW:
      dockwindow=(QtGuiDockWindow*)QtGuiNewObjectId(tag,parser);
      data_->dockwindow_->append(dockwindow);
      break;
    case MLParser::END_OF_FILE:
      break;
    default:
      if (tag<0) tag=-tag;
      qWarning("QtGuiApplication: tag %s unknown",
	     (tag==0 ? "UNKNOWN" : (const char*)parser->lasttagstr()));
      delete parser->args();
    }
  } while (tag!=MLParser::END_OF_FILE);
    if (data_->style_==1)
      QApplication::setStyle(new QWindowsStyle());
    else
      QApplication::setStyle(new QMotifStyle());
}

QtGuiApplication::~QtGuiApplication()
{
}

QWidget *QtGuiApplication::widget()
{
  return data_->widget_;
}

QWidget *QtGuiApplication::create(QWidget*, QLayout*)
{
#ifdef DEBUG_MSG
  qDebug("QtGuiApplication::create\n");
#endif
  data_->widget_=new QMainWindow(0,data_->name_);
  data_->createMenu("mainmenu",data_->widget_);
  data_->createToolBar("toolbar",data_->widget_);
  data_->createCentralWidget("centralwidget",data_->widget_);
  data_->createStatusBar("statusbar",data_->widget_);
  { // Create Dockwindows
    QPtrListIterator<QtGuiDockWindow> it(*data_->dockwindow_);
    while (it.current()) {
      (it.current())->create(data_->widget_);
      ++it;
    }
  }
  { // Create Dialogs
    QDictIterator<QtGuiDialog> it(*data_->dialog_);
    while (it.current()) {
      data_->createDialog(it.currentKey(),data_->widget_);
      ++it;
    }
  }
  data_->setAttribs();
  
#ifdef DEBUG_MSG
  qDebug("QtGuiApplication::create connecting\n");
#endif
  data_->connect();
  data_->connectMenu("mainmenu");
  data_->connectToolBar("toolbar");
  data_->connectCentralWidget("centralwidget");
  data_->connectStatusBar("statusbar");
  { // Connect Dockwindows
    QPtrListIterator<QtGuiDockWindow> it(*data_->dockwindow_);
    while (it.current()) {
      (it.current())->connectObject();
      ++it;
    }
  }
  { // Connect Dialogs
    QDictIterator<QtGuiDialog> it(*data_->dialog_);
    while (it.current()) {
      data_->connectDialog(it.currentKey());
      ++it;
    }
  }

  delete data_->argdict_;
  data_->argdict_=0;
  return data_->widget_;
}

void QtGuiApplication::showDialog(QString name)
{
  QtGuiDialog *dialog=(*data_->dialog_)[name];
  if (dialog) dialog->show();
}

void QtGuiApplication::hideDialog(QString name)
{
  QtGuiDialog *dialog=(*data_->dialog_)[name];
  if (dialog) dialog->hide();
}

QString QtGuiApplication::absFilename(const QString& filename)
{
#ifdef DEBUG_MSG
  qDebug("QtGuiApplication::absFilename %s\n",
	 (const char*)filename);
#endif
  return QDir(directory_).filePath(filename);
}

/*****************************************************************
CLASS: QtGuiApplPrivate
*****************************************************************/
void QtGuiApplPrivate::createMenu(QString name, QWidget *parent)
{
  QtGuiMenu *menu=(*menu_)[name];
  if (!menu) return;
  menu->create(parent);
}

void QtGuiApplPrivate::connectMenu(QString name)
{
  QtGuiMenu *menu=(*menu_)[name];
  if (!menu) return;
  menu->connectObject();
}

void QtGuiApplPrivate::createToolBar(QString name, QWidget *parent)
{
#ifdef DEBUG_MSG
  qDebug("Toolbar createtoolbar\n");
#endif
  QtGuiToolBar *toolbar=(*toolbar_)[name];
  if (!toolbar) return;
  toolbar->create(parent);
#ifdef DEBUG_MSG
  qDebug("Toolbar created\n");
#endif
}

void QtGuiApplPrivate::connectToolBar(QString name)
{
#ifdef DEBUG_MSG
  qDebug("Toolbar connectToolBar\n");
#endif
  QtGuiToolBar *toolbar=(*toolbar_)[name];
  if (!toolbar) return;
  toolbar->connectObject();
#ifdef DEBUG_MSG
  qDebug("Toolbar connected\n");
#endif
}

void QtGuiApplPrivate::createCentralWidget(QString name, QWidget *parent)
{
#ifdef DEBUG_MSG
  qDebug("CentralWidget createCentralWidget\n");
#endif
  QtGuiCentralWidget *centralwidget=(*centralwidget_)[name];
  if (!centralwidget) return;
  centralwidget->create(parent);
  ((QMainWindow*)widget_)->setCentralWidget(centralwidget->widget(0));
#ifdef DEBUG_MSG
  qDebug("CentralWidget created\n");
#endif
}

void QtGuiApplPrivate::connectCentralWidget(QString name)
{
#ifdef DEBUG_MSG
  qDebug("CentralWidget connectCentralWidget\n");
#endif
  QtGuiCentralWidget *centralwidget=(*centralwidget_)[name];
  if (!centralwidget) return;
  centralwidget->connectObject();
#ifdef DEBUG_MSG
  qDebug("CentralWidget created\n");
#endif
}

void QtGuiApplPrivate::createStatusBar(QString name, QWidget *parent)
{
#ifdef DEBUG_MSG
  qDebug("StatusBar createstatusbar\n");
#endif
  QtGuiStatusBar *statusbar=(*statusbar_)[name];
  if (!statusbar) return;
  statusbar->create(parent);
#ifdef DEBUG_MSG
  qDebug("StatusBar connected\n");
#endif
}

void QtGuiApplPrivate::connectStatusBar(QString name)
{
#ifdef DEBUG_MSG
  qDebug("StatusBar connectStatusBar\n");
#endif
  QtGuiStatusBar *statusbar=(*statusbar_)[name];
  if (!statusbar) return;
  statusbar->connectObject();
#ifdef DEBUG_MSG
  qDebug("StatusBar connected\n");
#endif
}

void QtGuiApplPrivate::createDialog(QString name, QWidget *parent)
{
  QtGuiDialog *dialog=(*dialog_)[name];
  if (!dialog) return;
  dialog->create(parent);
}

void QtGuiApplPrivate::connectDialog(QString name)
{
  QtGuiDialog *dialog=(*dialog_)[name];
  if (!dialog) return;
  dialog->connectObject();
}

/*****************************************************************
CLASS: Static Members QtGuiAppl
*****************************************************************/

