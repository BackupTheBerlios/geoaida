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
 * $Source: /data/cvs/gad/gad/GadGuiObjectTable.cpp,v $
 * $Revision: 1.3 $
 * $Date: 2002/07/05 04:43:27 $
 * $Author: pahl $
 * $Locker:  $
 */

#include <GadGui.h>
#include <GadIO.h>
#include <GadText.h>
#include <GadEnum.h>
#include <GadBool.h>
#include <GadModule.h>

static QDict < void >_gadguitable(17, false);

static struct GadGuiFuncTable
{
  GadGuiFuncTable(const QString & n, GadGuiNewObj * f)
  {
    name = n;
    func = f;
  }
  QString name;
  GadGuiNewObj *func;
}

_gadguifunctable[] = {
  GadGuiFuncTable("MODULE", (GadGuiNewObj *) newGadModule),
    GadGuiFuncTable("INPUT", (GadGuiNewObj *) newGadIO),
    GadGuiFuncTable("OUTPUT", (GadGuiNewObj *) newGadIO),
    GadGuiFuncTable("TEXT", (GadGuiNewObj *) newGadText),
    GadGuiFuncTable("ENUM", (GadGuiNewObj *) newGadEnum),
    GadGuiFuncTable("BOOL", (GadGuiNewObj *) newGadBool),
    GadGuiFuncTable("", (GadGuiNewObj *) 0)
};

MLTagTable *_gadguitagtable;

void GadGuiInitObjectTable()
{
  int size;
  int i;
  for (size = 0; !_gadguifunctable[size].name.isEmpty(); size++);
  QString *keyword = new QString[size + 1];
  for (i = 0; i < size; i++) {
    keyword[i] = _gadguifunctable[i].name;
    qDebug("GadGuiInitObjectTable %s\n",
           (const char *) _gadguifunctable[i].name);
    if ((void *) _gadguifunctable[i].func)
      _gadguitable.insert(_gadguifunctable[i].name,
                          (void *) _gadguifunctable[i].func);
  }
  keyword[size] = "";

  _gadguitagtable = new MLTagTable(keyword);

}

GadGui *GadGuiNewObject(int tagid, MLParser * parser, ArgDict * argdict)
{
  GadGuiNewObj *func;
  if (tagid > 0) {
    qDebug("GadguiNewObjectId %d %s\n",
           tagid, (const char *) _gadguifunctable[tagid - 1].name);
    func = (GadGuiNewObj *) _gadguifunctable[tagid - 1].func;
    if (func)
      return (GadGui *) (*func) (parser, argdict);
  }
  delete parser->args();
  return 0;
}
