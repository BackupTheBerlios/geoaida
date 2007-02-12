/***************************************************************************
                          test_bu.cpp  -  description
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
#include "bottomup.h"
#include "nodelist.h"
#include "regionsensor.h"

#include "gaimage.h"
#include <qlist.h>

#include "stack.h"
#include "stackelem.h"
#include "stackelemnodelist.h"
#include "stackelemnumber.h"
#include "stackelemstring.h"

//#define DEBUGMSG

void usage(char* prg)
{
  cout << "usage: "<< prg << " infile outfile maxdist [classes]" << endl
       << "\t infile   - 'XML' region descripion" << endl
       << "\t outfile  - 'XML' region descripion" << endl
       << "\t labelpic - path to the label image" << endl
       << "\t maxdist  - cluster all nodes with distance < maxdist" << endl
       << "\t classes  - list of exist classes, ALL for all classes" << endl
       << "  description: read infile, calculate the distance for all" << endl
       << "               'classes' and grouped all with a distance < 'maxdist'." << endl
       << "               The distance is calculate in the 'labelpic' and" << endl
       << "               the result is written to 'outfile'" << endl
       << "               !TEST only the distance to the first obj is calculate!" << endl
       << "               " << endl;
  exit(-1);
}

bool testfunction(Stack& stack)
{
  StackElem *el1=0;
  StackElem *el2=0;
  try {
    RegionSensor *rs = new RegionSensor();
    if (stack.count()<2) throw Stack::CleanUp(false);
    el1=stack.pop();
    el2=stack.pop();
    if (el1->type()!=StackElem::STRING && el2->type()!=StackElem::NODELIST)
      throw Stack::CleanUp(false);
    QString s=((StackElemString*)el1)->data();
    NodeList& nl=((StackElemNodeList*)el2)->data();
    nl.calcForSelect("ALL", s, rs);
    stack.push(new StackElemNodeList(nl));
    throw Stack::CleanUp(true);
  }
  catch(Stack::CleanUp e) {
    if (el1) delete el1;
    if (el2) delete el2;
    return e.status_;
  }

}
int main(int argc, char *argv[])
{
#if 0
  cout  << "argc:" << argc <<endl;
  if (argc < 6)
    usage(argv[0]);


  BottomUp bu(argv[1]);
	RegionSensor *rs = new RegionSensor(argv[3]);
	NodeList *nl = bu.selectClass(argv[5]);
#endif	
  BottomUp bu("/project/geoaida/share/data/test_files/stack/settlement_4_Haus_13_greenland_2_agriculture_6");
	NodeList *nl = bu.selectClass("ALL");
#if 0	
	nl->calcForSelect (argv[5], "size", rs);
//#if 0	
	for (int n=6; n<argc; n++) {
		*nl += (*(bu.selectClass(argv[n])));
		nl->calcForSelect (argv[n], "size", rs);
	}
	nl->info(); //INFO
	int dist = atoi(argv[4]);

	Node *node1, *node2;
	QDictIterator<Node> it( *nl ); // iterator for nl
  QFile fp(argv[2]);
  if (!fp.open(IO_WriteOnly)) qDebug("write: file not accesable\n");
  QTextStream str(&fp);
  while(it.current()) { //run over list
  	str<<"<REGION\n"; 	
		NodeList rl;
		node1=nl->take(it.currentKey()); //it.current();
		node1->write(str);//str<<"\n";
		QDictIterator<Node> it1( *nl ); // iterator for nl
		while ( it1.current() ) {
			node2=it1.current();
 			if (node1 != node2) {
  			float xc1 = ((*node1)["x_center"])->toFloat();
  			float yc1 = ((*node1)["y_center"])->toFloat();
  			float xc2 = ((*node2)["x_center"])->toFloat();
  			float yc2 = ((*node2)["y_center"])->toFloat();
  			if (((xc2-xc1)*(xc2-xc1)+(yc2-yc1)*(yc2-yc1))<(float)(dist*dist)) {
					node2=nl->take(it1.currentKey());
  				node2->write(str);
   			}
  		}
		 ++it1;
		}
		++it;
		str<<"/>\n";
		it.toFirst();
	}	
	fp.close();

	//cout << "FIN .. "<<endl;
#endif	

	// Stack test  settlement_4_Haus_13_greenland_2_agriculture_

	Stack stack(nl);
	stack.registerFunction("testfunction",testfunction);
	//stack.run("nodelist \"Haus\" selectClass nodelist nodelist \"settlement\" selectClass \"size\" calc 2 * 2 list2stack stack2list 2 * drop \"settlement\" selectClass 2 * ");
	stack.run("nodelist \"Haus\" selectClass 44 46 max maxDist calc ");
	stack.write("test1.regs");
	stack.run("nodelist \"Haus\" selectClass \"size\" calc ");
	stack.write("test2.regs");
	return 1;
	cout << "count01: "  << stack.count() << endl;
	stack.push(new StackElemNodeList(*nl));
	stack.push(new StackElemString("Haus"));
	stack.selectClass();
	cout << "count02: "  << stack.count() << endl;
	stack.push(new StackElemNodeList(*nl));
	stack.push(new StackElemString("agriculture"));
	stack.selectClass();
	cout << "count03: "  << stack.count() << endl;
	stack.push(new StackElemNodeList(*nl));
	stack.push(new StackElemString("greenland"));
	stack.selectClass();
	cout << "count04.: "  << stack.count() << endl;
	stack.push(new StackElemString("size"));
	cout << "count04..: "  << stack.count() << endl;
	stack.calcAttribute();
	cout << "count04...: "  << stack.count() << endl;
	stack.push(new StackElemNumber(2));
	cout << "count05: "  << stack.count() << endl;
	stack.mul();
	cout << "count06: "  << stack.count() << endl;
	stack.push(new StackElemNumber(2));
	cout << "count07: "  << stack.count() << endl;
	stack.list2stack();
	cout << "count08: "  << stack.count() << endl;
	stack.stack2list();
	stack.drop();
	cout << "count09: "  << stack.count() << endl;
	stack.push(new StackElemNumber(2));
	cout << "count05x: "  << stack.count() << endl;
	stack.mul();
	cout << "count05y: "  << stack.count() << endl;
	stack.push(new StackElemNumber(2));
	cout << "count051: "  << stack.count() << endl;
	stack.mul();
	cout << "count052: "  << stack.count() << endl;

//#if 0
	//stack.push(new StackElemNodeList(*nl));
	stack.run("nodelist nodelist nodelist 3 list2stack ");
/*	stack.run("nodelist \"size\" calc 5 * \"greenland\" selectClass");*/
	stack.write("stack.result");
	//stack.run("nodelist \"size\" calc 5 * 4 + \"acruculture\" select");
	return 1;
