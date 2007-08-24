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
//#include <gasplit2regionst.h>
#include <garegionsplitter.h>

using namespace Ga;
void Usage(const char *prg)
{
  cout << "Usage:" << endl;
  cout << "  " << prg << " <src-image> <labelimage> <regionfile> <minsize> <maxsize> [<class>]" << endl;
}

int main(int argc, char **argv)
{
  if (argc<6) {
    Usage(argv[0]);
    exit(EXIT_FAILURE);
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
    exit(EXIT_FAILURE);
  }
  Image labelimg(typeid(int),src.sizeX(),src.sizeY());
  Image maskimg(typeid(int),src.sizeX(),src.sizeY());
  maskimg= 1;
  RegionFinder rf(maskimg, src, 0);
  vector<RegDesc> regList = splitIntoRegions(labelimg, rf, minsize,maxsize);
  labelimg.write(labelname);
  int t=regionsToFile(regionname, regList);
  if (t == EXIT_FAILURE){
      cerr << "Can't open regionfile " << regionname << endl;
    exit (EXIT_FAILURE);
  }
  return EXIT_SUCCESS;

}
