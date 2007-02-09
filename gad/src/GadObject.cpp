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
 * $Source: /data/cvs/gad/gad/GadObject.cpp,v $
 * $Revision: 1.6 $
 * $Date: 2002/11/06 06:49:42 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "GadObject.h"
#include "GadWorkspace.h"

#include <qdialog.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qhbox.h>
#include <qpainter.h>
#include <qtooltip.h>
#include <qregexp.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qdir.h>
#include <qdatetime.h>
#include <qapplication.h>
#include <GadProcess.h>
#include "WildcardReplace.h"
#define GAD_WIDTH   50
#define GAD_HEIGHT  20
#define GAD_BORDER   2
#define GAD_SIZE    10
#define MIN_NUMITEMS 4

GadObject::GadObject(GadModule * module,
                     QWidget * parent, const char *name, WFlags f)
:QWidget(parent, name, f)
{
  qDebug("GadObject::GadObject\n");
  setBackgroundColor(QColor(142, 146, 179));
  module_ = module;
  className_ = "";
  name_ = "";
  argdict_ = new ArgDict();
  init();
}

GadObject::GadObject(const ClassDict & classDict,
                     ArgDict * argdict = 0,
                     QWidget * parent, const char *name, WFlags f)
:QWidget(parent, name, f)
{
  qDebug("GadObject::GadObject\n");
  setBackgroundColor(QColor(142, 146, 179));
  argdict_ = argdict;
  className_ = "";
  name_ = "";
  const QString *className = (*argdict)["obj_class"];
  if (className)
    className_ = *className;
  while (argdict_->remove("obj_class"));
  {
    const QString *name = (*argdict)["obj_name"];
    if (name)
      name_ = *name;
    while (argdict_->remove("obj_name"));
    module_ = GetModule(classDict, className_, name_);
    if (!module_) {
      qDebug("Warning: module %s (%s) not found!",className_.latin1(),name_.latin1());
    }
  }
  
  QDictIterator < QString > it(*argdict);
  while (it.current()) {
    if (it.current()->contains("gad-tmpdir"))
      argdict->remove(it.currentKey());
    else
      ++it;
  }
  qDebug("GadObject::GadObject\n");
  int x, y;
  MLParser::setInt(x, argdict_, "obj_x");
  MLParser::setInt(y, argdict_, "obj_y");
  while (argdict_->remove("obj_x"));
  while (argdict_->remove("obj_y"));
  qDebug("GadObject::GadObject\n");
  move(x, y);
  qDebug("GadObject::GadObject\n");
  init();
}

GadObject::~GadObject()
{
  if (argdict_)
    delete argdict_;
  outFilename_.clear();
}

const QString & GadObject::outputFilename(int no)
{
  *(outFilename_[no]) = module_->outputFilename(no, argdict_);

  qDebug("GadObject::outputFilename(%d): %s\n",
         no, (const char *) *(outFilename_[no]));
  return *(outFilename_[no]);
}

void GadObject::id(int no)
{
  if ((*argdict_)["obj_id"])
    (*argdict_)["obj_id"]->sprintf("%d", no);
  else {
    QString *s = new QString;
    s->sprintf("%d", no);
    argdict_->insert("obj_id", s);
  }
}

int GadObject::id()
{
  if ((*argdict_)["obj_id"])
    return (*argdict_)["obj_id"]->toInt();
  return 0;
}

void GadObject::save(QTextStream & fp)
{
  fp << "<OBJECT obj_class=\"" << className_
    << "\" obj_name=\"" << name_ << "\""
    << " obj_x=" << x() << " obj_y=" << y() << " ";

#if 0
  QDictIterator < QString > arg(*argdict_);
  for (; arg.current(); ++arg) {
    fp << " " << arg.currentKey() << "=\"" << *(arg.current()) << "\"";
  }
#endif
  fp << *argdict_ << endl;
  int i;
  for (i = 0; i < numInputs_; i++) {
    if (input_[i].obj)
      fp << " obj_connect=\"" << i << "," << input_[i].obj->id()
        << "," << input_[i].no << "\"" << endl;
  }
  fp << ">" << endl;
}

