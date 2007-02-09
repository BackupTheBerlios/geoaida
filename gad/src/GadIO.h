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
 * $Source: /data/cvs/gad/gad/GadIO.h,v $
 * $Revision: 1.2 $
 * $Date: 2002/05/24 06:14:36 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef _GadIO_h
#define _GadIO_h

#include <GadGui.h>
#include <qlineedit.h>
#include <qlabel.h>

class GadIO:public GadGui
{
Q_OBJECT public:
  enum
  { INPUT, OUTPUT };
    GadIO();
    GadIO(MLParser & parser, ArgDict * argdict = 0);
    virtual ~ GadIO();
  virtual void parse(MLParser & parser, ArgDict * argdict = 0);
  virtual QWidget *create(QWidget * parent = 0, QLayout * layout = 0);
  virtual QString getValue();
  virtual int mode()
  {
    return mode_;
  }
  virtual bool optional()
  {
    return optional_;
  }
  virtual bool check()
  {
    return !nocheck_;
  }
  virtual QString depend()
  {
    return depend_;
  }
  public slots:virtual void setValue(const QString *);
  virtual void show();
  virtual void hide();
  virtual void apply(void);
  virtual void cancel(void);

  protected slots:virtual void browse();

protected:
  virtual void init();

  QString value_;
  int mode_;
  bool optional_;
  bool nocheck_;
  QString depend_;
  // GUI
  QLineEdit *lineEdit_;
};


GadIO *newGadIO(MLParser & parser, ArgDict * argdict = 0);

#endif
