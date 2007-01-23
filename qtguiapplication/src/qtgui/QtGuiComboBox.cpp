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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiComboBox.cpp,v $
 * $Revision: 1.2 $
 * $Date: 2001/11/05 14:44:54 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiComboBox.h"
#include "QtGuiDialogItem.h"
#include "QtGuiLayoutPrivate.h"
#include <qlist.h>
#include <qcombobox.h>
#include <qstringlist.h>

class QtGuiComboBoxPrivate : public QtGuiLayoutPrivate {
public:
  QtGuiComboBoxPrivate() {
    name_="";
    combobox_=0;
  }
    
  void setAttribs() {
    QtGuiPrivate::setAttribs();
  }
    
  void create(QtGuiComboBox*,QWidget *parent, QLayout *layout);
  QComboBox *combobox_;
  int num_items_;
  int *id_array_;
  QStringList item_list_;
};

/*****************************************************************
CLASS: QtGuiComboBox
*****************************************************************/
QtGuiComboBox *newQtGuiComboBox(MLParser *parser, ArgDict* argdict)
{
  return new QtGuiComboBox(parser,argdict);
}

QtGuiComboBox::QtGuiComboBox(MLParser *parser, ArgDict* argdict)
{
  qDebug("QtGuiComboBox::QtGuiComboBox\n");
  gdata_=idata_=data_=new QtGuiComboBoxPrivate;
  if (!parser) return;

  // Evaluate arguments
  data_->argdict_=parser->args(argdict);
  data_->setVars("COMBOBOX");
  if (data_->varname_.isEmpty()) {
    qDebug("QtGuiComboBox(%s): no varname given\n",(const char*)data_->name_);
    data_->varname_=data_->name_;
  }
  qDebug("register object\n");
  data_->regObj(this);

  // Read Tags
  int tag;
  int array[1000];
  int count=0;
  ArgDict* dict;
  do {
    tag=parser->tag(*_qtguitagtable);
    switch (tag) {
    case TAG_ITEM:
      dict=parser->args();
      array[count]=data_->getInt(*dict,"ID",count);
      data_->item_list_+=data_->getString(*dict,"LABEL","(noname)");
      delete dict;
      count++;
      break;
    case -TAG_COMBOBOX:
      break;
    case MLParser::END_OF_FILE:
      break;
    default:
      break;
    }
      
  } while (tag!=-TAG_COMBOBOX);
  data_->num_items_=count;
  if (data_->num_items_>0) {
    data_->id_array_=new int[data_->num_items_];
    int i;
    for (i=0; i<data_->num_items_; i++) data_->id_array_[i]=array[i];
  }
}

QtGuiComboBox::~QtGuiComboBox()
{
}

QWidget *QtGuiComboBox::create(QWidget* parent, QLayout* layout)
{
  qDebug("QtGuiComboBox::create %s\n",(const char*)data_->name_);
  data_->create(this,parent,layout);
  variableGet();
  if (data_->isSet(*(data_->argdict_),"val")) 
    valueSet(data_->getInt(*(data_->argdict_),"val"));
  qDebug("QtGuiComboBox::create %s done\n",(const char*)data_->name_);
  delete data_->argdict_;
  data_->argdict_=0;

  return data_->widget_;
}

void QtGuiComboBox::valueSet(int val)
{
  qDebug("QtGuiComboBox::valueSet val=%d\n",val);
  int i;
  for (i=0; i<data_->num_items_; i++) if (data_->id_array_[i]==val) break;
  if (i<data_->num_items_) {
    qDebug("QtGuiComboBox::valueSet val=%d(%d) Done\n",val,i);
    data_->combobox_->setCurrentItem(i);
  }
  //  emit valueChanged(val);
}

int QtGuiComboBox::valueGetInt()
{
  int i=data_->combobox_->currentItem();
  if (i<data_->num_items_) return data_->id_array_[i];
  else return 0;
}


/*****************************************************************
CLASS: QtGuiComboBoxPrivate
*****************************************************************/
void QtGuiComboBoxPrivate::create(QtGuiComboBox *pub_data,
				     QWidget *parent,QLayout *layout)
{
  variable_=(*(_qtguiappl->varDict()))[varname_];
  if (!variable_) {
    qDebug("QtGuiComboBox::create varname=%s not found\n",
	   (const char*)varname_);
    }
  widget_=combobox_=new QComboBox(parent,name_);
  combobox_->setAutoResize(true);
  combobox_->insertStringList(item_list_);
  setAttribs();
  addItem(layout,parent);
  pub_data->connect(combobox_,SIGNAL(activated(int)),
		    pub_data,SLOT(stateChanged()));
}

#ifndef TMAKECONF
#include "QtGuiComboBox.moc"
#endif
