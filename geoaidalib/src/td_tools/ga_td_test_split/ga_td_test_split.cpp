/***************************************************************************
                          ga_td_test_split.cpp  -  description
                             -------------------
    begin                : Fri Dec 14 2001
    copyright            : (C) 2001 by soenke mueller
    email                : mueller@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// #define DEBUG_PRG

#include <fstream>
#include "pfm.h"
#include "gaimage.h"
#include "gagradient.h"
#include "garegion.h"
#include <qstring.h>
#include <gavariance.h>
#include <gasplit2regionst.h>

using namespace Ga;

void usage(char* prg)
{
  cout << "usage: "<< prg << " vis-img" << endl
       << "\t min_size   - min region size, should >= 2" << endl
       << "               " << endl;
  exit(-1);
}

/**
dedicated regions of class
return the max pixel value - higher value indicate more security
*/
int
calc_ortslage(Image& vis_h, Image& ir_h, Image& out_im, int sizeX, int sizeY) {

 Iterator out = out_im.begin();
 Iterator vis = vis_h.begin();
 Iterator ir = ir_h.begin();
 unsigned char val;
 for (int i=0; i<sizeX*sizeY; i++) {
  float vis_v = vis_h.getFloat(vis);
  float ir_v  = ir_h.getFloat(ir);
  if (vis_v<50) val=1; else val=0;
  if (ir_v > 10 && ir_v < 136) val++;

  out_im.set(out,val);

  out_im.nextCol(out);//next pixel
  vis_h.nextCol(vis);//next pixel
  ir_h.nextCol(ir);//next pixel
 }
 return 2;
}

/**
dedicated regions of class
return the max pixel value - higher value indicate more security
*/
int
calc_forest(Image& vis_h, Image& ir_h, Image& laser1_im, Image& laser2_im,
            Image& out_im, int sizeX, int sizeY){
 Image laser_d_im = laser1_im - laser2_im;
 laser_d_im.write("./laser_diff_image");
 Iterator out = out_im.begin();
 Iterator vis = vis_h.begin();
 Iterator ir = ir_h.begin();
 Iterator laser = laser_d_im.begin();
 unsigned char val;
 for (int i=0; i<sizeX*sizeY; i++) {
  float vis_v = vis_h.getFloat(vis);
  float ir_v  = ir_h.getFloat(ir);
  float laser_v  = laser_d_im.getFloat(laser);
  if ((vis_v>50)&&(ir_v < 10 || ir_v > 136)&&(laser_v > 10))
   val=1; else val=0;

  out_im.set(out,val);

  out_im.nextCol(out);//next pixel
  vis_h.nextCol(vis);//next pixel
  ir_h.nextCol(ir);//next pixel
  laser_d_im.nextCol(laser);//next pixel
 }
 return 1;
}

/**
dedicated regions of class
return the max pixel value - higher value indicate more security
*/
int
calc_greenland(Image& vis_h, Image& ir_h, Image& laser1_im, Image& laser2_im,
            Image& out_im, int sizeX, int sizeY){
 Image laser_d_im = laser1_im - laser2_im;
 laser_d_im.write("./laser_diff_image");
 Iterator out = out_im.begin();
 Iterator vis = vis_h.begin();
 Iterator ir = ir_h.begin();
 Iterator laser = laser_d_im.begin();
 unsigned char val;
 for (int i=0; i<sizeX*sizeY; i++) {
  float vis_v = vis_h.getFloat(vis);
  float ir_v  = ir_h.getFloat(ir);
  float laser_v  = laser_d_im.getFloat(laser);
  if ((vis_v>50)&&(ir_v < 10 || ir_v > 136)&&(laser_v < 10))
   val=1; else val=0;

  out_im.set(out,val);

  out_im.nextCol(out);//next pixel
  vis_h.nextCol(vis);//next pixel
  ir_h.nextCol(ir);//next pixel
  laser_d_im.nextCol(laser);//next pixel
 }
 return 1;
}

