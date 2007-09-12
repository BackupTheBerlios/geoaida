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

#include <gaimage.h>

#include <garegionsplitter.h>
#include <garegionsplittert.h>
#include <garegion.h>
#include <sstream>

#include "ga_threshold_segment.h"

using namespace Ga;
void Usage(const char *prg)
{
  cout << "Usage:" << endl;
  cout << "  " << prg << " <src-image> <maskimage> <labelimage> <regionfile> <minsize> <maxsize> <minval> <maxval> [<class>] " << endl;
}

int main(int argc, char **argv)
{
    clog << "Arguments Count " << argc << endl;
    for (int i=0; i < argc; i++)
        clog << "Argument " << i << ": " << argv[i]<< endl;

  if (argc<8) {
    Usage(argv[0]);
    exit(EXIT_FAILURE);
  }
  const char* srcname=argv[1];
  const char* maskname=argv[2];
  const char* labelname=argv[3];
  const char* regionname=argv[4];
  int minsize=0;
  int maxsize=INT_MAX;
  double minval=0;
  double maxval=0;
  double prop;

  sscanf(argv[5],"%d",&minsize);
  sscanf(argv[6],"%d",&maxsize);
  sscanf(argv[7],"%lf",&minval);
  sscanf(argv[8],"%lf",&maxval);
  const char* resultClass="undefined";
  if (argc>8) {
      resultClass=argv[9];
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

  Image srcOrig=src;

  Image labelimg(typeid(int),src.sizeX(),src.sizeY());

  Image maskimg;
  maskimg.read(maskname);

  if ((maskimg.sizeX() != src.sizeX() )|| (maskimg.sizeY() != src.sizeY())){
      cerr << "MaskImage " << maskname << "has to have the same size as the input image" << endl;
      exit(EXIT_FAILURE);
  }

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
  
  RegionFinderThres rf(maskimg, src, 0);
  vector<RegDescThres> regList;

  
	RegionSplitterT<RegDescThres, RegionFinderThres> rSplitter(regList,labelimg,rf,minsize,maxsize);
	rSplitter.setRegionClass(resultClass);
	rSplitter.setLabelFile(labelname);
	rSplitter.split();
  
  
  
  
  vector<double> avgValues((int)regList.size(), 0);
  vector<int> regionSizes(regList.size(), 0);
  for (int x=0; x < src.sizeX(); x++){
      for (int y=0; y < src.sizeY(); y++){
          avgValues[labelimg.getInt(x, y)] += srcOrig.getFloat(x, y);
          regionSizes[labelimg.getInt(x, y)]++;          
      }
  }

  for (int i=0; i<regList.size(); i++){

      clog << "RegList Entry " << i << " Sum: " << avgValues[regList[i].id_] << " Count: " << regionSizes[regList[i].id_]  << endl;
      if (regionSizes[regList[i].id_]!=0)
          regList[i].average_ = avgValues[regList[i].id_]/((double)regionSizes[regList[i].id_]);
          regList[i].regsize_ = regionSizes[regList[i].id_];

  }


  labelimg.write(labelname);
//  src.write(labelname);

  
//  int t=regionsToFile(regionname, regList);

	ostringstream out;
	// vector<RegDesc>::iterator regIter= reglist.begin();

	for (int i=2; i < regList.size(); i++) // Skip the first 2 elements because they contain id 0 and 1 (= background regions), 
                                         // I'm not sure why they are included in the list at all...
	{
		RegDescThres reg = regList[i];
    out << reg.toString() << endl;
  }
	ofstream outputFile(regionname, ios::out);

	if (!outputFile)
	{
      cerr << "can't open region output file \"" << regionname << "\"" << endl;
      return EXIT_FAILURE;
	}
	outputFile << out.str();

  return EXIT_SUCCESS;

}
