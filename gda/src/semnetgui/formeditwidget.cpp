/****************************************************************
**       
** formedit.cpp   created by Ulrike Waschkowski
**
****************************************************************/

#include <qapplication.h>
#include <qlayout.h>
#include <qvbox.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qmultilineedit.h>
#include <qcombobox.h>

#include <stdio.h>
#include "formeditwidget.h"

FormEditWidget::FormEditWidget( QWidget *parent, const char *name )
  : QWidget( parent, name )
{
  QVBoxLayout *vlayout = new QVBoxLayout( this, 30, 15 );// vertikale Box 

  formview = new FormViewWidget(this, "formview"  );

  // HBox Label Titel
  QHBoxLayout *hlayout1 = new QHBoxLayout((QLayout*)vlayout, 10 );

  QLabel *titel = new QLabel(this, "GEOAIDA - Formeleditor" , this );
  titel->setFont( QFont("Helvetica", 20, QFont::Bold)  );
  titel->setAlignment( AlignCenter );
  hlayout1->addWidget( titel  );

  // HBox Objektauswahl und Funktionsauswahl
  QHBoxLayout *hlayout2 = new QHBoxLayout((QLayout*)vlayout, 10 );

  QVBoxLayout *vlayout1 = new QVBoxLayout(this );

  QLabel *objtitel = new QLabel(this,"Objektauswahl:", this );
  objtitel->setFont( QFont("Helvetica", 14, QFont::Bold)  );
  vlayout1->addWidget( objtitel );

  QComboBox *objects = new QComboBox(FALSE, this, "objects");
  objects->insertItem("1 " );
  objects->insertItem("2 " );
  objects->insertItem("3 " );
  objects->insertItem("4 " );
  objects->insertItem("5 " );
  objects->insertItem("6 " );
  objects->insertItem("7 " );
  objects->insertItem("8 " );
  objects->insertItem("9 " );
  objects->insertItem("0 " );
 vlayout1->addWidget( objects );
  connect (objects, SIGNAL(activated(const QString&)), formview, SLOT(insert(const QString&)));

  QComboBox *objects2 = new QComboBox(FALSE, this, "objects2");
  QFile f("file1.txt");
  if (f.open(IO_ReadOnly) ) {    // file opened successfully
    QTextStream t( &f );        // use a text stream
    QString s;
    while ( !t.eof() ) {        // until end of file...
      s = t.readLine();       // line of text excluding '\n'
      objects2->insertItem(s);
    }
    f.close();
  }
  vlayout1->addWidget( objects2 );
  connect (objects2, SIGNAL(activated(const QString&)), formview, SLOT(insert(const QString&)));

  QLabel *obj3titel = new QLabel(this,"Objektfunktionen:", this );
  obj3titel->setFont( QFont("Helvetica", 14, QFont::Bold)  );
  vlayout1->addWidget( obj3titel );

  QComboBox *objects3 = new QComboBox(FALSE, this, "objects3");
  QFile f2("file2.txt");
  if (f2.open(IO_ReadOnly) ) {    // file opened successfully
    QTextStream t( &f2 );        // use a text stream
    QString s;
    while ( !t.eof() ) {        // until end of file...
      s = t.readLine();       // line of text excluding '\n'
      objects3->insertItem(s);
    }
    f.close();
  }
  vlayout1->addWidget( objects3 );
  connect (objects3, SIGNAL(activated(const QString&)), formview, SLOT(insert(const QString&)));

  hlayout2->addLayout (vlayout1);

  QVBoxLayout *vlayout2 = new QVBoxLayout(this );

  QLabel *functitel = new QLabel(this,"Arithmetische Funktionen:", this );
  functitel->setFont( QFont("Helvetica", 14, QFont::Bold)  );
  vlayout2->addWidget( functitel );

  QComboBox *func = new QComboBox(FALSE, this, "functions");
  func->insertItem(" +" );
  func->insertItem(" -" );
  func->insertItem(" *" );
  func->insertItem(" /" );
  connect (func, SIGNAL(activated(const QString&)), formview, SLOT(insert(const QString&)));
  vlayout2->addWidget( func );

  QLabel *func2titel = new QLabel(this,"Logische Funktionen:", this );
  func2titel->setFont( QFont("Helvetica", 14, QFont::Bold)  );
  vlayout2->addWidget( func2titel );

  QComboBox *func2 = new QComboBox(FALSE, this, "functions2");
  func2->insertItem(" <" );
  func2->insertItem(" >" );
  func2->insertItem(" =" );
  connect (func2, SIGNAL(activated(const QString&)), formview, SLOT(insert(const QString&)));
  vlayout2->addWidget( func2 );
 
  QLabel *func3titel = new QLabel(this,"Klammern:", this );
  func3titel->setFont( QFont("Helvetica", 14, QFont::Bold)  );
  vlayout2->addWidget( func3titel );

  QComboBox *func3 = new QComboBox(FALSE, this, "functions3");
  func3->insertItem(" ( )" );
  func3->insertItem(" [ ]" );
  connect (func3, SIGNAL(activated(const QString&)), formview, SLOT(changeCurs(const QString&)));
  vlayout2->addWidget( func3 );
 
  hlayout2->addLayout (vlayout2);

  //HBox MultiLineEdit Anzeige
  QHBoxLayout *hlayout3 = new QHBoxLayout((QLayout*)vlayout, 10 );
  hlayout3->addWidget( formview  );


  vlayout->activate();
 
  setMinimumSize( 600, 400 );
  setMaximumSize( 900, 700 );
  setGeometry( 100, 100, 300, 300);
 
}