void GadObject::getOutput(int no, GadConnect & obj)
{
  if (no < numInputs_) {
    obj = input_[no];
    return;
  }
  obj.obj = 0;
  obj.no = 0;
}

void GadObject::connectInput(int no, GadConnect & obj)
{
  qDebug("GadObject::connectInput\n");
  if (no < numInputs_) {
    input_[no] = obj;
  }
}

void GadObject::connectObject(QIntDict < GadObject > &dict)
{
  qDebug("GadObject::connectObject\n");
  while ((*argdict_)["obj_connect"]) {
    QString s = *((*argdict_)["obj_connect"]);
    s.replace(QRegExp("[,;:]"), " ");
    QTextIStream istr(&s);
    int inNo, objNo;
    GadConnect outObj;
    if (!istr.atEnd())
      istr >> inNo;
    if (!istr.atEnd())
      istr >> objNo;
    if (!istr.atEnd())
      istr >> outObj.no;
    outObj.obj = dict[objNo];
    connectInput(inNo, outObj);
    argdict_->remove("obj_connect");
  }
}

void GadObject::disconnectObject(GadObject * obj)
{
  qDebug("GadObject::connectObject\n");
  int i;
  for (i = 0; i < numInputs_; i++) {
    if (input_[i].obj == obj) {
      input_[i].obj = 0;
      input_[i].no = 0;
    }
  }
}

void GadObject::init()
{
  qDebug("GadObject::init!\n");
  if (!module_) {
    qDebug("GadObject::init: no module given!\n");
    numInputs_ = 0;
    numOutputs_ = 0;
  }
  else {
    numInputs_ = module_->numInputs();
    numOutputs_ = module_->numOutputs();
    className_ = module_->className();
    name_ = module_->name();
  }
  pid_ = 0;
  running_ = DONE;
  numPredecessors_ = 0;
  GadConnect c = { 0, 0 };
  input_.fill(c, numInputs_);
  outFilename_.resize(numOutputs_);
  int i;
  for (i = 0; i < numOutputs_; i++)
    outFilename_.insert(i, new QString);
  numItems_ = (numInputs_ > numOutputs_ ? numInputs_ : numOutputs_);
  width_ = GAD_WIDTH;
  height_ = (numItems_ < MIN_NUMITEMS
             ? MIN_NUMITEMS * GAD_HEIGHT + 2 * GAD_BORDER
             : (numItems_ + 1) * GAD_HEIGHT + 2 * GAD_BORDER);
  resize(width_, height_);
  if (!argdict_)
    argdict_ = new ArgDict();
  connect(this, SIGNAL(editPressed(GadObject *)), this, SLOT(edit()));
  connect(this, SIGNAL(runPressed(GadObject *)), this,
          SLOT(runStep1(GadObject *)));
  if (parentWidget()->inherits("GadWorkspace")) {
    GadWorkspace *ws = (GadWorkspace *) parentWidget();
    connect(this, SIGNAL(moveObject(GadObject *)), ws, SLOT(resize()));
    connect(this, SIGNAL(inputPressed(GadObject *, int)),
            ws, SLOT(inputSelected(GadObject *, int)));
    connect(this, SIGNAL(outputPressed(GadObject *, int)),
            ws, SLOT(outputSelected(GadObject *, int)));
    connect(this, SIGNAL(objectDeleted(GadObject *)),
            ws, SLOT(disconnectObject(GadObject *)));
  }
  if (!module_)
    return;
  if (!module_->tip().isEmpty()) {
    qDebug("GadObject: Tooltip %s added\n", (const char *) module_->tip());
    QToolTip::add(this, module_->tip());
  }
}

QPoint GadObject::inputPoint(int no)
{
  return (QPoint
          (x(), y() + GAD_BORDER + (no + 1) * GAD_HEIGHT + GAD_SIZE / 2));
}

