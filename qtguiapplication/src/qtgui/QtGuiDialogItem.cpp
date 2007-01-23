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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiDialogItem.cpp,v $
 * $Revision: 1.2 $
 * $Date: 2001/12/04 16:25:51 $
 * $Author: pahl $
 * $Locker:  $
 */

#include "QtGuiDialogItem.h"
#include "QtGuiDialogItemPrivate.h"

void
QtGuiDialogItem::variableSet()
{
  qDebug("QtGuiDialogItem::variableSet %s\n", (const char *) idata_->name_);
  if (!idata_->variable_)
    return;
  idata_->variable_->valueSetInt(valueGetInt());
}

void
QtGuiDialogItem::variableGet()
{
  qDebug("QtGuiDialogItem::variableGet %s\n", (const char *) idata_->name_);
  if (!idata_->variable_)
    return;
  valueSet(idata_->variable_->valueGetInt());
}

void
QtGuiDialogItem::enable(int val)
{
  qDebug("QtGuiDialogItem::enable(%d) should be overloaded\n", val);
}

#ifndef TMAKECONF
#include "QtGuiDialogItem.moc"
#endif
