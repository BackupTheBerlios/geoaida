/***************************************************************************
                          pbm2plm.cpp  -  description
                             -------------------
    begin                : Fri Mar 23 2001
    copyright            : (C) 2001 by jürgen bückner
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
#include "gaimage.h"
using namespace Ga;

//typedef ImageT<bool> GaBoolImage;

void Usage(const char* prg) {
  fprintf(stderr,"Usage:\n"
	 "  %s <pbm-file> <plm-file> <id>\n",prg);
  exit(1);
}

int main(int argc, char *argv[])
{
	if (argc!=4) Usage(argv[0]);
	
	FILE *fp=fopen(argv[1],"r");
	if (!fp) {
		fprintf(stderr,"File %s not found\n",argv[1]);
		exit (1);
	}
	Image bitmap(typeid(bool));
	bitmap.read(fp);
		
	int id;
	sscanf(argv[3],"%d",&id);
	Image im;
	if (id>255) {
		im.init(typeid(int),bitmap.sizeX(),bitmap.sizeY(),1);
		im.typeImage(_PGM);
	}
	else {
		im.init(typeid(unsigned char),bitmap.sizeX(),bitmap.sizeY(),1);
		im.typeImage(_PFM_SINT);
	}
	ConstIterator bit=bitmap.constBegin();
	ConstIterator bitEnd=bitmap.end();
	void *it=im.begin();
	while (bit!=bitEnd) {
		if (bitmap.getFloat(bit)) {
			im.set(it,id);
//			printf("found bit setting to %d\n",id);
		}
		else {
			im.set(it,0.0);
		}
		bitmap.nextCol(bit);
		im.nextCol(it);
	}
	im.write(argv[2]);
}
