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
 * $Source: /data/cvs/qtguiapplication/qtgui/gen_po_file.cpp,v $
 * $Revision: 1.1 $
 * $Date: 2000/12/15 14:25:48 $
 * $Author: pahl $
 * $Locker:  $
 */
#include <MLParser.h>
#include <qfile.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <stdio.h>

void Usage()
{
  printf("Usage:\n  gen_po_file sourcefile ... >project.po\n");
}

void WritePoFileHead()
{
  printf("# This is a Qt message file in .po format.  Each msgid starts with\n");
  printf("# a scope.  This scope should *NOT* be translated - eg. \"Foo::Bar\"\n");
  printf("# would be translated to \"Pub\", not \"Foo::Pub\".\n");
  printf("msgid \"\"\n");
  printf("msgstr \"\"\n");
  printf("\"Project-Id-Version: example-Qt-message-extraction\\n\"\n");
  printf("\"POT-Creation-Date: 1999-02-23 15:38+0200\\n\"\n");
  printf("\"PO-Revision-Date: 1999-02-23 15:38+0200\\n\"\n");
  printf("\"Last-Translator: \\n\"\n");
  printf("\"Content-Type: text/plain; charset=iso-8859-1\\n\"\n");
}

void WritePoFile(const QString& classname,
		 const ArgDict& args, 
		 const QString& name,
		 const char* filename,
		 int linenumber)
{
  QString *s=args[name];
  if (s)
    {
      printf("#%s:%d\n",filename,linenumber);
      printf("msgid \"%s::%s\"\n",
	     (const char*)classname,
	     (const char*)*s);
      printf("msgstr \"\"\n\n");
    }
}

int main(int argc, char **argv)
{
  if (argc<2) 
    {
      Usage();
      return -1;
    }
  
  int i;
  WritePoFileHead();
  for (i=1; i<argc; i++)
    {
      QFile fp(argv[i]);
      if (!fp.open(IO_ReadOnly)) continue;
      MLParser parser(&fp);

      QString keyword[]={""};
      MLTagTable tagtable(keyword);
      int tag;
      QString classname;
      int linenumber;

      // Read Tags
      do {
	tag=parser.tag(tagtable);
	linenumber=parser.lineNumber();
	if (tag != MLParser::END_OF_FILE)
	  {
	    ArgDict *dict=parser.args();
	    classname=((*dict)["name"])
	      ?*(*dict)["name"]
	      :parser.lasttagstr().upper();
	    WritePoFile(classname,*dict,"label",argv[i],linenumber);
	    WritePoFile(classname,*dict,"tip",argv[i],linenumber);
	  }
      } while (tag!=MLParser::END_OF_FILE);
    }
  return 0;
}