/**
dedicated regions of class
return the max pixel value - higher value indicate more security
*/
int
calc_wirtschaftsflaeche(Image& vis_h, Image& ir_h, Image& laser1_im, Image& laser2_im,
            Image& out_im, int sizeX, int sizeY){
 Image laser_d_im = laser1_im - laser2_im;
 Iterator out = out_im.begin();
 Iterator vis = vis_h.begin();
 Iterator ir = ir_h.begin();
 Iterator laser = laser_d_im.begin();
 unsigned char val;
 for (int i=0; i<sizeX*sizeY; i++) {
  float vis_v = vis_h.getFloat(vis);
  float ir_v  = ir_h.getFloat(ir);
  float laser_v  = laser_d_im.getFloat(laser);
  if (vis_v<50) val=0; else val=1;
  if (ir_v < 10 || ir_v > 136) val++;
  if (laser_v > 10) val++;

  out_im.set(out,val);

  out_im.nextCol(out);//next pixel
  vis_h.nextCol(vis);//next pixel
  ir_h.nextCol(ir);//next pixel
  laser_d_im.nextCol(laser);//next pixel
 }
 return 3;
}


/**
dedicated regions of class
return the max pixel value - higher value indicate more security
*/
int
calc_water(Image& vis_v, Image& ir_h, Image& ir_v, Image& laser1_im, Image& laser2_im,
           Image& out_im, int sizeX, int sizeY){
 Image laser_d_im = laser1_im - laser2_im;
 Iterator out = out_im.begin();
 Iterator vis = vis_v.begin();
 Iterator irh = ir_h.begin();
 Iterator irv = ir_v.begin();
 Iterator laser = laser_d_im.begin();
 unsigned char val;
 for (int i=0; i<sizeX*sizeY; i++) {
  float vvis = vis_v.getFloat(vis);
  float hir  = ir_h.getFloat(irh);
  float vir  = ir_v.getFloat(irv);
  float laser_v  = laser_d_im.getFloat(laser);
  if (isnan(laser_v) || ((laser_v * laser_v) < 0.06)) {
    if ((vvis>60 && vvis<120) && (hir>25 && hir<300)) val=1;
    else val=0;
  }

  out_im.set(out,val);

  out_im.nextCol(out);//next pixel
  vis_v.nextCol(vis);//next pixel
  ir_h.nextCol(irh);//next pixel
  ir_v.nextCol(irv);//next pixel
  laser_d_im.nextCol(laser);//next pixel
 }
 return 1;
}

