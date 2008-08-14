/***************************************************************************
                          regionsensor.h  -  description
                             -------------------
    begin                : Tue Jan 23 2001
    copyright            : (C) 2001 by jürgen bückner
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

//  #define DEBUGMSG

#ifndef REGIONSENSOR_H
#define REGIONSENSOR_H

#include <gasensor.h>
#include "pfm.h"
#include "point2d.h"
#include "gaimage.h"
#include <qlist.h>
#include <qvaluelist.h> 
#include <stdio.h>
#include "stackelemnumber.h"
//#include "stackelemstring.h"

using namespace Ga;


class RegionSensor : public GASensor  {
public:
	/** default constructor */
	RegionSensor();
#ifdef DONT_USE_GAIMAGE
	/** constructor with image file name*/
	RegionSensor(QString filename);
#endif	
	/** default destructor */
	~RegionSensor();
	
  /** return image pointer   */
  const int* data();
  /** set image pointer  */
  void image(Ga::Image& img);

#ifdef DONT_USE_GAIMAGE
  /** set image pointer and image size  */
  void image(int*, int, int);
  /** read image from file  */
  void readImage(QString filename) ;
#endif
	
	/** calculate the sensor value - first step to get the type*/
	void calc(QString str);

	/** calculate the sensor value - second step call the function*/
	template <class PixType>
	void calc_type(PixType label) {
		if(sensor_name.compare("size")==0) {
			this->size(label);
			return;
		}
		else if(sensor_name.compare("area")==0) {
			this->area(label);
			return;
		}
		else if(sensor_name.compare("x_center")==0) {
			this->x_center(label);
			return;
		}
		else if(sensor_name.compare("y_center")==0) {
			this->y_center(label);
			return;
		}
		else if(sensor_name.compare("xGeoCenter")==0) {
			this->xGeoCenter(label);
			return;
		}
		else if(sensor_name.compare("xGeoCenter")==0) {
			this->xGeoCenter(label);
			return;
		}
		else if(sensor_name.compare("llx")==0) {
			this->llx(label);
			return;
		}
		else if(sensor_name.compare("lly")==0) {
			this->lly(label);
			return;
		}
		else if(sensor_name.compare("urx")==0) {
			this->urx(label);
			return;
		}
		else if(sensor_name.compare("ury")==0) {
			this->ury(label);
			return;
		}
		else if(sensor_name.compare("circumference")==0) {
			this->circumference(label);
			return;
		}
		else if(sensor_name.compare("roundness")==0) {
			this->roundness(label);
			return;
		}
		else if(sensor_name.compare("squareness")==0) {
			this->squareness(label);
			return;
		}
		else if(sensor_name.compare("compactness")==0) {
			this->compactness(label);
			return;
		}
		else if(sensor_name.compare("thickness")==0) {
			this->thickness(label);
			return;
		}
		else if(sensor_name.compare("DELTA_PA")==0) {
			this->average_thickness(label);
			return;
		}    
		else if(sensor_name.compare("M11")==0) {
			this->M11(label);
			return;
		}
		else if(sensor_name.compare("M02")==0) {
			this->M02(label);
			return;
		}
		else if(sensor_name.compare("M20")==0) {
			this->M20(label);
			return;
		}
		else if(sensor_name.compare("orthogonallity")==0) {
      sensor_name="orthogonality";
			this->orthogonality(label); // ONLY for OLD PRGs
			return;
		}
		else if(sensor_name.compare("orthogonality")==0) {
			this->orthogonality(label);
			return;
		}
	};

//##################### BEGIN SENSOR FUNCTION  #####################

/** calculate some basic features */
template <class PixType>
void basic(PixType label) {

	unsigned int n(0), area(0); //area size of the region
	int stx(sizeX_), sty(0); //lower-left pixel of region (sp - start point)
	int llx(sizeX_-1), lly(0), urx(0), ury(sizeY_-1); //values of encase rectangle
	int bbLlx(0), bbLly(sizeY_-1), bbUrx(sizeX_-1), bbUry(0);
	unsigned int sx(0),  sy(0);//help variablen
	double dx(0.0), dy(0.0); //center of region
	const int* p = data();
	ASSERT(p);
	if (node()) {
  	llx=bbUrx=node()->urx();
  	lly=bbUry=node()->ury();
  	urx=bbLlx=node()->llx();
  	ury=bbLly=node()->lly();
  }
  ASSERT(bbLly<sizeY_);//qDebug(" %d > %d",bbLly,sizeY_);
  ASSERT(bbUrx<sizeX_);//qDebug(" %d > %d",bbUrx,sizeX_);
  qDebug("RegionSensor::basic Test: bbox llx:%3d < urx:%3d lly:%3d > ury:%3d  (label: %d)",
         bbLlx, bbUrx, bbLly, bbUry, label);

	for (int y=bbUry; y<=bbLly; y++) {
		for (int x=bbLlx; x<=bbUrx; x++) {
      if ((x<sizeX_) && (y<sizeY_)){
  			if(p[y*sizeX_+ x] == label) {
	  			n++;
		  		sx += x;
			  	sy += y;
				  if(x > urx) urx=x;
  				if(y < ury) ury=y;
	  			if(x < llx) llx=x;
		  		if(y > lly) lly=y;
			  	if(y > sty) {sty=y; stx=x;}
				  if(y == sty && x < stx) stx=x;
          }
			}
		//p++;
		}
		if(n) {
			dx = ((dx*(double)area)+(double)sx)/(double)(area+n);	
			dy = ((dy*(double)area)+(double)sy)/(double)(area+n);	
		}	
		area += n;
		sx = sy = n = 0;
	}
  qDebug("RegionSensor::basic: nbox llx:%3d < urx:%3d lly:%3d > ury:%3d size:%d",
         llx, urx, lly, ury, area);
  //if (!area) {
  qDebug("RegionSensor::basic: id=%d (%s) area=%d",label,node()->filename().latin1(),area);
  //}
	//qDebug("#....# area:%d  dx:%f  dy:%f  l:%d",area,dx,dy,label);
	//convert results to string and replace in NODE
	QString area_ps;
	area_ps.setNum(area);
	node_->replace("size", area_ps);
	float xd = node()->xResolution();
  float yd = node()->yResolution();
  if(xd*yd>0.001) node_->replace("area", area*xd*yd);

	node_->replace("x_center", dx);
	node_->replace("y_center", dy);
	node_->replace("llx", llx);
	node_->replace("lly",lly);
	node_->replace("urx", urx);
	node_->replace("ury", ury);	
	node_->replace("stx", stx);
	node_->replace("sty", sty);
	
	//calculate geo position of center
  float gn = node()->geoNorth();
  float gs = node()->geoSouth();
  float ge = node()->geoEast();
  float gw = node()->geoWest();
  float xgc =gw+((ge-gw)/sizeX_*dx);
	node_->replace("xGeoCenter", xgc);
  float ygc = gs+((gn-gs)/sizeY_*dy);
	node_->replace("yGeoCenter", ygc);
	qDebug("RegionSensor::basic: xres:%f, yres:%f, geox:%f, geoy:%f",
          xd,yd, xgc, ygc );
}

