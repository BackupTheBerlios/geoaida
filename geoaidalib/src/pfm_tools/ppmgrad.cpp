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
#include <gagradient.h>
using namespace Ga;

void Usage(int argc, char **argv)
{
	printf("Usage:\n  %s <src> <abs-file> <angle-file>\n",argv[0]);
	exit(0);
}

#ifdef linux
extern "C" {
int __isnanf(float);
	   }
#ifndef NAN
#define NAN sqrt(-1)
#endif
#endif

#define sqr(x) ((x)*(x))


int main(int argc, char **argv)
{
	if (argc<4)
		Usage(argc,argv);
	
	int winSize;
	sscanf(argv[3],"%d",&winSize);
	
	Image im;
	im.read(argv[1]);
  if (im.isEmpty()) {
    fprintf(stderr,"Can't open %s\n",argv[1]);
    return -1;
  }
  if (im.typeImage()!=_PPM) {
    fprintf(stderr,"Image not a PPM\n");
    return -1;
  }
  Image result=Ga::gradient2D3Channels(im);

  result.write(argv[2],0);
	result.write(argv[3],1);
}
