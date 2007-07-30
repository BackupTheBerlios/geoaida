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
#include <fftw3.h>

#include <string>
#include <iostream>
#include <stdlib.h>
#include <getopt.h>

#include <gaimage.h>
#include <garegion.h>
#include <gafft.h>

#include <unistd.h>

using namespace std;
using namespace Ga;

void usage()
{
  cout << "usage: gi_fft  [--filter mode]       select operation mode" << endl;
	cout << "                  mode: fft          execute 2D FFT" << endl;
	cout << "                        ifft         execute inverse 2D FFT" << endl;
/*
	cout << "                        gauss        apply gauss e^(-x*x+y*y)" << endl;
	cout << "                        cutbox       cutoff at Nyquist/factor" << endl;
	cout << "                        cutdisc" << endl; 
	cout << "                        ln           complex logarithm" << endl;
	cout << "                        exp          complex exp-func" << endl;
	cout << "               [--wgauss val]        filterwidth (gauss)" << endl;
	cout << "               [--xbox fac]          x factor for cutbox" << endl;
	cout << "               [--ybox fac]          y factor for cutbox" << endl;
	cout << "               [--disc fac]          x&y factor for cutdisc" << endl;
	cout << "               [--lowcut]            sets low instead of high frequencies to 0" << endl;
*/
  cout << "               [--real file]         Input Filename for real/magnitude data" << endl;
  cout << "               [--imaginary file]    Input Filename for imaginary/phase data" << endl;
  cout << "               [--realout file]      Output Filename for real/magnitude data (depending on -p)" << endl;
  cout << "               [--imaginaryout file] Putput Filename for imaginary/phase data (depending on -p)" << endl;
  cout << "               [-p]                  writes/reads frequency image in polar coordinates in standard modus" << endl;
  cout << "               [-v]                  verbose operation" << endl;
  cout << "               infile outfile        not used when using --real, --imaginary, --realout AND --imaginaryout" << endl;
  exit(-1);
}