//#endif	
	stack.push(new StackElemNodeList(*nl));
	stack.push(new StackElemString("agriculture"));
	stack.eq();
	//	stack.selectClass();
	stack.push(new StackElemString("size"));
	stack.calcAttribute();
	stack.push(new StackElemNumber(2));
	stack.mul();
	
	stack.push(new StackElemNodeList(*nl));
	stack.push(new StackElemString("agriculture"));
	stack.neq();
	//stack.selectClass();
	stack.push(new StackElemString("size"));
	stack.calcAttribute();
	stack.push(new StackElemNumber(4));
	stack.add();
	stack.push(new StackElemNumber(200));
	cout << "count01: "  << stack.count() << endl;
	stack.gt();	
	cout << "count02: "  << stack.count() << endl;
	stack.select();
	cout << "count03: "  << stack.count() << endl;
	return 1;
	
	stack.push(new StackElemNumber(8));
	stack.push(new StackElemNumber(7));
	stack.eq();
	cout << "count0: "  << stack.count() << endl;
	stack.push(new StackElemNumber(0));
	stack.eq();
	cout << "count0.: "  << stack.count() << endl;
	stack.push(new StackElemNumber(1));
	stack.eq();
	cout << "count0a: "  << stack.count() << endl;
	stack.push(new StackElemNumber(7));
	stack.push(new StackElemNumber(8));
	stack.gt();
	cout << "count0b: "  << stack.count() << endl;
	stack.push(new StackElemNumber(7));
	stack.push(new StackElemNumber(7));
	stack.gt();
	cout << "count0c: "  << stack.count() << endl;
	
	return 1;
	stack.push(new StackElemNodeList(*nl));
	stack.push(new StackElemString("agriculture"));
	stack.selectClass();
	
	stack.push(new StackElemString("size"));
	// exist fkt.: size x_center y_center llx lly urx ury circumference roundness squareness compactness thickness M11 M02 M20 orthogonality
	stack.calcAttribute();
	cout << "count1: "  << stack.count() << endl;
	stack.push(new StackElemNodeList(*nl));
	stack.push(new StackElemString("agriculture"));
	stack.selectClass();
	stack.push(new StackElemString("size"));
	stack.calcAttribute();
	cout << "count1a: "  << stack.count() << endl;
	stack.push(new StackElemNumber(4));
	cout << "count1b: "  << stack.count() << endl;
	stack.eq();
	cout << "count1c: "  << stack.count() << endl;
	stack.push(new StackElemString("agriculture"));
	stack.selectClass();
	stack.push(new StackElemNumber(5));
	cout << "count2a: "  << stack.count() << endl;
	stack.mul();
	return 1;
#if 0	
//	stack.add();	
//	cout << "count1b: "  << stack.count() << endl;
	stack.push(new StackElemNumber(10));
	cout << "count1d: "  << stack.count() << endl;
	stack.gt();
		
	stack.push(new StackElemNumber(8));
	stack.push(new StackElemNumber(7));
	cout << "count2: "  << stack.count() << endl;
	stack.mul();
#endif		
	
//#if 0
	stack.push(new StackElemNumber(5));
	cout << "count2a: "  << stack.count() << endl;
	stack.mul();
	stack.push(new StackElemNumber(4));
	cout << "count2b: "  << stack.count() << endl;
	stack.mul();
	
#if 0
	cout << "count3: "  << stack.count() << endl;
	stack.dup();
	cout << "count4: "  << stack.count() << endl;
	stack.mul();
#endif	
	stack.push(new StackElemNodeList(*nl));
	stack.push(new StackElemString("agriculture"));
	stack.selectClass();
	stack.push(new StackElemString("size"));
	stack.calcAttribute();
	cout << "count4a: "  << stack.count() << endl;
	stack.mul();
	cout << "count4b: "  << stack.count() << endl;
	
	
	StackElem* se = stack.pop();
	if (se) {
	  cout << "type: " << se->type() << endl;
	  if (se->type() == StackElem::NODELIST){
	    cout << "Start:" << endl;
  	  NodeList& nlo=((StackElemNodeList*)se)->data();
	    nlo.info();
	    cout << "- END" << endl;
    }
  }
//#endif
}
