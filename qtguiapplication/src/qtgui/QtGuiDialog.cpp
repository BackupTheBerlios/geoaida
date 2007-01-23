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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiDialog.cpp,v $
 * $Revision: 1.2 $
 * $Date: 2001/12/20 13:40:20 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiDialog.h"
#include "QtGuiDialogItem.h"
#include "QtGuiTabView.h"
#if 0
#include "QtGuiLineEdit.h"
#include "QtGuiLabel.h"
#include "QtGuiButton.h"
#include "QtGuiCheckBox.h"
#include "QtGuiRadioButton.h"
#endif
#include "QtGuiLayoutPrivate.h"
#include <qlist.h>
#include <qdialog.h>
#include <qtabdialog.h>
#include <qpushbutton.h>
#include <qvbox.h>
#include <qapplication.h>
class QtGuiDialogPrivate : public QtGuiLayoutPrivate {
public:
  QtGuiDialogPrivate() {
    item_=new QList<QtGuiDialogItem>;
    item_->setAutoDelete(true);
    tabview_=0;
    name_="dialog";
  }
    
  void setVars(QString classname)
    {
      QtGuiLayoutPrivate::setVars(classname);
      setString(okbutton_,argdict_,"okbutton");
      setString(applybutton_,argdict_,"applybutton");
      setString(cancelbutton_,argdict_,"cancelbutton");
      setString(helpbutton_,argdict_,"helpbutton");
    }
  void setAttribs() {
    QtGuiPrivate::setAttribs();
    widget_->setCaption(label_);
  }
    
  void create(QtGuiDialog *pub_data, QWidget *parent);
  void connectObject();
  QList<QtGuiDialogItem> *item_;
  QList<QtGuiTabView> *tabview_;
  // Attribs
  QString okbutton_;
  QString cancelbutton_;
  QString applybutton_;
  QString helpbutton_;
};

/*****************************************************************
CLASS: QDefaultDialog
*****************************************************************/
class QDefaultDialog : public QDialog {
public:
  QDefaultDialog(QWidget * parent=0, 
		 const char * name=0, bool modal=FALSE, WFlags f=0);
  ~QDefaultDialog();
  QWidget *mainWidget() {return main_widget_;}
  QButton *okbutton() {return ok_button_;}
  QButton *applybutton() {return apply_button_;}
  QButton *cancelbutton() {return cancel_button_;}
  QButton *helpbutton() {return help_button_;}
  void setDefaultButton(QString &string);
  void setApplyButton(QString &string);
  void setCancelButton(QString &string);
  void setHelpButton(QString &string);
private:
  QButton *ok_button_;
  QButton *apply_button_;
  QButton *cancel_button_;
  QButton *help_button_;
  QVBoxLayout *layout_widget_;
  QWidget *main_widget_;
  QFrame *button_widget_;
  QHBoxLayout *button_layout_;
};

QDefaultDialog::QDefaultDialog(QWidget * parent, 
			       const char * name, 
			       bool modal, 
			       WFlags f)
  : QDialog(parent,name,modal,f)

{
  ok_button_=0;
  apply_button_=0;
  cancel_button_=0;
  help_button_=0;
  button_widget_=0;
  button_layout_=0;
  layout_widget_=new QVBoxLayout(this);
  main_widget_=new QFrame(this);
  layout_widget_->addWidget(main_widget_,0,0);
}

QDefaultDialog::~QDefaultDialog()
{
}

void QDefaultDialog::setDefaultButton(QString& string)
{
  if (!button_widget_) {
    button_widget_=new QFrame(this);
    layout_widget_->addWidget(button_widget_);
    button_layout_=new QHBoxLayout(button_widget_);
  }
  ok_button_=new QPushButton(button_widget_,"Ok");
  ok_button_->setText(string);
  button_layout_->addWidget(ok_button_,0,1);
  connect(ok_button_,SIGNAL(clicked()),SLOT(accept()));
}

void QDefaultDialog::setApplyButton(QString& string)
{
  if (!button_widget_) {
    button_widget_=new QFrame(this);
    layout_widget_->addWidget(button_widget_);
    button_layout_=new QHBoxLayout(button_widget_);
  }
  apply_button_=new QPushButton(button_widget_,"Apply");
  apply_button_->setText(string);
  button_layout_->addWidget(apply_button_,0,1);
}

