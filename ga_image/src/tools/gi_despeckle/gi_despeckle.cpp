/***************************************************************************
                          gi_despeckle.cpp 
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
#include <garegion.h>
//#include <garegiont.h>
#include <unistd.h>

using namespace std;
using namespace Ga;

void usage()
{
  cout << "usage: gi_despeckle  [--size val]     remove speckles of size val, def 1" << endl;
	cout << "                     [--labelsrc val] original value, def 1" << endl;
	cout << "                     [--labeldst val] new value, def 0" << endl;
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
  int verbose = 0;

  int size = 1, labelsrc = 1, labeldst = 0;
  
  // evaluate the user defined options

  struct option long_options[] = {
    {"size", required_argument, 0, 0},
    {"labelsrc", required_argument, 0, 1},
    {"labeldst", required_argument, 0, 2},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
  };

  while ((option_char = getopt_long(argc, argv, "vuh", long_options, &option_index)) != EOF)
    switch (option_char)
    {
      case 0: size = atoi(optarg); break;
      case 1: labelsrc = atoi(optarg); break;
      case 2: labeldst = atoi(optarg); break;
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

  // despeckle image
  if (verbose) cout << "despeckle, " << endl;

	despeckle(in, size, labelsrc, labeldst);

  if (verbose) cout << "writing output image " << argv[optind + 1] << ", " << endl;
  
  assert(fp = fopen(argv[optind + 1], "w"));
  in.write(fp);
  
  if (verbose) cout << "ready." << endl;

  return EXIT_SUCCESS;
}
  
