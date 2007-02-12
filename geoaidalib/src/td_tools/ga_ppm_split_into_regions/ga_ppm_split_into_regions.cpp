/***************************************************************************
                          ga_split_into_regions.cpp  -  description
                             -------------------
    begin                : Mon Mar 25 2002
    copyright            : (C) 2002 by jrgen bckner
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

#include <getopt.h>
#include <gaimage.h>
#include <garegionsplittert.h>

using namespace Ga;
void Usage(const char *prg)
{
  cout << "Usage:" << endl;
  cout << "  " << prg << " [-r <regionfile>] <src-image> <labelimage> <min-v-level> <h-level> <s-level> <v-level> <minsize> <maxsize>" << endl;
}

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
    int setPixel(Image& lpic, int x, int y, int val)
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

class HRegion
{
  public:
    HRegion(const Image& mask, const Image& data, float level)
      : data_(data),mask_(mask),level_(level) {};
    bool operator()(const Image &lpic,
                    int x_center, int y_center,
                    int x_neighbour, int y_neighbour)
    {
      if (mask_.getInt(x_center,y_center)!=mask_.getInt(x_neighbour,y_neighbour)) return false;
      return (fabs(data_.getFloat(x_center,y_center)-data_.getFloat(x_neighbour,y_neighbour))<level_);
    }
    bool valid(const Image &lpic,
               int x, int y)
    {
      return true;
    }
  protected:
    const Image& data_;
    const Image& mask_;
    float level_;
};

class SRegion
{
  public:
    SRegion(const Image& mask, const Image& data, float level)
      : data_(data),mask_(mask),level_(level) {};
    bool operator()(const Image &lpic,
                    int x_center, int y_center,
                    int x_neighbour, int y_neighbour)
    {
      if (mask_.getInt(x_center,y_center)!=mask_.getInt(x_neighbour,y_neighbour)) return false;
      return (fabs(data_.getFloat(x_center,y_center)-data_.getFloat(x_neighbour,y_neighbour))<level_);
    }
    bool valid(const Image &lpic,
               int x, int y)
    {
      return true;
    }
  protected:
    const Image& data_;
    const Image& mask_;
    float level_;
};

class VRegion
{
  public:
    VRegion(const Image& mask, const Image& data, float level)
      : data_(data),mask_(mask),level_(level) {};
    bool operator()(const Image &lpic,
                    int x_center, int y_center,
                    int x_neighbour, int y_neighbour)
    {
      if (mask_.getInt(x_center,y_center)!=mask_.getInt(x_neighbour,y_neighbour)) return false;
      return (fabs(data_.getFloat(x_center,y_center)-data_.getFloat(x_neighbour,y_neighbour))<level_);
    }
    bool valid(const Image &lpic,
               int x, int y)
    {
      return true;
    }
  protected:
    const Image& data_;
    const Image& mask_;
    float level_;
};

inline int distance(int r1, int g1, int b1, int r2, int g2, int b2)
{
  int dr=r2-r1;
  int dg=g2-g1;
  int db=b2-b1;
  return dr*dr+dg*dg+db*db;
}

void Filter(Image& im)
{
  int ysize=im.sizeY();
  int xsize=im.sizeX();
  Image dest=im;
  int offset_x[]={+1,+1,0,-1,-1,-1,0,+1};
  int offset_y[]={0,-1,-1,-1,0,+1,+1,+1};
  for (int y=1; y<ysize-1; y++)
    for (int x=1; x<xsize-1; x++) {
      int pr[8],pg[8],pb[8];
      for (int r=0; r<8; r++) {
        pr[r]=im.getInt(x+offset_x[r],y+offset_y[r],0);
        pg[r]=im.getInt(x+offset_x[r],y+offset_y[r],1);
        pb[r]=im.getInt(x+offset_x[r],y+offset_y[r],2);
      }
      int cr=im.getInt(x,y,0);
      int cg=im.getInt(x,y,1);
      int cb=im.getInt(x,y,2);
      int v[4];
      v[0]=(distance(pr[0],pg[0],pb[0],cr,cg,cb)<distance(pr[4],pg[4],pb[4],cr,cg,cb) ? 0 : 4);
      v[1]=(distance(pr[1],pg[1],pb[1],cr,cg,cb)<distance(pr[5],pg[5],pb[5],cr,cg,cb) ? 1 : 5);
      v[2]=(distance(pr[2],pg[2],pb[2],cr,cg,cb)<distance(pr[6],pg[6],pb[6],cr,cg,cb) ? 2 : 6);
      v[3]=(distance(pr[3],pg[3],pb[3],cr,cg,cb)<distance(pr[7],pg[7],pb[7],cr,cg,cb) ? 3 : 7);
      cr=(pr[v[0]]+pr[v[1]]+pr[v[2]]+pr[v[3]])/4;
      cg=(pg[v[0]]+pg[v[1]]+pg[v[2]]+pg[v[3]])/4;
      cb=(pb[v[0]]+pb[v[1]]+pb[v[2]]+pb[v[3]])/4;
      dest.set(x,y,cr,0);
      dest.set(x,y,cg,1);
      dest.set(x,y,cb,2);
    }
  im=dest;
}

int main(int argc, char **argv)
{
  const char* regionfile=0;
  {
    int opt;
    char options[] = "r:";
    while ((opt = getopt (argc, argv, options)) != -1) {
      switch (opt) {
      case 'r':
        regionfile = optarg;
        break;
      }
    }
  }
  if (argc-optind<8) {
    Usage(argv[0]);
    exit(1);
  }
  const char* srcfile=argv[optind++];
  const char* labelfile=argv[optind++];
  int minV=0;
  sscanf(argv[optind++],"%d",&minV);
  int hLevel=1;
  sscanf(argv[optind++],"%d",&hLevel);
  int sLevel=1;
  sscanf(argv[optind++],"%d",&sLevel);
  int vLevel=1;
  sscanf(argv[optind++],"%d",&vLevel);
  int minsize=0;
  sscanf(argv[optind++],"%d",&minsize);
  int maxsize=INT_MAX;
  sscanf(argv[optind++],"%d",&maxsize);
  Image src;

  src.read(srcfile);
  if (src.isEmpty()) {
    cerr << "Couldn't read image " << srcfile << endl;
    exit(1);
  }
  if (src.noChannels()!=3) {
    cerr << "Source image isn't of type ppm" << endl;
    exit(1);
  }
  printf("Filtering\n");
  Filter(src);
  printf("Filtering done\n");
  src.write("filtered.ppm");
  src=src.hsv();
  Image hue=src.getChannel(0);
  Image sat=src.getChannel(1);
  src=src.getChannel(2);
  { // Remove very low values
    const void *vit=src.constBegin();
    void *hit=hue.begin();
    void *sit=sat.begin();
    int size=src.sizeImage();
    for (int i=0; i<size; i++) {
      if (src.getFloat(vit)<minV) {
        hue.set(hit,-1);
        sat.set(sit,0);
      }
      src.nextCol(vit);
      hue.nextCol(hit);
      sat.nextCol(sit);
    }
  }
  Image labelImage(typeid(int),src.sizeX(),src.sizeY());
  Image maskImage(typeid(int),src.sizeX(),src.sizeY());
  // Split h-Image
  {
    vector<RegDesc> hList;
    HRegion hRegion(maskImage,hue,hLevel);
    RegionSplitterT<RegDesc,HRegion> hSplitter(hList,labelImage,hRegion,minsize,maxsize);
    hSplitter.split();
    maskImage=labelImage;
    labelImage=0;
  }
  // Split s-Image
  {
    vector<RegDesc> sList;
    SRegion sRegion(maskImage,sat,sLevel);
    RegionSplitterT<RegDesc,SRegion> sSplitter(sList,labelImage,sRegion,minsize,maxsize);
    sSplitter.split();
    maskImage=labelImage;
    labelImage=0;
  }
  // Split v-Image
  {
    vector<RegDesc> vList;
    VRegion vRegion(maskImage,src,vLevel);
    RegionSplitterT<RegDesc,VRegion> vSplitter(vList,labelImage,vRegion,minsize,maxsize);
    vSplitter.split();
    maskImage=labelImage;
    labelImage=0;
  }
  labelImage=maskImage;


//  vector<RegDesc>* regList=splitIntoRegions<RegDesc>(src,labelimg,SameRegion(),minsize,maxsize);
  labelImage.write(labelfile);
  FILE *fp=fopen(regionfile,"w");
  if (!fp) {
    cerr << "Couldn't open regionfile " << regionfile << endl;
    exit (1);
  }
#if 0
  for (unsigned int i=2; i<regList->size(); i++) {
    RegDesc& reg=(*regList)[i];
    fprintf(fp,"<region class=\"%s\" id=%d size=%d llx=%d lly=%d urx=%d ury=%d file=\"%s\"/>\n",
            resultClass,reg.id(),reg.size(),reg.llx(),reg.lly(),reg.urx(),reg.ury(),
            labelfile);
  }
#endif
  fclose(fp);
}