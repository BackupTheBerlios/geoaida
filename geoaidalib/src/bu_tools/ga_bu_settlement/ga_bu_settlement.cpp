/***************************************************************************
                           ga_bu_settlement.cpp  -  description
                             -------------------
    begin                : Thu Dec  7 10:21:00 MET 2000
    copyright            : (C) 2000 by jürgen bückner
    email                : bueckner@tnt.uni-hannover.de
    info                 : BottomUp operator for settelment
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

void usage(char* prg)
{
  cout << "usage: "<< prg << " infile outfile outimage1 outgroupmask [minsize, classes]" << endl
       << "\t infile   - 'XML' region descripion" << endl
       << "\t outfile  - 'XML' region descripion" << endl
       << "\t outimage - new image with individual colors, descripion see 'outfile'" << endl
       << "\t outgroupmask - new image with group colors, descripion see 'outfile'" << endl
       << "\t minsize - minimum region size to ignore small splits" << endl
       << "\t max - maximum thickness in [m] of the region " << endl
       << "\t classes  - list of exist classes, 'ALL' for all classes" << endl
       << "\t   description: " << endl
       << "\t     read infile, and ... valued label images using the" << endl
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

  int sizeX(void) {return sizeX_;}
  int sizeY(void) {return sizeY_;}
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

  void area(unsigned int a) {area_ = a;}
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
  QString classes = "ALL";
  unsigned int minsize = 0;
  int maxth = 0;
  if(argc == 8) {
    classes = argv[7];
    argc = 7;
  }
  if(argc == 7) {
    maxth = atoi(argv[6]);
    argc = 6;
  }
  if(argc == 6) {
    minsize = atoi(argv[5]);
    argc = 5;
  }
  if (argc != 5 )
    usage(argv[0]);

//cout <<argv[0]<<" "<<argv[1]<<" "<<argv[2]<<" "<<argv[3]<<" "<<argv[4]<<" "<<argv[5]<<endl;

  float gN=0.0, gS=0.0, gW=0.0, gE=0.0, xRes=0.0, yRes=0.0, p=0.0;
  int newX=0, newY=0;
  BottomUp bu(argv[1]); // read infile 'XML' description file
	NodeList *nl = bu.selectClass("ALL"); //select given classes
	//NodeList *nl_h = bu.selectClass("Haus"); //select given classes
	//NodeList *nl_s = bu.selectClass("settlement"); //select given classes
	//NodeList *nl_g = bu.selectClass("greenland"); //select given classes
	//NodeList *nl_h = bu.selectClass(classes); //select given classes
	//nl_h->info(); //INFO
				
	RegionSensor *rs = new RegionSensor();
  nl->calcForSelect ("Haus","orthogonallity", rs);
	nl->calcForSelect ("Haus","thickness", rs);	
	nl->calcForSelect ("Haus","compactness", rs);	
	nl->calcForSelect ("Haus","squareness", rs);	
	nl->calcForSelect ("size", rs);
	QDictIterator<Node> it( *nl ); //iterator for nodelist
	
	Node* n;
	for (it.toFirst();it.current();++it) {
  	n = nl->find(it.currentKey());
		if ((n->classname()).compare("Haus") == 0){
			QString *val = new QString("0.0");
			float tmp=0.0;
			unsigned int size=((*n)["size"])->toUInt();
			float compactness=((*n)["compactness"])->toFloat();
			float squareness=((*n)["squareness"])->toFloat();
			int thickness=((*n)["thickness"])->toInt();
			float orthogonallity=((*n)["orthogonallity"])->toFloat();
			if(thickness<maxth && size>=minsize){
			  if(compactness>squareness)tmp=compactness;
			  else tmp=squareness;
			  if(orthogonallity>tmp) tmp=orthogonallity;
	      qDebug("$$$###id:%d - p=%f p+0.25=%f",n->id(),tmp,tmp+0.25);
	      tmp+=0.25; if(tmp>1.0)tmp=1.0;
		    val->setNum(tmp);
	    }
		  n->replace("p", val);
		}
	}
	
	for (it.toFirst();it.current();++it) { //Baumgruppen bewerten
  	n = nl->find(it.currentKey());
		if ((n->classname()).compare("forest") == 0){
			QString *val = new QString("0.0");
			unsigned int size=((*n)["size"])->toUInt();
			if(size>10000){
			  n->replace("p", val);
			}
		}
	}

	nl->calcNewGEOValues(gN,gS,gW,gE,newX,newY,xRes,yRes);
	
	MyList hl;
	it.toFirst();
	while(it.current()) {
		float p;
#ifdef WIN32
 	  if (!*(*it)["p"]) p=0.5;
 	  else p = (*(*it)["p"]).toFloat();	
qDebug("ga_bu_settlement p= %f",p);
#else
 	  if (!(**it)["p"]) p=0.5;
 	  else p=(**it)["p"]->toFloat();	
#endif
		Help* h = new Help(p,it.currentKey());
//qDebug("$$$## p %f - node %f - help %f",p,(*it)["p"]->toFloat(),h->p());
		hl.append(h);
		++it;
	}
	
	hl.sort();
	newX = int((gE-gW) / xRes);//calculate new image size
	newY = int((gN-gS) / yRes);//calculate new image size
	qDebug("$$$newX: %d, newY: %d, xRes: %f, yRes: %f, gN: %f, gS: %f, gE: %f, gW: %f",
	  newX, newY, xRes, yRes, gN, gS, gE, gW);
	
	QFile fp(argv[2]); // 'XML' - description
  if (!fp.open(IO_WriteOnly)) qDebug("write: file not accesable to %s\n",argv[2]);
  QTextStream str(&fp);
	
	QDict<int> dict( 17, FALSE ); //dictionary for the class nr.
	QArray<NodeInfo*> infolist(nl->size()+1); // array of pointers to NodeInfo
	Help *pl;
	int i = 1;
	int helplabel = 0;//, label = 0;
	Image out_img(typeid(signed int),newX,newY,1); //generate out image
#ifdef WIN32
	QArray<int> vec(nl->size()+1); 
	vec.fill(0, -1); //info for labeling
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
	  ni->p((node->getValue("p"))->toFloat());
    //qDebug("$$§§§§ node->p: %f",(node->getValue("p"))->toFloat());
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
  out_img.write(argv[3]);	

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

  p = 0.0;
  float urban = 0.0;
  float rest = 0.0;
  for (int j=1; j<i; j++) { //bewertung, hausflaechen und rest aufsummieren
    p += (infolist[j])->p();
    n = nl->find(infolist[j]->addr());
    if ((n->classname()).compare("Haus") == 0 )
      urban+=float(((*n)["size"])->toInt());
    else rest+=float(((*n)["size"])->toInt());
  }
  if(urban==0.0 || rest==0.0) p=0.0; //no house || all house => shit
  else {
    float faktor=10*urban/(urban+rest); if (faktor>1.0)faktor=1.0;
    p=p/(i-1);
    qDebug("$$$ p:%f, faktor:%f =>  p:%f",p,faktor,p*faktor);
    //p*=faktor;
    //qDebug("$$$urban:%f; rest:%f; 3*urban/(urban+rest):%f",urban,rest,urban/(urban+rest));
  }

cout <<", i:"<< i<<", helplabel:"<<helplabel <<", nl->size():"<<nl->size() <<endl;
   str << "<group id=\""<<1<<"\" file=\"" << argv[4]
      << "\" p=\"" << QString().sprintf("%f",p) << "\" "
      << "file_geoWest=\"" << QString().sprintf("%f",gW) << "\" "
      << "file_geoNorth=\"" << QString().sprintf("%f",gN) << "\" "
      << "file_geoEast=\"" << QString().sprintf("%f",gE) << "\" "
      << "file_geoSouth=\"" << QString().sprintf("%f",gS) << "\" "
      << ">\n";
 //str << "<group id=\""<<1<<"\" file=\"" << argv[4]<<"\" p=\"" << QString().sprintf("%f",p) << "\">\n";
  if (p>0.0)
  for (int k=0; k<helplabel; k++) {
    for (int j=1; j<i; j++) {
      Node *node;
      NodeInfo* ni = infolist[j];
      cout <<j<<" j - i "<<i<<", k:"<<k<<"; nl-size "<<nl->size()<<", classNr:"<<ni->classNr()
        <<", area:"<<ni->area()<<", minsize:"<<minsize<<" "<<ni->cl_name()<<" "<<ni->name()<<endl;
      if (ni) {
        node=nl->find(ni->addr());
        int size;
        MLParser::setInt(size,node,"size");
        cout <<j<<" j - i "<<i<<", k:"<<k<<" ?==? classNr:"<<ni->classNr()<<"..."
          << ", size:"<<size<<" > minsize:"<<minsize<<" "<<ni->cl_name()<<" "<<ni->name()<<endl;
        if ((k==ni->classNr()) && (size>minsize)){// node exist && correct group && label not overwritten
          node->replace("file", new QString(argv[3]));
#ifdef WIN32
/*        node->replace("llx", &(new QString)->setNum(ni->llx()));
          node->replace("lly", &(new QString)->setNum(ni->lly()));
          node->replace("urx", &(new QString)->setNum(ni->urx()));
          node->replace("ury", &(new QString)->setNum(ni->ury()));*/
          node->replace("file_geoNorth",&(new QString)->sprintf("%f",gN));
          node->replace("file_geoSouth",&(new QString)->sprintf("%f",gS));
          node->replace("file_geoEast", &(new QString)->sprintf("%f",gE));
          node->replace("file_geoWest", &(new QString)->sprintf("%f",gW));
          //node->replace("p", &(new QString)->setNum(ni->p()));
          node->replace("id", &(new QString)->setNum(ni->id()));
#else
/*        node->replace("llx", (new QString)->setNum(ni->llx()));
          node->replace("lly", (new QString)->setNum(ni->lly()));
          node->replace("urx", (new QString)->setNum(ni->urx()));
          node->replace("ury", (new QString)->setNum(ni->ury()));*/
          node->replace("file_geoNorth",(new QString)->sprintf("%f",gN));
          node->replace("file_geoSouth",(new QString)->sprintf("%f",gS));
          node->replace("file_geoEast", (new QString)->sprintf("%f",gE));
          node->replace("file_geoWest", (new QString)->sprintf("%f",gW));
          //node->replace("p", (new QString)->setNum(ni->p()));
          node->replace("id", (new QString)->setNum(ni->id()));
#endif
          node->write(str);
        }
      }
    }
   }
  str<<"</group>\n";
	fp.close();

  {
    for (Iterator pix = out_img.begin(); pix != out_img.end(); out_img.nextCol(pix)) {
      if (out_img.getInt(pix))
      out_img.set(pix,vec[1]);
    }
    out_img.write(argv[4]);
  }
	
	cout << "FIN ... "<<endl;
#ifdef WIN32
  return (0);
#endif
}
