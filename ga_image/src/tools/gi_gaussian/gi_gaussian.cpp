/***************************************************************************
                          gi_gaussian.cpp  -  description
                             -------------------
    begin                : Fri Mar 9 2007
    copyright            : (C) 2007 by Julian Raschke
    email                : raschke@tnt.uni-hannover.de
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
#include <gagaussian.h>
using namespace Ga;

void Usage(int argc, char **argv)
{
	printf("Usage:\n"
         "  %s <input-file> <sigma> <output-file>\n",
         argv[0]);
	exit(0);
}

int main(int argc, char **argv)
{
  if (argc != 4)
      Usage(argc,argv);

  const char* inputfile=argv[1];
  double sigma = atof(argv[2]);
  const char* outputfile=argv[3];

  Image input;
  input.read(inputfile);
  if (input.isEmpty()) {
    fprintf(stderr,"Can't open inputfile %s\n", inputfile);
    return -1;
  }

  Image output = gaussianBlur(input, sigma);
  output.write(outputfile, 0);
}