//##################### CIRCUMFERENCE #####################
/** help function for calc_circumference */
float nextPoint(const int* pic, int& x, int& y, int& dir, int label);

/**calculate the circumference of the label and the middel value of radius*/
template <class PixType>
void calc_circumference(PixType label) {
  float circumference = 0.0;
  float xc=x_center(label, "x_center"); //x-center of region
  float yc=y_center(label, "y_center"); //y-center of region
	int stx_=stx(label, "stx");
  int sty_=sty(label, "sty");
	int xp=stx_;
  int yp=sty_;
  const int* pic = data();
  int i=0, dir=0;
	ASSERT(pic);
  double radius=0.0;

  circumference = nextPoint(pic,xp,yp,dir,label);
  //PP QList<Point2D> pointList;
  //PP pointList.append(new Point2D(xp,yp));
  while ((xp!=stx_) || (yp!=sty_)) {
    if (yp>sty_) fprintf(stderr,"Warning: (%d, %d) not in border..(%d, %d)\n",xp,yp,stx_,sty_);
    circumference += nextPoint(pic,xp,yp,dir,label);
    //PP pointList.append(new Point2D(xp,yp));
    radius += sqrt(((double(xp)-xc)*(double(xp)-xc)) + ((double(yp)-yc)*(double(yp)-yc))); //radien summieren
    i++;
  }
  //Point2D *p;
  //for (p=pointList.first(); p!=0;p=pointList.next()) printf("(%d, %d)\n",p->x(),p->y());

 	radius /= i; //radius bestimmen
 	qDebug("(%s): label: %d, radius: %f",((*node_)["name"])->latin1(),label,radius);
	QString *circumference_s = new QString;
	circumference_s->setNum(circumference);
	node_->replace("circumference", circumference_s);	
	
	QString *radius_s = new QString;
	radius_s->setNum(radius);
	node_->replace("radius", radius_s);	
}

//.......
/** circumference of the label region  - put the value of the local stack*/
template <class PixType>
float circumference(PixType label, QString sn="") {
  int i=0;
  if (sn.isEmpty()) {sn=sensor_name;i=1;}
	if (!node_->find(sn))
	  this->calc_circumference(label);	
	  float s=((*node_)[sn])->toFloat();
    if (i) node_->stackPush(  new StackElemNumber( double(s) ));
	  return s;
}

//##################### orthogonality #####################
template <class PixType>
/**calculate the orthogonality of the label using hought
   using for judge buildings
*/
void calc_orthogonality(PixType label) {
  float orthogonality = 0.0;
	int stx_=stx(label, "stx");
  int sty_=sty(label, "sty");
	int xp=stx_;
  int yp=sty_;
  const int* pic = data();
  int i=0, dir=0;
  nextPoint(pic,xp,yp,dir,label);

  const double conv=3.1415926535/180.0;
  int xhc=sizeX_/2;
  int yhc=sizeY_/2;
  int rmax=(int)(sqrt(double(sizeY_*sizeY_)+double(sizeX_*sizeX_))/2.0);
  int rmax2=2*rmax+2;
  int ra,alpha;
  double rr;
  double sarr[180],carr[180];
  int *imOut = new int [rmax2*180];
  assert(imOut);
  for( alpha=0; alpha<rmax2*180; alpha++) imOut[alpha]=0;
  //Initialize a table of sine and cosine values
  for( alpha=0; alpha<180; alpha++ ) {
    sarr[alpha] = sin((double)alpha*conv);
    carr[alpha] = cos((double)alpha*conv);
  }
  while ((xp!=stx_) || (yp!=sty_)) {
    if (yp>sty_) fprintf(stderr,"Warning: (%d, %d) not in border.(%d, %d)\n",xp,yp,stx_,sty_);
    nextPoint(pic,xp,yp,dir,label);
    for( alpha=0; alpha<180; alpha++ ) {
//      rr=(yp-yhc)*sarr[alpha]-(xp-xhc)*carr[alpha]; Jürgens Formel
      rr=(yp-yhc)*sarr[alpha]+(xp-xhc)*carr[alpha];
      if(rr<0.0) ra=(int)rr;
      else ra=(int)rr+1;
//      printf("%d\n",(rmax+ra)*180 + alpha);
      assert((rmax+ra)*180 + alpha<rmax2*180);
      imOut[(rmax+ra)*180 + alpha]+=1;//(imOut[(rmax+ra)*180 + alpha])+1;
    }
    i++;
  }

 	//projektion
 	int xm=0,ym=0, anz1=0, anz2=0, max = 0;
 	unsigned int *count = new unsigned int [180];
  for(int i=0; i<180; i++) count[i]=0; //initialisieren
 	for(int x=0; x<180;x++)
 	  for(int y=0; y<rmax2;y++) {
 	    if(imOut[y*180+x]>0){
   	    count[x]++;//count nr of entries
 	      if (imOut[y*180+x]>max) { //search max
 	        max=imOut[y*180+x];
 	        xm=x; ym=y; //xm = angle
 	      }
 	    }
 	  }

 	int ang, xm2;
 	if(xm>90) xm2=xm-90; else xm2=xm+90; //2. Min bei +/- 90°
 	ang=xm2;
 	unsigned int minp = count[xm2];
  for(i=0; i<180; i++)
    if(count[i]<count[xm]*1.1) //magic number
      anz2+=count[i];
  for(i=-6; i<13; i++){ //im 6° Bereich Min. suchen
    int v = xm2-i;
    if (v>0)
      if(count[v]<minp) {
        minp=count[v];//qDebug("##%d - minp2:%d",v, minp);
        ang=v;
      }
 	}
 	anz1=count[xm]+minp;

  qDebug("##Label:%d, minp1:%d minp2:%d angel:%d anz1:%d anz2:%d - %f %f ((anz2)/180)/((anz1)/2):%f %f",
    label,count[xm],minp,xm2+ang,anz1,anz2,float(anz1)/2,float(anz2)/180,(float(anz1)/2)/(float(anz2)/180), 1-(float(anz2)/180)/(float(anz1)/2) );

  orthogonality=1-(float(anz2)/180)/(float(anz1)/2);
  if(orthogonality<0.0) orthogonality=0.0;
  QString *orthogonality_s = new QString;
  orthogonality_s->setNum(orthogonality);
  node_->replace("orthogonality", orthogonality_s);
  delete imOut;
  delete count;
}


/** orthogonality for the label region - put the value of the local stack */
template <class PixType>
float orthogonality(PixType label, QString sn="")
{
  int i=0;
  if (sn.isEmpty()) {sn=sensor_name;i=1;}
  if (!node_->find(sn))
    this->calc_orthogonality(label);	
  float s=((*node_)[sn])->toFloat();
  if (i) node_->stackPush(  new StackElemNumber( double(s) ));
  return s;
}

