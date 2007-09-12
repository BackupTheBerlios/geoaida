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
#include "gaimage.h"
#include <qlist.h>

using namespace Ga;

//#define _TEST_DB_

void usage(char* prg)
{
  cout << "usage: "<< prg << " infile outfile outimage1 outgroupmask [minsize, classes]" << endl
       << "\t infile   - 'XML' region descripion" << endl
       << "\t outfile  - 'XML' region descripion" << endl
       << "\t outimage - new image with individual colors, descripion see 'outfile'" << endl
       << "\t outgroupmask - new image with group colors, descripion see 'outfile'" << endl
       << "\t minsize - minimum region size to ignore small splits" << endl
       << "\t classes  - list of exist classes, 'ALL' for all classes" << endl
       << "\t   description: " << endl
       << "\t     read infile, and merge the label images using the" << endl
       << "\t     the weighing of classes. Worse classes are covert." << endl
       << "\t     The result is written to 'outfile' and 'outimage'" << endl
       << "               " << endl;
  exit(-1);
}
// TEST: siehe  /project/geoaida/share/data/test_files/ga_bu_merge_rating

class Help { // help class to handel the correct order; include compare fkt.and object name
	public:
	Help(){p_=0.0;cl_name_="";}
	Help(float f, QString st) {p_=f; cl_name_=st;}
	~Help(){};
  Help(Help& rval){p_=rval.p(); cl_name_=rval.cl_name();}
	
	QString cl_name (void) {return cl_name_;}
	void cl_name (QString st) {cl_name_=st;}
	float p (void) {return p_;}
	void p (float st) {p_=st;}

	bool operator == (Help& lval){return (p_==lval.p());}
	bool operator < (Help& lval){return (p_<lval.p());}
	
	protected: // Protected attributes
	float p_;
	QString cl_name_;
};


class MyList : public QList<Help> { //sort list of help objects
 public:
	MyList() : QList<Help> () {};
	~MyList() {};
	
	virtual int compareItems(Item il, Item ir) {
		Help *lval = (Help*)il;
		Help *rval = (Help*)ir;
		if (lval->p() == rval->p()) return 0;
		if (lval->p()<rval->p()) return -1;
		return 1;
	}
};

class NodeInfo { // class to handel node info like llx, lly, urx and ury
 public:
  NodeInfo() : llx_(-1), lly_(-1), urx_(-1), ury_(-1), id_(0), i(0),
    sizeX_(0), sizeY_(0), classNr_(0), area_(0), sx(0.0), sy(0.0), p_(0.0) {cl_name_=""; name_=""; addr_="";};
  NodeInfo(int x, int y) : id_(0), i(0), classNr_(0), area_(0), sx(0.0), sy(0.0), p_(0.0)
    {cl_name_=""; name_=""; addr_=""; sizeX_=x; sizeY_=y; llx_=x; urx_=0; lly_=0; ury_=y;};
  ~NodeInfo() {};

  int llx(void) {return llx_;}
  int lly(void) {return lly_;}
  int urx(void) {return urx_;}
  int ury(void) {return ury_;}
  int id(void)  {return id_;}
  float p(void) {return p_/float(i);}
  QString cl_name(void) {return cl_name_;}
  QString name(void) {return name_;}
  QString addr(void) {return addr_;}
  int classNr(void)  {return classNr_;} //classNr - help Nr to group the nodes
  void classNr(int v) {classNr_=v;}
  void addr(QString v) {addr_=v;}
  void llx(int v) {if(llx_==-1) llx_=v ; else if(v<llx_) llx_=v;}
  void lly(int v) {if(lly_==-1) lly_=v ; else if(v>lly_) lly_=v;}
  void urx(int v) {if(urx_==-1) urx_=v ; else if(v>urx_) urx_=v;}
  void ury(int v) {if(ury_==-1) ury_=v ; else if(v<ury_) ury_=v;}
  void id(int v)  {id_=v ;}
  void p(float v)  {p_+=v ; i++;} //calculate average value
  void cl_name(QString v)  {cl_name_ = v;}
  void name(QString v)  {name_ = v;}

  void add(int x, int y) {
    area_++;
    sx += x;
    sy += y;
    if(x > urx_) urx_=x;
    if(y < ury_) ury_=y;
    if(x < llx_) llx_=x;
    if(y > lly_) lly_=y;
  }

  unsigned int area(void) {return area_;}
  double centerX(void) {return sx/double(area_);}
  double centerY(void) {return sy/double(area_);}

