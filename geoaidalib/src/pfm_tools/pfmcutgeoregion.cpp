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
  printf
    ("Usage:\n  %s  [-pic|-geo] <src> <result> <geoWest> <geoNorth> <geoEast> <geoSouth>\n",
     argv[0]);
  exit(0);
}

int main(int argc, char **argv)
{
  if (argc < 8)
    Usage(argc, argv);

  double gW, gN, gE, gS;
  int llx, lly, urx, ury;
  Image im;
  im.read(argv[2]);
  printf("geo source: %f %f %f %f\n",im.geoWest(),im.geoNorth(),im.geoEast(),im.geoSouth());
  if (strcmp(argv[1], "-pic") == 0) {
    sscanf(argv[4], "%d", &llx);
    sscanf(argv[5], "%d", &ury);
    sscanf(argv[6], "%d", &urx);
    sscanf(argv[7], "%d", &lly);
  }
  else {
    sscanf(argv[4], "%lf", &gW);
    sscanf(argv[5], "%lf", &gN);
    sscanf(argv[6], "%lf", &gE);
    sscanf(argv[7], "%lf", &gS);
    im.imageBBox(gW, gN, gE, gS, llx, lly, urx, ury);
    printf("geo region: %f %f %f %f\n", gW, gN, gE, gS);
  }

  Image result(llx, ury, urx - llx + 1, lly - ury + 1, im);
  im.geoBBox(llx, lly, urx, ury, gW, gN, gE, gS);
  printf("geo region: %f %f %f %f\n", gW, gN, gE, gS);
  printf("Cutting (%d,%d) (%d,%d)\n", llx, ury, urx, lly);
  result.setGeoCoordinates(gW, gN, gE, gS);
  result.write(argv[3]);
}