void QDefaultDialog::setCancelButton(QString& string)
{
  if (!button_widget_) {
    button_widget_=new QFrame(this);
    layout_widget_->addWidget(button_widget_);
    button_layout_=new QHBoxLayout(button_widget_);
  }
  cancel_button_=new QPushButton(button_widget_,"Cancel");
  cancel_button_->setText(string);
  button_layout_->addWidget(cancel_button_,0,1);
  connect(cancel_button_,SIGNAL(clicked()),SLOT(reject()));
}

void QDefaultDialog::setHelpButton(QString& string)
{
  if (!button_widget_) {
    button_widget_=new QFrame(this);
    layout_widget_->addWidget(button_widget_);
    button_layout_=new QHBoxLayout(button_widget_);
  }
  button_layout_->addStretch(10);
  help_button_=new QPushButton(button_widget_,"Help");
  help_button_->setText(string);
  button_layout_->addWidget(help_button_,0,1);
}

/*****************************************************************
CLASS: QtGuiDialog
*****************************************************************/
QtGuiDialog *newQtGuiDialog(MLParser *parser, ArgDict* argdict)
{
  return new QtGuiDialog(parser,argdict);
}

QtGuiDialog::QtGuiDialog(MLParser *parser, ArgDict* argdict)
{
  qDebug("QtGuiDialog::QtGuiDialog\n");
  int tag;
  QtGuiDialogItem *item;
  QtGuiTabView *tabview;

  gdata_=data_=new QtGuiDialogPrivate;
  if (!parser) return;


  // Evaluate arguments
  data_->argdict_=(argdict?argdict:new ArgDict());
  data_->argdict_->insert("PARENT",new QString("DIALOG"));
  data_->argdict_=parser->args(data_->argdict_);
  data_->setVars("DIALOG");
  qDebug("register object\n");
  data_->regObj(this);

  // Read Tags
  do {
    tag=parser->tag(*_qtguitagtable);
    switch (tag) {
    case -TAG_DIALOG:
      break;
    case MLParser::END_OF_FILE:
      break;
    case TAG_TABVIEW:
      if (!data_->tabview_) {
	data_->tabview_=new QList<QtGuiTabView>;
	data_->tabview_->setAutoDelete(true);
      }
      tabview=(QtGuiTabView*)QtGuiNewObjectId(tag,parser);
      data_->tabview_->append(tabview);
      break;
    default:
      item=(QtGuiDialogItem*)QtGuiNewObjectId(tag,parser);
      data_->item_->append(item);
      break;
    }
      
  } while (tag!=-TAG_DIALOG);
}

QtGuiDialog::~QtGuiDialog()
{
}

QWidget *QtGuiDialog::create(QWidget* parent, QLayout*)
{
  qDebug("QtGuiDialog::create %s\n",(const char*)data_->name_);
  data_->create(this,parent);
  delete data_->argdict_;
  data_->argdict_=0;
  return data_->widget_;
}

void QtGuiDialog::connectObject()
{
  data_->connectObject();
  QtGui::connectObject();
}

void QtGuiDialog::show()
{
  qDebug("QtGuiDialog::show %s\n",(const char*)data_->name_);
  QtGuiDialogItem *item;
  item=data_->item_->first();
  while (item) {
    item->variableGet();
    item=data_->item_->next();
  }
  
  if (data_->tabview_) {
    QtGuiTabView *tvitem;
    tvitem=data_->tabview_->first();
    while (tvitem) {
      tvitem->variableGet();
      tvitem=data_->tabview_->next();
    }
  }
  
  data_->widget_->show();
  data_->widget_->raise();
  qDebug("QtGuiDialog::show %s done\n",(const char*)data_->name_);
}

void QtGuiDialog::hide()
{
  data_->widget_->hide();
}

void QtGuiDialog::labelSet(QString val)
{
  qDebug("QtGuiDialog::labelSet(%s)\n",(const char*)val);
  data_->widget_->setCaption(val);
}

void QtGuiDialog::applySlot()
{
  QtGuiDialogItem *item;
  item=data_->item_->first();
  while (item) {
    item->variableSet();
    item=data_->item_->next();
  }

  if (data_->tabview_) {
    QtGuiTabView *tvitem;
    tvitem=data_->tabview_->first();
    while (tvitem) {
      tvitem->variableSet();
      tvitem=data_->tabview_->next();
    }
  }
  emit apply();
}

