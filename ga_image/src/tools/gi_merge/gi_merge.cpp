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
	printf("Usage:\n  %s <dest> <pics/row> <src 1> <src 2> ... <src n>\n"
         "  alls source must be of same type and dimension\n",argv[0]);
	exit(0);
}

int main(int argc, char **argv)
{
	if (argc<4)
		Usage(argc,argv);
	
	int cols;
	sscanf(argv[2],"%d",&cols);
	
	int numPics=argc-3;
	int numRows=(numPics-1)/cols+1;
	
  Image im;
  im.read(argv[3]);
  
  int sizeX=im.sizeX();
  int sizeY=im.sizeY();
  int tSizeX=cols*sizeX;
  int tSizeY=numRows*sizeY;
	Image result(im.typeId(),tSizeX,tSizeY,im.noChannels());
  result.typeImage(im.typeImage());
	int n=0;
	for (int y=0; y<numRows; y++) {
	  for (int x=0; x<cols; x++) {
      if (n>=numPics) break;
      printf("Reading(%d,%d) %s\n",x,y,argv[n+3]);
      im.read(argv[n+3]);
      result.copyInto(im,x*sizeX,y*sizeY);
      n++;
    }
  }
	result.write(argv[1]);
}