int main(int argc, char *argv[])
{
  if (argc < 1)
    usage();

  FILE *fp;
  int option_index, option_char;
  int verbose = 0, lowcut = 0;
  bool polar=false;
	int readfiles=0, writefiles=0;
  float wgauss = 1.0, xbox = 1.0, ybox = 1.0, disc = 1.0;
  // string realinfile ='', iminfile ='', realoutfile='', imoutfile='';
  
  char filter[80];
  
  // evaluate the user defined options

  struct option long_options[] = {
    {"filter", required_argument, 0, 0},
		{"wgauss", required_argument, 0, 1},
		{"xbox", required_argument, 0, 2},
		{"ybox", required_argument, 0, 3},
		{"disc", required_argument, 0, 4},
		{"real", required_argument, 0, 5},
		{"imaginary", required_argument, 0, 6},
		{"realout", required_argument, 0, 7},
		{"imaginaryout", required_argument, 0, 8},
		{"lowcut", no_argument, 0, 9},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
  };

  while ((option_char = getopt_long(argc, argv, "pvuh", long_options, &option_index)) != EOF)
    switch (option_char)
    {
    case 0: 
				strcpy(filter, optarg);
				break;
    case 1: wgauss = atof(optarg); break;
    case 2: xbox = atof(optarg); break;
    case 3: ybox = atof(optarg); break;
    case 4: disc = atof(optarg); break;
/*
    case 5: realinfile=atof(optarg); break;
    case 6: iminfile=atof(optarg); break;
    case 7: realoutfile=atof(optarg); break;
    case 8: imoutfile=atof(optarg); break;
*/
    case 9: lowcut = 1; break;
    case 'v': verbose = 1; break;
    case 'p': polar=true;break;
    case 'h':
    case 'u': usage(); break;
    }

  if (optind + 2 > argc)
    usage();
/*
  if ((realinfile) ^ (iminfile)){//xor
      usage();
  }
  if ((realoutfile) ^ (imoutfile)){//xor
      usage();
  }
  
  if (realinfile && iminfile){
      readfiles = 1;
  }
  if (realoutfile && imoutfile){
      writefiles = 1;
  }
*/
  Image in;

  // load the input image
  if (verbose) clog << "Loading image " << argv[optind] << ", " << endl;
/*
  if (readfiles){
      Image real;
      Image im;
      if (!real.read(realinfile)){
          cerr << "Error: Could not open file " << argv[optind] << endl;
          cerr << "Abort." << endl;
          return false;
      }
      if (!im.read(iminfile)){
          cerr << "Error: Could not open file " << argv[optind] << endl;
          cerr << "Abort." << endl;
          return false;
      }
      if ((real.sizeX()!=im.sizeX())|| 
          (real.sizeY()!=im.sizeY())){
          cerr << "Error: Input files have to have equal size."<< endl;
          cerr << "Abort." << endl;
          return false;
      }

      in(typeid(float), real.sizeX(), real.sizeY());
      for (int x=0; x<real.sizeX(); x++){
          for (int y=0; y<real.sizeY(); y++){
              in.setPixel(x, y, real.getFloat(x, y));
              in.setPixel(x + real.sizeX(), y, im.getFloat(x, y));              
          }          
      }      
  }
  else{
*/
      if (!in.read(argv[optind])){
          cerr << "Error: Could not open file " << argv[optind] << endl;
          cerr << "Abort." << endl;
          return false;
      }
      // This fft algorithm is designed to work on grayscale images, so make sure we have one.
      if (in.noChannels() != 1) {
          in = in.convert2Luminance();
      }  

  int M = in.sizeY();
  int N = in.sizeX()-2;
 	int N21 = N / 2 + 1;
 
  // filter image
  
	if (!strcmp(filter, "fft"))
      {
      if (verbose) clog << "fft2D, " << endl;
      
      Image out=fft(in, polar);

      if (verbose) clog << "writing output image " << argv[optind + 1] << ", " << endl;
  
      assert(fp = fopen(argv[optind + 1], "w"));
      out.write(fp);
	}


  else if (!strcmp(filter, "ifft"))
	{
	  if (verbose) clog << "inv. fft2D, " << endl;
    
    Image out=ifft(in, polar);
    
    assert(fp = fopen(argv[optind + 1], "w"));
    out.write(fp);
	}
/*
  else if (!strcmp(filter, "gauss"))
	{
	  if (verbose) clog << "Gauss filter, " << endl;

	  // Gaussian filter
  	//
    //	 The Fourier image consists of N21 complex values. Zero
	  //   frequencies are clustered around y= M/2 and x = 0 (origin).
  	//   Apply the exp-filter to real and imaginary part independently
	  //

  	float scale = 0.0;

    for (int y = 0 ; y < M; y ++)
 	  	for (int x = 0; x < N21; x ++)
      {
        scale = exp(-(pow((double)x / (double)N21, 2.0) + (pow((in.sizeY() / 2 - y) / ((double)M / 2.0), 2.0))) * wgauss);

        in.set(2 * x, y, in.getFloat(2 * x, y, 0) * scale, 0);
        in.set(2 * x + 1, y, in.getFloat(2 * x + 1, y, 0) * scale, 0);
      }

   	if (verbose) clog << "writing output image " << argv[optind + 1] << ", " << endl;
  
	  assert(fp = fopen(argv[optind + 1], "w"));
	  in.write(fp);
	}
  else if (!strcmp(filter, "cutbox"))
	{
	  if (verbose) clog << "Box filter, " << endl;

    for (int y = 0 ; y < M; y ++)
 	  	for (int x = 0; x < N21; x ++)
      {
				if ((float)x < (float)N21 / xbox && (fabs(float(in.sizeY() / 2 - y)) < (float)M / (2 * ybox)))
				{
					if (lowcut)
					{
		        in.set(2 * x, y, 0, 0);
	        	in.set(2 * x + 1, y, 0, 0);
					}
				}
				else 
					if (!lowcut)
					{
					  in.set(2 * x, y, 0, 0);
	        	in.set(2 * x + 1, y, 0, 0);
					}
      }

   	if (verbose) clog << "writing output image " << argv[optind + 1] << ", " << endl;
  
	  assert(fp = fopen(argv[optind + 1], "w"));
	  in.write(fp);
	}
	else if (!strcmp(filter, "cutdisc"))
	{
	  if (verbose) clog << "Disc filter, " << endl;

    for (int y = 0 ; y < M; y ++)
 	  	for (int x = 0; x < N21; x ++)
      {
				if (pow(x, 2.0) + pow(double(in.sizeY() / 2 - y), 2.0) < pow(double(N21 / disc), 2.0))
				{
					if (lowcut)
					{
		        in.set(2 * x, y, 0, 0);
	        	in.set(2 * x + 1, y, 0, 0);
					}
				}
				else 
					if (!lowcut)
					{
					  in.set(2 * x, y, 0, 0);
	        	in.set(2 * x + 1, y, 0, 0);
					}
      }

   	if (verbose) clog << "writing output image " << argv[optind + 1] << ", " << endl;
  
	  assert(fp = fopen(argv[optind + 1], "w"));
	  in.write(fp);
	}
  else if (!strcmp(filter, "ln"))
	{
	  if (verbose) clog << "Natural logarithm, " << endl;

	  //
    //	 The Fourier image consists of N21 complex values. Zero
	  //   frequencies are clustered around y= M/2 and x = 0 (origin).
  	//   Apply the log-filter with  
	  //

  	float re, im;

    for (int y = 0 ; y < M; y ++)
 	  	for (int x = 0; x < N21; x ++)
      {
				re = in.getFloat(2 * x, y, 0);
				im = in.getFloat(2 * x + 1, y, 0);

				in.set(2 * x, y, 0.5 * log(re*re+im*im), 0);
				in.set(2 * x + 1, y, atan2(im, re), 0);
      }

   	if (verbose) clog << "writing output image " << argv[optind + 1] << ", " << endl;
  
	  assert(fp = fopen(argv[optind + 1], "w"));
	  in.write(fp);
	}
  else if (!strcmp(filter, "exp"))
	{
	  if (verbose) clog << "Exponential function, " << endl;

  	float re, im;

    for (int y = 0 ; y < M; y ++)
 	  	for (int x = 0; x < N21; x ++)
      {
				re = in.getFloat(2 * x, y, 0);
				im = in.getFloat(2 * x + 1, y, 0);

				in.set(2 * x, y, exp(re)*cos(im), 0);
				in.set(2 * x + 1, y, exp(re)*sin(im), 0);
      }

   	if (verbose) clog << "writing output image " << argv[optind + 1] << ", " << endl;
  
	  assert(fp = fopen(argv[optind + 1], "w"));
	  in.write(fp);
	}

*/

  if (verbose) clog << "ready." << endl;

  return EXIT_SUCCESS;
}
  