//#####################  #####################

//##################### ROUNDNESS #####################
/**calculate the roundness of the label*/
template <class PixType>
void calc_roundness(PixType label) {
  float circum=circumference(label, "circumference");
  int a=size(label, "size");
  double roundness = 0.0;

  float val=circum*circum/a;
  if(val<17.0) roundness=1-(val-12.5664)/(17.0-12.5664);

  qDebug("(%s): label: %d, circum: %f, size: %d, U^2/F: %f, roundness: %f",
          ((*node_)["name"])->latin1(),label, circum, a, 1/(float(a)/(circum*circum)), roundness);
	
	QString *roundness_s = new QString;
	roundness_s->setNum(roundness);
	node_->replace("roundness", roundness_s);	
	
	/*if(roundness>0.5) {
  	QString *radius_s = new QString;
	  radius_s->setNum(radius);
	  node_->replace("radius", radius_s);	
	}*/
}

/** roundness of the label region [0..1] - put the value of the local stack*/
template <class PixType>
float roundness(PixType label, QString sn="") {
  int i=0;
  if (sn.isEmpty()) {sn=sensor_name;i=1;}
	if (!node_->find(sn))
	  this->calc_roundness(label);	
	  float s=((*node_)[sn])->toFloat();
    if (i) node_->stackPush(  new StackElemNumber( double(s) ));
	  return s;
}
//##################### SQUARNESS #####################
/**calculate the squareness of the label*/
template <class PixType>
void calc_squareness(PixType label) {
  float circum=circumference(label, "circumference");
  int a=size(label, "size");
  double squareness = 0.0;

  float val=circum*circum/a;
  if(val<20.5 && val>14.0)
    squareness=1-sqrt((val-16.0)*(val-16.0)) / 4.5;

  qDebug("(%s): label: %d, circum: %f, size: %d, U^2/F: %f, squareness: %f",
          ((*node_)["name"])->latin1(),label, circum, a, 1/(float(a)/(circum*circum)), squareness);
	
	QString *squareness_s = new QString;
	squareness_s->setNum(squareness);
	node_->replace("squareness", squareness_s);	
	
/*	if(squareness > 0.5) {
  	QString *radius_s = new QString;
	  radius_s->setNum(r_);
	  node_->replace("branch_line", radius_s);	
	}*/
}

/** roundness of the label region [0..1] - put the value of the local stack*/
template <class PixType>
float squareness(PixType label, QString sn="") {
  int i=0;
  if (sn.isEmpty()) {sn=sensor_name;i=1;}
	if (!node_->find(sn))
	  this->calc_squareness(label);	
	  float s=((*node_)[sn])->toFloat();
    if (i) node_->stackPush(  new StackElemNumber( double(s) ));
	  return s;
}

//##################### COMPACTNESS #####################
/**calculate the compactness of the label*/
template <class PixType>
void calc_compactness(PixType label) {
  float circum=circumference(label, "circumference");
  int a=size(label, "size");
  double compactness = 0.0;

  compactness=3.14159265*4*a/(circum*circum);
  //	check for splitted regions of one label
  if (compactness > 1.0)
		compactness= 0.0;
  qDebug("(%s): label: %d, circum: %f, size: %d, compactness: %f",
          ((*node_)["name"])->latin1(), label, circum, a, compactness);
	
	QString *compactness_s = new QString;
	compactness_s->setNum(compactness);
	node_->replace("compactness", compactness_s);	
}

/** compactness of the label region [0..1] - put the value of the local stack*/
template <class PixType>
float compactness(PixType label, QString sn="") {
  int i=0;
  if (sn.isEmpty()) {sn=sensor_name;i=1;}
	if (!node_->find(sn))
	  this->calc_compactness(label);	
	  float s=((*node_)[sn])->toFloat();
    if (i) node_->stackPush(  new StackElemNumber( double(s) ));
	  return s;
}


//##################### THICKNESS #####################
#define checkBounds(x,y,xmin,ymin,xmax,ymax) \
  (((x)>=(xmin)) && ((y)>=(ymin)) && ((x)<=(xmax)) && ((y)<=(ymax)))
//calculate the thickness of the label = count shrinking steps
template <class PixType>
void calc_thickness(PixType label) {
  int thickness(0);
  int x1=llx(label, "llx");
  int y2=lly(label, "lly");
  int x2=urx(label, "urx");
  int y1=ury(label, "ury");
  //  cout<<"x:"<<x1<<"-"<<x2<<" y:"<<y1<<"-"<<y2<<endl;  

  const int fg  = 0;
  const int bg  = 1;
  const int tmp = 2;
  
  int dx = x2-x1+1;
  int dy = y2-y1+1;
//  cout<<"bounding box "<<dx<<" X "<<dy<<" Pixel"<<endl;
  char *part;
  if (dx>2 && dy>2) {
    part = new char[dx*dy];
    char* pp = part;
    const int* pd;
    for (int y=y1; y<=y2; y++) { //bereich ausschneiden
      pd=&(data())[y*sizeX_+x1];
      for (int x=x1; x<=x2; x++) {
        if (*pd == label) *pp = fg; //fg = object = 0
        else *pp = bg; //bg = rest = 1
        pd++;
        pp++;
      }
    }
    //Rand setzen
    pp = part; 
  	int x;
    for (x=0; x<dx; x++) {*pp=bg; pp++;}
    pp = &part[(dy-1)*dx];      
    for (x=0; x<dx; x++) {*pp=bg; pp++;}
    for (int y1=0; y1<dy; y1++)
      {part[y1*dx]=bg; part[(y1+1)*dx-1]=bg;}
    int doit = 1;
    while (doit) { //auf 0 schrumpfen
      char *p0 =&part[dx+1];
      char *p1 =&part[1];     //    | |1| |
      char *p2 =&part[dx];    //    |2|0|3|
      char *p3 =&part[dx+2];  //    | |4| |
      char *p4 =&part[2*dx+1];//    | | | |

      doit = 0;
      thickness++;
      for (int i=0; i<(dx-2)*(dy-2); i++) {
        if(*p0 == fg) //rand finden
          if (*p1==bg || *p2==bg || *p3==bg || *p4==bg) {
            *p0=tmp;
            doit=1;
          }
        p0++; p1++; p2++; p3++; p4++;
      }
      p0 =&part[dx+1]; //rand zum hintergrund machen
      for (int i1=0; i1<(dx-2)*(dy-2); i1++) {
        if(*p0 == tmp) *p0=bg;
          p0++;
      }
    }
  }
  
  qDebug("(%s): label: %d, thickness: %d",
          ((*node_)["name"])->latin1(),label, thickness);
	
	QString *thickness_s = new QString;
	thickness_s->setNum(thickness);
	node_->replace("thickness", thickness_s);
}

