/***************************************************************************
                               gi_fft.cpp 
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

#include <math.h>

#include <string>
#include <iostream>
#include <cstdlib>
#include <getopt.h>

#include <gaimage.h>
//#include <garegion.h>
#include <gaconvolution.h>

#include <unistd.h>

using namespace std;
using namespace Ga;

void usage()
{
  cout << "usage: gi_convolution               " << endl;


  cout << "               [-v]                  verbose operation" << endl;
  cout << "               input output "<< endl;
  exit(-1);
}

int main(int argc, char *argv[])
{
  if (argc < 4)
    usage();

  FILE *fp;
  int option_index, option_char;
  int verbose = 0, lowcut = 0;
  bool polar=false;
	int readfiles=0, writefiles=0;
  float wgauss = 1.0, xbox = 1.0, ybox = 1.0, disc = 1.0;

  
  char filter[80];
  
  // evaluate the user defined options

  struct option long_options[] = {
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
  };

  while ((option_char = getopt_long(argc, argv, "uvh", long_options, &option_index)) != EOF)
    switch (option_char)
    {

    case 'v': verbose = 1; break;
    case 'h':
    case 'u': usage(); break;
    }

  if (optind + 3 > argc)
    usage();



  // load the input image
  if (verbose) clog << "Loading image " << argv[optind] << ", " << endl;

    
  Image image;
  Image kernel;
  if (!image.read(argv[optind])){
      cerr << "Error: Could not open file " << argv[optind] << endl;
      cerr << "Abort." << endl;
      return EXIT_FAILURE;
  }
  if (!kernel.read(argv[optind + 1])){
      cerr << "Error: Could not open file " << argv[optind + 1] << endl;
      cerr << "Abort." << endl;
      return EXIT_FAILURE;
  }
  

  // This fft algorithm is designed to work on grayscale images, so make sure we have one.
  if (image.noChannels() != 1) {
      image = image.convert2Luminance();
  }  
  
  
  Image outimage = convolve(image, kernel);
 
    
  assert(fp = fopen(argv[optind + 2], "w"));
  outimage.write(fp);

  
  if (verbose) clog << "ready." << endl;

  return EXIT_SUCCESS;
}
  