void QtGuiDialog::okSlot()
{
  QtGuiDialogItem *item;
  item=data_->item_->first();
  while (item) {
    item->variableSet();
    item=data_->item_->next();
  }

  if (data_->tabview_) {
    QtGuiTabView *tvitem;
    tvitem=data_->tabview_->first();
    while (tvitem) {
      tvitem->variableSet();
      tvitem=data_->tabview_->next();
    }
  }
  emit ok();
}

void QtGuiDialog::cancelSlot()
{
  emit cancel();
}

void QtGuiDialog::helpSlot()
{
  emit help();
}

/*****************************************************************
CLASS: QtGuiDialogPrivate
*****************************************************************/
void QtGuiDialogPrivate::create(QtGuiDialog *pub_data, QWidget *parent)
{
  if (tabview_) {
    QTabDialog *dialog;
    widget_=dialog=new QTabDialog(parent,name_);
    setAttribs();
    //    createLayout(widget_);
    QTGUI_DO_ITEMS(QtGuiTabView,tabview_,create(widget_,layout_))
    if (!okbutton_.isEmpty()) dialog->setDefaultButton(okbutton_);
    if (!applybutton_.isEmpty()) dialog->setApplyButton(applybutton_);
    if (!cancelbutton_.isEmpty()) dialog->setCancelButton(cancelbutton_);
    if (!helpbutton_.isEmpty()) dialog->setHelpButton(helpbutton_);
    QObject::connect(dialog,SIGNAL(defaultButtonPressed()),
		     pub_data,SLOT(okSlot()));
    QObject::connect(dialog,SIGNAL(applyButtonPressed()),
		     pub_data,SLOT(applySlot()));
    QObject::connect(dialog,SIGNAL(cancelButtonPressed()),
		     pub_data,SLOT(cancelSlot()));
    QObject::connect(dialog,SIGNAL(helpButtonPressed()),
		     pub_data,SLOT(helpSlot()));
#if 0
    connectSignal(pub_data,SIGNAL(ok()),oksignal_);
    connectSignal(pub_data,SIGNAL(apply()),applysignal_);
    connectSignal(dialog,SIGNAL(cancelButtonPressed()),cancelsignal_);
    connectSignal(dialog,SIGNAL(helpButtonPressed()),helpsignal_);
#endif
  }
  else {
    QDefaultDialog *dialog;
    widget_=dialog=new QDefaultDialog(parent,name_);
    setAttribs();
    createLayout(dialog->mainWidget());
    QTGUI_DO_ITEMS(QtGuiDialogItem,item_,create(dialog->mainWidget(),layout_))
    if (!okbutton_.isEmpty()) dialog->setDefaultButton(okbutton_);
    if (!applybutton_.isEmpty()) dialog->setApplyButton(applybutton_);
    if (!cancelbutton_.isEmpty()) dialog->setCancelButton(cancelbutton_);
    if (!helpbutton_.isEmpty()) dialog->setHelpButton(helpbutton_);
    if (dialog->okbutton())
      QObject::connect(dialog->okbutton(),SIGNAL(clicked()),
		       pub_data,SLOT(okSlot()));
    if (dialog->applybutton())
      QObject::connect(dialog->applybutton(),SIGNAL(clicked()),
		       pub_data,SLOT(applySlot()));
    if (dialog->cancelbutton())
      QObject::connect(dialog->cancelbutton(),SIGNAL(clicked()),
		       pub_data,SLOT(cancelSlot()));
    if (dialog->helpbutton())
      QObject::connect(dialog->helpbutton(),SIGNAL(clicked()),
		       pub_data,SLOT(helpSlot()));
#if 0
    connectSignal(pub_data,SIGNAL(defaultPressed()),oksignal_);
    connectSignal(pub_data,SIGNAL(applyPressed()),applysignal_);
    connectSignal(dialog->cancelbutton(),SIGNAL(clicked()),cancelsignal_);
    connectSignal(dialog->helpbutton(),SIGNAL(clicked()),helpsignal_);
#endif
  }
  
}

void QtGuiDialogPrivate::connectObject()
{
  if (tabview_) {
    QTGUI_DO_ITEMS(QtGuiTabView,tabview_,connectObject())
  }
  else {
    QTGUI_DO_ITEMS(QtGuiDialogItem,item_,connectObject())
  }
  
}

#ifndef TMAKECONF
#include "QtGuiDialog.moc"
#endif
