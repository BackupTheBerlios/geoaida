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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtparser/MLParser.cpp,v $
 * $Revision: 1.4 $
 * $Date: 2005/01/27 08:15:28 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "MLParser.h"
#include <ctype.h>
#include <qstringlist.h>
#include <qregexp.h>

// #define DEBUG_MSG

QTextStream& ArgDict::write(QTextStream& fp) const
{
	{
		QDictIterator<QString> it=QDictIterator<QString>(*this);
  	for (;it.current(); ++it) {
  		QString s=(*it.current());
   	 	fp << it.currentKey() << "=\""
   	   	 << (s.replace(QRegExp("\\\\"),QString("\\\\"))).replace(QRegExp("\""),QString("\\\""))
   	     << "\" ";
  	}
  }	
 	return fp;
}

void ArgDict::insert(QString name, int val) {
	QString *s=new QString();
	s->sprintf("%d",val);
	QDict<QString>::insert(name,s);
}

void ArgDict::insert(QString name, double val) {
	QString *s=new QString();
	s->sprintf("%f",val);
	QDict<QString>::insert(name,s);
}

void ArgDict::insert(QString name, QString val) {
	QString *s=new QString(val);
	QDict<QString>::insert(name,s);
}

void ArgDict::insert(QString name, bool val) {
	QString *s=new QString();
	if (val) *s="TRUE";
	else *s="FALSE";
	QDict<QString>::insert(name,s);
}

void ArgDict::insert(QString name, QString *s) {
  if (s)
  	QDict<QString>::insert(name,new QString(*s));
}

void ArgDict::insert(QString name, const char *s) {
  if (s)
  	QDict<QString>::insert(name,new QString(s));
}

void ArgDict::replace(QString name, int val) {
	QString *s=new QString();
	s->sprintf("%d",val);
	QDict<QString>::replace(name,s);
}

void ArgDict::replace(QString name, double val) {
	QString *s=new QString();
	s->sprintf("%f",val);
	QDict<QString>::replace(name,s);
}

void ArgDict::replace(QString name, QString val) {
	QString *s=new QString(val);
	QDict<QString>::replace(name,s);
}

void ArgDict::replace(QString name, bool val) {
	QString *s=new QString();
	if (val) *s="TRUE";
	else *s="FALSE";
	QDict<QString>::replace(name,s);
}

void ArgDict::replace(QString name, QString *s) {
  if (s)
  	QDict<QString>::replace(name,new QString(*s));
}

void ArgDict::replace(QString name, const char *s) {
  if (s)
  	QDict<QString>::replace(name,new QString(s));
}

QTextStream& operator<<(QTextStream& fp, ArgDict& argDict)
{
	return argDict.write(fp);
}

/*****************************************************************
CLASS: MLTagTable
*****************************************************************/
MLTagTable::MLTagTable() : QDict<int>(17,false) 
{
  tagtable_=0;
}

MLTagTable::MLTagTable(QString *table) : QDict<int>(17,false) 
{
  set(table);
}

void MLTagTable::set(QString *table)
{
  int i;
  int size;

  for (size=0; !table[size].isEmpty(); size++);
  
  tagtable_=new int[size];

  for (i=0; !table[i].isEmpty(); i++) {
    tagtable_[i]=i+1;
    insert(table[i],&tagtable_[i]);
  }
}

MLTagTable::~MLTagTable()
{

}

/*****************************************************************
CLASS: MLParserPrivate
*****************************************************************/
class MLParserPrivate {
public:
  MLParserPrivate(QTextStream *istr) {
    istr_=istr; strstorage_=0; voidstorage_=0; 
    linenumber_=1; lineoffset_=0; lc_=0;
  }
  ~MLParserPrivate() {
    if (strstorage_) delete strstorage_;
    if (voidstorage_) delete voidstorage_;
  }
  void readOneChar() {     
    istr_->readRawBytes(&lc_,1);
    if (lc_=='\n') lineoffset_++;
  }
  void incLineNumber() { linenumber_+=lineoffset_; lineoffset_=0; }
  QString readWord(bool acceptSlash=true);
  QString readTagWord();
  /* Variables */
  QTextStream *istr_;
  char lc_; // last character
  bool endtag_;
  QString lasttag_;
  int linenumber_;
  int lineoffset_;
  QDict<QString> *strstorage_;
  QDict<void> *voidstorage_;
};

