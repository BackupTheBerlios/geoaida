/***************************************************************************
                          plmmask.cpp  -  description
                             -------------------
    begin                : Mon Mar 19 2001
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

using namespace Ga;

void Usage(char* prg)
{
  fprintf(stderr,"Usage:\n"
	 "  %s <in_img> <mask-file(pbm)> <out_img(pbm)>\n "
	 "     out_img = in_img & mask-file \n"
   "     result is written in PBM-file 'out_img'.\n\n", prg);
  exit(1);
}


int main(int argc, char **argv)
{
  if ( argc != 4) Usage(argv[0]);

  Image im1;
//	im1.read("input.ppm");
	im1.read(argv[1]);
	Image im2;
//  im2.read("mask.pbm");
  im2.read(argv[2]);
  if (im2.typeId()!=typeid(bool)) {
  	fprintf(stderr,"mask file must be of type pbm (bool)\n");
  	exit(1);
  }
  if (im1.sizeX()!=im2.sizeX() || im1.sizeY()!=im2.sizeY()) {
  	fprintf(stderr,"image-file and mask file must be of same dimension (%dx%d)!=(%dx%d)\n",
  					im1.sizeX(),im1.sizeY(),im2.sizeX(),im2.sizeY());
  	exit(1);
  }

	for (int c=0; c<im1.noChannels(); c++) {
		void* oit1=im1.begin(0,c);
		const void *it2=im2.constBegin();
 		const void* endIt2=im2.end();
		for (;it2!=endIt2; im2.nextCol(it2), im1.nextCol(oit1)) {
			if (im2.getFloat(it2)==0.0) {
				im1.set(oit1,0.0);
			}			
		}
	}
//  im1.write("ausgabe.ppm");
  im1.write(argv[3]);
}
