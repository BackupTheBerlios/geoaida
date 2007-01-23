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
 * $Source: /data/cvs/qtguiapplication/qtgui/qtgui/QtGuiVarModifier.h,v $
 * $Revision: 1.1 $
 * $Date: 2001/07/31 11:57:04 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef _QtGuiVarModifier_h
#define _QtGuiVarModifier_h
 
#include <qstring.h>

class QtGuiVarModifier {
 public:
  enum {
    VOID,
    STRING,
    INT,
    DOUBLE,
    PASSWORD
  };
  QtGuiVarModifier(void *addr, int type=VOID, int len=0, int frac=-1) {
    varSetAddr(addr,type,len,frac);
  }
  void valueSetDouble(double val);
  void valueSetInt(int val);
  void valueSetString(QString val);
  void varSetAddr(void *addr, int type=VOID, int len=0, int frac=-1) {
    var_void_=addr;
    type_=type;
    len_=len;
    frac_=frac;
  }
  QString valueGetString();
  double valueGetDouble();
  int valueGetInt();
  void *varGetAddr() {return var_void_;}
    
 protected:
  union {
    void* var_void_;
    double *var_double_;
    int *var_int_;
    QString *var_string_;
  };
  int type_;
  int len_;
  int frac_;
};

#endif