/** thickness of the label region [0..1] - put the value of the local stack*/
template <class PixType>
int thickness(PixType label, QString sn="") {
  int i=0;
  if (sn.isEmpty()) {sn=sensor_name;i=1;}
	if (!node_->find(sn)){
    int labelnummer= label;

    this->calc_thickness(label);
		}
	  int s=((*node_)[sn])->toInt();
    if (i) node_->stackPush(  new StackElemNumber( double(s) ));
	  return s;
}

//##################### DELTA_PA #####################
#define checkBounds(x,y,xmin,ymin,xmax,ymax) \
  (((x)>=(xmin)) && ((y)>=(ymin)) && ((x)<=(xmax)) && ((y)<=(ymax)))
/**calculate the average width of the label = skeleton -> calc average width*/
template <class PixType>
void calc_average_thickness(PixType label){

  int x1=llx(label, "llx");
  int y2=lly(label, "lly");
  int x2=urx(label, "urx");
  int y1=ury(label, "ury");

  const float fg= 1.0;
  const float bg= 0.0;
  int dx = x2-x1+1;
  int dy = y2-y1+1;
  float *Matrix1(0), *Matrix2(0), *Labelmatrix(0);

  Matrix1= new float[dx*dy];  
  Matrix2= new float[dx*dy];
  Labelmatrix= new float[dx*dy];
  Image original(typeid(int), dx, dy);
  bool overflow= false;

  if (dx>2 && dy>2) {
    const int *pd;
//    cout<<"Bereich ausschneiden"<<endl;
    for (int y=y1; y<=y2; y++) {
      pd=&(data())[y*sizeX_+x1];
      for (int x=x1; x<=x2; x++) {
        if (*pd == label){
          Matrix2[x-x1+(y-y1)*dx]= fg;
          Labelmatrix[x-x1+(y-y1)*dx]= fg;          
           }
        else{
          Matrix2[x-x1+(y-y1)*dx]= bg;
          Labelmatrix[x-x1+(y-y1)*dx]= bg;          
          }
        pd++;
      }
    }

//    cout<<"Original Labelbild:"<<endl;
    for (int y=0; y<dy; y++){
      for (int x=0; x<dx; x++){
//        cout<<Matrix2[x+y*dx];
        original.set (x, y, int(Matrix2[x+y*dx]), 0);
        }       
//      cout<<endl;
      }

    bool Ende=false;
    float Gewicht=1.0;
    while (!Ende){
      //  Matrix 2 kopieren
      for (int y=0; y<dy; y++)
        for (int x=0; x<dx; x++)
          Matrix1[x+y*dx]= Matrix2[x+y*dx];

      for (int y=1; y<dy-1; y++)
        for (int x=1; x<dx-1; x++)
          if (Matrix1[x+y*dx]==Gewicht)                                
            //  Nachbarn aufaddieren                                      //    3
            Matrix2[x+y*dx]=  Matrix1[x-1+y*dx]+    Matrix1[x+1+y*dx]+    //  1 x 2
                              Matrix1[x+(y-1)*dx]+  Matrix1[x+(y+1)*dx];  //    4
      Gewicht*=4.0;
//      cout<<"Gewicht ="<<Gewicht<<endl;
      float Maximum=0.0;
      for (int y=0; y<dy; y++)
        for (int x=0; x<dx; x++)
          if (Matrix2[x+y*dx] > Maximum)
            Maximum= Matrix2[x+y*dx];
//      cout<<"Maximum ="<<Maximum<<endl;            
      Ende= (Maximum < Gewicht);
      if (Gewicht > 7.0e+37){
        Ende= true;
        overflow=true;
        }
      }

    float Nachbar[8];      
    //Vergleichsmatrix berechnen

    //Matrix1 nullen
    for (int y=0; y<dy; y++)
      for (int x=0; x<dx; x++)
        Matrix1[x+y*dx]=0.0;

/*        
    cout<<"// aufsummierte Gewichte"<<endl;
    for (int y=0; y<dy; y++){
      for (int x=0; x<dx; x++)
          cout<<Matrix2[x+y*dx]<<" ";
      cout<<endl;
      }
*/      
            
    for (int y=1; y<dy-1; y++)
      for (int x=1; x<dx-1; x++){
        Nachbar[0]=Matrix2[x+(y-1)*dx]; Nachbar[1]=Matrix2[x+1+(y-1)*dx];  // 7 0 1
        Nachbar[2]=Matrix2[x+1+y*dx];   Nachbar[3]=Matrix2[x+1+(y+1)*dx];  // 6 X 2
        Nachbar[4]=Matrix2[x+(y+1)*dx]; Nachbar[5]=Matrix2[x-1+(y+1)*dx];  // 5 4 3 
        Nachbar[6]=Matrix2[x-1+y*dx];   Nachbar[7]=Matrix2[x-1+(y-1)*dx];
        float value= Matrix2[x+y*dx];
        bool Mittelachse= true;
        //  Punkt=Mittelachsenpunkt?
        for (int k=0; k<4; k++)
          if (!(((Nachbar[k]< value) &&  (Nachbar[k+4]<=value)) ||
               ((Nachbar[k]<=value) &&  (Nachbar[k+4]< value)))  )
            Mittelachse= false;
          
        float Gewicht=0.0;
        for (int k=0; k<4; k++){
          if ((Nachbar[k  ]<=value) &&
              (Nachbar[k+4]<=value))
            Gewicht++;
          }

         if (value>0.0)
          Matrix1[x+y*dx]= Gewicht;
        else
          Matrix1[x+y*dx]= 0.0;

//        Matrix1[x+y*dx]= Mittelachse ? Gewicht : 0.0;
        }

/*            
    cout<<"// Ausgabe Matrix1 nach Normierung"<<endl;
    for (int y=0; y<dy; y++){
      for (int x=0; x<dx; x++)
        if (Matrix1[x+y*dx] > 0.0)
          cout<<Matrix1[x+y*dx];
        else
          cout<<"X";
      cout<<endl;
      }
*/

    //Matrix1 kopieren
    for (int y=0; y<dy; y++)
      for (int x=0; x<dx; x++)
        Matrix2[x+y*dx]=Matrix1[x+y*dx];
    
    Gewicht= 1.0;
    for (int y=1; y<dy-1; y++)
      for (int x=1; x<dx-1; x++){
        if (Matrix1[x+y*dx] == Gewicht){
          int bnach=0;
          Nachbar[0]=Matrix2[x+(y-1)*dx]; Nachbar[1]=Matrix2[x+1+(y-1)*dx];  // 7 0 1
          Nachbar[2]=Matrix2[x+1+y*dx];   Nachbar[3]=Matrix2[x+1+(y+1)*dx];  // 6 X 2
          Nachbar[4]=Matrix2[x+(y+1)*dx]; Nachbar[5]=Matrix2[x-1+(y+1)*dx];  // 5 4 3
          Nachbar[6]=Matrix2[x-1+y*dx];   Nachbar[7]=Matrix2[x-1+(y-1)*dx];
          for (int k=0; k<8; k++)
            if (Nachbar[k] >= Matrix1[x+y*dx]) bnach++;                    
          //  mehr als 1 groesserer Nachchbar vorhanden
          if (bnach > 2)
            Matrix1[x+y*dx]=0.0;
          }          
        }

  /*
  char name[1024];
  sprintf(name, "%s/%i_skelett.pgm",node()->filename().ascii(), label);
  cout<<name<<endl;

    cout<<"// Verbleibendes Skelett"<<endl;
    for (int y=0; y<dy; y++){
      for (int x=0; x<dx; x++)
        if (Matrix1[x+y*dx] > 3.0){
          cout<<"X";
          original.set (x, y, 255, 0);
          }
        else
          cout<<" ";
        }

  FILE *fp;
  fp = fopen(name, "w");
  original.write(fp);
  fclose(fp);*/

  }
    
    //  Berechnung der mittleren Breite

  std::cout<<"Berechnung der mittleren Breite"<<endl;
    int box=5;
    float mx=0.0, count=0.0;
    for (int y=0+box; y<dy-box; y++)
      for (int x=0+box; x<dx-box; x++)
        //  Skelettpunkt?
        if (Matrix2[x+y*dx]>2.0){
//            cout<<"Skelettpunkt: ("<<x<<","<<y<<")"<<endl;          
            int spkte=0, s1x=0, s1y=0, s2x=0, s2y=0;
            //  Schnittpunkte mit Boundingbox bestimmen
            //  um die Richtung der Skelettlinie festzustellen
            //  x-Richtung variieren
            for (int r=x-box; r<x+box; r++){
              //  oben
              if (Matrix2[r+(y-box)*dx]>2.0){              
                if (spkte==0) {s1x=r; s1y=y-box;}
                else {s2x=r; s2y=y-box;}
                spkte++;
                }
              //  unten                
              if (Matrix2[r+(y+box)*dx]>2.0){
                if (spkte==0){s1x=r; s1y=y+box;}
                else {s2x=r; s2y=y+box;}
                spkte++;
                }
              if (spkte==2) break;
              }
            //  y-Richtung variieren              
            for (int s=y-box; s<y+box; s++){
              //  links
              if (Matrix2[x-box+s*dx]>2.0){
                if (spkte==0){s1x=x-box; s1y=s;}
                else {s2x=x-box; s2y=s;}
                spkte++;
                }
              //  rechts
              if (Matrix2[x+box+s*dx]>2.0){
                if (spkte==0){s1x=x+box; s1y=s;}
                else {s2x=x+box; s2y=s;}
                spkte++;
                }
              if (spkte==2) break;                
              }

            //  nur wenn 2 Schnittpunkte vorhanden, kann Richtung bestimmt werden
            if (spkte == 2){
              //  2 gleiche Schnittpunkte gefunden
              if ( (s1x==s2x) && (s1y==s2y) ){                
                if ( (s1x < x) && (s1y <y) ){
                  s2x+= 2 * box;                // 1   3
                  s2y+= 2 * box;                //   x
                  }                             // 2   4
                else if ( (s1x < x) && (s1y > y) ){
                  s2x+= 2 * box;                
                  s2y-= 2 * box;
                  }
                else if ( (s1x > x) && (s1y < y) ){
                  s2x-= 2 * box;
                  s2y+= 2 * box;
                  }
                else if ( (s1x > x) && (s1y > y) ){
                  s2x-= 2 * box;
                  s2y-= 2 * box;
                  }
                }
              
              //  Richtung des Skelettsegmentes bestimmen
//              cout<<spkte<<" Schnittpunkte gefunden S1=("<<s1x<<","<<s1y<<") S2=("<<s2x<<","<<s2y<<")"<<endl;
              float alpha=0.0;
              if ((s1x > s2x) && (s1y > s2y))                   //        1 s2   5 s2   3 s2
                alpha= M_PI-atan ( (s1y-s2y) / (s1x-s2x) );     //  180°  6 s2    s1    8 s2  0°
              else if ((s1x > s2x) && (s1y < s2y))              //        2 s2   7 s2   4 s2
                alpha= M_PI+atan ( (s2y-s1y) / (s1x-s2x) );
              else if ((s1x < s2x) && (s1y > s2y))    
                alpha= atan ( (s1y-s2y) / (s2x-s1x) );
              else if ((s1x < s2x) && (s1y < s2y))    
                alpha= (2*M_PI)-atan ( (s2y-s1y) / (s2x-s1x) );
              else if ((s1x == s2x) && (s1y > s2y))
                alpha= M_PI/2.0;
              else if ((s1x > s2x) && (s1y == s2y))
                alpha= M_PI;
              else if ((s1x == s2x) && (s1y < s2y))
                alpha= 1.5*M_PI;
              else if ((s1x < s2x) && (s1y == s2y))
                alpha= 0.0;
                                              
//              cout<<"Richtung des Skelettsegmentes="<<alpha*180.0/M_PI<<endl;
              //  Lot faellen
              alpha+= 0.5*M_PI;
              if (alpha > 2.0*M_PI)
                alpha-= 2.0*M_PI;
              
              //  Breite des Labels fuer aktuellen Skelettpunkt (x,y) bestimmen
              //  2 Strahlen verfolgen bis Labelmatrix in bg laeuft
              //  st:Startpunkt fuer Strahlen
              int stx= x, sty= y;
              float alpha2;
              if (alpha <= M_PI)
                alpha2= alpha+M_PI;
              else
                alpha2= 2.0*M_PI-alpha;
//              cout<<"Strahl in Richtung="<<alpha*180.0/M_PI<<" und "<<alpha2*180.0/M_PI<<" verfolgen"<<endl;                                    
              //  Strahlen verfolgen
              float breite= 0.0;
              int breiten= 0;
              for (int stra=0; stra<2; stra++){
                float alpha1;
                if (!stra)
                  alpha1=alpha;
                else
                  alpha1=alpha2;
                int xnew= stx, ynew= sty;
//                cout<<"Strahl "<<stra+1<<" verfolgen"<<endl;
                bool stop=false;
                if ( (alpha1 < 0.25*M_PI) || (alpha1 > 1.75*M_PI) ){        //    5
//                cout<<"Fall 1"<<endl;                                     //    3
                  while (!stop){                                            // 2     1
                    xnew++;                                                 //    4                    
                    ynew= sty - int ( (xnew-stx)*tan(alpha1) );             //    6
                    //  aus Labelmatrix herausgelaufen?
                    if ( (xnew >= dx) || (xnew < 0) || (ynew >= dy) || (ynew < 0) ){
                      breite+= sqrt( pow ((xnew-stx),2) + pow((ynew-sty),2) );
                      breiten++;
//                      cout<<"Randpunkt "<<stra+1<<"=("<<xnew<<","<<ynew<<")"<<endl;                        
                      stop= true;
                      }
                    //  Strahl noch im Label?
                    if (!stop){
                      if (!Labelmatrix[xnew+ynew*dx]){
                        breite+= sqrt( pow ((xnew-stx),2) + pow((ynew-sty),2) );
                        breiten++;
//                        cout<<"Randpunkt "<<stra+1<<"=("<<xnew<<","<<ynew<<")"<<endl;
                        stop= true;
                        }
                      }
                    }
                  }
                else if ( (alpha1 > 0.75*M_PI) && (alpha1 < 1.25*M_PI) ){
//                  cout<<"Fall 2"<<endl;                                                  
                  while (!stop){
                    xnew--;
                    ynew= sty - int ( (xnew-stx)*tan(alpha1) );
                    //  aus Labelmatrix herausgelaufen?
                    if ( (xnew >= dx) || (xnew < 0) || (ynew >= dy) || (ynew < 0) ){
                      breite+= sqrt( pow ((xnew-stx),2) + pow((ynew-sty),2) );
                      breiten++;
//                      cout<<"Randpunkt "<<stra+1<<"=("<<xnew<<","<<ynew<<")"<<endl;                      
                      stop= true;
                      }
                    //  Strahl noch im Label?
                    if (!stop){
                      if (!Labelmatrix[xnew+ynew*dx]){
                        breite+= sqrt( pow ((xnew-stx),2) + pow((ynew-sty),2) );
                        breiten++;                          
//                        cout<<"Randpunkt "<<stra+1<<"=("<<xnew<<","<<ynew<<")"<<endl;
                        stop= true;
                        }
                      }
                    }
                  }
                else if ( ((alpha1 > 0.25*M_PI) && (alpha1 < 0.49*M_PI)) ||
                          ((alpha1 > 0.51*M_PI) && (alpha1 < 0.75*M_PI)) ){
//                  cout<<"Fall 3"<<endl;                                                  
                  while (!stop){
                    ynew--;
                    xnew= stx - int ( (ynew-sty)/tan(alpha1) );
                    //  aus Labelmatrix herausgelaufen?
                    if ( (xnew >= dx) || (xnew < 0) || (ynew >= dy) || (ynew < 0) ){
                      breite+= sqrt( pow ((xnew-stx),2) + pow((ynew-sty),2) );
                      breiten++;
//                      cout<<"Randpunkt "<<stra+1<<"=("<<xnew<<","<<ynew<<")"<<endl;                        
                      stop= true;
                      }
                    //  Strahl noch im Label?
                    if (!stop){
                      if (!Labelmatrix[xnew+ynew*dx]){
                        breite+= sqrt( pow ((xnew-stx),2) + pow((ynew-sty),2) );
                        breiten++;                          
//                        cout<<"Randpunkt "<<stra+1<<"=("<<xnew<<","<<ynew<<")"<<endl;
                        stop= true;
                        }
                      }
                    }
                  }
                else if ( ((alpha1 > 1.25*M_PI) && (alpha1 < 1.49*M_PI)) ||
                        ((alpha1 > 1.51*M_PI) && (alpha1 < 1.75*M_PI)) ){
//                  cout<<"Fall 4"<<endl;                                                  
                  while (!stop){
                    ynew++;
                    xnew= stx - int ( (ynew-sty)/tan(alpha1) );
                    //  aus Labelmatrix herausgelaufen?
                    if ( (xnew >= dx) || (xnew < 0) || (ynew >= dy) || (ynew < 0) ){
                      breite+= sqrt( pow ((xnew-stx),2) + pow((ynew-sty),2) );
                      breiten++;
//                      cout<<"Randpunkt "<<stra+1<<"=("<<xnew<<","<<ynew<<")"<<endl;                        
                      stop= true;
                      }
                    //  Strahl noch im Label?
                    if (!stop){
                      if (!Labelmatrix[xnew+ynew*dx]){
                        breite+= sqrt( pow ((xnew-stx),2) + pow((ynew-sty),2) );
                        breiten++;                          
//                        cout<<"Randpunkt "<<stra+1<<"=("<<xnew<<","<<ynew<<")"<<endl;
                        stop= true;
                        }
                      }
                    }
                  }
                else if ((alpha1 > 0.49*M_PI) && (alpha1 < 0.51*M_PI)){
//                  cout<<"Fall 5"<<endl;
                  while (!stop){
                    ynew--;
                    xnew= stx;
                    //  aus Labelmatrix herausgelaufen?
                    if ( (xnew >= dx) || (xnew < 0) || (ynew >= dy) || (ynew < 0) ){
                      breite+= sty-ynew-1;
                      breiten++;
//                      cout<<"Randpunkt "<<stra+1<<"=("<<xnew<<","<<ynew<<")"<<endl;
                      stop= true;
                      }
                    //  Strahl noch im Label?
                    if (!stop){
                      if (!Labelmatrix[xnew+ynew*dx]){
                        breite+= sty-ynew;
                        breiten++;
//                        cout<<"Randpunkt "<<stra+1<<"=("<<xnew<<","<<ynew<<")"<<endl;
                        stop= true;
                        }
                      }
                    }
                  }
                else if ((alpha1 > 1.49*M_PI) && (alpha1 < 1.51*M_PI)){
//                  cout<<"Fall 6"<<endl;
                  while (!stop){
                    ynew++;
                    xnew= stx;
                    //  aus Labelmatrix herausgelaufen?
                    if ( (xnew >= dx) || (xnew < 0) || (ynew >= dy) || (ynew < 0) ){
                      breite+= ynew-sty-1;
                      breiten++;
//                      cout<<"Randpunkt "<<stra+1<<"=("<<xnew<<","<<ynew<<")"<<endl;
                      stop= true;
                      }
                    //  Strahl noch im Label?
                    if (!stop){
                      if (!Labelmatrix[xnew+ynew*dx]){
                        breite+= ynew-sty;
                        breiten++;
//                        cout<<"Randpunkt "<<stra+1<<"=("<<xnew<<","<<ynew<<")"<<endl;
                        stop= true;
                        }
                      }
                    }
                  }
                }                  
                if (breiten == 2){
                  count+=1.0;
                  mx+= breite;
                  }
//                cout<<"Breite="<<breite<<endl;
              }
            }        
                                                
//    qDebug("(%s): label: %d", ((*node_)["name"])->latin1(),label);
	  QString *av_thickness_s = new QString;
    
    float av_thickness, av_thickness_pixel;
    if (count != 0.0)
      av_thickness= mx/count;
    else
      av_thickness= 0.0;
    if (overflow)
      av_thickness= 9999.0;
    av_thickness_pixel= av_thickness;      
    //  Vorgabe BRIAN
    av_thickness*= 0.25;

//    cout<<"Mittlere BRIAN-Breite="<<av_thickness<<" Pixel"<<endl;
    
  	float xd = node()->xResolution();
    float yd = node()->yResolution();

    //  Umrechnung Pixel->m
    av_thickness*=xd;
        
    std::cout<<"Mittlere BRIAN-Breite="<<av_thickness<<" Meter"<<endl;


    //  Erzeuge gen-file der Skelettlinie   
    float gn = node()->geoNorth();
    float gs = node()->geoSouth();
    float ge = node()->geoEast();
    float gw = node()->geoWest();
    
    std::cout<<"Globale Lage der BB: north="<<gn<<" south="<<gs<<" west="<<gw<<" east="<<ge<<" file="<<node()->filename()<<endl;
    std::cout<<"Lage der BB: llx="<<x1<<" urx="<<x2<<" ury="<<y1<<" lly="<<y2<<endl;
    std::cout<<"BB: x-dim="<<dx<<" ydim="<<dy<<endl;

    //  gen-file oeffnen
    FILE *gen_fp;
    QValueList<QPoint> skeleton_list;    
    char genfilename[1024];
    sprintf(genfilename, "%s.gen",node()->filename().ascii());
    //  gen-file anfuegen
    gen_fp = fopen(genfilename, "a");
   
    //  label id schreiben
    fprintf (gen_fp,"%i\n", label);        

    // feed skeleton list first        
    for (int y=0+box; y<dy-box; y++)
      for (int x=0+box; x<dx-box; x++)
        //  fuer alle sicheren Skelettpunkte
        if (Matrix2[x+y*dx]>3.0){
          skeleton_list.append(QPoint(x,y));
          std::cout<<"Skelettpunktliste: ("<<x<<","<<y<<")"<<endl;
          }

    //  write skeleton list to gen file with correct topology
    QValueList<QPoint>::iterator it;
    //  look for start point of vectrisation
    float diag= sqrt (dx*dx+dy*dy);
    float min_dist[4]={diag, diag, diag, diag};                                             //    0 1
    QPoint corner[4]={QPoint(0,0), QPoint(dx-1, 0), QPoint(0,dy-1), QPoint(dx-1,dy-1)},     //    2 3
           startp[4];
    for (int i=0; i<4; i++)           
      for (it= skeleton_list.begin(); it!= skeleton_list.end(); ++it){      
        if ( dist (*it, corner[i]) < min_dist[i]){
          min_dist[i]= dist(*it, corner[i]);
          startp[i]= QPoint((*it).x(), (*it).y());
          }
        }
    //  select closest startpoint to a corner
    float min_dist_all= diag;
    int startp_nr;
    for (int i=0; i<4; i++){
        std::cout<<"Startpunkt "<<i<<"=("<<startp[i].x()<<","<<startp[i].y()<<") Dist="<<min_dist[i]<<endl;
      if (min_dist[i]<min_dist_all){
        min_dist_all= min_dist[i];
        startp_nr= i;
        }
      }

    //   write gen-file
    QPoint center, new_center= startp[startp_nr];
    float m_sum= 0.0;
    int m_count= 0;
    while (!skeleton_list.isEmpty()){
      float min_dist= diag;
      center= new_center;
      for (it= skeleton_list.begin(); it!= skeleton_list.end(); ++it){
        if (dist(center, *it)==0.0)
          it= skeleton_list.remove(it);
        else if (dist(center, *it)<min_dist){
          min_dist= dist(center, *it);
          new_center= *it;
          }
        }
      m_count++;
      m_sum+= min_dist;
      float m_step= m_sum/m_count;
      bool avoid_back= m_count > 20;
      
      //  avoid backsteps and write to gen-file        
//    if ((!avoid_back) || (min_dist < 1.5*m_step)){
      if ((!avoid_back) || (min_dist < 10.0*av_thickness_pixel)){
          std::cout<<"Skelettpunkt Label: ("<<center.x()<<","<<center.y()<<")"<<endl;
          std::cout<<"Skelettpunkt global: ("<<(node()->llx()+center.x())*xd+gw<<","<<gn-(node()->ury()+center.y())*xd<<")"<<endl;          
        fprintf (gen_fp,"%f %f\n",(node()->llx()+center.x())*xd+gw, gn-(node()->ury()+center.y())*xd);        
        }
      //  delete outlier new_center
      else{
          std::cout<<"delete "<<new_center.x()<<","<<new_center.y()<<")"<<endl;
      for (it= skeleton_list.begin(); it!= skeleton_list.end(); ++it)
        if (dist(new_center, *it)==0.0)
          it= skeleton_list.remove(it);
      new_center= center;
      }
    }    
    
    fprintf (gen_fp,"END\n");
    fclose(gen_fp);
                                                                                                                                
  	av_thickness_s->setNum(av_thickness);
  	node_->replace("DELTA_PA", av_thickness_s);
}

