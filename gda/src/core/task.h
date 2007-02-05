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
 * $Source: /data/cvs/gda/gda/core/task.h,v $
 * $Revision: 1.10 $
 * $Date: 2002/05/13 10:17:42 $
 * $Author: gerhards $
 * $Locker:  $
 */

#ifndef _Task_h
#define _Task_h
#ifdef WIN32
#define maxProcess 20
#endif
#include "inode.h"
#include <qtimer.h>
#include <qintdict.h>
#include <qptrqueue.h>
#include <qstring.h>

class QListViewItem;

class Task:public QObject
{
Q_OBJECT public:
  Task(unsigned int maxJobs = 0);
   ~Task();
  int start(QString command);
  struct ProcessEntry;
  void kill(int pid);
  void add(int pid, INode *);
  void remove(INode *);
  /** Wait until process pid finished   */
  void wait(int jid);
  /** push a command in the job queue. job id is returned */
  int queue(QString command, INode * obj);
  /** set the maximum number of jobs running parallel */
  void setMaxJobs(int maxJobs);
  /** No descriptions */
  void setGuiPtr(ProcessEntry * pEntry, QListViewItem * ptr);
  /** return the system load on Linux-Systems */
  bool systemLoad();
protected slots:
  void check();
public:
  struct ProcessEntry
  {
    int jid_;                   // job id
    int pid_;                   // process id
    int status_;                // exit status
    QString cmd_;               // command line
    INode *node_;
    QListViewItem *guiPtr_;
  };
 protected:
  int execNext();
  int jid_;
  unsigned int maxJobs_;
#ifdef WIN32
   int process_count;
   ProcessEntry process_ [maxProcess];//maximale ProcessZahl,
						//weil QIntDictIterator nicht läuft
#else
    QIntDict < ProcessEntry > process_;
#endif
    QIntDict < ProcessEntry > job_;
    QPtrQueue < ProcessEntry > jobQueue_;
    QPtrQueue < ProcessEntry > failedJobs_;
  QTimer *timer_;
  bool timerRunning_;
  /** system load */
  float systemLoad_;
  /** start jobs in random order */
  bool random_;
signals:                    // Signals
  /** Signal is send when a new process was queued */
  void newProcess(ProcessEntry *);
  /** signal is emitted whenever the state of process changes.
    guiPtr, pid, nodename and commandline are transmitted */
  void stateChanged(QListViewItem *, int, QString, QString, float load);
  /** signal is emitted when a process is finished */
  void processFinished(QListViewItem *);
};

extern Task taskTable;

#endif