QPoint GadObject::outputPoint(int no)
{
  return (QPoint
          (x() + GAD_WIDTH,
           y() + GAD_BORDER + (no + 1) * GAD_HEIGHT + GAD_SIZE / 2));
}

void GadObject::paintEvent(QPaintEvent * ev)
{
  qDebug("GadObject::paintEvent(width=%d, height=%d)\n",
         GAD_WIDTH, (numItems_ + 1) * GAD_HEIGHT);
  QPainter paint(this);
  paint.setPen(Qt::black);
  paint.drawRect(0, 0, width_, height_);
  int i;
  for (i = 0; i < numInputs_; i++) {
    if (module_->inputObject(i)->optional())
      paint.fillRect(GAD_BORDER, GAD_BORDER + (i + 1) * GAD_HEIGHT,
                     GAD_SIZE, GAD_SIZE, Qt::yellow);
    else
      paint.fillRect(GAD_BORDER, GAD_BORDER + (i + 1) * GAD_HEIGHT,
                     GAD_SIZE, GAD_SIZE, Qt::blue);

  }
  for (i = 0; i < numOutputs_; i++) {
    paint.fillRect(GAD_WIDTH - GAD_BORDER - GAD_SIZE,
                   GAD_BORDER + (i + 1) * GAD_HEIGHT, GAD_SIZE, GAD_SIZE,
                   Qt::blue);

  }
  paint.setPen(Qt::black);
  QPointArray a(3);
  a.setPoints(3, 0, 0, GAD_SIZE, 0, 0, GAD_SIZE);
  paint.setBrush(QBrush::SolidPattern);
  paint.drawPolygon(a);
  switch (running_) {
  case RUNNING:
    paint.setPen(Qt::green);
    paint.fillRect((width_ - GAD_SIZE) / 2, 2 * GAD_SIZE, GAD_SIZE, GAD_SIZE,
                   Qt::green);
    break;
  case WAITING:
    paint.setPen(Qt::yellow);
    paint.fillRect((width_ - GAD_SIZE) / 2, 2 * GAD_SIZE, GAD_SIZE, GAD_SIZE,
                   Qt::yellow);
    break;
  case DONE:
    paint.setPen(Qt::red);
    paint.fillRect((width_ - GAD_SIZE) / 2, 2 * GAD_SIZE, GAD_SIZE, GAD_SIZE,
                   Qt::red);
  }
}

#define inRect(x,y,x1,y1,width,height) ((x>=x1) && (x<=x1+width) \
					&& (y>=y1) && (y<=y1+height))

void GadObject::mousePressEvent(QMouseEvent * ev)
{
  moveWindow_ = false;
  int x = ev->x();
  int y = ev->y();
  if (inRect(x, y, 0, 0, GAD_SIZE, GAD_SIZE)) {
    qDebug("Edit button pressed\n");
    if (ev->button() == LeftButton)
      emit editPressed(this);
    return;
  }
  if (inRect(x, y, (width_ - GAD_SIZE) / 2, 2 * GAD_SIZE, GAD_SIZE, GAD_SIZE)) {
    qDebug("Run button pressed\n");
    if (ev->button() == LeftButton)
      emit runPressed(this);
    return;
  }
  int i;
  for (i = 0; i < numInputs_; i++) {
    if (inRect(x, y, GAD_BORDER, GAD_BORDER + (i + 1) * GAD_HEIGHT,
               GAD_SIZE, GAD_SIZE)) {
      qDebug("Input Button %d button pressed\n", i);
      switch (ev->button()) {
      case RightButton:
        {
          if (!input_[i].obj)
            return;
          QPopupMenu *popup;
          popup = new QPopupMenu;
          popup->insertItem("Delete Connection", 0);
          if (popup->exec(ev->globalPos()) < 0) {
            delete popup;
            return;
          }
          delete popup;
          input_[i].obj = 0;
          input_[i].no = 0;
          QWidget *parent = (QWidget *) this->parent();
          parent->repaint();
          return;
        }
        break;
      default:
        emit inputPressed(this, i);
        return;
      }
    }
  }
  for (i = 0; i < numOutputs_; i++) {
    if (inRect(x, y,
               GAD_WIDTH - GAD_BORDER - GAD_SIZE,
               GAD_BORDER + (i + 1) * GAD_HEIGHT, GAD_SIZE, GAD_SIZE)) {
      qDebug("Output Button %d button pressed\n", i);
      emit outputPressed(this, i);
      return;
    }
  }
  switch (ev->button()) {
  case RightButton:
    {
      QPopupMenu *popup;
      popup = new QPopupMenu;
      popup->insertItem("Delete Object", 0);
      if (popup->exec(ev->globalPos()) < 0) {
        delete popup;
        return;
      }
      delete popup;
      emit objectDeleted(this);
      QWidget *parent = (QWidget *) this->parent();
      module_->hide();
      this->close(true);
      parent->repaint();
      return;
    }
    break;
  default:
    oldX_ = x;
    oldY_ = y;
    moveWindow_ = true;
  }
}

