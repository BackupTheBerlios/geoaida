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
 * $Source: /data/cvs/gad/gad/GadModule.cpp,v $
 * $Revision: 1.5 $
 * $Date: 2002/06/24 16:33:11 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "GadModule.h"
#include <stdlib.h>
#include <qdialog.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qhbox.h>
#include <qregexp.h>
#include <unistd.h>
#include <qfileinfo.h>
#include <qdir.h>
#include "WildcardReplace.h"
#include <qtgui.h>

QString GadGetTmpDir()
{
  static QString dir;
  if (dir.isEmpty()) {
    dir = getenv("GA_DATADIR");
    if (dir.isEmpty()) {
      dir = getenv("HOME");
      if (!dir.isEmpty()) {
        dir += "/.gad";
        QDir tmpdir(dir);
        if (!tmpdir.exists()) {
          qDebug("Creating tmpdir %s", dir.latin1());
          tmpdir.mkdir(dir, true);
        }
        dir += "/tmp";
      }
      else {
        if (_qtguiappl->prefix() == "")
#ifdef WIN32
          dir = getenv("TEMP");
#else
          dir = TMPDIR;
#endif
        else
          dir = _qtguiappl->prefix() + "/tmp";
      }
    }
    QDir tmpdir(dir);
    if (!tmpdir.exists()) {
      qDebug("Creating tmpdir %s", dir.latin1());
      tmpdir.mkdir(dir, true);
    }
  }

  return dir;

#if 0
  static QString dir;
  if (dir.isEmpty()) {
    dir = getenv("GA_DATADIR");
    if (dir.isEmpty())
      dir = TMPDIR;
    dir = QString().sprintf("%s/gad-tmpdir-%d", dir.latin1(), getpid());
    QDir tmpdir;
    qDebug("GadGetTmpDir: %s", dir.latin1());
    tmpdir.mkdir(dir, true);
  }
  return dir;
#endif
}

QString GadGetTmpDirPID()
{
  static QString dir;
  if (dir.isEmpty()) {
    dir = GadGetTmpDir();
    dir = QString().sprintf("%s/gad-tmpdir-%d", dir.latin1(), getpid());
  }

  QDir tmpdir(dir);
  qDebug("GadGetTmpDirPID: %s",dir.latin1());
  if (!tmpdir.exists()) {
    qDebug("Creating tmpdir %s", dir.latin1());
    tmpdir.mkdir(dir, true);
  }
  return dir;
}

static QString GetTmpFilename()
{
  static int no = 0;
  static QString dir = GadGetTmpDirPID();
  return dir + QString().sprintf("/%d-gad", no++);
}

GadModule *newGadModule(MLParser & parser, ArgDict * argdict)
{
  return new GadModule(parser, argdict);
}


GadModule::GadModule()
{
  init();
}

GadModule::GadModule(MLParser & parser, ArgDict * argdict)
{
  init();
  parse(parser, argdict);
}

void GadModule::init()
{
  cmd_ = "";
  className_ = "";
  numInputs_ = 0;
  numOutputs_ = 0;
  widget_ = 0;
}

GadModule::~GadModule()
{
  if (widget_)
    delete widget_;
}

void GadModule::parse(MLParser & parser, ArgDict * argdict)
{
  qDebug("GadModule::parse\n");
  argdict_ = parser.args(argdict);
  GadGui::parse(parser, argdict_);
  parser.setString(cmd_, argdict_, "cmd");
  parser.setString(className_, argdict_, "class");
  parser.setString(tip_, argdict_, "tip");
  int tag;
  GadGui *object = 0;
  do {
    tag = parser.tag(*_gadguitagtable);
    qDebug("LastTag(%d)=%s\n", tag, (const char *) parser.lasttagstr());
    switch (tag) {
    case TAG_INPUT:
      object = GadGuiNewObject(tag, &parser);
      if (object) {
        numInputs_++;
        objectList_.append(object);
        inputDict_.insert(object->name(), (GadIO *) object);
      }
      break;
    case TAG_OUTPUT:
      object = GadGuiNewObject(tag, &parser);
      if (object) {
        numOutputs_++;
        objectList_.append(object);
      }
      break;
    default:
      object = GadGuiNewObject(tag, &parser);
      if (object) {
        objectList_.append(object);
      }
      break;
    }
  } while ((tag != MLParser::END_OF_FILE && tag != -TAG_MODULE));
  inputList_ = new GadIO*[numInputs_];
  outputList_ = new GadIO*[numOutputs_];
  int i = 0;
  int o = 0;
  for (object = objectList_.first(); object != 0; object = objectList_.next()) {
    if (object->isA("GadIO")) {
      switch (((GadIO *) object)->mode()) {
      case GadIO::INPUT:
        inputList_[i++] = (GadIO *) object;
        break;
      case GadIO::OUTPUT:
        outputList_[o++] = (GadIO *) object;
        break;
      }
    }
  }
}