QString MLParserPrivate::readWord(bool acceptSlash)
{
  QString word="";
  if ((lc_=='>') || (lc_=='<') || (lc_=='/')) return word;
  //  istr_->skipWhiteSpace();
  do {
    readOneChar();
  } while ((QChar(lc_).isSpace()) && !istr_->atEnd());
  //  istr_->readRawBytes(&lc_,1);
  //  if (lc_=='\n') lineoffset_++;
#ifdef DEBUGMSG
  //  qDebug("read %c(%d)\n",lc_,lc_);
#endif
  if (lc_=='"') {
    readOneChar();
    while ((lc_!='"') && (!istr_->atEnd())) {
#ifdef DEBUGMSG
      //      qDebug("read %c(%d)\n",lc_,lc_);
#endif
      if (lc_=='\\') {
	readOneChar();
      }
      word+=lc_;
      readOneChar();
    }
  }
  else {
    if (!acceptSlash && (lc_=='/')) return word;
    else
      while ((lc_!='=') && (lc_!='>') && (lc_!='<') 
	     && !isspace(lc_) && (!istr_->atEnd())) {
#ifdef DEBUGMSG
	//      qDebug("read %c(%d)\n",lc_,lc_);
#endif
	if (lc_=='\\') {
	  readOneChar();
	}
	word+=lc_;
	readOneChar();
      }
  }
#ifdef DEBUGMSG
  //      qDebug("read %c(%d)\n",lc_,lc_);
#endif
  
  return word;
}

QString MLParserPrivate::readTagWord()
{
  QString keyword;
  endtag_=false;

  while (1) {
    while((lc_ != '<') && (lc_ != '/') && !istr_->atEnd()) {
      readOneChar();
    }
    if (lc_ != '/') {
      readOneChar();
    }
    if (lc_=='/') {
      if (lasttag_.isEmpty())
        {
          readOneChar();
          continue;
        }
      endtag_=true;
      readOneChar();
    }
    break;
  }
  incLineNumber();
  while (isgraph(lc_) && !isspace(lc_) && lc_!='>' && !istr_->atEnd()) {
    keyword+=lc_;
    readOneChar();
#ifdef DEBUGMSG
    //    qDebug("char=%c(%d)\n",lc_,lc_);
#endif
  }
  if (keyword.isEmpty() && endtag_ ) keyword=lasttag_;
  lasttag_=keyword;
  return keyword;
}

/*****************************************************************
CLASS: MLParser
*****************************************************************/
MLParser::MLParser(QIODevice *fp) : QTextStream(fp)
{
  this->setEncoding(Latin1);
  data_=new MLParserPrivate(this);
}

MLParser::~MLParser()
{
	delete data_;
}

int MLParser::tag(const MLTagTable &tagtable)
{
  QString keyword;
  int *tag;
  data_->incLineNumber();
  keyword=data_->readTagWord();
  if (keyword.isEmpty()) return END_OF_FILE;;
  tag=tagtable[keyword];
  if (!tag) return 0;
  else {
    if (data_->endtag_) return -*tag;
    else return *tag;
  }
}

ArgDict *MLParser::args(ArgDict *argdict, bool insertMode)
{
  QString argname;
  ArgDict *dict;
  data_->incLineNumber();
  if (argdict) dict=argdict;
  else { 
    dict=new ArgDict();
  }
  do {
    argname=data_->readWord(false);
#ifdef DEBUGMSG
    //  qDebug("lastchar=%c\n",data_->lc_);
#endif
    if (!argname.isEmpty()) {
      QString *argval = new QString();
      if (data_->lc_=='=') {
	*argval=data_->readWord(true);
      } else
	*argval="";
      if (insertMode)
	dict->insert(argname,argval);
      else
	dict->replace(argname,argval);
#ifdef DEBUGMSG
      qDebug("%s=%s\n",(const char*)argname,(const char*)*argval);
#endif
    }
  } while (!argname.isEmpty());
  return dict;
}