float dist(QPoint p1, QPoint p2){
  return sqrt( (p1.x()-p2.x())*(p1.x()-p2.x()) + (p1.y()-p2.y())*(p1.y()-p2.y()) );
}  

/** DELTA_PA of the label region - put the value of the local stack*/
template <class PixType>
int average_thickness(PixType label, QString sn="") {
  int i=0;
  if (sn.isEmpty()) {sn=sensor_name;i=1;}
	if (!node_->find(sn)){
    int labelnummer= label;
 		std::cout<<"in average_thickness"<<endl;
    std::cout<<"labelnummer= "<<labelnummer<<endl;
    
	  calc_average_thickness(label);
		}
	  float s=((*node_)[sn])->toFloat();
    if (i) node_->stackPush(  new StackElemNumber( double(s) ));
	  return static_cast<int>(s);
}

//##################### MOMENTUM #####################
/**calculate the moments M11, M02 and M20 of the label*/
template <class PixType>
void calc_momente(PixType label) {
  float xc = x_center(label, "x_center");
  float yc = y_center(label, "y_center");
	const int* p = data();
	double  m11(0.0),//Trägheitsprodukt der Achsen durch den Schwerpunkt parallel zu den Koordinatenachsen.
	        m02(0.0),//Moment 2. Ordnung (zeilenabhängig).
	        m20(0.0);//Moment 2. Ordnung (spaltenabhängig).
	ASSERT(p);
	for (int y=0; y<sizeY_; y++) {
		for (int x=0; x<sizeX_; x++) {
			if(*p == label) {
			  m11+=(xc-x)*(yc-y);
			  m02+=(yc-y)*(yc-y);
			  m20+=(xc-x)*(xc-x);
			}
		p++;
		}
  }
  double h = (m20 + m02) / 2.0;
  double ha1 = h + sqrt ( h*h - m20 * m02 + m11*m11);
  double ha2 = h - sqrt ( h*h - m20 * m02 + m11*m11);

  qDebug("(%s): label: %d, M11: %f, M02: %f, M20: %f, h: %f, HA1: %f, HA2: %f",
          ((*node_)["name"])->latin1(),label,m11,m02,m20,h,ha1,ha2);
	
	QString *m11_s = new QString;
	m11_s->setNum(m11);
	node_->replace("M11", m11_s);	
	QString *m02_s = new QString;
	m02_s->setNum(m02);
	node_->replace("M02", m02_s);	
	QString *m20_s = new QString;
	m20_s->setNum(m20);
	node_->replace("M20", m20_s);	
	
}

