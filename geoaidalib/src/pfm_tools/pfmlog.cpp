/***************************************************************************
                          pfmlog.cpp  -  description
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
  printf("  %s <infile> <outfile>\n",prg);
  exit(0);
}

int main(int argc, char **argv)
{
  if (argc<3) {
    Usage(argv[0]);
  }
  Image src;
  src.read(argv[1]);
  if (src.isEmpty()) {
    fprintf(stderr,"Can't find %s\n",argv[1]);
  }
  float min=src.matrixMin();
  float max=src.matrixMax();
  src.setMinValue(min);
  src.setMaxValue(max);
  Image result(typeid(float),src.sizeX(),src.sizeY());
  const void* sit=src.constBegin();
  void* rit=result.begin();
  int size=result.sizeImage();
  float f=9/(max-min);
  for (int i=0; i<size; i++) {
    float v=src.getFloat(sit);
    v=log10((v-min)*f+1);
    result.set(rit,v);
    src.nextCol(sit);
    result.nextCol(rit);
  }
  result.write(argv[2]);
}
