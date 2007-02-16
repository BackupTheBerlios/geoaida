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
  printf("  %s <infile> <outfile> <id>\n\n"
         "  Generate a blank label image with the given id\n",prg);
  exit(0);
}

int main(int argc, char **argv)
{
  if (argc<4) {
    Usage(argv[0]);
  }
  Image image;
  image.read(argv[1]);
  if (image.isEmpty()) {
    fprintf(stderr,"Can't find %s\n",argv[1]);
  }
  int id;
  sscanf(argv[3],"%d",&id);
  Image result(typeid(unsigned char),image.sizeX(),image.sizeY());
  result=id;
  result.write(argv[2]);
}