/** momentum M11 of the label  and put the value of the local stack*/
template <class PixType>
double M11(PixType label, QString sn="") {
  int i=0;
  if (sn.isEmpty()) {sn=sensor_name;i=1;}
	if (!node_->find(sn))
	  this->calc_momente(label);	
	  double s=((*node_)[sn])->toFloat();
    if (i) node_->stackPush(  new StackElemNumber( s ));
	  return s;
}
	
/** momentum M20 of the label and put the value of the local stack */
template <class PixType>
double M20(PixType label, QString sn="") {
  int i=0;
  if (sn.isEmpty()) {sn=sensor_name;i=1;}
	if (!node_->find(sn))
	  this->calc_momente(label);	
	  double s=((*node_)[sn])->toFloat();
    if (i) node_->stackPush(  new StackElemNumber( s ));
	  return s;
}

/** momentum M02 of the label and put the value of the local stack */
template <class PixType>
double M02(PixType label, QString sn="") {
  int i=0;
  if (sn.isEmpty()) {sn=sensor_name;i=1;}
	if (!node_->find(sn))
	  this->calc_momente(label);	
	  double s=((*node_)[sn])->toFloat();
    if (i) node_->stackPush(  new StackElemNumber( s ));
	  return s;
}	
//#####################  #####################


