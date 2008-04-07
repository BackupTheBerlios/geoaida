/***************************************************************************
                          attribute.cpp  -  description
                             -------------------
    begin                : Mon Oct 9 2000
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
 * $Source: /data/cvs/gda/gda/core/attribute.cpp,v $
 * $Revision: 1.14 $
 * $Date: 2002/12/13 06:22:13 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "attribute.h"
#include <qregexp.h>

Attribute::Attribute()
{
  init();
}

Attribute::Attribute(const Attribute & a)
{
  init(a.name_, a.label_, a.tip_);
  cmd_ = a.cmd_;
  defaultValue_ = a.defaultValue_;
  type_ = a.type_;
  switch (type_) {
  case INT:
    i_.min_ = a.i_.min_;
    i_.max_ = a.i_.max_;
    break;
  case DOUBLE:
    d_.min_ = a.d_.min_;
    d_.max_ = a.d_.max_;
    break;
  case BOOL:
    b_.onValue_ = new QString(*(a.b_.onValue_));
    b_.offValue_ = new QString(*(a.b_.offValue_));
    break;
  case ENUM:
    e_.options_ = new QStringList(*(a.e_.options_));
    break;
  }
}

/** Constructor for STRING-Attribute */
Attribute::Attribute(QString name, QString defaultValue, int type,
                     QString label, QString tip)
{
  init(name, label, tip);
  ASSERT(type == STRING || type == BOOL || type == COLOR);
#if 0
  qDebug("%d", type);
#endif
  type_ = type;
  defaultValue_ = defaultValue;
  if (type==BOOL) {
    b_.onValue_=new QString("true");
    b_.offValue_=new QString("false");
  }
}

/** Constructor for INT-Attribute */
Attribute::Attribute(QString name, QString defaultValue, int min, int max,
                     QString label, QString tip)
{
  init(name, label, tip);
  type_ = INT;
  i_.min_ = min;
  i_.max_ = max;
  defaultValue_ = defaultValue;
}

/** Constructor for DOUBLE-Attribute */
Attribute::Attribute(QString name, QString defaultValue, double min,
                     double max, QString label, QString tip)
{
  init(name, label, tip);
  type_ = INT;
  d_.min_ = min;
  d_.max_ = max;
  defaultValue_ = defaultValue;
}

/** Constructor for ENUM-Attribute */
Attribute::Attribute(QString name, QString defaultValue,
                     QStringList * options, QString label, QString tip)
{
  init(name, label, tip);
  type_ = ENUM;
  e_.options_ = options;
  defaultValue_ = defaultValue;
}

/** Constructor for OPERATOR-Attribute */
Attribute::Attribute(QString name, QString defaultValue, QString typeOfOp,
                     QString label, QString tip)
{
  init(name, label, tip);
  type_ = OPERATOR;
  o_.typeOfOp_ = new QString(typeOfOp);
}

Attribute::~Attribute()
{
  if (type_ == ENUM)
    if (e_.options_)
      delete e_.options_;
}

/** Initialize attribute */
void
Attribute::init(QString name, QString label, QString tip)
{
  name_ = name;
  cmd_ = "";
  prefix_="";
  defaultValue_ = "";
  type_ = STRING;
  label_ = (label.isEmpty()? name_ : label);
  tip_ = (tip.isEmpty()? QString("") : tip);
}

/** name of attribute */
QString
Attribute::name()
{
  return name_;
}

/** label for attribute */
QString
Attribute::label()
{
  if (label_.isEmpty())
    return name_;
  return label_;
}

/** tip for attribute */
QString
Attribute::tip()
{
  return tip_;
}

