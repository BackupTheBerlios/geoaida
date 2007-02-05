/***************************************************************************
                          pfmvariance.cpp  -  description
                             -------------------
    begin                : Fri Oct 12 2001
    copyright            : (C) 2001 by jrgen bckner
    email                : bueckner@tnt.uni-hannover.de
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
#include <gavariance.h>
using namespace Ga;

void Usage(int argc, char **argv)
{
	printf("Usage:\n"
         "  %s -c <infile> <outfile> <outvalues> <winsize>\n"
         "    -c - circular scale\n",
         argv[0]);
	exit(0);
}

#ifdef linux
extern "C" {
int __isnanf(float);
	   }
#ifndef NAN
#define NAN sqrt(-1)
#endif
#endif

#define sqr(x) ((x)*(x))


int main(int argc, char **argv)
{
  bool circular=false;
  {
    int opt;
    char options[] = "c";
    while ((opt = getopt (argc, argv, options)) != -1) {
      switch (opt) {
      case 'c':
        circular=true;
        break;
      }
    }
  }
	if (argc-optind<4)
		Usage(argc,argv);
	
  const char* infile=argv[optind++];
  const char* outfile=argv[optind++];
  const char* outvalues=argv[optind++];  
  int winSize=3;
  sscanf(argv[optind++],"%d",&winSize);

  Image im;
  im.read(infile);
  if (im.isEmpty()) {
    fprintf(stderr,"Can't open %s\n",argv[1]);
    return -1;
  }
  Image result;
  if (circular)
    result=Ga::variance(im,winSize,360);
  else
    result=Ga::variance(im,winSize);

  result.write(outfile);

  //  calc mean variance
  float mvar=0.0;
  for (int y = 1; y < result.sizeY()-1; y ++)
    for (int x = 1; x < result.sizeX()-1; x ++)
      if (result.getFloat(x,y,0) != 0.0)
        mvar+=result.getFloat(x,y,0);
  
  FILE *fp;
  if (!(fp = fopen(outvalues, "a")))
    {
      fprintf(stderr, "Can't open outputfile %s\n", outvalues);
      Usage(argc,argv);
      exit(-1);
    }

   	fprintf(fp, "%f;", mvar/(result.sizeX()*result.sizeY()));

    fclose(fp);
  
  
}
