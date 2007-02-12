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

#include <gaimage.h>
#include <gasplit2regionst.h>

using namespace Ga;
void Usage(const char *prg)
{
  cout << "Usage:" << endl;
  cout << "  " << prg << " <src-image> <labelimage> <regionfile> <minsize> <maxsize> [<class>]" << endl;
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

class SameRegion
{
  public:
    SameRegion() {};
    bool operator()(const Image &dpic,
                    const Image &lpic,
                    int x_center, int y_center,
                    int x_neighbour, int y_neighbour)
    {
      return (dpic.getFloat(x_center,y_center) ==dpic.getFloat(x_neighbour,y_neighbour));
    }
    bool valid(const Image &dpic,
               const Image &lpic,
               int x, int y)
    {
      return true;
    }
};

int main(int argc, char **argv)
{
  if (argc<6) {
    Usage(argv[0]);
    exit(1);
  }
  const char* srcname=argv[1];
  const char* labelname=argv[2];
  const char* regionname=argv[3];
  int minsize=0;
  int maxsize=INT_MAX;
  sscanf(argv[4],"%d",&minsize);
  sscanf(argv[5],"%d",&maxsize);
  const char* resultClass="test";
  if (argc>6) {
    resultClass=argv[6];
  }
  Image src;
  src.read(srcname);
  if (src.isEmpty()) {
    cerr << "Couldn't read image " << srcname << endl;
    exit(1);
  }
  Image labelimg(typeid(int),src.sizeX(),src.sizeY());
  vector<RegDesc>* regList=splitIntoRegions<RegDesc>(src,labelimg,SameRegion(),minsize,maxsize);
  labelimg.write(labelname);
  FILE *fp=fopen(regionname,"w");
  if (!fp) {
    cerr << "Couldn't open regionfile " << regionname << endl;
    exit (1);
  }
  for (unsigned int i=2; i<regList->size(); i++) {
    RegDesc& reg=(*regList)[i];
    fprintf(fp,"<region class=\"%s\" id=%d size=%d llx=%d lly=%d urx=%d ury=%d file=\"%s\"/>\n",
            resultClass,reg.id(),reg.size(),reg.llx(),reg.lly(),reg.urx(),reg.ury(),
            labelname);
  }
  fclose(fp);
}