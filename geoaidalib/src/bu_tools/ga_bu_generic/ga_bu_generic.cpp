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
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
#include <cstdio>
#include <cstdlib>
#include "stack.h"
#include "bottomup.h"
#include "neighbour.h"
#include <iostream>


#include <cstdlib>

//  #define DEBUG_PRG

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
  //  cout  << "argc:" << argc <<endl;

  QString classes = "ALL";

  if (argc!=4)
    usage(argv[0]);
  BottomUp bu(argv[1]); // read infile tnt.uni-hannover.de/

  NodeList *nl = bu.selectClass("ALL");
  qDebug("no elem %d",nl->count());
  if (nl->isEmpty()) return 0;

  Stack stack(nl,argv[2]);
  QString command(argv[3]);

  qDebug("Command=%s",command.latin1());
  stack.registerFunction("neighbour", neighbour);
  if (!stack.run(command)){
    std::cerr << "ga_bu_generic: Error while running stack" << std::endl;
    return EXIT_FAILURE;
  }

  stack.write(argv[2]);
  nl->writeOutImage(QString(argv[2])+".bu.node.plm");
  nl->writeGroupImage(QString(argv[2])+".bu.plm");


}

// /home/bueckner/project/geoAIDA/geoaidalib/geoaida/bu_tools/ga_bu_generic
// /project/geoaida/share/data/test_files/stack
// settlement_4_Haus_13_greenland_2_agriculture_6
// OUT "nodelist \"Haus\" selectClass  \"squareness\" calc \"orthogonallity\" calc dup max \"p\" set "
