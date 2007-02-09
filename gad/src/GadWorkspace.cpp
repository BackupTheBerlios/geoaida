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
 * $Source: /data/cvs/gad/gad/GadWorkspace.cpp,v $
 * $Revision: 1.5 $
 * $Date: 2002/11/06 06:49:42 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "GadWorkspace.h"
#include "MLParser.h"
#include "QtGuiApplication.h"

#include <qdialog.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qhbox.h>
#include <qpainter.h>
#include <qfile.h>
#include <qobjcoll.h>
#include <qintdict.h>
#include <qdatetime.h>
#include <qmenubar.h>
#include <qcursor.h>

GadWorkspace::GadWorkspace(QWidget * parent, const char *name, WFlags f)
:QWidget(parent, name, f)
{
  // Dummy widget
  (new QWidget(this))->setFixedSize(0, 0);
  init();
}

GadWorkspace::~GadWorkspace()
{
  if (popup_) {
    if (menubar_) {
      menubar_->removeItem(popupId_);
    }
    delete popup_;
  }
}

void GadWorkspace::newWS(const ClassDict & classDict)
{
  classDict_ = &classDict;
  genMenu();
}

void GadWorkspace::load(const ClassDict & classDict, const QString & filename)
{
  qDebug("GadWorkspace::load\n");
  classDict_ = &classDict;
  genMenu();
  QFile fp(filename);
  if (!fp.open(IO_ReadOnly))
    return;
  MLParser parser(&fp);
  QString keywords[] = { "OBJECT", "" };
  MLTagTable tagtable(keywords);

  int tag;
  QIntDict < GadObject > dict;

  while ((tag = parser.tag(tagtable)) != MLParser::END_OF_FILE) {
    GadObject *obj;
    qDebug("tag=%d\n", tag);
    switch (tag) {
    case 1:
      obj = new GadObject(classDict, parser.args(), this);
      dict.insert(obj->id(), obj);
      break;
    default:
      delete parser.args();
    }
  }
  qDebug("GadWorkspace::load: connecting\n");
  QIntDictIterator < GadObject > it(dict);
  for (; it.current(); ++it) {
    it.current()->connectObject(dict);
  }
  resize();
  qDebug("GadWorkspace::load: connecting done\n");
}

void GadWorkspace::save(const QString & filename)
{
  int id = 1;
  QFile fp(filename);
  if (!fp.open(IO_WriteOnly))
    return;
  QTextStream ostr(&fp);

  ostr << "# "
    << QDate::currentDate().toString() << "  "
    << QTime::currentTime().toString() << endl;
  QListIterator < QObject > obj(*children());
  for (; obj.current(); ++obj) {
    if (obj.current()->inherits("GadObject")) {
      GadObject *gobj = (GadObject *) obj.current();
      gobj->id(id++);
    }
  }

  obj.toFirst();
  for (; obj.current(); ++obj) {
    if (obj.current()->inherits("GadObject")) {
      GadObject *gobj = (GadObject *) obj.current();

      gobj->save(ostr);
    }
  }
  fp.close();
}

void GadWorkspace::genMenu()
{
  qDebug("GadWorkspace::genMenu\n");
  if (!menubar_) {
    QtGui *mainmenu = (QtGui *) (*(_qtguiappl->classDict()))["mainmenu"];
    if (mainmenu) {
      QWidget *w = mainmenu->widget();
      if (w && w->inherits("QMenuBar"))
        menubar_ = (QMenuBar *) w;
    }
  }

  const ClassDict & classDict = *classDict_;
  int size = 0;
  QStringList classList;
  QDictIterator < ModuleDict > cit(classDict);
  for (; cit.current(); ++cit) {
    size += cit.current()->count();
    classList += cit.currentKey();
  }

  if (classNameMenu_)
    delete classNameMenu_;
  classNameMenu_ = new ClassNameMenu[size];
  if (popup_)
    delete popup_;
  popup_ = new QPopupMenu(0);
  classList.sort();
  QStringList::Iterator clit;
  int item = 0;
  for (clit = classList.begin(); clit != classList.end(); ++clit) {

    qDebug("Class %s\n", (const char *) *clit);
    QPopupMenu *popup = new QPopupMenu();
    popup_->insertItem(*clit, popup);
    connect(popup, SIGNAL(activated(int)), this, SLOT(newObjectSlot(int)));
    ModuleDict *moduleDict = classDict[*clit];
    QDictIterator < GadModule > mit(*moduleDict);
    QStringList moduleList;
    qDebug("test\n");
    for (; mit.current(); ++mit)
      moduleList += mit.currentKey();
    moduleList.sort();
    QStringList::Iterator mlit;
    qDebug("test\n");
    for (mlit = moduleList.begin(); mlit != moduleList.end(); ++mlit) {
      qDebug("item=%d\n", item);
      classNameMenu_[item].className = *clit;
      classNameMenu_[item].name = *mlit;
      popup->insertItem(*mlit, item++);
      //      popup->insertItem(*mlit,this,SLOT(genNewObject(int)),0,item);
      qDebug("test\n");
    }
  }
  if (menubar_)
    popupId_ = menubar_->insertItem("&Glyphs", popup_);
  qDebug("     connecting with mainMenu\n");
  //  _qtguiappl->insertClass("classmenu",popup_);
}

void GadWorkspace::resize(void)
{
  QRect crect = childrenRect();
  QSize wsize = QSize(crect.x(), crect.y()) + crect.size() + QSize(50, 50);
  qDebug("GadWorkspace::resize(%d,%d)\n", wsize.width(), wsize.height());
  if (wsize != size())
    QWidget::resize(wsize);
  else
    repaint();
}

