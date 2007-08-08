/***************************************************************************
                          gi_histothres.cpp 
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
  cout << "usage: gi_histothres [--thres %]    histogram threshold, def: 50%" << endl;
	cout << "                     [--left]       use values left of threshold, def: right" << endl;
  cout << "                     [-v]           verbose operation" << endl;
  cout << "                     infile outfile" << endl;
  exit(-1);
}

int main(int argc, char *argv[])
{
  if (argc < 3)
    usage();

  FILE *fp;
  int option_index, option_char;
  int verbose = 0, left = 0;

  float threshold = 0.5;
  
  // evaluate the user defined options

  struct option long_options[] = {  
    {"thres", required_argument, 0, 0},
		{"left", no_argument, 0, 1},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
  };

  while ((option_char = getopt_long(argc, argv, "vuh", long_options, &option_index)) != EOF)
    switch (option_char)
    {   
      case 0: threshold = atof(optarg) / 100.0; break;
			case 1: left = 1; break;
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

  // calculate the histogram of the image ...
  if (verbose) cout << "histogram, " << endl;
  
  const int histoSteps=1000;
  Image histo = in.calcHistogram(in.matrixMin(), in.matrixMax(), histoSteps);

  // ... and the pixel value at x percent of the total distribution
  int sumpixel = 0, tindex = 0;
  while ((tindex < histoSteps) && (sumpixel < (int)(in.sizeImage() * threshold)))
    {
      sumpixel += histo.getInt(tindex, 0);
      tindex ++;
    }

  // memorize this value ...
  threshold = tindex * (in.matrixMax() - in.matrixMin()) / histoSteps * 1.0;

  if (verbose) cout << "histogram split: " << tindex << " " << threshold << ", " << endl;

  // ... and apply the threshold 
  if (verbose) cout << "threshold, " << endl;

  ImageT<unsigned char> label(in.sizeX(), in.sizeY(), 1);
  
  for (int y = 0; y < in.sizeY(); y ++)
    for (int x = 0; x < in.sizeX(); x ++)
    {
			if (in.getFloat(x, y) < threshold)
			  label.set(x, y, left ? 1 : 0);
			else
	  		label.set(x, y, left ? 0 : 1);
     }

  if (verbose) cout << "writing output image " << argv[optind + 1] << ", " << endl;
  
  assert(fp = fopen(argv[optind + 1], "w"));
  label.write(fp);
  
  if (verbose) cout << "ready." << endl;

  return EXIT_SUCCESS;
}
  
