/***************************************************************************
                          gi_regression  -  description
                             -------------------
    begin                : Tue Feb 27 2007
    copyright            : (C) 2007 by christian becker
    email                : becker@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <getopt.h>
#include <gaimage.h>
#include <iostream>

using namespace Ga;
using namespace std;

void Usage(int argc, char **argv)
{
    clog << endl << endl;
    clog << "Calculates ndvi image from irrg image."<<endl;
    clog << "Output format: pfm (-1 <= pix-value <= 1)" << endl;
    clog << endl;
    clog << "Usage:" << endl;
    clog << argv[0] << " <src> <dest>" << endl;

    clog << endl << endl << endl;

    exit(0);
}


int main(int argc, char **argv)
{
  bool hsv;
  {
    int opt;
    char options[] = "h";
    while ((opt = getopt (argc, argv, options)) != -1) {
      switch (opt) {
      case 'h':
        hsv=true;
        break;
      }
    }
  }
	if (argc-optind<2)
		Usage(argc,argv);
	
   const char* infile=argv[optind++];
   const char* out=argv[optind++];

   Image im;
   im.read(infile);   
   if (im.isEmpty()) {
       cerr << "Can't open" << argv[1] << endl;;
       return -1;
   }
   

   
   Image result;
   result=im.convert2ndvi();
   
   result.write(out,0);

}