//#####################  #####################

/** Sum of all pixels in the region and put the 'pixelsize' of the local stack*/
template <class PixType>
unsigned int size(PixType label, QString sn="") {
  int i=0;
  if (sn.isEmpty()) {sn=sensor_name;i=1;}
	if (!node_->find(sn))
	  this->basic(label);	
  unsigned int s=((*node_)[sn])->toUInt();
  if (i) node_->stackPush(  new StackElemNumber( double(s) ));
	return s;
}

/** Sum of all pixels in the region multiply with x and y resolution
    and put the 'size' of the local stack*/
template <class PixType>
float area(PixType label, QString sn="") {
  int i=0;
  if (sn.isEmpty()) {sn=sensor_name;i=1;}
	if (!node_->find(sn))
	  this->basic(label);
  float s=((*node_)[sn])->toFloat();
  if (i) node_->stackPush(  new StackElemNumber( double(s) ));
	return s;
}

/** x position of the center and put the value of the local stack*/
template <class PixType>
float x_center(PixType label, QString sn="") {
  int i=0;
  if (sn.isEmpty()) {sn=sensor_name;i=1;}
	if (!node_->find(sn))
	  this->basic(label);	
	  float s=((*node_)[sn])->toFloat();
    if (i) node_->stackPush(  new StackElemNumber( double(s) ));
	  return s;
}

