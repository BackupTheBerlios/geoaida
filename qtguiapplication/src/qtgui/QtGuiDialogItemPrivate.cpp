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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiDialogItemPrivate.cpp,v $
 * $Revision: 1.4 $
 * $Date: 2002/02/22 14:34:21 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiDialogItemPrivate.h"
#include <qstatusbar.h>
#include <qscrollview.h>
void
QtGuiDialogItemPrivate::addItem(QLayout * layout, QWidget * parent)
{
  if (layout) {
    if (layout->isA("QGridLayout")) {
      int xpos = getInt(*argdict_, "X", -1);
      int ypos = getInt(*argdict_, "Y", -1);
      qDebug("QtGuiDialogItemPrivate::addItem GridLayout %d %d\n", xpos,
             ypos);
      int cols = getInt(*argdict_, "COLSPAN", 0);
      int rows = getInt(*argdict_, "ROWSPAN", 0);
      if ((xpos == -1) || (ypos == -1)) {
        layout->add(widget_);
      }
      else {
        if ((cols == 0) && (rows == 0))
          ((QGridLayout *) layout)->addWidget(widget_, ypos, xpos);
        else
          ((QGridLayout *) layout)->addMultiCellWidget(widget_,
                                                       ypos, ypos + cols - 1,
                                                       xpos, xpos + rows - 1);
        int colstretch = getInt(*argdict_, "COLSTRETCH", 0);
        int rowstretch = getInt(*argdict_, "ROWSTRETCH", 0);
        qDebug("QtGuiDialogItemPrivate::addItem GridLayout stretch %d %d\n",
               colstretch, rowstretch);
        if (colstretch)
          ((QGridLayout *) layout)->setColStretch(xpos, colstretch);
        if (rowstretch)
          ((QGridLayout *) layout)->setRowStretch(ypos, rowstretch);
      }
    }
    if (layout->inherits("QBoxLayout")) {
      int stretch = getInt(*argdict_, "STRETCH", 0);
      QString option[] = { "LEFT",
        "RIGHT",
        "HCENTER",
        "TOP",
        "BoTTOM",
        "VCENTER",
        "CENTER",
        "WORDBREAK",""
      };
      int optionval[] = { Qt::AlignLeft,
        Qt::AlignRight,
        Qt::AlignHCenter,
        Qt::AlignTop,
        Qt::AlignBottom,
        Qt::AlignVCenter,
        Qt::AlignCenter,
        Qt::WordBreak
      };
      int align = getBitMask(*argdict_, "ALIGN", option, optionval, 0);
      qDebug("QtGuiDialogItemPrivate::addItem BoxLayout\n");
      ((QBoxLayout *) layout)->addWidget(widget_, stretch, align);
    }
  }
  if (parent) {
    if (parent->inherits("QStatusBar")) {
      qDebug
        ("QtGuiDialogItemPrivate::additem: StatusBar stretch=%d perm=%d\n",
         getInt(*argdict_, "STRETCH", 0), getBool(*argdict_, "PERMANENT",
                                                  false));


      ((QStatusBar *) parent)->addWidget(widget_,
                                         getInt(*argdict_, "STRETCH", 0),
                                         getBool(*argdict_, "PERMANENT",
                                                 false));
    }
    if (parent->inherits("QScrollView")) {
      qDebug("QtGuiDialogItemPrivate::additem: ScrollView\n");
      ((QScrollView*)parent)->addChild(widget_);
    }
  }
  qDebug("addItem done\n");
}
