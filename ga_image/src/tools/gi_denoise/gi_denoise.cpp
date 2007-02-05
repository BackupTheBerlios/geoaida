/***************************************************************************
                          gi_denoise.cpp  -  description
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
#include <garegion.h>

// #define DEBUG_PRG
// #define DEBUG_MSG

using namespace std;
using namespace Ga;

void Usage(char *prg)
{
  printf ("Usage:\n"
          "  %s -r [-n] [-p] <infile> <outfile>\n",prg);
  exit(1);
}

int main(int argc, char **argv)
{
  const char *infile=0;
  const char *outfile=0;
#if 0
  {
    int opt;
    char options[] = "raknp";
    while ((opt = getopt (argc, argv, options)) != -1) {
      switch (opt) {
      case 'r':
        scale=true;
        option=true;
        break;
      case 'a':
        scale=false;
        option=true;
        break;
      case 'n':
        sampleFunc=1;
        break;
      case 'p':
        sampleFunc=2;
        break;
      case 'k':
        keepMinMax=true;
        break;
      }
    }
  }
#else
  optind=1;
#endif
  if (argc-optind!=2) Usage(argv[0]);
  infile=argv[optind++];
  outfile=argv[optind++];
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
  denoise(im);
  im.write(outfile);
  return 0;
}

