/***************************************************************************
                          plm2map.cpp  -  description
                             -------------------
    begin                : Fri May 18 2001
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

#if 0
 #include "gaimage.h"
#include "MLParser.h"
#include <qfile.h>
#include <qlist.h>

typedef ImageT<bool> GaMaskImage;

QDict<Image> labelImageDict;

void Usage(const char* prg) {
  fprintf(stderr,"Usage:\n"
	 "  %s <labelimage> <mapfile>\n"
	 "Convert a labelimage to vector mapfile",prg);
  exit(1);

}

int main(int argc, char *argv[])
{	
	if (argc!=3) Usage(argv[0]);
	
	// Read labelimage
	FILE *lfp=fopen(argv[2],"r");
//	FILE *mfp=fopen("/project/geoaida/tmp/mask.pbm","r");
	
	if (!lfp) {
		fprintf(stderr,"lsbelimage %s not found\n",argv[2]);
		return 1;
	}
	GaMaskImage labelImage;
	labelImage.read(lfp);
	fclose(lfp);
	
}
	
#endif