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
 * $Source: /data/cvs/gad/gad/GadModule.h,v $
 * $Revision: 1.3 $
 * $Date: 2002/05/24 06:14:36 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef _GadModule_h
#define _GadModule_h

#include <GadGui.h>
#include <GadIO.h>

#include <qlist.h>

class GadModule:public GadGui
{
Q_OBJECT public:
  GadModule();
  GadModule(MLParser & parser, ArgDict * argdict = 0);

  virtual ~ GadModule();
  virtual void parse(MLParser & parser, ArgDict * argdict = 0);
  virtual QWidget *create(QWidget * parent = 0, QLayout * layout = 0);
  virtual void connectGUI();
  virtual const QString & className()
  {
    return className_;
  }
  virtual const QString command(ArgDict & argdict);
  virtual int numInputs()
  {
    return numInputs_;
  }
  virtual int numOutputs()
  {
    return numOutputs_;
  }
  virtual QString outputFilename(int no, ArgDict * argdict);
  virtual QString outputFilename(QString name, ArgDict * argdict);
  virtual QString inputFilename(int no, ArgDict * argdict);
  virtual QString inputFilename(QString name, ArgDict * argdict);
  const QString & tip();
  GadIO *inputObject(int no);
  GadIO *outputObject(int no);
  public slots:virtual void edit(ArgDict * argdict, QWidget * parent);
  virtual void show();
  virtual void hide();
  virtual void apply(void);
  virtual void cancel(void);
protected:
  virtual void init();
  QString className_;
  QString tip_;
  GadIO **inputList_;
  int numInputs_;
  QDict < GadIO > inputDict_;
  GadIO **outputList_;
  int numOutputs_;
  QList < GadGui > objectList_;
  ArgDict *argDict_;
};

GadModule *newGadModule(MLParser & parser, ArgDict * argdict = 0);

QString GadGetTmpDir();
QString GadGetTmpDirPID();

inline GadIO *GadModule::inputObject(int no)
{
  return inputList_[no];
}

inline GadIO *GadModule::outputObject(int no)
{
  return outputList_[no];
}

inline const QString & GadModule::tip()
{
  return tip_;
}

#endif