  protected: // Protected attributes
	int llx_, lly_, urx_, ury_, id_, i, sizeX_, sizeY_, classNr_;
	unsigned int area_;
	double sx, sy;
	float p_;
	QString cl_name_, name_, addr_;
};

int main(int argc, char *argv[])
{
#ifndef _TEST_DB_
  QString classes = "ALL";
  unsigned int minsize = 0;
  if(argc == 6) {
    minsize = atoi(argv[5]);
    argc = 5;
  }
  if(argc == 7) {
    minsize = atoi(argv[5]);
    classes = argv[6];
    argc = 5;
  }
  if (argc != 5 )
    usage(argv[0]);

  char* infile = argv[1];
  char* outfile = argv[2];
  char* outimg = argv[3];
  char* outgimg = argv[4];
#endif

#ifdef _TEST_DB_
  char infile[255] = "/project/geoaida/tmp/20685/Szene/0256-input";
  char outfile[255]  = "/project/geoaida/tmp/20685/Szene/0257-output";
  char outimg[255]  = "/project/geoaida/tmp/20685/Szene/0257-output.bu.node.plm";
  char outgimg[255]  = "/project/geoaida/tmp/20685/Szene/0257-output.bu.plm";
  QString classes = "ALL";
  unsigned int minsize = 10;
#endif
//cout <<argv[0]<<" "<<argv[1]<<" "<<argv[2]<<" "<<argv[3]<<" "<<argv[4]<<" "<<argv[5]<<endl;

  float gN=0.0, gS=0.0, gW=0.0, gE=0.0, xRes=0.0, yRes=0.0;
  int newX=0, newY=0;
  BottomUp bu(infile); // read infile 'XML' description file
	NodeList *nl = bu.selectClass(classes); //select given classes
	if (nl->isEmpty()) return 0;
	//nl->info(); //INFO
	nl->calcNewGEOValues(gN,gS,gW,gE,newX,newY,xRes,yRes);
	MyList hl;
	QDictIterator<Node> it( *nl ); //iterator for nodelist
	it.toFirst();
	while(it.current()) {
		float p;
 	  if (!(**it)["p"]) p=1.0;
 	  else p=(**it)["p"]->toFloat();
		Help* h = new Help(p,it.currentKey());
		hl.append(h);
		++it;
	}
	
	hl.sort();
	newX = int((gE-gW) / xRes);//calculate new image size
	newY = int((gN-gS) / yRes);//calculate new image size
	qDebug("$$$newX: %d, newY: %d, xRes: %f, yRes: %f, gN: %f, gS: %f, gE: %f, gW: %f",
	  newX, newY, xRes, yRes, gN, gS, gE, gW);
	
	QFile fp(outfile); // 'XML' - description
  if (!fp.open(IO_WriteOnly)) qDebug("write: file not accesable to %s\n",outfile);
  QTextStream str(&fp);

  QDict<int> dict( 17, FALSE ); //dictionary for the class nr.
	QArray<NodeInfo*> infolist(nl->size()+1); // array of pointers to NodeInfo
	Help *pl;
	int i = 1;
	int helplabel = 0;//, label = 0;
	Image out_img(typeid(signed int),newX,newY,1); //generate out image
#ifdef WIN32
	QArray<int> vec(nl->size()+1); 
	vec.fill(0,-1); //info for labeling
#else
	int vec[nl->size()+1]; 
	for (int ix=0; ix<nl->size()+1; ix++) vec[ix]=0; //info for labeling
#endif
	out_img.setGeoCoordinates(gW,gN,gE,gS);
  for ( pl=hl.first(); pl != 0; pl=hl.next() ) {
		Node *node=nl->find(pl->cl_name());
		//label = node->id();
		NodeInfo* ni = new NodeInfo(newX, newY);
		ni->id(i);
		ni->cl_name(node->classname());
		ni->name(node->name());
	  ni->p(node->p());
		ni->addr(node->addr());
		
		if(dict.find(node->classname())) //class with nr.
		  ni->classNr(*(dict.find(node->classname())));
		else {
		  int *l = new int(helplabel);
		  dict.insert(node->classname(),l);
		  ni->classNr(helplabel);
		  helplabel++;
		}	
		infolist[i] = ni;
		vec[i]=helplabel;
  	//cout <<"$$$ label:"<<i<<") oldlabel:"<<node->id()<<", helplabel: "<<ni->classNr()<<" - "<<node->filename()<<endl;
		
		Image img(typeid(int));
		img.read(node->filename().latin1());
    img.setGeoCoordinates(node->geoWest(), node->geoNorth(), node->geoEast(), node->geoSouth());
	
		out_img.geoMerge(img, node->id(), i);//XX
		i++;
  }
  out_img.write(outimg);	

  int x, y, val; //calculate the new position of the label
  ConstIterator pix = out_img.constBegin();
  //for (ConstIterator pix = out_img.constBegin(); pix != out_img.end(); out_img.nextCol(pix)) {
  for (y=0;y<newY;y++)//calculate the new values of the label
    for (x=0;x<newX;x++) {
    val = out_img.getInt(pix);
    if(val>0) {
      (infolist[val])->add(x,y);
    }

    out_img.nextCol(pix);
  }

  float p = 0.0;

  float area = 0.0;
  for (int j=1; j<i; j++) {
    p += (infolist[j])->p() * (infolist[j])->area();
    area+=float((infolist[j])->area());//   (((*n)["size"])->toInt());
  }
  if (area>0.0) p=p/area;
/*
  for (int j=1; j<i; j++)
    p += (infolist[j])->p();
  p=p/(i-1);
*/
//cout <<", i:"<< i<<", helplabel:"<<helplabel <<", nl->size():"<<nl->size() <<endl;
  str << "<group id=\""<<1<<"\" file=\"" << outgimg
      << "\" p=\"" << QString().sprintf("%f",p) << "\" "
      << "file_geoWest=\"" << QString().sprintf("%f",gW) << "\" "
      << "file_geoNorth=\"" << QString().sprintf("%f",gN) << "\" "
      << "file_geoEast=\"" << QString().sprintf("%f",gE) << "\" "
      << "file_geoSouth=\"" << QString().sprintf("%f",gS) << "\" "
      << ">\n";
  for (int k=0; k<helplabel; k++) {
    for (int j=1; j<i; j++) {
      NodeInfo* ni = infolist[j];
      //cout <<j<<" j - i "<<i<<"; nl-size "<<nl->size()<<endl;
      //if ((ni) && (k==ni->classNr()) && (ni->llx()<=ni->urx()))
      if ((ni) && (k==ni->classNr()) && (ni->area()>minsize)){// node exist && correct group && label not overwritten
        Node *node=nl->find(ni->addr());
        node->replace("file", new QString(outimg));
#ifdef WIN32
        node->replace("llx", &(new QString)->setNum(ni->llx()));
        node->replace("lly", &(new QString)->setNum(ni->lly()));
        node->replace("urx", &(new QString)->setNum(ni->urx()));
        node->replace("ury", &(new QString)->setNum(ni->ury()));
        node->replace("file_geoNorth",&(new QString)->sprintf("%f",gN));
        node->replace("file_geoSouth",&(new QString)->sprintf("%f",gS));
        node->replace("file_geoEast", &(new QString)->sprintf("%f",gE));
        node->replace("file_geoWest", &(new QString)->sprintf("%f",gW));
        node->replace("p", &(new QString)->setNum(ni->p()));
        node->replace("id", &(new QString)->setNum(ni->id()));
#else
        node->replace("llx", (new QString)->setNum(ni->llx()));
        node->replace("lly", (new QString)->setNum(ni->lly()));
        node->replace("urx", (new QString)->setNum(ni->urx()));
        node->replace("ury", (new QString)->setNum(ni->ury()));
        node->replace("file_geoNorth",(new QString)->sprintf("%f",gN));
        node->replace("file_geoSouth",(new QString)->sprintf("%f",gS));
        node->replace("file_geoEast", (new QString)->sprintf("%f",gE));
        node->replace("file_geoWest", (new QString)->sprintf("%f",gW));
        node->replace("p", (new QString)->setNum(ni->p()));
        node->replace("id", (new QString)->setNum(ni->id()));
#endif
        node->write(str);
      }
    }
   }
  str<<"</group>\n";
	fp.close();

  {
    for (Iterator pix = out_img.begin(); pix != out_img.end(); out_img.nextCol(pix)) {
      if (out_img.getInt(pix))
      out_img.set(pix,vec[1]);
      //out_img.set(pix,vec[out_img.getInt(pix)]);
    }
    out_img.write(outgimg);
  }
	
	cout << "FIN ... "<<endl;
#ifdef WIN32
  vec.~QArray();
  return(0);
#endif
}