const QString GadModule::command(ArgDict & argdict)
{
  qDebug("GadModule::command\n");
  QString cmd = cmd_;
  int i;
  for (i = 0; i < numInputs_; i++) {
    QString inName = inputFilename(i, &argdict);
    if (inName.contains("*") + inName.contains("?") + inName.contains("#") >
        0) {
      QString command = "";
      QString inputName = inputObject(i)->name();
      inName.replace(QRegExp("#[0-9]"), "*");
      QStringList outName;
      for (i = 0; i < numOutputs_; i++) {
        if (outputFilename(i, &argdict).contains("#"))
          outName += outputList_[i]->name();
      }
      QFileInfo finfo(inName);
      QString inMask = finfo.fileName();
      QDir d(finfo.dirPath(), inMask);
      for (i = 0; i < d.count(); i++) {
        QStringList::Iterator it;
        argdict.insert(inputName, new QString(finfo.dirPath() + "/" + d[i]));
        for (it = outName.begin(); it != outName.end(); ++it) {
          argdict.insert(*it,
                         new QString(wreplace(d[i], inMask,
                                              outputFilename(*it,
                                                             &argdict))));
        }
        cmd = cmd_;
        qDebug("GadModule::command cmd=%s\n", (const char *) cmd);
        GadGui *object;
        //      qDebug("cmd1=%s",(const char*)cmd);
        for (object = objectList_.first(); object != 0;
             object = objectList_.next()) {
          cmd.replace(QRegExp("@" + object->name() + "@", false),
                      object->command(argdict));
        }
        //      qDebug("cmd2=%s",(const char*)cmd);
        command += cmd + ";\n";
        argdict.remove(inputName);
        for (it = outName.begin(); it != outName.end(); ++it) {
          argdict.remove(*it);
        }
      }
      qDebug("GadModule::command %s\n", (const char *) cmd);
      return command;
    }
  }
  GadGui *object;
  qDebug("GadModule::command cmd=%s\n", (const char *) cmd);
  for (object = objectList_.first(); object != 0; object = objectList_.next()) {
    cmd.replace(QRegExp("@" + object->name() + "@", false),
                object->command(argdict));
  }
  qDebug("GadModule::command %s\n", (const char *) cmd);
  return cmd;
}

QString GadModule::outputFilename(int no, ArgDict * argdict)
{
  qDebug("GadModule::outputFilename\n");

  QString *s = (*argdict)[outputList_[no]->name()];
  if (s && (!s->isEmpty()))
    return *s;
  s = new QString(GetTmpFilename());
  QString depend = outputList_[no]->depend();
  if (!depend.isEmpty()) {
    QString *iname = (*argdict)[depend];
    if (iname && (!iname->isEmpty())) {
      int wc =
        iname->contains("*") + iname->contains("?") + iname->contains("#");
      int i;
      for (i = 1; i <= wc; i++) {
        *s += QString().sprintf("~#%d", i);
      }

    }
  }
  argdict->insert(outputList_[no]->name(), s);
  qDebug("GadModule::outputFilename TMP=%s\n", (const char *) *s);
  return *s;
}

QString GadModule::outputFilename(QString name, ArgDict * argdict)
{
  qDebug("GadModule::outputFilename\n");

  QString *s = (*argdict)[name];
#if 0
  /*! Outputfilename wird hier nicht automatisch generiert ! */
  if (s && (!s->isEmpty()))
    return *s;
  s = new QString(GetTmpFilename());
  QString depend = outputList_[no]->depend();
  if (!depend.isEmpty()) {
    QString *iname = (*argdict)[depend];
    if (iname && (!iname->isEmpty())) {
      int wc = iname->contains("*") + iname->contains("?");
      int i;
      for (i = 1; i <= wc; i++) {
        *s += QString().sprintf("~#%d", i);
      }

    }
  }
  argdict->insert(outputList_[no]->name(), s);
  qDebug("GadModule::outputFilename TMP=%s\n", (const char *) *s);
#endif
  return *s;

}

QString GadModule::inputFilename(int no, ArgDict * argdict)
{
  qDebug("GadModule::inputFilename\n");
  QString *s = (*argdict)[inputList_[no]->name()];
  if (s)
    return *s;
  return "";
}

QString GadModule::inputFilename(QString name, ArgDict * argdict)
{
  qDebug("GadModule::inputFilename\n");
  QString *s = (*argdict)[name];
  if (s)
    return *s;
  return "";
}


QWidget *GadModule::create(QWidget * parent, QLayout * layout)
{
  qDebug("GadModule::create\n");

  QDialog *dialog = new QDialog(parent, name_);
  dialog->setCaption(name_);
  QVBoxLayout *dlayout = new QVBoxLayout(dialog, 0, 5);
  dlayout->setAutoAdd(TRUE);
  GadGui *object;
  for (object = objectList_.first(); object != 0; object = objectList_.next()) {
    object->create(dialog);
  }

  QHBox *box = new QHBox(dialog);
  QPushButton *ok = new QPushButton("Ok", box);
  ok->setFocus();
  connect(ok, SIGNAL(clicked()), dialog, SLOT(accept()));
  connect(ok, SIGNAL(clicked()), this, SLOT(apply()));
  QPushButton *cancel = new QPushButton("Cancel", box);
  connect(cancel, SIGNAL(clicked()), dialog, SLOT(reject()));
  widget_ = dialog;
  return widget_;
}

void GadModule::edit(ArgDict * argdict, QWidget * parent)
{
  if (!widget_)
    create(_qtguiappl->widget());
  GadGui *object;
  argdict_ = argdict;
  for (object = objectList_.first(); object != 0; object = objectList_.next()) {
    object->setValue((*argdict)[object->name()]);
  }
  show();
}

void GadModule::connectGUI()
{
  qDebug("GadModule::connectObject not implemented\n");
}

void GadModule::show()
{
  qDebug("GadModule::show\n");
  if (widget_) {
    widget_->show();
    widget_->raise();
  }
}

void GadModule::hide()
{
  if (widget_)
    widget_->hide();
  qDebug("GadModule::hide\n");
}

void GadModule::apply(void)
{
  qDebug("GadModule::apply\n");
  GadGui *object;
  for (object = objectList_.first(); object != 0; object = objectList_.next()) {
    qDebug("   %s = %s\n",
           (const char *) object->name(), (const char *) object->getValue());
    argdict_->replace(object->name(), new QString(object->getValue()));
  }
  command(*argdict_);
  argdict_->replace("obj_changed", new QString("true"));
}

void GadModule::cancel(void)
{
  qDebug("GadModule::cancel not implemented\n");
}

