/***************************************************************************
                          ga_bu_generic.cpp  -  description
                             -------------------
    begin                : Wed Nov 21 2001
    copyright            : (C) 2001 by juergen bueckner
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

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "bottomup.h"
#include "neighbour.h"

// #define DEBUG_PRG

void usage(char* prg)
{
  printf("Usage\n"
         "  %s <infile> <outfile> <cmd>\n"
         "  processes the nodelist given <infile> using the given command <cmd>\n"
         "  and writes the result nodelist to <outfile>\n",prg);
  exit(-1);
}

//llx="1035" lly="218" urx="1044" ury="209"

int main(int argc, char *argv[])
{
  //cout  << "argc:" << argc <<endl;
  QString classes = "ALL";
  if (argc!=4)
    usage(argv[0]);
  BottomUp bu(argv[1]); // read infile
  NodeList *nl = bu.selectClass("ALL");
  qDebug("no elem %d",nl->count());
  if (nl->isEmpty()) return 0;
  Stack stack(nl,argv[2]);
  QString command(argv[3]);
  qDebug("Command=%s",command.latin1());
  stack.registerFunction("neighbour", neighbour);
  stack.run(command);
  qDebug("writing result");
  stack.write(argv[2],"region");
#if 0
  nl->writeOutImage(QString(argv[2])+".bu.node.plm");
  nl->writeGroupImage(QString(argv[2])+".bu.plm");
#endif
}