/**
*/
int
calc_class(Image& laser1_im, Image& laser2_im, Image& ir_im,
           Image& vis_im, Image& out_im,
           int sizeX, int sizeY, char *classname) {
 if(strcmp(classname,"Ortslage") == 0) {
   Image vis_h = vis_im.hsv().getChannel(0);
   Image ir_h  = ir_im.hsv().getChannel(0);
   return calc_ortslage(vis_h, ir_h, out_im, sizeX, sizeY);
 } else if(strcmp(classname,"Wirtschaftsflaeche") == 0) {
   Image vis_h = vis_im.hsv().getChannel(0);
   Image ir_h  = ir_im.hsv().getChannel(0);
   return calc_wirtschaftsflaeche(vis_h, ir_h, laser1_im, laser2_im, out_im, sizeX, sizeY);
 } else if(strcmp(classname,"Greenland") == 0) {
   Image vis_h = vis_im.hsv().getChannel(0);
   Image ir_h  = ir_im.hsv().getChannel(0);
   return calc_greenland(vis_h, ir_h, laser1_im, laser2_im, out_im, sizeX, sizeY);
 } else if(strcmp(classname,"Forest") == 0) {
   Image vis_h = vis_im.hsv().getChannel(0);
   Image ir_h  = ir_im.hsv().getChannel(0);
   return calc_forest(vis_h, ir_h, laser1_im, laser2_im, out_im, sizeX, sizeY);
 } else if(strcmp(classname,"Water") == 0) {
   Image vis_v = vis_im.hsv().getChannel(2);
   Image ir_h  = ir_im.hsv().getChannel(0);
   Image ir_v  = ir_im.hsv().getChannel(2);
   return calc_water(vis_v, ir_h, ir_v, laser1_im, laser2_im, out_im, sizeX, sizeY);
 } else {
    printf("ERROR (calc_class): wrong class name\n");
    exit(1);
 }

return 0;
// Image laser_diff_im;
// laser_diff_im = laser2_im - laser1_im; //diff-img to detect trees
// laser_diff_im.write("./height");
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class RegDesc
{
  public:
    RegDesc()
      {
        llx_=INT_MAX;
        lly_=INT_MIN;
        urx_=INT_MIN;
        ury_=INT_MAX,
        size_=0;
        id_=0;
      };
    int setPixel(const Image& dpic, Image& lpic, int x, int y, int val)
    {
      lpic.set(x, y, val);
      size_++;
      if (x < llx_)
        llx_ = x;
      if (y < ury_)
        ury_ = y;
      if (x > urx_)
        urx_ = x;
      if (y > lly_)
        lly_ = y;
      return val;
    }
    int setId(int val) {id_=val;return val;}
    int size() {return size_;}
    int id() {return id_;}
    int llx() {return llx_;}
    int lly() {return lly_;}
    int urx() {return urx_;}
    int ury() {return ury_;}
  protected:
    int llx_,lly_,urx_,ury_;
    int id_;
    int size_;
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class SameRegion
{
  public:
    SameRegion() {};
    bool operator()(const Image &dpic,
                    const Image &lpic,
                    int x_center, int y_center,
                    int x_neighbour, int y_neighbour)
    {
      if(dpic.getFloat(x_center,y_center) == 0) return false;
      return (dpic.getFloat(x_center,y_center) ==dpic.getFloat(x_neighbour,y_neighbour));
    }
    bool valid(const Image &dpic,
               const Image &lpic,
               int x, int y) { return true; }
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int main(int argc, char *argv[])
{
#ifndef DEBUG_PRG
if(argc > 6)
    usage(argv[0]);	// =Programm-name

 char* vis_name = argv[1];
 int min_size = atoi(argv[2]);
 int haus = atoi(argv[3]);
 char* label_name = argv[argc-2];
 char* xml = argv[argc-1];

#else
 int min_size = 10;
 int haus= 0;
 char* vis_name = "/project/geoaida/projects/test/symbol.ppm";
 char* label_name = "/project/geoaida/projects/test/label.pgm";
 char* xml = "/project/geoaida/projects/test/region.reg";
#endif

 Image vis_im; 		//empty image

 if(vis_name)
  vis_im.read(vis_name);
 int sizeX= vis_im.sizeX();
 int sizeY= vis_im.sizeY();

 FILE *fp=fopen(xml,"w");
 if (!fp) {
   cerr << "Couldn't open regionfile " << xml << endl;
   exit (1);
			 }

if (!haus){
//	select class "Ortslage"
 Image labelimg (typeid(int), sizeX, sizeY);
 Image out_im(typeid(unsigned char), sizeX, sizeY);

	
	for (int y=0; y<sizeY; y++)
		for (int x=0; x<sizeX; x++){
			if ( ((int) vis_im.getFloat(x,y,0) > 185) && ((int) vis_im.getFloat(x,y,0) < 195) ||
				 ((int) vis_im.getFloat(x,y,0) > 200) && ((int) vis_im.getFloat(x,y,0) < 210)   )
				labelimg.set(x, y,1);                                                                                  																																							
														}

 int max_size= sizeX*sizeY;
 vector<RegDesc>* regList=splitIntoRegionsT<RegDesc>(labelimg, out_im, SameRegion(), min_size, max_size);

 out_im.write(QString(label_name)+"_1");

 for (unsigned int i=2; i<regList->size(); i++) {
   RegDesc& reg=(*regList)[i];
   fprintf(fp,"<region class=Ortslage id=%d size=%d llx=%d lly=%d urx=%d ury=%d file=%s_1 >\n",
           reg.id(),reg.size(),reg.llx(),reg.lly(),reg.urx(),reg.ury(),label_name);
 }

//	select class "Industrie"
 Image labelimg2 (typeid(int), sizeX, sizeY);
 Image out_im2(typeid(unsigned char), sizeX, sizeY);

	
	for (int y=0; y<sizeY; y++)
		for (int x=0; x<sizeX; x++){
			if ( ((int) vis_im.getFloat(x,y,0) > 155) && ((int) vis_im.getFloat(x,y,0) < 165))
				labelimg2.set(x, y,1);
														}

 vector<RegDesc>* regList2=splitIntoRegionsT<RegDesc>(labelimg2, out_im2, SameRegion(), min_size, max_size);

 out_im2.write(QString(label_name)+"_2");

 for (unsigned int i=2; i<regList2->size(); i++) {
   RegDesc& reg=(*regList2)[i];
   fprintf(fp,"<region class=Industrie id=%d size=%d llx=%d lly=%d urx=%d ury=%d file=%s_2 >\n",
           reg.id(),reg.size(),reg.llx(),reg.lly(),reg.urx(),reg.ury(),label_name);
 }


//	select class "Wirtschaftsflaeche"
 Image labelimg3 (typeid(int), sizeX, sizeY);
 Image out_im3(typeid(unsigned char), sizeX, sizeY);
	
	for (int y=0; y<sizeY; y++)
		for (int x=0; x<sizeX; x++){
				if ( (int) vis_im.getFloat(x,y,1) >= 180 )
				labelimg3.set(x, y,1);
														}

 vector<RegDesc>* regList3=splitIntoRegionsT<RegDesc>(labelimg3, out_im3, SameRegion(), min_size, max_size);

 out_im3.write(QString(label_name)+"_3");

 for (unsigned int i=2; i<regList3->size(); i++) {
   RegDesc& reg=(*regList3)[i];
   fprintf(fp,"<region class=Wirtschaftsflaeche id=%d size=%d llx=%d lly=%d urx=%d ury=%d file=%s_3 >\n",
           reg.id(),reg.size(),reg.llx(),reg.lly(),reg.urx(),reg.ury(),label_name);
 }
	}
else{

//	select class "Haus"
 Image labelimg (typeid(int), sizeX, sizeY);
 Image out_im(typeid(unsigned char), sizeX, sizeY);

		for (int y=0; y<sizeY; y++)
		for (int x=0; x<sizeX; x++){
			if ( ((int) vis_im.getFloat(x,y,0) > 200) && ((int) vis_im.getFloat(x,y,0) < 210))
				labelimg.set(x, y,1);                                                                                  																																							
														}

 int max_size= sizeX*sizeY;
 vector<RegDesc>* regList=splitIntoRegionsT<RegDesc>(labelimg, out_im, SameRegion(), min_size, max_size);

 out_im.write(label_name);

 FILE *fp=fopen(xml,"w");
 if (!fp) {
   cerr << "Couldn't open regionfile " << xml << endl;
   exit (1);
 }

 for (unsigned int i=2; i<regList->size(); i++) {
   RegDesc& reg=(*regList)[i];
   fprintf(fp,"<region class=Haus id=%d size=%d llx=%d lly=%d urx=%d ury=%d file=%s >\n",
           reg.id(),reg.size(),reg.llx(),reg.lly(),reg.urx(),reg.ury(),label_name);
																				 }

	}


 fclose(fp);

}
