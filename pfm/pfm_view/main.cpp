/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Mon Aug 7 2000
    copyright            : (C) 2000 by jürgen bückner
    email                : bueckner@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "picwin.h"
#include <qscrollview.h>
//test (begin):
//#define DEBUG
//test (end):

int main( int argc, char **argv )
{
    QApplication a( argc, argv );
#if 0
argc=2;	
#endif
    PicWin base;
    if (argc == 1 ) {
      base.resize( 437, 267 );
      base.scaleImgSize(2.0,2.0);
    }
    base.setCaption( "PFM and more View" );
  	QObject::connect(&a,SIGNAL(lastWindowClosed()),&a,SLOT(quit()));
#if 0  	
    base.loadImage("laser_1th.pfm");
#endif
#if 1  	
    if (argc > 1 ) {
       base.loadImage(argv[1]);
       for (int i = 2; i < argc; i++) {
         PicWin *base2 = new PicWin;
         base2->loadImage(argv[i]);
         base2->show();
       }
    }
#endif
    base.show();
    return a.exec();
}
