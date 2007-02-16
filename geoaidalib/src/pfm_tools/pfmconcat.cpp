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
	printf("Usage:\n  %s <dest> <pics/row> <src 1> <src 2> ... <src n>\n",argv[0]);
	exit(0);
}

int main(int argc, char **argv)
{
	if (argc<4)
		Usage(argc,argv);
	
	int cols;
	sscanf(argv[2],"%d",&cols);
	
	int numPics=argc-3;
	int numRows=numPics/cols;
	
	int n=0;
	Image result[numRows];
	for (int y=0; y<numRows; y++) {
	  printf("Reading(%d,%d) %s\n",0,y,argv[n+3]);
  	result[y].read(argv[n+3]);
  	n++;
  	if (n>=numPics) break;
	  for (int x=1; x<cols; x++) {
	    Image im;
  	  printf("Reading(%d,%d) %s\n",x,y,argv[n+3]);
	    im.read(argv[n+3]);
	    result[y].concat(result[y],im,true);
    	n++;
    	if (n>=numPics) break;
	  }
  	if (n>=numPics) break;
	}
	for (int y=1; y<numRows; y++) {
    result[0].concat(result[0],result[y],false);
	}
	result[0].write(argv[1]);
}