void GadObject::mouseMoveEvent(QMouseEvent * ev)
{
  if (!moveWindow_)
    return;
  move(ev->x() - oldX_ + x(), ev->y() - oldY_ + y());
}

void GadObject::mouseReleaseEvent(QMouseEvent * ev)
{
  if (!moveWindow_)
    return;
  move(ev->x() - oldX_ + x(), ev->y() - oldY_ + y());
  moveWindow_ = false;
  emit moveObject(this);
}

QSize GadObject::sizeHint() const
{
  qDebug("GadObject::sizeHint(%d,%d)\n", width_ + 2, height_ + 2);
  return QSize(width_ + 50, height_ + 50);
}

QSize GadObject::minimumSizeHint() const
{
  qDebug("GadObject::minimumSizeHint(%d,%d)\n", width_ + 2, height_ + 2);
  return QSize(width_ + 50, height_ + 50);
}

QSizePolicy GadObject::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed, false);
}

void GadObject::edit()
{
  qDebug("GadObject::edit\n");
  if (!module_)
    return;
  module_->edit(argdict_, this);
  /*! Ausgabefiles löschen */
}

void GadObject::run()
{
  runStep1(0);
}

void GadObject::runStep1(GadObject * initiator)
{
  qDebug("GadObject::runStep1\n");
  if (initiator == this)
    initiator = 0;
  if (pid_ && !initiator) {
    qDebug("GadObject::run kill %d\n", pid_);
    process.kill(pid_);
    pid_ = 0;
    return;
  }

  switch (running_) {
  case RUNNING:
  case WAITING:
    if (initiator)
      initiator_.push(initiator);
    //    if (initiator) initiator->runStep2(this);
    return;
    break;
  case DONE:
    if (initiator)
      initiator_.push(initiator);
    running_ = WAITING;
    warning_ = "";
    int i;
    repaint();

    // Start all predecessor objects
    numPredecessors_++;
    for (i = 0; i < numInputs_; i++) {
      if (input_[i].obj) {
        numPredecessors_++;
        input_[i].obj->runStep1(this);
      }
    }
  }

  runStep2(0);
}

