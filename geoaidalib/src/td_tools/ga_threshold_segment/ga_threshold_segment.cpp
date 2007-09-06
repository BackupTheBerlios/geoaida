/***************************************************************************
                          ga_threshold_segment.cpp  -  description
                             -------------------
    begin                : Fri Aug 24 2007
    copyright            : (C) 2007 Christian Becker
    email                : becker@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <gaimage.h>b

//#include <gasplit2regionst.h>
#include <garegionsplitter.h>
#include <garegion.h>

using namespace Ga;
void Usage(const char *prg)
{
  cout << "Usage:" << endl;
  cout << "  " << prg << " <src-image> <labelimage> <regionfile> <minsize> <maxsize> <minval> <maxval> [<class>]" << endl;
}

int main(int argc, char **argv)
{
  if (argc<7) {
    Usage(argv[0]);
    exit(EXIT_FAILURE);
  }
  const char* srcname=argv[1];
  const char* labelname=argv[2];
  const char* regionname=argv[3];
  int minsize=0;
  int maxsize=INT_MAX;
  double minval=0;
  double maxval=0;
  sscanf(argv[4],"%d",&minsize);
  sscanf(argv[5],"%d",&maxsize);
  sscanf(argv[6],"%lf",&minval);
  sscanf(argv[7],"%lf",&maxval);
  const char* resultClass="undefined";
  if (argc>8) {
      resultClass=argv[8];
  }
  Image src;
  src.read(srcname);
  if (src.isEmpty()) {
    cerr << "Couldn't read image " << srcname << endl;
    exit(EXIT_FAILURE);
  }
  
  if (src.noChannels() != 1)
      if (src.noChannels() == 3){
          src=src.convert2ndvi();
      }
      else{
          cerr << "Image " << srcname << "has to be NDVI Image (1 channel) or IrRG-Image (3 channels)" << endl;
          exit(EXIT_FAILURE);
      }
  
  Image labelimg(typeid(int),src.sizeX(),src.sizeY());
  Image maskimg(typeid(int),src.sizeX(),src.sizeY());
  
  
  for (int x=0; x < src.sizeX(); x++){
      for (int y=0; y < src.sizeY(); y++){
          double val = src.getFloat(x, y);
          if ((val >= minval) && (val <= maxval)){
              src.set(x, y, 2);
          }
          else{
              src.set(x, y, 0);
          }
      }
  }


  despeckle(src, 500, 0, 2);
  despeckle(src, 500, 2, 0);




  blowshrink(src, -2);
  blowshrink(src, 2);

  maskimg= 1;
  RegionFinder rf(maskimg, src, 0);
  vector<RegDesc> regList = splitIntoRegions(labelimg, rf, resultClass, labelname, minsize,maxsize);
  labelimg.write(labelname);
//  src.write(labelname);

  
  int t=regionsToFile(regionname, regList);
  if (t == EXIT_FAILURE){
      cerr << "Can't open regionfile " << regionname << endl;
    exit (EXIT_FAILURE);
  }
  return EXIT_SUCCESS;

}
