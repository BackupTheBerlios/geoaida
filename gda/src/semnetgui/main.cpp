/****************************************************************
**       
** formeleditor  created by Ulrike Waschkowski
**
****************************************************************/

#include <qapplication.h>
#include <qlayout.h>
#include <qvbox.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qfont.h>
#include <qcombobox.h>

#include "formeditwidget.h"





int main( int argc, char **argv )
{
    QApplication a( argc, argv );

    FormEditWidget w;
    a.setMainWidget( &w );
    w.show();
    return a.exec();
}
