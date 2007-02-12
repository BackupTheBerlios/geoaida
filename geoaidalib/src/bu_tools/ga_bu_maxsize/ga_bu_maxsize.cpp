/***************************************************************************
                          ga_bu_maxsize.cpp  -  description
                             -------------------
    begin                : Thu Dec  7 10:21:00 MET 2000
    copyright            : (C) 2000 by jürgen bückner
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
#include <iostream.h>
#ifdef WIN32
#include "..\bottomup\bottomup.h"
#include "..\bottomup\regionsensor.h"
#else
#include "bottomup.h"
#include "regionsensor.h"
#endif


void usage(char* prg)
{
  cout << "usage: "<< prg << " infile outfile maxdist [classes ...]" << endl
       << "\t infile   - 'XML' region descripion" << endl
       << "\t outfile  - 'XML' region descripion" << endl
       << "\t maxsize  - cluster all nodes with size < maxsize" << endl
       << "\t classes  - list of exist classes, ALL for all classes" << endl
       << "\t   description: read infile, calculate the size (in pixel) for all" << endl
       << "\t     'classes' and grouped all with a 'size <= maxsize'." << endl
       << "\t     The result is written to 'outfile'" << endl
       << "               " << endl;
  exit(-1);
}

int main(int argc, char *argv[])
{
  //cout  << "argc:" << argc <<endl;
  QString classes = "ALL";
  if (argc < 4) // || argc > 5 )
    usage(argv[0]);
  if (argc >= 5) classes = argv[4];

	unsigned int maxsize = atoi(argv[3]);
  BottomUp bu(argv[1]); // read infile
  //BottomUp bu("/project/geoaida/share/data/test_files/ga_bu_maxsize/input-27656-0171");
	NodeList *nl = bu.selectClass(classes); //select given classes
	RegionSensor *rs = new RegionSensor();
	for (int n=5; n<argc; n++) {
	  cout <<argv[n]<<",   ";
		*nl += (*(bu.selectClass(argv[n])));
	}
	nl->calcForSelect ("size", rs);
	
	Node *node1;
	QDictIterator<Node> it( *nl ); // iterator for nl
  QFile fp(argv[2]);
  //QFile fp("/project/geoaida/share/data/test_files/ga_bu_maxsize/outfile");
  if (!fp.open(IO_WriteOnly)) qDebug("write: file not accesable to %s\n",argv[2]);
  QTextStream str(&fp);
	str<<"<REGION\n"; 	
  while(it.current()) { //run over list
 		node1=nl->take(it.currentKey()); //it.current();
		unsigned int size = ((*node1)["size"])->toUInt();
		if (size<=maxsize) node1->write(str);
		++it;
		it.toFirst();
	}	
	str<<"/>\n";
	fp.close();

	cout << "FIN .. "<<endl;
#ifdef WIN32
  return(0);
#endif

}

