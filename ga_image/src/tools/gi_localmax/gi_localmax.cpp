/***************************************************************************
                          gi_localmax.cpp 
                             -------------------
    begin                : Fri Feb 13 13:02:00 MET 2002
    copyright            : (C) 2002 by Soenke Mueller
    email                : mueller@tnt.uni-hannover.de
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
  cout << "usage: gi_localmax   [--thres]    histogram threshold" << endl;
  cout << "                     [-v]           verbose operation" << endl;
  cout << "                     infile outfile outvalues" << endl;
  exit(-1);
}

int main(int argc, char *argv[])
{
  if (argc < 4)
    usage();

  FILE *fp;
  int option_index, option_char;
  int verbose = 0;

  float threshold;
  
  // evaluate the user defined options

  struct option long_options[] = {  
    {"thres", required_argument, 0, 0},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
  };

  while ((option_char = getopt_long(argc, argv, "vh", long_options, &option_index)) != EOF)
    switch (option_char)
    {   
      case 0: threshold = atof(optarg); break;
      case 'v': verbose = 1; break;
      case 'h': usage(); break;
    }

  if (optind + 2 > argc)
    usage();

  Image in;

  // load the input image
  if (verbose) cout << "Loading image " << argv[optind] << ", " << endl;

  in.read(argv[optind]);

  if (verbose) cout << "threshold= " <<threshold<<endl;

  ImageT<float> thres_image(in.sizeX(), in.sizeY(), 1);
  
  //  threshold for the hole image
  //  threshold= float(in.sizeX())*float(in.sizeY())/223.0;
  for (int y = 0; y < in.sizeY(); y ++)
    for (int x = 0; x < in.sizeX(); x ++)
    {
			if (in.getFloat(x, y) < threshold)
			  thres_image.set(x, y, 0.0);
      else
        thres_image.set(x, y, in.getFloat(x, y));      
     }
     
  ImageT<float> result(in.sizeX(), in.sizeY(), 1);

  //  look for local maxima
  for (int y = 1; y < in.sizeY()-1; y ++)
    for (int x = 1; x < in.sizeX()-1; x ++)
    { float ampl=thres_image.get (x , y);
    //  take all 8 neighbours
      if ((thres_image.get(x-1, y) < ampl) && (thres_image.get(x-1, y) != 0.0) &&
          (thres_image.get(x, y-1) < ampl) && (thres_image.get(x, y-1) != 0.0) &&
          (thres_image.get(x, y+1) < ampl) && (thres_image.get(x, y)+1 != 0.0) &&
          (thres_image.get(x+1, y) < ampl) && (thres_image.get(x+1, y) != 0.0) &&
          (thres_image.get(x+1, y+1) < ampl) && (thres_image.get(x+1, y+1) != 0.0) &&
          (thres_image.get(x-1, y-1) < ampl) && (thres_image.get(x-1, y-1) != 0.0) &&
          (thres_image.get(x+1, y-1) < ampl) && (thres_image.get(x+1, y-1) != 0.0) &&
          (thres_image.get(x-1, y+1) < ampl) && (thres_image.get(x-1, y+1) != 0.0))
        result.set(x, y, ampl);
      else
        result.set(x, y, 0.0);
     }

  if (verbose) cout << "writing output image " << argv[optind + 1] << ", " << endl;
  
  assert(fp = fopen(argv[optind + 1], "w"));
  result.write(fp);

  //  calc lengthness
  float length=0.0;
  for (int y = 1; y < in.sizeY()-1; y ++)
    for (int x = 1; x < in.sizeX()-1; x ++)
      if (result.get(x,y) != 0.0)
        length+=result.get(x,y);
        
    if (!(fp = fopen(argv[optind + 2], "a")))
    {
      fprintf(stderr, "Can't open outputfile %s\n", argv[optind + 2]);
      usage();
      exit(-1);
    }

   	fprintf(fp, "%f];", length/(result.sizeX()*result.sizeY()));

    fclose(fp);

  
  if (verbose) cout << "ready." << endl;

  return EXIT_SUCCESS;
}
  
