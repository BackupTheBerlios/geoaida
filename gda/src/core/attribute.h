/***************************************************************************
                          attribute.h  -  description
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
 * $Source: /data/cvs/gda/gda/core/attribute.h,v $
 * $Revision: 1.11 $
 * $Date: 2002/12/12 16:02:06 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <qstring.h>
#include <qstringlist.h>
#include <MLParser.h>

/**Holds the name, type, parameter and default value of an attrbute
  *@author Martin Pahl
  */

class Attribute
{
public:
  Attribute();
        /** Copy constructor */
  Attribute(const Attribute & a);
  /** Constructor for STRING/COLOR-Attribute */
    Attribute(QString name, QString defaultValue, int type =
              STRING, QString label = "", QString tip = "");
        /** Constructor for INT-Attribute */
    Attribute(QString name, QString defaultValue, int min, int max,
              QString label = "", QString tip = "");
        /** Constructor for DOUBLE-Attribute */
    Attribute(QString name, QString defaultValue, double min, double max,
              QString label = "", QString tip = "");
        /** Constructor for ENUM-Attribute */
    Attribute(QString name, QString defaultValue, QStringList * options,
              QString label = "", QString tip = "");
        /** Constructor for OPERATOR-Attribute */
    Attribute(QString name, QString defaultValue, QString typeOfOp,
              QString label = "", QString tip = "");
   ~Attribute();
  /** name of attribute */
  QString name();
        /** label for attribute */
  QString label();
        /** tip for attribute */
  QString tip();
  /** Read data from provided attribute list */
  void set(ArgDict & attribs);
  /** Returns the default value of this attribute */
  QString value();
  /** Build the command string using the given attribute-values for this attribute */
  QString command(ArgDict & argdict);
  /** Type of this attribute */
  int type();
  /** Returns the options for an enum attribute, otherwise 0 */
  QStringList *options();
  /** Returns the type of operator */
  QString typeOfOperator();
  /** Returns the image type */
  QString imageType();
  /** Returns the file type */
  QString fileType();
  /** returns true, if changing this attribute requires an update of other attributes */
  bool updateOther();
  /** returns the full name of this attribute containing the prefix and name */
  QString fullname();
  /** set the prefix */
  void prefix(QString prefix);
  /** return true, if the given val is the on-value of a bool attribute */
  bool isOn(QString val);
  /** return off-value for bool attributes */
  QString off();
  /** return on-value for bool attributes */
  QString on();
  enum
  {
    STRING = 1,
    INT,
    BOOL,
    DOUBLE,
    FORMULA,
    ENUM,
    OPERATOR,
    IMAGE,
    COLOR,
    FILE
  };
protected:                     // Protected methods
  /** Initialize attribute */
  void init(QString name = "", QString label = "", QString tip = "");
protected:                     // Protected attributes
  union
  {
    struct
    {
      double min_, max_;
    }
    d_;
    struct
    {
      int min_, max_;
    }
    i_;
    struct {
      QString *onValue_;
      QString *offValue_;
    }
    b_;
    struct
    {
      QStringList *options_;
    }
    e_;
    struct
    {
      QString *typeOfOp_;
    }
    o_;
    struct
    {
      QString *imageType_;
    }
    im_;
    struct
    {
      QString *fileType_;
    }
    file_;
  };
  /** name of attribute */
  QString name_;
  /** type of attribute (STRING, INT, DOUBLE, ENUM...) */
  int type_;
  /** hold the default value for this attribute */
  /** command string for this attribute */
  QString cmd_;
  QString defaultValue_;
  QString label_;
  QString tip_;
public: // Public attributes
  /** prefix describing the section this attribute is used in */
  QString prefix_;
};

#endif
