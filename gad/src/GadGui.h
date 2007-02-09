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
 * $Source: /data/cvs/gad/gad/GadGui.h,v $
 * $Revision: 1.2 $
 * $Date: 2002/05/24 06:14:36 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef _Gadgui_h
#define _Gadgui_h

#include <MLParser.h>
#include <qwidget.h>

class GadGui:public QObject
{
Q_OBJECT public:
  enum TAGS
  {
    TAG_MODULE = 1,
    TAG_INPUT,
    TAG_OUTPUT,
    TAG_TEXT,
  };
    GadGui();
    GadGui(MLParser & parser, ArgDict * argdict = 0);

    virtual ~ GadGui();
  virtual void parse(MLParser & parser, ArgDict * argdict = 0);
  virtual QWidget *create(QWidget * parent = 0, QLayout * layout = 0);
  virtual const QString & name()
  {
    return name_;
  }
  virtual const QString command(ArgDict & argdict);

  public slots: virtual void setValue(const QString *);
  virtual QString getValue();
  virtual void show();
  virtual void hide();
  virtual void apply(void);
  virtual void cancel(void);
protected:
  virtual void init();
  QString name_;
  QString label_;
  QString tip_;
  int x_, y_;
  QString cmd_;
  ArgDict *argdict_;
  QWidget *widget_;
};

typedef GadGui *GadGuiNewObj(MLParser *, ArgDict *);

extern MLTagTable *_gadguitagtable;

void GadGuiInitObjectTable();

GadGui *GadGuiNewObject(int tagid, MLParser * parser, ArgDict * argdict = 0);
#endif
