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
#ifndef DEBUG_PRG
  if (argc!=4)
    usage(argv[0]);
  BottomUp bu(argv[1]); // read infile                                                              nt.uni-hannover.de/
#else
  //BottomUp bu("/project/geoaida/share/data/test_files/stack/settlement_4_Haus_13_greenland_2_agriculture_6");
  BottomUp bu("/project/geoaida/share/data/test_files/ga_bu_generic/schattern.regs");
  //  BottomUp bu("/project/geoaida/share/data/test_files/ga_bu_generic/0016-input");
  //BottomUp bu("/home/bueckner/.gda/tmp/8445/Szene/Region_013/Ortslage_013/Industrie_013/0012-input");
#endif
  NodeList *nl = bu.selectClass("ALL");
  qDebug("no elem %d",nl->count());
  if (nl->isEmpty()) return 0;
#ifndef DEBUG_PRG
  Stack stack(nl,argv[2]);
  QString command(argv[3]);
#else
// QString command("nodelist \"5 6 * \" runlocal \"test\" set \"test\" get");
  Stack stack(nl,"/project/geoaida/share/data/test_files/stack/output.regs");
  QString command("nodelist \"size\" calc 400 > select merge");
  //QString command("nodelist \"size\"  calc \"thickness\"  calc");
  //QString command(" nodelist \"Haus\" selectClass \"compactness\" calc \"p\" set nl_max set_judgement nodelist \"settlement\" selectClass \"size\" calc 6 > select concat merge nodelist \"compactness\" calc");
  //QString command("nodelist \"Gebaeude\" selectClass \"size\" calc 675 > select  \"squareness\" calc \"orthogonallity\" calc dup max  \"compactness\" calc dup max 0.0001 + \"p\" set nl_average set_judgement nodelist \"settlement\" selectClass  \"size\" calc 4 > select concat merge nodelist  \"compactness\" calc");
  //QString command("nodelist \"Gebaeude\" selectClass \"size\" calc 675 > select \"squareness\" calc \"orthogonallity\" calc dup max \"compactness\" calc dup max 0.15 + \"p\" set nl_average set_judgement nodelist \"settlement\" selectClass \"size\" calc \"dup\" runlocal 20 > select 23000 < select  concat nodelist \"Haus\" selectClass concat nodelist \"greenland\" selectClass \"size\" calc  \"dup\" runlocal 20 > select 5000 < select concat nodelist \"agriculture\" selectClass \"size\" calc  \"dup\" runlocal 20 > select 5000 < select  concat merge nodelist  \"compactness\" calc");
//  QString command("nodelist \"Gebaeude\" selectClass \"size\" calc 675 > select nodelist \"agriculture\" selectClass \"size\" calc \"dup\" runlocal 6000 < select concat merge \"compactness\" calc");
  //QString command("nodelist \"Haus\" selectClass \"size\" calc \"compactness\" calc merge \"compactness\" calc");
  //QString command("nodelist \"Haus\" selectClass  \"size\" calc 200 > select \"squareness\" calc \"orthogonallity\" calc \"compactness\" calc dup max dup max \"p\" set nl_max set_judgement ");//nl_max set_judgement ");
#endif
  qDebug("Command=%s",command.latin1());
  stack.registerFunction("neighbour", neighbour);
  if (!stack.run(command)){
    std::cerr << "ga_bu_generic: Error while running stack" << std::endl;
    return EXIT_FAILURE;
  }
#ifndef DEBUG_PRG
  stack.write(argv[2]);
  nl->writeOutImage(QString(argv[2])+".bu.node.plm");
  nl->writeGroupImage(QString(argv[2])+".bu.plm");
#else
  nl->info();// /home/bueckner/.gda/tmp/8445/Szene/Region_013/Ortslage_013/Industrie_013/0013-output
  //stack.write("/home/bueckner/.gda/tmp/8445/Szene/Region_013/Ortslage_013/Industrie_013/0013-output");
  //nl->writeOutImage("/home/bueckner/.gda/tmp/8445/Szene/Region_013/Ortslage_013/Industrie_013/0013-output");
  //nl->writeGroupImage("");
  stack.write("/project/geoaida/share/data/test_files/stack/output.regs");
  nl->writeOutImage("/project/geoaida/share/data/test_files/stack/output.regs.bu.node.plm");
  nl->writeGroupImage("/project/geoaida/share/data/test_files/stack/output.regs.bu.plm");
#endif
#ifdef WIN32
  return(0);
#endif
}

// /home/bueckner/project/geoAIDA/geoaidalib/geoaida/bu_tools/ga_bu_generic
// /project/geoaida/share/data/test_files/stack
// settlement_4_Haus_13_greenland_2_agriculture_6
// OUT "nodelist \"Haus\" selectClass  \"squareness\" calc \"orthogonallity\" calc dup max \"p\" set "
