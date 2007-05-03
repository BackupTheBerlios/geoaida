/***************************************************************************
                          propertyitem.cpp  -  description
                             -------------------
    begin                : Mon Sep 11 2000
    copyright            : (C) 2000 by Martin Pahl
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
 * $Source: /data/cvs/gda/gda/semnetgui/propertyitem.cpp,v $
 * $Revision: 1.19 $
 * $Date: 2004/02/27 10:30:17 $
 * $Author: mueller $
 * $Locker:  $
 */

#include "propertyitem.h"
#include "propertyeditor.h"
#include "qheader.h"
#include "attribute.h"
#include "qcombobox.h"
#include "qcheckbox.h"
#include "qlineedit.h"
#include "operatorlist.h"
#include "geoimagelist.h"
#include "qtooltip.h"
#include "qcolordialog.h"
#include <qtextedit.h>
#include "QtGuiApplication.h"
#include <qregexp.h>
// #define DEBUGMSG

PropertyItem::PropertyItem(GNode * node, PropertyEditor * parent,
                           QString name, QString val, bool readOnly,
                           Attribute * attrib)
:QListViewItem(parent, name, val)
{
  node_ = node;
  name_ = name;
  value_ = val;
  parent_ = parent;
  editor_ = 0;
  nameEditor_ = 0;
  readOnly_ = readOnly;
  attrib_ = attrib;
  if (attrib_) {
    setText(0, attrib_->label());
    connect(this, SIGNAL(requestUpdate()), parent_, SLOT(updateNode()));
  }
}

PropertyItem::PropertyItem(GNode * node, PropertyItem * parent, QString name,
                           QString val, bool readOnly, Attribute * attrib)
:QListViewItem(parent, name, val)
{
#ifdef DEBUGMSG
  qDebug("PropertyItem::PropertyItem(name=%s)", name.latin1());
#endif
  node_ = node;
  name_ = name;
  value_ = val;
  parent_ = (PropertyEditor *) (this->listView());
  editor_ = 0;
  nameEditor_ = 0;
  readOnly_ = readOnly;
  attrib_ = attrib;
  if (attrib_) {
    setText(0, attrib_->label());
    connect(this, SIGNAL(requestUpdate()), parent_, SLOT(updateNode()));
  }
}

PropertyItem::~PropertyItem()
{
  if (editor_)
    delete editor_;
}

/** Edit the attribute value */
void
PropertyItem::edit()
{
  if (readOnly_)
    return;
#ifdef DEBUGMSG
  qDebug("edit: name=%s\n", (const char *) name_);
#endif
  if (editor_) {
    delete editor_;
    editor_=0;
  }
  int type;
  if (!attrib_)
    type = Attribute::STRING;
  else {
    type = attrib_->type();
    if (value_.isEmpty())
      value_ = attrib_->value();
  }
  switch (type) {
  case Attribute::ENUM:{
      QStringList *options = attrib_->options();
      if (!options) return;
      editor_ = new QComboBox(parent_->viewport(), name_);
      ((QComboBox *) editor_)->insertStringList(*options);
      ((QComboBox *) editor_)->setCurrentItem(options->findIndex(value_));
    }
    break;
  case Attribute::BOOL:{
      editor_ = new QCheckBox(parent_->viewport(), name_);
      bool val = false;
      if (attrib_->isOn(value_))
        val = true;
      ((QCheckBox *) editor_)->setChecked(val);
    }
    break;
  case Attribute::OPERATOR:{
      QStringList *options = operatorList_.keys(attrib_->typeOfOperator());
      if (!options) return;
      editor_ = new QComboBox(parent_->viewport(), name_);
      ((QComboBox *) editor_)->insertStringList(*options);
      ((QComboBox *) editor_)->setCurrentItem(options->findIndex(value_));
      //                      connect(editor_,SIGNAL(activated(int)),this,SLOT(editDone()));
  }
    break;
  case Attribute::IMAGE:{
#ifdef DEBUGMSG
      qDebug("PropertyItem::edit() IMAGE type=%s",
             (const char *) attrib_->imageType());
#endif
      editor_ = new QComboBox(parent_->viewport(), name_);
      ((QComboBox *) editor_)->insertStringList(geoImageList_.
                                                list(attrib_->imageType()));
      ((QComboBox *) editor_)->setCurrentItem(1);
    }
    break;
  case Attribute::FORMULA:
    {
      static QTextEdit *formulaEditor =
        (QTextEdit *) _qtguiappl->getClass("formulaEditor");
      static QtGui *dialogGui =
        ((QtGui *) _qtguiappl->getClass("formulaEditorWin"));
      if (formulaEditor)
        editor_ = formulaEditor;
      else {
        QWidget *dialog = 0;
        if (dialogGui)
          dialog = dialogGui->widget();
        editor_ = new QTextEdit(dialog, name_);
      }
      ((QTextEdit *) editor_)->setText(value_);
      if (dialogGui) {
        dialogGui->show();
      }
      else
        editor_->show();
    }
    break;
  case Attribute::COLOR:{
#ifdef DEBUGMSG
      qDebug("PropertyItem::edit() COLOR color=%s", (const char *) value_);
#endif
      int r = 0;
      int g = 0;
      int b = 0;
      QString attrib = value_;
      attrib.replace(QRegExp("[,;:]"), " ");
      QTextIStream istr(&attrib);
      if (!istr.atEnd())
        istr >> r;
      if (!istr.atEnd())
        istr >> g;
      if (!istr.atEnd())
        istr >> b;

      QColor color =
        QColorDialog::getColor(QColor(r, g, b), parent_->viewport(),
                               "Color dialog");
      editor_ = 0;
      if (color.isValid()) {
        value_.sprintf("%d:%d:%d", color.red(), color.green(), color.blue());
        node_->attributeRemove(name_);
        node_->attribute(name_, value_);
        setText(1, value_);
        repaint();
      }
    }
  default:
    editor_ = new QLineEdit(parent_->viewport(), name_);
    ((QLineEdit *) editor_)->setText(value_);
    break;
  }
  QRect r = listView()->itemRect(this);
  r.setX(r.x() + parent_->columnWidth(0));
  if (editor_  && type!=Attribute::FORMULA) {
    editor_->setGeometry(r);
    editor_->show();
    editor_->setFocus();
  }
#ifdef DEBUGMSG
  qDebug("edit: pos=(%d,%d)\n", r.x(), r.y());
#endif
}

