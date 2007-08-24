/***************************************************************************
                          ga_td_label_laser_ir_vis.cpp  -  description
                             -------------------
    begin                : Fri Dec 14 2001
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

//#define DEBUG_PRG

#include <fstream.h>
#include "pfm.h"
#include "gaimage.h"
#include <gavariance.h>
#include <gasplit2regionst.h>

using namespace Ga;

void usage(char* prg)
{
  cout << "usage: "<< prg << " laser1-img laser2-img ir-img vis-img resultfile-img resultfile" << endl
       << "\t min_size   - min region size, should >= 2" << endl
       << "\t max_size   - max region size, 0 = no limit" << endl
       << "\t smooth     - smooth factor" << endl
       << "\t laser1-img - laser 1st pulse input image" << endl
       << "\t laser2-img - laser 2st pulse input image" << endl
       << "\t ir-img     - IR input image (optional)" << endl
       << "\t vis-img    - VIS input image (optional)" << endl
       << "\t classname  - class name {Ortslage, Wirtschaftsflaeche, Greenland, Forest, Versiegelt, Water, B}" << endl
//       << "\t classname  - class name for 'XML' region descripion" << endl
       << "\t resultfile-img - result image with label" << endl
       << "\t resultfile  - 'XML' region descripion" << endl
       << "\t # description: " << endl
       << "\t   - read given input files and generate a label image and a description XML file" << endl
       << "\t     resample all files to the dimension of the laser img. " << endl
       << "\t   - laser1 and laser2 must have the same dimension. " << endl
       << "\t     XML description is written to 'resultfile'. " << endl
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
  //printf("%f . ",laser_v);
  //if ((vis_v>10)&&(ir_v < 10 || ir_v > 70)&&(laser_v > 10))
  if ((vis_v>1)&&(ir_v < 20 || ir_v > 50)&&(laser_v > 10))
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
 if(strcmp(classname,"Ortslage") == 0 || strcmp(classname,"Versiegelt") == 0) {
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
   if (vis_im.noChannels()==3) {
     vis_im=vis_im.hsv();
     vis_im=vis_im.getChannel(2);
   }
   Image vis_v = vis_im;
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
               int x, int y) {return true;}
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int main(int argc, char *argv[])
{
//printf ("argc: %d  %s %s %s %s %s %s\n",argc, argv[1],argv[2],argv[3],argv[4],argv[5],argv[6]);
#ifndef DEBUG_PRG
//printf ("argc: %d  %f",argc, argv);
 if(argc > 11  ||  argc < 9)
    usage(argv[0]);

 int min_size = atoi(argv[1]);
 int max_size = atoi(argv[2]);
 int smooth = atoi(argv[3]);
 char* laser1_name = argv[4];
 char* laser2_name = argv[5];
 char* classname = argv[argc-3];
// char* classoutname = argv[argc-3];
 char* label_name = argv[argc-2];
 char* xml = argv[argc-1];

 char *ir_name=0, *vis_name=0;
 if (argc > 8) ir_name = argv[6];
 if (argc > 9) vis_name = argv[7];

#else
 int min_size = 0;
 int min_size = 9999999999999;
 int smooth = 2;
 char* laser1_name = "/project/geoaida/projects/ravensburg_klaerwerk/laser_1th.pfm";
 char* laser2_name = "/project/geoaida/projects/ravensburg_klaerwerk/laser_2th.pfm";
 char* classname = "Water";
// char* classoutname = "wasser"
 char* label_name = "Water.plm";
 char* xml = "Water.xml";
 char* ir_name = "/project/geoaida/projects/ravensburg_klaerwerk/cir_ra.pnm";
 char* vis_name = "/project/geoaida/projects/ravensburg_klaerwerk/vis_ra.pnm";
#endif

 Image laser1_im, laser2_im, ir_im, vis_im; //empty input images
 laser1_im.read(laser1_name);
 laser2_im.read(laser2_name);

 if ((laser1_im.sizeX() != laser2_im.sizeX()) || (laser1_im.sizeY() != laser2_im.sizeY())){
  printf("ERROR (ga_td_label_laser_ir_vis): laserscan data have different size\n");
  exit(1);
 }

 int sizeX=laser1_im.sizeX();
 int sizeY=laser1_im.sizeY();
 Image out_im(typeid(unsigned char),sizeX,sizeY);

 printf("x: %d, y %d\n", sizeX,sizeY);
 printf("IR\n");
 if(ir_name) {
  ir_im.read(ir_name);
  ir_im = ir_im.resampleNN(sizeX,sizeY);
  /*
  Image ir_hsv_im = ir_im.hsv();
  ir_hsv_im.write("./ir_h",0);
  ir_hsv_im.write("./ir_s",1);
  ir_hsv_im.write("./ir_v",2);
  ir_im.typeImage(_PGM);//make 3 bands available
  ir_im.write("./ir_r",0);
  ir_im.write("./ir_g",1);
  ir_im.write("./ir_b",2);
  */
 }

 printf("VIS\n");
 if(vis_name) {
  vis_im.read(vis_name);
  vis_im = vis_im.resampleNN(sizeX,sizeY);
  /*
  Image vis_hsv_im = vis_im.hsv();
  vis_hsv_im.write("./vis_h",0);
  vis_hsv_im.write("./vis_s",1);
  vis_hsv_im.write("./vis_v",2);
  vis_im.typeImage(_PGM);//make 3 bands available
  vis_im.write("./vis_r",0);
  vis_im.write("./vis_g",1);
  vis_im.write("./vis_b",2);
  */
 }

 printf("calc_class: %s\n",classname);
 int secure = calc_class(laser1_im, laser2_im, ir_im, vis_im, out_im, sizeX, sizeY, classname);

 printf("blow1\n");
 for (int i=0; i<=secure; i++)
   out_im.blow(i, smooth);
 printf("blow2\n");
 for (int i=0; i<=secure; i++)
   out_im.blow(secure-i, smooth);
 printf("blow3\n");
 for (int i=0; i<=secure; i++)
   out_im.blow(i, smooth);
out_im.write("out1");
 printf("maxValue\n");
  out_im.maxValue(1);
out_im.write("out2");
//out_im.write(label_name);

 printf("labelimg\n");
 Image labelimg(typeid(int),sizeX,sizeY);
 if(max_size==0) max_size=sizeX*sizeY;
 vector<RegDesc>* regList=splitIntoRegionsT<RegDesc>(out_im,labelimg,SameRegion(),min_size,max_size);
 labelimg.write(label_name);
 FILE *fp=fopen(xml,"w");
 if (!fp) {
   cerr << "Couldn't open regionfile " << xml << endl;
   exit (1);
 }
 for (unsigned int i=2; i<regList->size(); i++) {
   RegDesc& reg=(*regList)[i];
   fprintf(fp,"<region class=%s id=%d size=%d llx=%d lly=%d urx=%d ury=%d p=0.6 file=%s >\n",
           classname,reg.id(),reg.size(),reg.llx(),reg.lly(),reg.urx(),reg.ury(),label_name);
 }
 fclose(fp);

}