void GadWorkspace::inputSelected(GadObject * obj, int no)
{
  qDebug("GadWorkspace::inputSelected\n");
  input_.obj = obj;
  input_.no = no;
  if (output_.obj)
    connectObjects();
}

void GadWorkspace::outputSelected(GadObject * obj, int no)
{
  qDebug("GadWorkspace::outputSelected\n");
  output_.obj = obj;
  output_.no = no;
  if (input_.obj)
    connectObjects();
}

void GadWorkspace::disconnectObject(GadObject * dobj)
{
  qDebug("GadWorkspace::disconnectObject\n");
  const QObjectList *list = children();
  QListIterator < QObject > obj(*list);
  for (; obj.current(); ++obj) {
    if (obj.current()->inherits("GadObject")) {
      GadObject *gobj = (GadObject *) obj.current();
      gobj->disconnectObject(dobj);
    }
  }
}

void GadWorkspace::connectObjects()
{
  qDebug("GadWorkspace::connectObjects(%s(%d) -> %s(%d))\n",
         (const char *) output_.obj->module()->name(), output_.no,
         (const char *) input_.obj->module()->name(), input_.no);
  input_.obj->connectInput(input_.no, output_);
  input_.obj = 0;
  output_.obj = 0;
  input_.no = 0;
  output_.no = 0;
  repaint();
}

void GadWorkspace::init()
{
  input_.obj = 0;
  input_.no = 0;
  output_.obj = 0;
  output_.no = 0;
  popup_ = 0;
  classNameMenu_ = 0;
  classDict_ = 0;
  menubar_ = 0;
  mousePressed_ = false;
}

void GadWorkspace::newObjectSlot(int index)
{
  qDebug("GadWorkSpace::newObjectSlot(%d)\n", index);
  if (mousePressed_)
    return;
  newObject(index);
}

void GadWorkspace::newObject(int index, int x, int y)
{
  qDebug("GadWorksapce::newObject(%d, (%d,%d))\n", index, x, y);
  if (!classNameMenu_)
    return;
  if (index < 0)
    return;
  qDebug("  className=%s\n   name=%s\n",
         (const char *) classNameMenu_[index].className,
         (const char *) classNameMenu_[index].name);
  GadModule *module = GetModule(*classDict_,
                                classNameMenu_[index].className,
                                classNameMenu_[index].name);
  GadObject *obj = new GadObject(module, this);
  obj->move(x, y);
  obj->show();
  resize();
}

#ifdef NOT_NEEDED
void GadWorkspace::removeObject(GadObject * object)
{
  delete object;
}
#endif

void GadWorkspace::paintEvent(QPaintEvent * ev)
{
  qDebug("GadWorkspace::paintEvent\n");
  QPainter paint(this);
  const QObjectList *list = children();
  QListIterator < QObject > obj(*list);
  for (; obj.current(); ++obj) {
    if (obj.current()->inherits("GadObject")) {
      GadObject *gobj = (GadObject *) obj.current();
      if (!gobj->module())
        continue;
      paint.setPen(Qt::black);
      QString s =
        gobj->module()->name() + "(" + gobj->module()->className() + ")";
      int width = paint.fontMetrics().width(s);
      paint.drawText(gobj->x() + (gobj->width() - width) / 2,
                     gobj->y() + gobj->height() +
                     paint.fontMetrics().height(), s, 0, -1);

      qDebug("Connecting %s\n", (const char *) gobj->module()->name());
      int i;
      for (i = 0; i < gobj->numInputs(); i++) {
        GadConnect outputObj;
        gobj->getOutput(i, outputObj);
        if (!outputObj.obj)
          continue;
        QPoint p1 = gobj->inputPoint(i);
        QPoint p2 = outputObj.obj->outputPoint(outputObj.no);
        paint.setPen(QPen(Qt::black, 5));
        paint.drawLine(p1,                p1 + QPoint(-20, 0));
        paint.drawLine(p2,                p2 + QPoint( 20, 0));
        paint.drawLine(p1+QPoint(-20, 0), p2 + QPoint( 20, 0));
        paint.setPen(QPen(Qt::yellow, 3));
        paint.drawLine(p1,                p1 + QPoint(-20, 0));
        paint.drawLine(p2,                p2 + QPoint( 20, 0));
        paint.drawLine(p1+QPoint(-20, 0), p2 + QPoint( 20, 0));
      }
    }
  }


}

#define inRect(x,y,x1,y1,width,height) ((x>=x1) && (x<=x1+width) \
					&& (y>=y1) && (y<=y1+height))

void GadWorkspace::mousePressEvent(QMouseEvent * ev)
{
  qDebug("GadWorkspace::mousePressEvent");
  if (ev->button() != RightButton)
    return;
  if (popup_) {
    mousePressed_ = true;
    newObject(popup_->exec(QCursor::pos()), ev->x(), ev->y());
    mousePressed_ = false;
  }
}

QSize GadWorkspace::sizeHint() const
{
  QRect rect = childrenRect();
  qDebug("GadWorkspace::sizeHint(%d,%d)(%d,%d)\n",
         rect.x(), rect.y(), rect.width(), rect.height());
  return childrenRect().size();
}

QSize GadWorkspace::minimumSizeHint() const
{
  QSize size = childrenRect().size();
  qDebug("GadWorkspace::minimumSizeHint(%d,%d)\n", size.width(),
         size.height());
  return childrenRect().size();
}

QSizePolicy GadWorkspace::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum, false);
}