/** Edit the PropertyName */
void
PropertyItem::editName()
{
  if (readOnly_)
    return;
#ifdef DEBUGMSG
  qDebug("editName: name=%s", (const char *) name_);
#endif
  if (nameEditor_)
    delete nameEditor_;
  nameEditor_ = new QLineEdit(parent_->viewport(), name_);
  QRect r = listView()->itemRect(this);
  nameEditor_->setGeometry(r);
  nameEditor_->setText(name_);
  nameEditor_->show();
  nameEditor_->setFocus();
  nameEditor_->setSelection(0, 30);
#ifdef DEBUGMSG
  qDebug("edit: pos=(%d,%d)", r.x(), r.y());
#endif
}

/** Finish editing process */
void
PropertyItem::editDone()
{
  if (readOnly_)
    return;
  int type;
  if (!attrib_)
    type = Attribute::STRING;
  else
    type = attrib_->type();
  if (editor_) {
    switch (type) {
    case Attribute::ENUM:
    case Attribute::OPERATOR:
    case Attribute::IMAGE:
      value_ = ((QComboBox *) editor_)->currentText();
      addToolTip();
      break;
    case Attribute::BOOL:
      value_ = (((QCheckBox *) editor_)->isChecked()? attrib_->on() : attrib_->off());
      break;
    case Attribute::FORMULA:
      value_ = ((QTextEdit *) editor_)->text();
      editor_ = 0;
      static QtGui *dialogGui =
        ((QtGui *) _qtguiappl->getClass("formulaEditorWin"));
      if (dialogGui)
        dialogGui->hide();
      else
        editor_->hide();
      break;
    default:
      value_ = ((QLineEdit *) editor_)->text();
    }
#if 0
    if (name_ == "name")
      node_->name(value_);
    else if (name_ == "min")
      node_->min(value_.toInt());
    else if (name_ == "max")
      node_->max(value_.toInt());
    else if (name_ == "isCompound")
      node_->isCompound(value_.toInt() != 0);
    else {
#endif
      node_->attributeRemove(name_);
      node_->attribute(name_, value_);
#if 0
    }
#endif
    setText(1, value_);
    repaint();
    if (editor_)
      delete editor_;
    parent_->triggerUpdate (); 
    editor_ = 0;
    if (attrib_ && attrib_->updateOther())
      emit requestUpdate();
  }
  if (nameEditor_) {
    name_ = nameEditor_->text();
    node_->attributeRemove(name_);
#if 0
    if (name_ == "name")
      node_->name(value_);
    else if (name_ == "min")
      node_->min(value_.toInt());
    else if (name_ == "max")
      node_->max(value_.toInt());
    else {
#endif
      node_->attribute(name_, value_);
#if 0
    }
#endif
    setText(0, name_);
    repaint();
    delete nameEditor_;
    parent_->triggerUpdate (); 
    nameEditor_ = 0;
  }
}

/** Deletes this attribute from nodes attributeList */
bool
PropertyItem::attributeRemove()
{
#if 0
  if (!node_->predefinedAttribs().contains(name_)) {
#endif
    node_->attributeRemove(name_);
    return true;
#if 0
  }
  return false;
#endif
}

/** add a tooltip to this item */
void
PropertyItem::addToolTip()
{
  QRect r = parent_->itemRect(this);
  int height = r.height();
  r.setY(r.y() + height);       //parent_->columnWidth(0));
  r.setHeight(height);
#ifdef DEBUGMSG
  qDebug("Rectangle=(%d %d / %d %d )", r.x(), r.y(), r.width(), r.height());
#endif
  if (attrib_)
    switch (attrib_->type()) {
    case Attribute::OPERATOR:{
        Operator *op = operatorList_[value_];
        QToolTip::remove(parent_, r);
        if (op)
          QToolTip::add(parent_, r, op->tip());

      }
      break;
    default:
      QToolTip::add(parent_, r, attrib_->tip());
    }
//      else
//              QToolTip::add(parent_,r,"Keine <b>Hilfe</b>");
}
