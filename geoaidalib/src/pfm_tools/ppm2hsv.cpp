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
	printf("Usage:\n  %s <src> <h-file> <s-file> <v-file>\n",argv[0]);
	exit(0);
}

int main(int argc, char **argv)
{
	if (argc<5)
		Usage(argc,argv);
	
	int winSize;
	sscanf(argv[3],"%d",&winSize);
	
	Image im;
	im.read(argv[1]);
	Image result=im.hsv();
	result.write(argv[2],0);
	result.write(argv[3],1);
	result.write(argv[4],2);
	
}
