/***************************************************************************
                          plot2d.h  -  description
                             -------------------
    begin                : Thu May 16 2002
    copyright            : (C) 2002 by jrgen bckner
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

#ifndef PLOT2D_H
#define PLOT2D_H

#include <qwidget.h>
#include <qpicture.h>

/**Plotting area for 2D-data
  *@author jrgen bckner
  */

class Plot2D : public QWidget  {
   Q_OBJECT
public: 
	Plot2D(QWidget *parent=0, const char *name=0);
	~Plot2D();
  /** set the picture to be drawn */
  void set(const QPicture& p);
protected:	
  /** No descriptions */
  void paintEvent(QPaintEvent* pev) ;
protected: // Protected attributes
  /**  */
  QPicture picture_;
};

#endif
