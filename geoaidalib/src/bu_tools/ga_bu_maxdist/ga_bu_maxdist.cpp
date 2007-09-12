/***************************************************************************
                          ga_bu_maxdist.cpp  -  description
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
//TEST ./ga_bu_maxdist test_in_maxdist test_out_maxdist /project/geoaida/share/data/images/region004.pgm 100 ALL

#include <cstdio>
#include <cstdlib>
#include <iostream>
#ifdef WIN32
#include "..\bottomup\bottomup.h"
#include "..\bottomup\regionsensor.h"
#else
#include "bottomup.h"
#include "regionsensor.h"
#endif
#include <qlist.h>

void usage(char* prg)
{
  cout << "usage: "<< prg << " infile outfile maxdist [classes ...]" << endl
       << "\t infile   - 'XML' region descripion" << endl
       << "\t outfile  - 'XML' region descripion" << endl
       //<< "\t labelpic - path to the label image" << endl
       << "\t maxdist  - cluster all nodes with distance < maxdist" << endl
       << "\t classes  - list of exist classes, ALL for all classes" << endl
       << "\t description: read infile, calculate the distance (in pixel) for all" << endl
       << "\t  'classes' and grouped all with a distance < 'maxdist' (center to center)." << endl
       << "\t  The distance is calculate in the 'labelpic' and" << endl
       << "\t  the result is written to 'outfile'" << endl
       << "               " << endl;
  exit(-1);
}

class Help {
	public:
	Help(){x_=0.0;y_=0.0;name_="";}
	Help(float x, float y, QString st) {x_=x; y_=y; name_=st;}
	~Help(){};
  Help(Help& rval){x_=rval.x();y_=rval.y(); name_=rval.name();}
	
	QString name (void) {return name_;}
	void name (QString st) {name_=st;}
	float x (void) {return x_;}
	void x (float x) {x_=x;}
	float y (void) {return y_;}
	void y (float y) {y_=y;}
	
	float dist(Help &h) {return ((x_-h.x())*(x_-h.x())+(y_-h.y())*(y_-h.y()));};
	
	protected: // Protected attributes
	float x_;
	float y_;
	QString name_;
};

int main(int argc, char *argv[])
{
  QString classes = "ALL";
  /*
  if (argc < 4) // || argc > 5 )
    usage(argv[0]);
  if (argc >= 5) classes = argv[4];
*/
	int dist = 33;//xyx atoi(argv[3])*atoi(argv[3]);
	//xyx BottomUp bu(argv[1]); // read infile
  BottomUp bu("/project/geoaida/share/data/test_files/ga_bu_maxdist/input");
	NodeList *nl = bu.selectClass(classes); //select given classes
	RegionSensor *rs = new RegionSensor();
	for (int n=5; n<argc; n++) {
	  cout <<argv[n]<<",   ";
		*nl += (*(bu.selectClass(argv[n])));
	}
	
	// TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST
	/*
	nl->calcForSelect ("M11", rs);	
	nl->calcForSelect ("thickness", rs);	
	nl->calcForSelect ("compactness", rs);	
	nl->calcForSelect ("squareness", rs);	
	nl->calcForSelect ("roundness", rs);	
	nl->calcForSelect ("circumference", rs);
	*/
	// TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST
	nl->calcForSelect ("size", rs);
	
/*	  cout  << "argc:" << argc <<endl;
  if (argc < 6)
    usage(argv[0]);

  BottomUp bu(argv[1]);
	RegionSensor *rs = new RegionSensor(argv[3]);
	NodeList *nl = bu.selectClass(argv[5]);
//	nl->calcForSelect (argv[5], "size", rs);
	for (int n=6; n<argc; n++) {
		*nl += (*(bu.selectClass(argv[n])));
//		nl->calcForSelect (argv[n], "size", rs);
	}
	nl->calcForSelect ("size", rs);
*/
	//nl->info(); //INFO

  QList<Help> inlist;
	QDictIterator<Node> it( *nl );
   while(it.current()) { //run over list
		Help* h = new Help((**it)["x_center"]->toFloat(),
		                   (**it)["y_center"]->toFloat(),
		                   it.currentKey());
		inlist.append(h);
		++it;
  }
  QList<Help> tmplist;	
  QList<Help> reslist;
  Help *obj;
  //xyx QFile fp(argv[2]);
  QFile fp("/project/geoaida/share/data/test_files/ga_bu_maxdist/output_xx");
  if (!fp.open(IO_WriteOnly)) qDebug("write: file not accesable to %s\n",argv[2]);
  QTextStream str(&fp);
	Node *node;
	
  while (!inlist.isEmpty() ){
    obj = inlist.take(0);
    reslist.append(obj);
    for (int i=0; i<(int)inlist.count(); i++) {
    cout <<(obj->dist(*(inlist.at(i))) < (float)dist)<<" ("<<sqrt(dist)
      <<") DIST:"<<sqrt(obj->dist(*(inlist.at(i))))
      <<" "<<obj->name()<<" ("<<obj->x() <<","<< obj->y()<<") "
      <<" "<<(*(inlist.at(i))).name()<<" ("<<(*(inlist.at(i))).x() <<","<< (*(inlist.at(i))).y()<<") "<<endl;
      if (obj->dist(*(inlist.at(i))) < (float)dist)
        tmplist.append(inlist.take(i));
    }
    while (!tmplist.isEmpty() ){
      obj = tmplist.take(0);
      reslist.append(obj);
      for (int i=0; i<(int)inlist.count(); i++) {
        if (obj->dist(*(inlist.at(i))) < (float)dist)
          tmplist.append(inlist.take(i));
      }
    }
    str<<"<REGION\n";
    while (!reslist.isEmpty() ){
      obj = reslist.take(0);
      node=nl->find(obj->name());
      node->write(str);
    }
    str<<"/>\n";
  }

  fp.close();

	cout << "FIN .. "<<endl;
#ifdef WIN32
	return (0);
#endif
}
