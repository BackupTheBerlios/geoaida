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
       << "\t min - minimum thickness in [m] of the region " << endl
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

  void area(int a) {area_ = a;}
  int area(void) {return area_;}
  double centerX(void) {return sx/double(area_);}
  double centerY(void) {return sy/double(area_);}

  protected: // Protected attributes
	int llx_, lly_, urx_, ury_, id_, i, sizeX_, sizeY_, classNr_;
	int area_;
	double sx, sy;
	float p_;
	QString cl_name_, name_, addr_;
};

int main(int argc, char *argv[])
{
  QString classes = "ALL";
  unsigned int minsize = 0;
  int minth = 0;
  if(argc == 8) {
    classes = argv[7];
    argc = 7;
  }
  if(argc == 7) {
    minth = atoi(argv[6]);
    argc = 6;
  }
  if(argc == 6) {
    minsize = atoi(argv[5]);
    argc = 5;
  }
  if (argc != 5 )
    usage(argv[0]);

//cout <<argv[0]<<" "<<argv[1]<<" "<<argv[2]<<" "<<argv[3]<<" "<<argv[4]<<" "<<argv[5]<<endl;

  float gN=0.0, gS=0.0, gW=0.0, gE=0.0, xRes=0.0, yRes=0.0;
  int newX=0, newY=0;
  BottomUp bu(argv[1]); // read infile 'XML' description file
  //NodeList *nl = bu.selectClass(classes); //select given classes
	NodeList *nl_i = bu.selectClass(classes); //select given classes
	//NodeList *nl_i = bu.selectClass("Gebaeude"); //select given classes
	//NodeList *nl_s = bu.selectClass("settlement"); //select given classes
	//NodeList *nl_g = bu.selectClass("greenland"); //select given classes
	
	//nl_i->info(); //INFO
		
	RegionSensor *rs = new RegionSensor();
  //nl_i->calcForSelect ("M11", rs);
	nl_i->calcForSelect ("Gebaeude","orthogonallity", rs);	
	nl_i->calcForSelect ("Haus","orthogonallity", rs);	
	nl_i->calcForSelect ("Gebaeude","thickness", rs);	
	nl_i->calcForSelect ("Gebaeude","compactness", rs);	
	nl_i->calcForSelect ("Gebaeude","squareness", rs);	
	nl_i->calcForSelect ("Haus","squareness", rs);	
	nl_i->calcForSelect ("size", rs);
	QDictIterator<Node> it( *nl_i ); //iterator for nodelist
	
	Node* n;
	for (it.toFirst();it.current();++it) { //Gebaeude bewerten
  	n = nl_i->find(it.currentKey());
		if ((n->classname()).compare("Gebaeude") == 0){
			QString *val = new QString("0.0");
			float tmp=0.0;
			float orthogonallity=((*n)["orthogonallity"])->toFloat();
			float compactness=((*n)["compactness"])->toFloat();
			float squareness=((*n)["squareness"])->toFloat();
			int thickness=((*n)["thickness"])->toInt();
			unsigned int size=((*n)["size"])->toUInt();
			if(thickness>=minth && size>=minsize){
			  if(compactness>squareness) tmp=compactness;
			  else tmp=squareness;
			  if(orthogonallity>tmp) tmp=orthogonallity;
			  tmp+=0.25; if(tmp>1.0)tmp=1.0;
			  val->setNum(tmp);
			}
			//if (size>=minsize) val->setNum(0,87654);
		  //cout << "##### " <<": "<< n->name() << " - " << n->classname()<< " - "<<(n->getValue("p"))->toFloat()<<endl;
			n->replace("p", val);
		}
	}

	for (it.toFirst();it.current();++it) { //Haeuser
  	n = nl_i->find(it.currentKey());
		if ((n->classname()).compare("Haus") == 0){
			QString *val = new QString("0.0");
			float tmp=0.0;
// WIN32: warning C4553: '==' : Operator hat keine Auswirkungen; ist '=' beabsichtigt?
//			tmp==((*n)["orthogonallity"])->toFloat();
			tmp=((*n)["orthogonallity"])->toFloat();
			float squareness=((*n)["squareness"])->toFloat();
			if (squareness>tmp) tmp=squareness;
			tmp+=0.25; if(tmp>1.0)tmp=1.0;
		  val->setNum(tmp);
			n->replace("p", val);
		}
	}
		

	for (it.toFirst();it.current();++it) { //Parkplaetze bewerten
  	n = nl_i->find(it.currentKey());
		if ((n->classname()).compare("agriculture") == 0){
			QString *val = new QString("0.0");
			unsigned int size=((*n)["size"])->toUInt();
			if(size>4700){
			  n->replace("p", val);
			}
		}
	}

	nl_i->calcNewGEOValues(gN,gS,gW,gE,newX,newY,xRes,yRes);
	MyList hl;
	it.toFirst();
	while(it.current()) {
		float p;
//<<<<<<< ga_bu_industrie.cpp
// 	  if (!(*it)["p"]) p=0.5;
// 	  else p=(*it)["p"]->toFloat();
//		if(p>0.0000001) {
//		  Help* h = new Help(p,it.currentKey());
//		  hl.append(h);
//		}
//=======
 	  if (!(**it)["p"]) p=(float)0.987;
 	  else p=(**it)["p"]->toFloat();
		Help* h = new Help(p,it.currentKey());
		hl.append(h);
//>>>>>>> 1.6
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
	QArray<NodeInfo*> infolist(nl_i->size()+1); // array of pointers to NodeInfo
	Help *pl;
	int i = 1;
	int helplabel = 0;//, label = 0;
	Image out_img(typeid(signed int),newX,newY,1); //generate out image
#ifdef WIN32
	QArray <int> vec(nl_i->size()+1); 
#else
	int vec[nl_i->size()+1]; 
#endif
	for (int ix=0; ix<nl_i->size()+1; ix++) vec[ix]=0; //info for labeling
	out_img.setGeoCoordinates(gW,gN,gE,gS);
  for ( pl=hl.first(); pl != 0; pl=hl.next() ) {
		Node *node=nl_i->find(pl->cl_name());
		//label = node->id();
		NodeInfo* ni = new NodeInfo(newX, newY);
		ni->id(i);
		ni->cl_name(node->classname());
		ni->name(node->name());
		//cout << "##### " << i <<": "<< node->name() << " - " << node->classname()<<endl;//<<"; p:"<< (node->getValue("p"))->toFloat()<<endl;
	  ni->p((node->getValue("p"))->toFloat());
//	    qDebug("$$§§§§ node->p: %f",node->getValue("p"))->toFloat());
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
	
	  float np=((*n)["p"])->toFloat();
		if (np>0.00001) out_img.geoMerge(img, node->id(), i);//XX
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


  float p = 0.0;
  float industrie = 0.0;
  float rest = 0.0;
  int ii = 0;
  for (int j=1; j<i; j++) {
    qDebug("$$$%s %d industrie p:%f",((infolist[j])->name()).latin1(),(infolist[j])->classNr(),(infolist[j])->p());
    p += (infolist[j])->p();
    n = nl_i->find(infolist[j]->addr());
    if ((n->classname()).compare("Gebaeude") == 0 && (infolist[j])->p()>0.0){
      industrie+=float(((*n)["size"])->toInt());
      ii++;
    }
    else rest+=float(((*n)["size"])->toInt());
  }
  if(industrie==0.0) p=0.0; //no house|| all house => shit
  else {
    float faktor=10*industrie/(industrie+rest); if (faktor>1.0)faktor=1.0;
    p=p/(ii);
    //qDebug("$$$ p:%f, faktor:%f =>  p:%f",p,faktor,p*faktor);
    //p*=faktor;
    qDebug("$$$industrie:%f; rest:%f; 3*industrie/(industrie+rest):%f",industrie,rest,industrie/(industrie+rest));
  }
  if (p>1.0) p=1.0;

//cout <<", i:"<< i<<", helplabel:"<<helplabel <<", nl_i->size():"<<nl_i->size() <<endl;
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
        NodeInfo* ni = infolist[j];
        cout <<j<<" j - i "<<i<<"; nl_i-size "<<nl_i->size()<<endl;
        //if ((ni) && (k==ni->classNr()) && (ni->llx()<=ni->urx()))
         cout <<j<<" j - i "<<i<<", k:"<<k<<" ?==? classNr:"<<ni->classNr()<<"..."
          << ", size:"<<minsize<<" < ni->area:"<<ni->area()<<" "<<ni->cl_name()<<" "<<ni->name()<<endl;
        if ((ni) && (k==ni->classNr()) && (ni->p()>0.0000001))// node exist && correct group && label not overwritten
          if(ni->area()>minsize || (ni->cl_name()=="Haus" && ni->area()>25)){
          Node *node=nl_i->find(ni->addr());
          node->replace("file", new QString(argv[3]));
#ifdef WIN32
//error C2664: 'replace' : Konvertierung des Parameters 2 von 'class QString' in 'const class QString *' nicht moeglich
//Kein benutzerdefinierter Konvertierungsoperator verfuegbar, der diese Konvertierung durchfuehren kann, oder der Operator kann nicht aufgerufen werden
          node->replace("llx", &(new QString)->setNum(ni->llx()));
          node->replace("lly", &(new QString)->setNum(ni->lly()));
          node->replace("urx", &(new QString)->setNum(ni->urx()));
          node->replace("ury", &(new QString)->setNum(ni->ury()));
          node->replace("file_geoNorth",&(new QString)->sprintf("%f",gN));
          node->replace("file_geoSouth",&(new QString)->sprintf("%f",gS));
          node->replace("file_geoEast", &(new QString)->sprintf("%f",gE));
          node->replace("file_geoWest", &(new QString)->sprintf("%f",gW));
          //node->replace("p", &(new QString)->setNum(ni->p()));
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
          //node->replace("p", (new QString)->setNum(ni->p()));
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
    out_img.write(argv[4]);
  }
	cout << "FIN ... "<<endl;
#ifdef WIN32
	vec.~QArray();
	return (0);
#endif

}
