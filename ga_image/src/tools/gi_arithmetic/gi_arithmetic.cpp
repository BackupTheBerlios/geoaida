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
#include <garegion.h>
#include <gafft.h>

#include <unistd.h>

using namespace std;
using namespace Ga;

void usage()
{
  cout << "usage: gi_arithmetic mode select operation mode" << endl;
	cout << "                  mode: mul          execute 2D FFT" << endl;
	cout << "                        div           execute inverse 2D FFT" << endl;
	cout << "                        add           execute inverse 2D FFT" << endl;
	cout << "                        sub           execute inverse 2D FFT" << endl;
  cout << "               [-v]                   verbose operation" << endl;
  cout << "               file1 file2 file3      file3 = file1 mode file2" << endl;
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
  if (argc < 3)
    usage();

  FILE *fp;
  int option_index, option_char;
  bool verbose = false;
  bool typeerror=false;
  
  char filter[80];
  
  // evaluate the user defined options

  struct option long_options[] = {
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
  };

  while ((option_char = getopt_long(argc, argv, "vh", long_options, &option_index)) != EOF)
    switch (option_char)
    {
    case 0: 
				strcpy(filter, optarg);
				break;
    case 'v': verbose = true; break;
    case 'h':
    case 'u': usage(); break;
    }

  if (optind + 3 > argc)
    usage();



  // load the input image
  if (verbose) clog << "Loading image " << argv[optind] << ", " << endl;

  Image file1;
  if (!file1.read(argv[optind+1])){
      cerr << "Error: Could not open file " << argv[optind + 1] << endl;
      cerr << "Abort." << endl;
      return EXIT_FAILURE;
  }
  Image file2;
  if (!file2.read(argv[optind+2])){
      cerr << "Error: Could not open file " << argv[optind+2] << endl;
      cerr << "Abort." << endl;
      return EXIT_FAILURE;
  }


  if (file1.typeId()!=file2.typeId()){
      typeerror=true;
      if (verbose)
          clog << "Image type problem.." << endl;
  }
  typeerror=true;

  Image img1=file1, img2=file2;


  if ((file1.sizeX()!= file2.sizeX()) ||
      (file1.sizeY()!= file2.sizeY()) ||
      typeerror){

      int sizeX=max(file1.sizeX(), file2.sizeX());
      int sizeY=max(file1.sizeY(), file2.sizeY());
      



      img1.init(typeid(float), sizeX, sizeY, file1.noChannels());
      img1.clear();
      for (int x=0; x < file1.sizeX(); x++){
          for (int y=0; y < file1.sizeY(); y++){
              for (int c=0; c < file1.noChannels(); c++){
                  img1.set(x, y, file1.getFloat(x, y, c), c);
              }
              }
      }

          


      
      img2.init(typeid(float), sizeX, sizeY, file2.noChannels());
      img2.clear();
      for (int x=0; x < file2.sizeX(); x++){
          for (int y=0; y < file2.sizeY(); y++){
              for (int c=0; c < file2.noChannels(); c++){
                  img2.set(x, y, file2.getFloat(x, y, c), c);
              }
          }
      }

          
      if (verbose)
          clog << "Images now have same size and same type" << endl;
      
  }
/*
  for (int x=0; x < img1.sizeX(); x++){
      for (int y=0; y < img1.sizeY(); y++){
          for (int c=0; c < img1.noChannels(); c++){
              img2.set(x, y, file2.getFloat(x, y, c), c);
          }
      }
  }
*/
  assert(img1.noChannels() == img2.noChannels());
  char* modus=argv[optind];
  
	if (!strcmp(modus, "add"))
      {
          if (verbose) clog << "add.." << endl;
          img1 += img2;
      }
  else 
      if (!strcmp(modus, "sub"))
          {
              if (verbose) clog << "sub.." << endl;
              img1 -= img2;
          }
      else 
          if (!strcmp(modus, "mul"))
              {
                  Image t(typeid(float), img1.sizeX(), img1.sizeY(), img1.noChannels());
                  if (verbose) clog << "mul.." << endl;
//                  img1 *= img2; // Who knows what this operation really does..
                  for (int x = 0; x < img1.sizeX(); x++)
                      for (int y = 0; y < img1.sizeY(); y++)
                          for (int c = 0; c < img1.noChannels(); c++){
                              t.set(x, y, img1.getFloat(x, y, c)*img2.getFloat(x, y, c), c);
                          }
                  img1=t;

              }


  assert(fp = fopen(argv[optind + 3], "w"));
  img1.write(fp);
  fclose(fp);

  
  if (verbose) clog << "ready." << endl;

  return EXIT_SUCCESS;
}
  
