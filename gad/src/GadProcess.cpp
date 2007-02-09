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
 * $Source: /data/cvs/gad/gad/GadProcess.cpp,v $
 * $Revision: 1.2 $
 * $Date: 2002/05/24 06:14:37 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "GadProcess.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>


GadProcess::GadProcess()
{
  timer_ = new QTimer(this, "Process timer");
  connect(timer_, SIGNAL(timeout()), this, SLOT(check()));
}

GadProcess::~GadProcess()
{
}

int GadProcess::start(QString command)
{
  int pid;
  extern char **environ;

  if (command.isEmpty())
    return 0;
  pid = fork();
  if (pid == -1)
    return -1;
  if (pid == 0) {
    char *argv[4];
    argv[0] = "sh";
    argv[1] = "-c";
    argv[2] = strdup(command);
    argv[3] = 0;
    execve("/bin/sh", argv, environ);
    exit(127);
  }
  return pid;
}

void GadProcess::kill(int pid)
{
  ::kill(pid, SIGTERM);
}

void GadProcess::add(int pid, GadObject * obj)
{
  dict_.insert(pid, obj);
  if (dict_.count() == 1) {
    timer_->start(200);
  }
}

void GadProcess::remove(GadObject * obj)
{
  /*! */
  qDebug("GadProcess::remove not implemented\n");
}

void GadProcess::check()
{
  //  qDebug("GadProcess::check(%d)\n",dict_.count());
  int status;
  int pid = waitpid(-1, &status, WNOHANG);
  switch (pid) {
  case -1:
    qDebug("GadProcess::check error\n");
    break;
  case 0:
    //    qDebug("GadProcess::check no child exited\n");
    break;
  default:
    GadObject * gobj = dict_[pid];
    dict_.remove(pid);
    qDebug("GadProcess::child child %d exited - %d left\n", pid,
           dict_.count());
    if (dict_.count() == 0)
      timer_->stop();
    if (gobj)
      gobj->processFinished(pid);
    break;
  }
}

GadProcess process;


