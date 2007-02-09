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
 * $Source: /data/cvs/gad/gad/GadObject.h,v $
 * $Revision: 1.2 $
 * $Date: 2002/05/24 06:14:37 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef _GadObject_h
#define _GadObject_h

#include "GadModule.h"
#include "ClassDict.h"
#include "MLParser.h"
#include <qwidget.h>
#include <qintdict.h>
#include <qvector.h>
#include <qdatetime.h>
#include <qstack.h>

class GadObject;

struct GadConnect
{
  GadObject *obj;
  int no;
};

class GadObject:public QWidget
{
Q_OBJECT public:
  GadObject(GadModule * module,
            QWidget * parent = 0, const char *name = 0, WFlags f = 0);
    GadObject(const ClassDict & classDict, ArgDict * argdict,
              QWidget * parent = 0, const char *name = 0, WFlags f = 0);
    virtual ~ GadObject();
  virtual const QString & outputFilename(int no);
  void id(int);
  int id();
  virtual void save(QTextStream & fp);
  GadModule *module()
  {
    return module_;
  }
  void getOutput(int no, GadConnect & obj);
  int numInputs()
  {
    return numInputs_;
  }
  int numOutputs()
  {
    return numOutputs_;
  }
  QPoint inputPoint(int no);
  QPoint outputPoint(int no);
  public slots:virtual void edit();
  virtual void connectInput(int, GadConnect &);
  virtual void connectObject(QIntDict < GadObject > &);
  virtual void disconnectObject(GadObject *);
  virtual void run();
  virtual void runStep1(GadObject * initiator);
  virtual void runStep2(GadObject *);
  virtual void processFinished(int pid);
  virtual void runStep3(int pid);
signals:
  void editPressed(GadObject *);
  void runPressed(GadObject *);
  void inputPressed(GadObject *, int);
  void outputPressed(GadObject *, int);
  void moveObject(GadObject *);
  void objectDeleted(GadObject *);
protected:
  virtual void init();
  virtual void paintEvent(QPaintEvent *);
  virtual void mousePressEvent(QMouseEvent *);
  virtual void mouseMoveEvent(QMouseEvent *);
  virtual void mouseReleaseEvent(QMouseEvent *);
  virtual QSize sizeHint() const;
  virtual QSize minimumSizeHint() const;
  virtual QSizePolicy sizePolicy() const;

  GadModule *module_;
  ArgDict *argdict_;
  QString className_;
  QString name_;
  int numInputs_;
  int numOutputs_;
  int numItems_;
  QArray < GadConnect > input_;
  QVector < QString > outFilename_;
  int width_;
  int height_;
  int oldX_;
  int oldY_;
  bool moveWindow_;

  QString warning_;
  int pid_;
  int numPredecessors_;
  enum
  { DONE, WAITING, RUNNING }
  running_;
  QStack < GadObject > initiator_;
};


#endif
