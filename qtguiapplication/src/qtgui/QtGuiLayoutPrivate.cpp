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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiLayoutPrivate.cpp,v $
 * $Revision: 1.1 $
 * $Date: 2001/07/31 11:57:04 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiLayoutPrivate.h"

void QtGuiLayoutPrivate::createLayout(QWidget *parent, 
				      QLayout *parentlayout)
{
  if (parentlayout) {
    switch (layoutstyle_) {
    case VBOX:
      layout_=new QVBoxLayout();
      break;
    case HBOX:
      layout_=new QHBoxLayout();
      break;
    case GRID:
      layout_=new QGridLayout(rows_,cols_);
      break;
    default:
      layout_=0;
      return;
    }
    if (parentlayout->inherits("QBoxLayout"))
      ((QBoxLayout*)parentlayout)->addLayout(layout_);
    if (parentlayout->inherits("QGridLayout"))
#if 0
      ((QGridLayout*)parentlayout)->addLayout(layout_,xpos_,ypos_);
#endif
      ((QGridLayout*)parentlayout)->addLayout(layout_,
					      getInt(*argdict_,"X"),
					      getInt(*argdict_,"Y"));
  }
  else {
    switch (layoutstyle_) {
    case VBOX:
      layout_=new QVBoxLayout(parent);
      layout_->setMargin(10);
      layout_->setSpacing(10);
      break;
    case HBOX:
      layout_=new QHBoxLayout(parent);
      layout_->setMargin(10);
      layout_->setSpacing(10);
      break;
    case GRID:
      layout_=new QGridLayout(parent,rows_,cols_);
      break;
    default:
      layout_=0;
      return;
    }    
  }
}