/** y position of the center and put the value of the local stack */
template <class PixType>
float y_center(PixType label, QString sn="") {
  int i=0;
  if (sn.isEmpty()) {sn=sensor_name;i=1;}
	if (!node_->find(sn))
	  this->basic(label);	
	  float s=((*node_)[sn])->toFloat();
    if (i) node_->stackPush(  new StackElemNumber( double(s) ));
	  return s;
}

/** calculate x geo position of the center and put the value of the local stack*/
template <class PixType>
float xGeoCenter(PixType label, QString sn="") {
  int i=0;
  if (sn.isEmpty()) {sn=sensor_name;i=1;}
	if (!node_->find(sn))
	  this->basic(label);	
	  float s=((*node_)[sn])->toFloat();
    if (i) node_->stackPush(  new StackElemNumber( double(s) ));
	  return s;
}

/** calculate y geo position of the center and put the value of the local stack*/
template <class PixType>
float yGeoCenter(PixType label, QString sn="") {
  int i=0;
  if (sn.isEmpty()) {sn=sensor_name;i=1;}
	if (!node_->find(sn))
	  this->basic(label);	
	  float s=((*node_)[sn])->toFloat();
    if (i) node_->stackPush(  new StackElemNumber( double(s) ));
	  return s;
}

/** lower left x-point of enclose rectangle and put the value of the local stack*/
template <class PixType>
unsigned int llx(PixType label, QString sn="") {
  int i=0;
  if (sn.isEmpty()) {sn=sensor_name;i=1;}
	if (!node_->find(sn))
	  this->basic(label);	
	  unsigned int s=((*node_)[sn])->toUInt();
    if (i) node_->stackPush(  new StackElemNumber( double(s) ));
	  return s;
}

/** lower left y-point of enclose rectangle and put the value of the local stack*/
template <class PixType>
unsigned int lly(PixType label, QString sn="") {
  int i=0;
  if (sn.isEmpty()) {sn=sensor_name;i=1;}
	if (!node_->find(sn))
	  this->basic(label);	
	  unsigned int s=((*node_)[sn])->toUInt();
    if (i) node_->stackPush(  new StackElemNumber( double(s) ));
	  return s;
}

/** upper right x-point of enclose rectangle and put the value of the local stack*/
template <class PixType>
unsigned int urx(PixType label, QString sn="") {
  int i=0;
  if (sn.isEmpty()) {sn=sensor_name;i=1;}
	if (!node_->find(sn))
	  this->basic(label);	
	  unsigned int s=((*node_)[sn])->toUInt();
    if (i) node_->stackPush(  new StackElemNumber( double(s) ));
	  return s;
}

/** upper right y-point of enclose rectangle and put the value of the local stack*/
template <class PixType>
unsigned int ury(PixType label, QString sn="") {
  int i=0;
  if (sn.isEmpty()) {sn=sensor_name;i=1;}
	if (!node_->find(sn))
	  this->basic(label);	
	  unsigned int s=((*node_)[sn])->toUInt();
    if (i) node_->stackPush(  new StackElemNumber( double(s) ));
	  return s;
}

/** upper left x-point of region and put the value of the local stack*/   //sensor_name
template <class PixType>
unsigned int stx(PixType label, QString sn="") {
  int i=0;
  if (sn.isEmpty()) {sn=sensor_name;i=1;}
	if (!node_->find(sn))
	  this->basic(label);	
	  unsigned int s=((*node_)[sn])->toUInt();
    if (i) node_->stackPush(  new StackElemNumber( double(s) ));
	  return s;
}

/** upper left y-point of region and put the value of the local stack*/
template <class PixType>
unsigned int sty(PixType label, QString sn="") {
  int i=0;
  if (sn.isEmpty()) {sn=sensor_name;i=1;}
	if (!node_->find(sn))
	  this->basic(label);	
	  unsigned int s=((*node_)[sn])->toUInt();
    if (i) node_->stackPush(  new StackElemNumber( double(s) ));
	  return s;
}

//##################### END SENSOR FUNCTION  #####################

protected: // Protected methods
  /** init the members */
  void init (void);

private: // Private attributes
  int sizeX_, sizeY_;
  float minval_, maxval_;

  /** pointer to the image data*/
	const int* data_;
	QString sensor_name;
#if 0	
	QString fileName_;
#endif	
	
};

#endif
