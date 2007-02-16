/***************************************************************************
                          pfmthreshhold.cpp  -  description
                             -------------------
    begin                : Fri May 17 2002
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

#include <stdio.h>
#include <gaimage.h>
#include <float.h>

using namespace Ga;
using namespace std;

void Usage(const char* prg)
{
  printf("Usage:\n");
  printf("  %s <infile> <outfile> <min> <max>\n",prg);
  exit(0);
}

int main(int argc, char **argv)
{
  if (argc<5) {
    Usage(argv[0]);
  }
  Image image;
  image.read(argv[1]);
  if (image.isEmpty()) {
    fprintf(stderr,"Can't find %s\n",argv[1]);
  }
  float min,max;
  sscanf(argv[3],"%f",&min);
  sscanf(argv[4],"%f",&max);
  Image result=image;
  if (max<min) max=FLT_MAX;
  for (int c=0; c<image.noChannels(); c++) {
    for (int y=0; y<image.sizeY(); y++) {
      const void* sit=image.constBegin(y,c);
      void *dit=result.begin(y,c);
      for (int x=0; x<image.sizeX(); x++) {
        float v=image.getFloat(sit);
        if (v<min || v>max) result.set(dit,0);
        else result.set(dit,1);
        image.nextCol(sit);
        result.nextCol(dit);
      }
    }
  }
  result.write(argv[2]);
}