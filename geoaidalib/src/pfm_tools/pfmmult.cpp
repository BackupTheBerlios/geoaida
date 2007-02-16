/***************************************************************************
                          pfmmult.cpp  -  description
                             -------------------
    begin                : Tue Mar 13 2001
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

#include "gaimage.h"

using namespace Ga;

void Usage(char* prg)
{
  fprintf(stderr,"Usage:\n"
	 "  %s <in_img1> <in_img2> <out_img>\n "
	 "     out_img = in_img1 * in_img2 \n"
   "     result is written in PFM-file 'out_img'.\n\n", prg);
  exit(1);
}


int main(int argc, char **argv)
{
  if ( argc != 4) Usage(argv[0]);

  Image im1;
//  im1.read("/project/geoaida/share/data/ascona/ascona.closed.dgm.pfm");
	im1.read(argv[1]);
	Image im2(im1.typeId());
//  im2.read("/project/geoaida/share/data/ascona/ascona.rmk.pgm");
  im2.read(argv[2]);
  im2.resampleNN(im1.sizeX(), im1.sizeY());
  Image out;
  out.multElements(im1, im2);
//  out.write("ausgabe.pgm");
  out.write(argv[3]);
}
