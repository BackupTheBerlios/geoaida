// QtGui - Library 
// Copyright (C) 1999 Martin Pahl, Universität Hannover
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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtparser/MLParser.h,v $
 * $Revision: 1.4 $
 * $Date: 2005/01/27 08:15:28 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef _MLParser_h
#define _MLParser_h

#include <qdict.h>
#include <qtextstream.h>

typedef QDictIterator<QString> ArgDictIterator;
class ArgDict : public QDict<QString> {
 public:
  ArgDict() : QDict<QString>(17,FALSE){setAutoDelete(true);}
  ArgDict(const ArgDict& dict) : QDict<QString>(17,FALSE) {
    copy(dict);
  }
  ArgDict& operator=(const ArgDict& dict) {
    return copy(dict);
  }
  ArgDict& copy(const ArgDict& dict) {
    if (this!=&dict) {
      clear();
      for (ArgDictIterator it(dict); it.current(); ++it)
        insert(it.currentKey(),new QString(*(it.current())));
    }
    return *this;
  }
  /** write the contents of dictionary to the given QTextStream */
  virtual QTextStream& write(QTextStream&) const;
  void insert(QString name, int val);
  void insert(QString name, double val);
  void insert(QString name, QString val);
  void insert(QString name, bool val);
	void insert(QString name, QString *s);
	void insert(QString name, const char* s);
  void replace(QString name, int val);
  void replace(QString name, double val);
  void replace(QString name, QString val);
  void replace(QString name, bool val);
	void replace(QString name, QString *s);
	void replace(QString name, const char* s);
};

class MLTagTable : public QDict<int> {
 public:
  MLTagTable();
  MLTagTable(QString *table);
  ~MLTagTable();
  void set(QString *table);
 private:
  int *tagtable_;
};

class MLParserPrivate;
class MLParser : protected QTextStream {
 public:
  MLParser(QIODevice *fp);
  ~MLParser();
  int tag(const MLTagTable &tagtable);
  ArgDict *args(ArgDict *argdict=0, bool inserMode=true);
  QString readWord();
  QString lasttagstr();
  int lineNumber();
  int addToLineNumber(int offset);
  enum { END_OF_FILE = -1000};
  void storeStr(const QString& key, const QString& value);
  QString* getStr(const QString& key);
  void store(const QString& key, void *value);
  void* get(const QString& key);

  // functions to set ArgDict attributes
  static void setString(QString &str, ArgDict *dict, QString name);
  static void setBool(bool &var, ArgDict *dict, QString name);
  static void setInt(int &var, ArgDict *dict, QString name);
  static void setFloat(float &var, ArgDict *dict, QString name);
  static void setDouble(double &var, ArgDict *dict, QString name);
  static int getOption(QString val, QString *keywords);
  static void setOption(int &var, ArgDict *dict, 
			QString name, QString *options);
  static void setBitMask(int &var, ArgDict *dict, 
			QString name, QString *options, int *optionvals=0);
 private:
  MLParserPrivate *data_;
};

QTextStream& operator<<(QTextStream& fp, ArgDict&);


#endif
