/***************************************************************************
                               gi_histogram.cpp 
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

using namespace Ga;
using namespace std;

void usage()
{
  cout << "usage: gi_histogram  [--slots #]    number of histogram slots, def: 100" << endl;
	cout << "                     [-v]           verbose operation" << endl;
  cout << "                     infile outfile outfile.txt" << endl;
  exit(-1);
}

int main(int argc, char *argv[])
{
  if (argc < 3)
    usage();

  FILE *fp;
  int option_index, option_char;
  int verbose = 0, slots = 100;

  // evaluate the user defined options

  struct option long_options[] = {
		{"slots", required_argument, 0, 0},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
  };

  while ((option_char = getopt_long(argc, argv, "vuh", long_options, &option_index)) != EOF)
    switch (option_char)
    {
			case 0: slots = atoi(optarg); break;
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
	
  double min = in.matrixMin(), max = in.matrixMax();
//  double min = 0, max = 255;
  double slotwidth = (max - min) / slots;

	if (!slotwidth)
	{
	  fprintf(stderr, "Can't determine histogram - slotwidth = 0!\n");
    usage();
    exit(-1);
  }	

  long histcount[slots + 1];
	memset(histcount, 0, sizeof(long) * slots);

	double val;
  int nan=0;

	if (verbose) cout << "Computing histogram, " << endl;

  for (int y = 0; y < in.sizeY(); y++)
    for (int x = 0; x < in.sizeX(); x++)
		{
      val = in.getFloat(x, y);
      if (isnan(val)) { nan ++; continue; }
			histcount[(int)((val - min) / slotwidth)] ++;
    }

	if (verbose) cout << "Writing histogram " << argv[optind + 1] << ", " << endl;

  if (!(fp = fopen(argv[optind + 1], "w")))
  {
    fprintf(stderr, "Can't open outputfile %s\n", argv[optind + 1]);
    usage();
    exit(-1);
  }

	if (verbose){
  	fprintf(fp, "# Histogram for image %s, %d slots of %f size, NaN: %d\n", argv[optind], slots, slotwidth, nan);
	  fprintf(fp, "#\n");
  	fprintf(fp, "#  slot        value        count       part     partsum\n");
	  fprintf(fp, "#\n");
    }

  long num_points = in.sizeImage() - nan;
	double histpart, histpartsum = 0.0;
  float quota=0.0;
  max=0;

  for (int s = 0; s < slots; s++)
  {
    if (s<71)
      quota+=float(histcount[s]);
		histpart = (float)histcount[s] / num_points;
    if (histpart > max)
      max= histpart;    
		histpartsum += histpart;
  	if (verbose)
      fprintf(fp, "%8d %15f %10ld %.8f %.8f\n", s, min + s * slotwidth, histcount[s], histpart, histpartsum);
	}

//  fprintf(fp, "Summe 0-79=%f quota=%f\%\n", quota, quota/float(num_points));
  //  measurement for shades
  fprintf(fp,"X=[%f;", quota/float(num_points));
  fclose(fp);

  //  genarate histogram image
  ImageT <unsigned char> histo_image (slots, 500, 1);
  bool swit=false;
    
  for (int x = 0; x < histo_image.sizeX(); x++){
 	 histpart = (float)histcount[x] / num_points;             
   // scale histogram
   histpart= (histpart/max)*histo_image.sizeY();
   for (int y = 0; y < histo_image.sizeY(); y++)
      if (histo_image.sizeY()-(int)histpart < y){
        if (swit)
          histo_image.set (x,y,0);
        else
          histo_image.set (x,y,125);
        }  
      else
        histo_image.set (x,y,255);
      swit=!swit;      
     }  

  if (verbose) cout << "Writing histogram image " <<argv[optind + 2]<< endl;
  histo_image.write (argv[optind + 2]);

  if (verbose) cout << "ready." << endl;

  return EXIT_SUCCESS;
}
  