/** Read data from provided attribute list */
void
Attribute::set(ArgDict & attribs)
{
  MLParser::setString(name_, &attribs, "name");
  MLParser::setString(label_, &attribs, "label");
  MLParser::setString(tip_, &attribs, "tip");
  type_ = STRING;
  QString options[] =
    { "STRING", "INT", "BOOL", "DOUBLE", "FORMULA", "ENUM", "OPERATOR", "IMAGE", "FILE", "" };
  MLParser::setOption(type_, &attribs, "type", options);
  qDebug("Type %d\n", type_);
  switch (type_) {
  case STRING:
  case FORMULA:
    break;
  case INT:
    MLParser::setInt(i_.min_, &attribs, "min");
    MLParser::setInt(i_.max_, &attribs, "max");
    break;
  case DOUBLE:
    MLParser::setDouble(d_.min_, &attribs, "min");
    MLParser::setDouble(d_.max_, &attribs, "max");
    break;
  case BOOL:
    b_.onValue_=new QString();
    MLParser::setString(*b_.onValue_, &attribs, "on");
    b_.offValue_=new QString();
    MLParser::setString(*b_.offValue_, &attribs, "off");
    break;
  case ENUM:
    {
      QString val;
      MLParser::setString(val, &attribs, "options");
      e_.options_ = new QStringList();
      if (!val.isEmpty())
        *(e_.options_) = QStringList::split(",", val);
      break;
    }
  case OPERATOR:
    {
      o_.typeOfOp_ = new QString();
      MLParser::setString(*(o_.typeOfOp_), &attribs, "typeofop");
    }
    break;
  case IMAGE:
    {
      im_.imageType_ = new QString();
      MLParser::setString(*(im_.imageType_), &attribs, "imagetype");
    }
  case FILE:
    {
      file_.fileType_ = new QString();
      MLParser::setString(*(file_.fileType_), &attribs, "filetype");
    }
    break;
  }
  MLParser::setString(defaultValue_, &attribs, "value");
  cmd_ = "@" + name_ + "@";
  MLParser::setString(cmd_, &attribs, "cmd");
}

/** Build the command string using the given attribute-values for this attribute */
QString
Attribute::command(ArgDict & argdict)
{
  QString cmd = cmd_;
  if (!argdict[name_] || argdict[name_]->isEmpty())
    return "";
  QDictIterator < QString > it(argdict);
  for (; it.current(); ++it) {
    cmd.replace(QRegExp("@" + it.currentKey() + "@", false), *it.current());
  }
  qDebug("Attribute::command %s\n", (const char *) cmd);
  return cmd;
}

/** Returns the default value of this attribute */
QString
Attribute::value()
{
  return defaultValue_;
}

/** Type of this attribute */
int
Attribute::type()
{
  return type_;
}

/** Returns the options for an enum attribute, otherwise 0 */
QStringList *
Attribute::options()
{
  if (type_ == ENUM)
    return e_.options_;
  else
    return 0;
}

/** Returns the type of operator */
QString
Attribute::typeOfOperator()
{
  if (type_ == OPERATOR)
    return *(o_.typeOfOp_);
  else
    return "";
}

/** Returns the image type */
QString
Attribute::imageType()
{
  if (type_ == IMAGE)
    return *(im_.imageType_);
  else
    return "";
}

/** Returns the file type */
QString
Attribute::fileType()
{
  if (type_ == FILE)
    return *(file_.fileType_);
  else
    return "";
}

/** returns true, if changing this attribute requires an update of other attributes */
bool
Attribute::updateOther()
{
  if (type_ == OPERATOR)
    return true;
  else
    return false;
}

/** set the prefix */
void Attribute::prefix(QString prefix)
{
  prefix_=prefix;
}

/** returns the full name of this attribute containing the prefix and name */
QString Attribute::fullname()
{
  return prefix_+name_;
}
/** return on-value for bool attributes */
QString Attribute::on()
{
  if (type_ == BOOL)
    return *b_.onValue_;
  return QString("");
}

/** return off-value for bool attributes */
QString Attribute::off()
{
  if (type_ == BOOL)
    return *b_.offValue_;
  return QString("");
}

/** return true, if the given val is the on-value of a bool attribute */
bool Attribute::isOn(QString val)
{
  if (type_ != BOOL) return false;
  return val==*b_.onValue_;
}
