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
 * $Source: /data/cvs/gad/gad/GadEnum.h,v $
 * $Revision: 1.3 $
 * $Date: 2002/07/05 04:43:27 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef _GadEnum_h
#define _GadEnum_h

#include <GadGui.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qstringlist.h>

class GadEnum:public GadGui
{
Q_OBJECT public:
  enum
  { INPUT, OUTPUT };
    GadEnum();
    GadEnum(MLParser & parser, ArgDict * argdict = 0);
    virtual ~ GadEnum();
  virtual void parse(MLParser & parser, ArgDict * argdict = 0);
  virtual QWidget *create(QWidget * parent = 0, QLayout * layout = 0);
  virtual QString getValue();
  public slots:virtual void setValue(const QString *);
  virtual void show();
  virtual void hide();
  virtual void apply(void);
  virtual void cancel(void);

protected:
  virtual void init();

  QString cmd_;
  QString value_;
  QStringList options_;
  QStringList optionsLabel_;

  // GUI
  QComboBox *editor_;
};


GadEnum *newGadEnum(MLParser & parser, ArgDict * argdict = 0);

#endif
