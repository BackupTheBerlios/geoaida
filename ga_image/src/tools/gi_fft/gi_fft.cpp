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
#include "rfftw.h"

#include <iostream>
#include <stdlib.h>
#include <getopt.h>

#include <gaimage.h>
#include <garegion.h>
#include <unistd.h>

using namespace std;
using namespace Ga;

void usage()
{
  cout << "usage: gi_fft  [--filter mode]       select operation mode" << endl;
	cout << "                  mode: fft          execute 2D FFT" << endl;
	cout << "                        ifft         execute inverse 2D FFT" << endl;
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
  cout << "               [-v]                  verbose operation" << endl;
  cout << "               infile outfile" << endl;
  exit(-1);
}

int main(int argc, char *argv[])
{
  if (argc < 3)
    usage();

  FILE *fp;
  int option_index, option_char;
  int verbose = 0, lowcut = 0;
	float wgauss = 1.0, xbox = 1.0, ybox = 1.0, disc = 1.0;

  char filter[80];
  
  // evaluate the user defined options

  struct option long_options[] = {
    {"filter", required_argument, 0, 0},
		{"wgauss", required_argument, 0, 1},
		{"xbox", required_argument, 0, 2},
		{"ybox", required_argument, 0, 3},
		{"disc", required_argument, 0, 4},
		{"lowcut", no_argument, 0, 5},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
  };

  while ((option_char = getopt_long(argc, argv, "vuh", long_options, &option_index)) != EOF)
    switch (option_char)
    {
      case 0: 
				strcpy(filter, optarg);
				break;
			case 1: wgauss = atof(optarg); break;
			case 2: xbox = atof(optarg); break;
			case 3: ybox = atof(optarg); break;
			case 4: disc = atof(optarg); break;
			case 5: lowcut = 1; break;
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

  int M = in.sizeY();
  int N = in.sizeX() - 2;
 	int N21 = N / 2 + 1;
 
  // filter image

	if (!strcmp(filter, "fft"))
	{
	  if (verbose) cout << "fft2D, " << endl;
     	
    rfftwnd_plan fftp = rfftw2d_create_plan(in.sizeY(), in.sizeX(), FFTW_REAL_TO_COMPLEX, FFTW_IN_PLACE);

	  fftw_real *data = (fftw_real *) calloc((in.sizeX() + 2) * in.sizeY(), sizeof(fftw_real));

	  for (int y = 0; y < in.sizeY(); y ++)
  	  for (int x = 0; x < in.sizeX(); x ++)
	      *(data + y * (in.sizeX() + 2) + x) = in.getFloat(x, y, 0) * pow(-1.0, double(y));

    rfftwnd_one_real_to_complex(fftp, data, NULL);

	  Image out(typeid(float), in.sizeX() + 2, in.sizeY());

    for (int y = 0; y < in.sizeY(); y ++)
	    for (int x = 0; x < in.sizeX() + 2; x ++)
  	    out.set(x, y, *(data + y * (in.sizeX() + 2) + x) / in.sizeX() / in.sizeY() , 0);

  	if (verbose) cout << "writing output image " << argv[optind + 1] << ", " << endl;
  
	  assert(fp = fopen(argv[optind + 1], "w"));
	  out.write(fp);
	}
  else if (!strcmp(filter, "ifft"))
	{
	  if (verbose) cout << "inv. fft2D, " << endl;
 
    rfftwnd_plan fftp = rfftw2d_create_plan(M, N, FFTW_COMPLEX_TO_REAL, FFTW_IN_PLACE);

	  fftw_complex *data = (fftw_complex *) calloc(M * N21, sizeof(fftw_complex));
	  fftw_real *result = (fftw_real *)data;

		for (int y = 0; y < M; y ++)
    	for (int x = 0; x < N21; x ++)
      {
        (*(data + y * N21 + x)).re = in.getFloat( 2 * x, y, 0);
        (*(data + y * N21 + x)).im = in.getFloat( 2 * x + 1, y, 0);
      }

    rfftwnd_one_complex_to_real(fftp, data, NULL);

	  Image out(typeid(float), N, M);

  	for (int y = 0; y < M; y ++)
    	for (int x = 0; x < N; x ++)
      	out.set(x, y, *(result + y * (N + 2) + x) * pow(-1-0, float(y)) , 0);

  	if (verbose) cout << "writing output image " << argv[optind + 1] << ", " << endl;
  
	  assert(fp = fopen(argv[optind + 1], "w"));
	  out.write(fp);
	}
  else if (!strcmp(filter, "gauss"))
	{
	  if (verbose) cout << "Gauss filter, " << endl;

	  /* Gaussian filter
  	
    	 The Fourier image consists of N21 complex values. Zero
	     frequencies are clustered around y= M/2 and x = 0 (origin).
  	   Apply the exp-filter to real and imaginary part independently
	  */

  	float scale = 0.0;

    for (int y = 0 ; y < M; y ++)
 	  	for (int x = 0; x < N21; x ++)
      {
        scale = exp(-(pow((double)x / (double)N21, 2.0) + (pow((in.sizeY() / 2 - y) / ((double)M / 2.0), 2.0))) * wgauss);

        in.set(2 * x, y, in.getFloat(2 * x, y, 0) * scale, 0);
        in.set(2 * x + 1, y, in.getFloat(2 * x + 1, y, 0) * scale, 0);
      }

   	if (verbose) cout << "writing output image " << argv[optind + 1] << ", " << endl;
  
	  assert(fp = fopen(argv[optind + 1], "w"));
	  in.write(fp);
	}
  else if (!strcmp(filter, "cutbox"))
	{
	  if (verbose) cout << "Box filter, " << endl;

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

   	if (verbose) cout << "writing output image " << argv[optind + 1] << ", " << endl;
  
	  assert(fp = fopen(argv[optind + 1], "w"));
	  in.write(fp);
	}
	else if (!strcmp(filter, "cutdisc"))
	{
	  if (verbose) cout << "Disc filter, " << endl;

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

   	if (verbose) cout << "writing output image " << argv[optind + 1] << ", " << endl;
  
	  assert(fp = fopen(argv[optind + 1], "w"));
	  in.write(fp);
	}
  else if (!strcmp(filter, "ln"))
	{
	  if (verbose) cout << "Natural logarithm, " << endl;

	  /*
    	 The Fourier image consists of N21 complex values. Zero
	     frequencies are clustered around y= M/2 and x = 0 (origin).
  	   Apply the log-filter with  
	  */

  	float re, im;

    for (int y = 0 ; y < M; y ++)
 	  	for (int x = 0; x < N21; x ++)
      {
				re = in.getFloat(2 * x, y, 0);
				im = in.getFloat(2 * x + 1, y, 0);

				in.set(2 * x, y, 0.5 * log(re*re+im*im), 0);
				in.set(2 * x + 1, y, atan2(im, re), 0);
      }

   	if (verbose) cout << "writing output image " << argv[optind + 1] << ", " << endl;
  
	  assert(fp = fopen(argv[optind + 1], "w"));
	  in.write(fp);
	}
  else if (!strcmp(filter, "exp"))
	{
	  if (verbose) cout << "Exponential function, " << endl;

  	float re, im;

    for (int y = 0 ; y < M; y ++)
 	  	for (int x = 0; x < N21; x ++)
      {
				re = in.getFloat(2 * x, y, 0);
				im = in.getFloat(2 * x + 1, y, 0);

				in.set(2 * x, y, exp(re)*cos(im), 0);
				in.set(2 * x + 1, y, exp(re)*sin(im), 0);
      }

   	if (verbose) cout << "writing output image " << argv[optind + 1] << ", " << endl;
  
	  assert(fp = fopen(argv[optind + 1], "w"));
	  in.write(fp);
	}


  if (verbose) cout << "ready." << endl;

  return EXIT_SUCCESS;
}
  
