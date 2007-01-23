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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiBrowser.cpp,v $
 * $Revision: 1.3 $
 * $Date: 2002/02/22 14:34:21 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiBrowser.h"
#include "QtGuiDialogItemPrivate.h"
#include <qlist.h>
#include <qtextbrowser.h>

class QtGuiBrowserPrivate : public QtGuiDialogItemPrivate {
public:
  enum {
    STRING=1,
    INT,
    DOUBLE,
    PASSWORD
  };
  QtGuiBrowserPrivate() {
    location_="";
    home_="";
    browser_=0;
    variable_=0;
  }
  void setVars(QString classname) {
    QtGuiDialogItemPrivate::setVars(classname);
    setString(location_,argdict_,"location");
    setString(home_,argdict_,"home");
  }

  void setAttribs() {
    if (home_=="")
      browser_->setText(label_);
    else {
      browser_->setSource(home_);
      browser_->mimeSourceFactory()->addFilePath(location_);
      browser_->mimeSourceFactory()->setExtensionType("html", "text/utf8");
    }

    QtGuiPrivate::setAttribs();
  }
  void create(QWidget* parent, QLayout* layout);
  QTextBrowser *browser_;
  QtGuiVarModifier *variable_;
  // Attribs
  QString location_;
  QString home_;
};

/*****************************************************************
CLASS: QtGuiBrowser
*****************************************************************/
QtGuiBrowser *newQtGuiBrowser(MLParser *parser, ArgDict* argdict)
{
  return new QtGuiBrowser(parser,argdict);
}

QtGuiBrowser::QtGuiBrowser(MLParser *parser, ArgDict* argdict)
{
  qDebug("QtGuiBrowser::QtGuiBrowser\n");

  gdata_=idata_=data_=new QtGuiBrowserPrivate;
  if (!parser) return;


  // Evaluate arguments
  data_->argdict_=parser->args(argdict);
  data_->setVars("BROWSER");
  data_->regObj(this);
}

QtGuiBrowser::~QtGuiBrowser()
{
}

QWidget *QtGuiBrowser::create(QWidget* parent, QLayout* layout)
{
  qDebug("QtGuiBrowser::create\n");
  data_->create(parent,layout);
  variableGet();
  connect(data_->browser_,SIGNAL(textChanged()),
	  this,SLOT(stateChanged()));
  delete data_->argdict_;
  data_->argdict_=0;
  return data_->widget_;
}


QString QtGuiBrowser::valueGetStr()
{
  qDebug("QtGuiBrowser::valueGet(%s)\n",
	 (const char*)data_->browser_->context());
  return data_->browser_->context();
}

void QtGuiBrowser::valueSet(QString val)
{
  qDebug("QtGuiBrowser::valueSet(%s)\n",(const char*)val);
  if (data_->home_=="")
    data_->browser_->setText(val);
  else
    data_->browser_->setSource(val);
}


void QtGuiBrowser::variableGet()
{
  qDebug("QtGuiBrowser::variableGet\n");
  if (!data_->variable_) return;
  valueSet(data_->variable_->valueGetString());
}

void QtGuiBrowser::variableSet()
{
  qDebug("QtGuiBrowser::variableSet\n");
  if (!data_->variable_) return;
  data_->variable_->valueSetString(valueGetStr());
}

void QtGuiBrowser::stateChanged()
{
  if (idata_->sending_) {
    qDebug("QtGuiBrowser::stateChanged name=%s recursion detected\n",
	 (const char*)idata_->name_);
    return;
  }
  idata_->sending_=true;
  qDebug("QtGuiBrowser::stateChanged() name=%s val=%s\n",
	 (const char*)idata_->name_,(const char*)valueGetStr());
  if (idata_->variable_ && idata_->immediately_) {
    idata_->variable_->valueSetString(valueGetStr());
  }
  emit valueChanged(valueGetStr());
  idata_->sending_=false;
}

void QtGuiBrowser::backward()
{
  data_->browser_->backward();
}

void QtGuiBrowser::forward()
{
  data_->browser_->forward();
}

void QtGuiBrowser::home()
{
  data_->browser_->home();
}

/*****************************************************************
CLASS: QtGuiBrowserPrivate
*****************************************************************/
void QtGuiBrowserPrivate::create(QWidget *parent, QLayout *layout)
{
  variable_=(*(_qtguiappl->varDict()))[varname_];
  widget_=browser_=new QTextBrowser(parent,name_);
  setAttribs();
  addItem(layout,parent);
}

#ifndef TMAKECONF
#include "QtGuiBrowser.moc"
#endif
