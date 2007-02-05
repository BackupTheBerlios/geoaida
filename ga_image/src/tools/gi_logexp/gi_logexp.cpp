/***************************************************************************
                              gi_logexp.cpp 
                             -------------------
    begin                : Tue Jun 25 13:02:00 MET 2002
    copyright            : (C) 2002 by Oliver Stahlhut
    email                : stahlhut@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <stdlib.h>
#include <getopt.h>

#include <gaimage.h>
#include <gaimaget.h>
#include <gadraw.h>
#include <garegiont.h>
#include <unistd.h>

using namespace std;
using namespace Ga;

void usage()
{
  cout << "usage: gi_logexp [--mode log,exp] apply logarithm or exp-function to the image" << endl;
  cout << "                     [-v]             verbose operation" << endl;
  cout << "                     infile outfile" << endl;
  exit(-1);
}

int main(int argc, char *argv[])
{
  if (argc < 3)
    usage();

  FILE *fp;
  int option_index, option_char;
  int verbose = 0, mode = 0;

  // evaluate the user defined options

  struct option long_options[] = {
    {"mode", required_argument, 0, 0},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
  };

  while ((option_char = getopt_long(argc, argv, "vuh", long_options, &option_index)) != EOF)
    switch (option_char)
    {
      case 0: if (!strcmp(optarg, "exp")) mode = 1; break;
      case 'v': verbose = 1; break;
      case 'h':
      case 'u': usage(); break;
    }

  if (optind + 2 > argc)
    usage();

  Image in;

  // load the input image
  if (verbose) cout << "Loading image " << argv[optind] << ", " << endl;

  in.read(argv[optind]);

	if (!mode)
	{
	  // calculate the minimum of the image ...
		float offset = in.matrixMin();
		if (offset <= 0)
			offset = -offset + 1.0;
		else
			offset = 0;
  
	  for (int y = 0; y < in.sizeY(); y ++)
   	 for (int x = 0; x < in.sizeX(); x ++)
 		  in.set(x, y, log(in.getFloat(x, y) + offset), 0);
	}
	else
	{
    for (int y = 0; y < in.sizeY(); y ++)
   	 for (int x = 0; x < in.sizeX(); x ++)
 		  in.set(x, y, exp(in.getFloat(x, y)) - 1.0, 0);
  }

  if (verbose) cout << "writing output image " << argv[optind + 1] << ", " << endl;
  
  assert(fp = fopen(argv[optind + 1], "w"));
  in.write(fp);
  
  if (verbose) cout << "ready." << endl;

  return EXIT_SUCCESS;
}
  
