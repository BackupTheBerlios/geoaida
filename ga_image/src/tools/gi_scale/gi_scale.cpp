/***************************************************************************
                          detectbuilding.cpp  -  description
                             -------------------
    begin                : Thu May 16 2002
    copyright            : (C) 2002 by Martin Pahl
    email                : pahl@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//#define DEBUG_NOSPLITREGION

#include <stdio.h>
#include <getopt.h>
#include <gaimage.h>

// #define DEBUG_PRG
// #define DEBUG_MSG

using namespace std;
using namespace Ga;

void Usage(char *prg)
{
  printf ("Usage:\n"
          "  Use gi_resample. This programm will rescale data values in future!!!\n"
          "  %s <infile> <outfile> <multiplicator> <divisor>\n",
          prg);
  exit(1);
}

int main(int argc, char **argv)
{
  const char *infile=0;
  const char *outfile=0;
  int sizeX, sizeY;
  float multiply,divisor;
  optind=1;
  if (argc!=5) {
    Usage(argv[0]);
  }
  infile=argv[optind];
  outfile=argv[optind+1];
  sscanf(argv[optind+2],"%f",&multiply);
  sscanf(argv[optind+3],"%f",&divisor);
  double scale=multiply/divisor;
  Image im;
  if (!im.read(infile)) {
    fprintf(stderr,"Can't open file %s\n",infile);
    exit(1);
  }
  if (im.isEmpty()) {
    fprintf(stderr,"Image %s is empty\n",infile);
    exit(1);
  }
  printf("Read %s\n",infile);
  printf("Scaling image by %f/%f = %f\n",
         multiply,divisor,scale);
  int size=im.sizeImage();
  void* it=im.begin();
  for (int i=0; i<size; i++) {
    im.set(it,im.getFloat(it)*scale);
//    printf("%d: %f\n",i,im.getFloat(it));
    im.nextCol(it);
  }
  im.write(outfile);
  return 0;
}

