/***************************************************************************
                          taskviewer.h  -  description
                             -------------------
    begin                : Thu Nov 15 2001
    copyright            : (C) 2001 by Martin pahl
    email                : pahl@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 * $Source: /data/cvs/gda/gda/semnetgui/taskviewer.h,v $
 * $Revision: 1.2 $
 * $Date: 2001/12/07 12:32:02 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef TASKVIEWER_H
#define TASKVIEWER_H

#include <qwidget.h>
#include <qlistview.h>

struct ProcessEntry;
/**Shows the running tasks
  *@author Martin pahl
  */
class TaskViewer:public QListView
{
Q_OBJECT public:
  TaskViewer(QWidget * parent = 0, const char *name = 0);
   ~TaskViewer();
  public slots:                 // Public slots
  /** Update taskItem */
  void stateChanged(QListViewItem * taskItem, int pid, QString name,
                    QString cmd, float load);
  public slots:                 // Public slots
  /** remove taskItem from the TaskViewer */
  void processFinished(QListViewItem * taskItem);
  public slots:                 // Public slots
  /** register a new process */
  void newProcess(ProcessEntry *);
signals: // Signals
  /** emit the system load */
  void systemLoad(double);
};

#endif