void GadObject::runStep2(GadObject *)
{
  qDebug("GadObject::runStep2\n");
  if (numPredecessors_)
    numPredecessors_--;
  if (numPredecessors_)
    return;
  running_ = RUNNING;
  int i;
  QDateTime youngestInput;
  QDateTime oldestOutput;

  // Test for youngest inputfile
  for (i = 0; i < numInputs_; i++) {
    if (input_[i].obj)
      argdict_->replace(module_->inputObject(i)->name(),
                        new QString(input_[i].obj->
                                    outputFilename(input_[i].no)));
    QString s = module_->inputFilename(i, argdict_);
    qDebug("GadObject::runStep2: infilename=%s", (const char *) s);
    if (s.contains("#") > 0) {
      s.replace(QRegExp("#[0-9]"), "*");
      qDebug("replaced ~ %s", (const char *) s);
    }
    if ((s.contains("*") + s.contains("?")) > 0) {
      QFileInfo finfo(s);
      QDir d(finfo.dirPath(), finfo.fileName());
      int i;
      for (i = 0; i < d.count(); i++) {
        qDebug("Multiple: %s", (const char *) d[i]);
        QFileInfo finfo(s);
        if (!youngestInput.isValid())
          youngestInput = finfo.lastModified();
        else {
          if (youngestInput < finfo.lastModified()) {
            youngestInput = finfo.lastModified();
          }
        }
      }
    }
    else {
      QFileInfo finfo(s);
      if (finfo.exists()) {
        if (!youngestInput.isValid())
          youngestInput = finfo.lastModified();
        else {
          if (youngestInput < finfo.lastModified()) {
            youngestInput = finfo.lastModified();
          }
        }
      }
      else {
        if (module_->inputObject(i)->check()) {
          QString s = QString().sprintf("Input(%d) %s not connected!\n",
                                        i,
                                        (const char *) module_->
                                        inputObject(i)->name());
          warning_ += s;
        }
      }
    }
  }

  // Generate outputfilename and test for oldest outputfile
  for (i = 0; i < numOutputs_; i++) {
    QString outName = outputFilename(i);
    if (outName.contains("#") > 0) {
      QString depend = module_->outputObject(i)->depend();
      QString inName = module_->inputFilename(depend, argdict_);
      inName.replace(QRegExp("#[0-9]"), "*");
      qDebug("Wildcard output: %s depend=%s inName=%s",
             (const char *) outName,
             (const char *) depend, (const char *) inName);
      QFileInfo finfo(inName);
      QString inMask = finfo.fileName();
      QDir d(finfo.dirPath(), inMask);
      int i;
      for (i = 0; i < d.count(); i++) {
        QFileInfo finfo(wreplace(d[i], inMask, outName));
        qDebug("Multiple out(%s): %s",
               (const char *) d[i], (const char *) finfo.filePath());
        if (finfo.exists()) {
          if (!oldestOutput.isValid())
            oldestOutput = finfo.lastModified();
          else {
            if (oldestOutput < finfo.lastModified()) {
              oldestOutput = finfo.lastModified();
            }
          }

        }
      }
    }
    QFileInfo finfo(outName);
    if (finfo.exists()) {
      if (!oldestOutput.isValid())
        oldestOutput = finfo.lastModified();
      else {
        if (oldestOutput < finfo.lastModified()) {
          oldestOutput = finfo.lastModified();
        }
      }
    }
  }

  if (!warning_.isEmpty()) {
    qDebug("%s\n", (const char *) warning_);
    goto exit;
  }

  if ((youngestInput < oldestOutput)
      && (!initiator_.isEmpty())
      && (!(*argdict_)["obj_changed"])) {
    qDebug("GadObject::run %s is up to date\n",
           (const char *) module_->name());
    goto exit;
  }

  {
    qDebug("GadObject::run\n%s", (const char *) warning_);
    argdict_->remove("obj_changed");
    QString cmd = module_->command(*argdict_);
    qDebug("Starting:\n  %s\n", (const char *) cmd);
    /*! Eigenen Prozess starten */
    pid_ = process.start(cmd);
    if (pid_ > 0) {
      qDebug("    pid  %d\n", pid_);
      process.add(pid_, this);
      repaint();
      return;
    }
  }
exit:
  running_ = DONE;
  GadObject *initiator;
  while (!initiator_.isEmpty()) {
    initiator = initiator_.pop();
    initiator->runStep2(this);
  }
  repaint();
  return;
}

void GadObject::processFinished(int pid)
{
  runStep3(pid);
}

void GadObject::runStep3(int pid)
{
  qDebug("GadObject::runStep3(%d)\n", pid);
  pid_ = 0;
  GadObject *initiator;
  while (!initiator_.isEmpty()) {
    initiator = initiator_.pop();
    initiator->runStep2(this);
  }
  running_ = DONE;
  repaint();
}