QString MLParser::readWord()
{
  if (data_->lc_=='>') data_->readOneChar();
  data_->incLineNumber();
  return data_->readWord();
}

QString MLParser::lasttagstr()
{
  return data_->lasttag_;
}

int MLParser::lineNumber()
{
  return data_->linenumber_;
}

int MLParser::addToLineNumber(int offset)
{
  data_->linenumber_+=offset;
  return data_->linenumber_;
}

void MLParser::storeStr(const QString& key, const QString& value)
{
  if (!data_->strstorage_) {
    data_->strstorage_=new QDict<QString>();
    data_->strstorage_->setAutoDelete(true);
  }
  data_->strstorage_->insert(key,new QString(value));
}

void MLParser::store(const QString& key, void *value)
{
  if (!data_->voidstorage_) {
    data_->voidstorage_=new QDict<void>();
    data_->voidstorage_->setAutoDelete(false);
  }
  data_->voidstorage_->insert(key,value);
}

QString* MLParser::getStr(const QString& key)
{
  if (!data_->strstorage_) return 0;
  return (*data_->strstorage_)[key];
}

void* MLParser::get(const QString& key)
{
  if (!data_->voidstorage_) return 0;
  return (*data_->voidstorage_)[key];
}

/*****************************************************************
CLASS: MLParser static functions
*****************************************************************/

void MLParser::setString(QString &str, ArgDict *dict, 
				       QString name) 
{
  QString *s=(*dict)[name];
  if (s) str=*s;
}

void MLParser::setBool(bool &var, ArgDict *dict, QString name) 
{
  QString *s=(*dict)[name];
  if (s) {
      var=true;
      if ((s->upper()=="OFF")
	  || (s->upper()=="0")
	  || (s->upper()=="FALSE")
	  || (s->upper()=="AUS")) var=false;
  }
}

void MLParser::setInt(int &var, ArgDict *dict, QString name) {
  QString *s=(*dict)[name];
  if (s) {
    QTextIStream istr(s);
    if (!istr.atEnd()) istr >> var;
  }
}

void MLParser::setDouble(double &var, ArgDict *dict,
				       QString name) 
{
  QString *s=(*dict)[name];
  if (s) var=s->toDouble();
}

void MLParser::setFloat(float &var, ArgDict *dict,
				       QString name)
{
  QString *s=(*dict)[name];
  if (s) var=s->toFloat();
}


int MLParser::getOption(QString val, QString *keywords)
{
  MLTagTable tagtable(keywords);
  int *tag=tagtable[val];
  if (!tag) return 0;
  else return *tag;
}

void MLParser::setOption(int &var, ArgDict *dict, 
				       QString name, QString *options)
{
  QString *s=(*dict)[name];
  if (s) var=getOption(*s,options);
}

void MLParser::setBitMask(int &var, ArgDict *dict, 
			  QString name, QString *options, int *optionvals)
{
  QString *s=(*dict)[name];
  MLTagTable tagtable(options);

  if (s) {
    var=0;
    QString attrib=*s;
    attrib.replace(QRegExp("[|,\\.;:]")," ");
    QTextIStream istr(&attrib);
    QString part;
    while (!istr.atEnd()) {
      istr >>part;
      int *tag=tagtable[part];
      if (tag) {
	if (optionvals)
	  var+=optionvals[*tag-1];
	else
	  var+=(1 << (*tag-1));
      }
    }

#ifdef DEBUGMSG
    qDebug("MLParser::setBitMask(): name=%s val=%s(%d)\n",
	   (const char*)name,(const char*)*s,var);
#endif
  }
}


