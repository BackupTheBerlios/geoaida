/***************************************************************************
                          regionsensor.cpp  -  description
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
    
#define DEBUGMSG

#include "regionsensor.h"
using namespace std;

RegionSensor::RegionSensor(){
	init();
}

#if 0
/** constructor - read image file  */
RegionSensor::RegionSensor(QString filename){
	init();
	readImage(filename);
}
#endif

RegionSensor::~RegionSensor(){
	//if(data_) delete[] data_;
}

#if 0
/** read image from file  */
void RegionSensor::readImage(QString filename) {
  FILE *fp;
  if(fileName_.compare(filename)==0) return;

	if(data_) delete[] data_;
  fileName_ = filename;
  fp=fopen(filename.latin1(),"r");
	if(!fp) {
		qDebug("ERROR: RegionSensor::RegionSensor(QString filename) - %s wrong fileformat",filename.latin1());
		exit(0);
	}	  
	data_=(int*)pfm_readpfm_type(fp,&sizeX_,&sizeY_,&minval_,&maxval_,PFM_SINT,0);
  fclose(fp);
}
#endif
/** init the members */
void RegionSensor::init (void){
	data_=0;
	sizeX_ = sizeY_ = 0;
	minval_ = maxval_ = 0.0;
#ifdef DONT_USE_GAIMAGE
	fileName_="";
#endif	
}

/** return image pointer   */
const int* RegionSensor::data (void) {
	return data_;
}

/** set image pointer and size  */
void RegionSensor::image (Ga::Image& img){
  if (img.typeId()!=typeid(int)) {
    cerr << "Error: RegionSensor::image image is not signed integer" << endl;
    exit (1);
  }
	data_=(const int*)img.constBegin();
	sizeX_ = img.sizeX();
	sizeY_ = img.sizeY();
}

#ifdef DONT_USE_GAIMAGE
/** set image pointer and size  */
void RegionSensor::image (int* ga_b, int x, int y){
	data_=ga_b;
	sizeX_ = x;
	sizeY_ = y;
}
#endif

/** calculate the sensor value - first step to get the type*/
void RegionSensor::calc(QString str) {
	sensor_name=str;
	int label = (int) ((*node_)["id"])->toInt();
	calc_type(label);	
}

//##################### CIRCUMFERENCE #####################
/** help function for calc_circumference */
float RegionSensor::nextPoint(const int* pic, int& x, int& y, int& dir, int label)
{
  //int indir=dir;
  dir=(dir+5)%8;
  int offset_x[8]={+1,+1,0,-1,-1,-1,0,+1};
  int offset_y[8]={0,-1,-1,-1,0,+1,+1,+1};
  int olddir=dir;
  do {
    int nx=x+offset_x[dir];
    int ny=y+offset_y[dir];
    if (nx>=0 && ny>=0 && nx<sizeX_ && ny<sizeY_) {
      if (pic[nx+sizeX_*ny]==label) {
        x=nx;
        y=ny;
        if((dir%2)==1)return 1.41421356;
        return 1.0;
        //qDebug("label: %d; (%d, %d) dir: %d indir: %d  1.0", label, x,y,dir,indir);return 1.0;
      }
    }
    dir=(dir+1)%8;
  } while (dir!=olddir);
  return 0.0;
}
