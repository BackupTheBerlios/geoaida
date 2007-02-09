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
 * $Source: /data/cvs/gad/gad/GadProcess.h,v $
 * $Revision: 1.2 $
 * $Date: 2002/05/24 06:14:37 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef _GadProcess_h
#define _GadProcess_h
#include "GadObject.h"
#include <qtimer.h>

class GadProcess:public QObject
{
Q_OBJECT public:
  GadProcess();
  ~GadProcess();
  int start(QString command);
  void kill(int pid);
  void add(int pid, GadObject *);
  void remove(GadObject *);
  protected slots: void check();
protected:
    QIntDict < GadObject > dict_;
  QTimer *timer_;
};

extern GadProcess process;

#endif
