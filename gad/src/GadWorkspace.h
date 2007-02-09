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
 * $Source: /data/cvs/gad/gad/GadWorkspace.h,v $
 * $Revision: 1.2 $
 * $Date: 2002/05/24 06:14:37 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef _GadWorkspace_h
#define _GadWorkspace_h

#include "GadModule.h"
#include "ClassDict.h"
#include <qwidget.h>
#include "GadObject.h"
#include <qpopupmenu.h>
#include <qmenubar.h>

struct ClassNameMenu
{
  QString className;
  QString name;
};

class GadWorkspace:public QWidget
{
Q_OBJECT public:
  GadWorkspace(QWidget * parent = 0, const char *name = 0, WFlags f = 0);
    virtual ~ GadWorkspace();
  virtual void newWS(const ClassDict & classDict);
  virtual void load(const ClassDict & classDict, const QString & filename);
  virtual void save(const QString & filename);
  public slots:virtual void resize();
  virtual void inputSelected(GadObject *, int);
  virtual void outputSelected(GadObject *, int);
  virtual void disconnectObject(GadObject *);
  protected slots: virtual void newObjectSlot(int index);
protected:
    virtual void init();
  virtual void genMenu();
  virtual void newObject(int index, int x = 0, int y = 0);
  virtual void connectObjects();
  virtual void paintEvent(QPaintEvent *);
  virtual void mousePressEvent(QMouseEvent *);
  virtual QSize sizeHint() const;
  virtual QSize minimumSizeHint() const;
  virtual QSizePolicy sizePolicy() const;

  GadConnect input_;
  GadConnect output_;
  int outputNo_;
  QPopupMenu *popup_;
  QMenuBar *menubar_;
  int popupId_;
  bool mousePressed_;
  const ClassDict *classDict_;
  ClassNameMenu *classNameMenu_;
};

#endif
