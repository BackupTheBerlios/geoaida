/***************************************************************************
                          pfmvariance.cpp  -  description
                             -------------------
    begin                : Fri Oct 12 2001
    copyright            : (C) 2001 by jrgen bckner
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
#include <gavariance.h>
using namespace Ga;

void Usage(int argc, char **argv)
{
	printf("Usage:\n  %s [-sqrt] <src> <result> <windowSize> [<scaleSize>]\n",argv[0]);
	exit(0);
}

int main(int argc, char **argv)
{
  bool sqrtMode=false;
  int argoffset=0;
	if (argc<4)
		Usage(argc,argv);
	
  if (strcmp(argv[1],"-sqrt")==0) {
    printf("Sqrt-Mode\n");
    sqrtMode=true;
    argoffset=1;
  	if (argc<5)
	  	Usage(argc,argv);
  }
	int winSize;
	sscanf(argv[3+argoffset],"%d",&winSize);
	
	Image im;
	im.read(argv[1+argoffset]);
  Image result;
	if (argc>4+argoffset) {
	  double scaleSize;
	  sscanf(argv[4+argoffset],"%lf",&scaleSize);
    result=variance(im,winSize,scaleSize);
	}
	else
    result=variance(im,winSize);
  if (sqrtMode) {
    printf("Sqrt-Mode\n");
    void* it=result.begin();
    int size=result.sizeImage();
    for (int i=0; i<size; i++) {
      result.set(it,sqrt(result.getFloat(it)));
      result.nextCol(it);
    }
  }
	result.write(argv[2+argoffset]);
	
}
