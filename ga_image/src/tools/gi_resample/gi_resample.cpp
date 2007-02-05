/***************************************************************************
                          gi_resample.cpp  -  description
                             -------------------
    begin                : Thu May 16 2002
    copyright            : (C) 2002 by Martin Pahl
    email                : pahl@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//#define DEBUG_NOSPLITREGION

#include <stdio.h>
#include <getopt.h>
#include <gaimage.h>

// #define DEBUG_PRG
// #define DEBUG_MSG

using namespace std;
using namespace Ga;

void Usage(char *prg)
{
  printf ("Usage:\n"
          "  %s -r [-n] [-p] <infile> <outfile> <factor-x> <factor-y>\n"
          "  %s -a [-n] [-p] <infile> <outfile> <size-x> <size-y>\n\n"
          "    -r relative scale\n"
          "    -a absolute scale\n"
          "    -n nearest neighbour\n"
          "    -p nearest neighbour plus\n"
          "    -k keep min/max-values\n\n",
          prg,prg);
  exit(1);
}

int main(int argc, char **argv)
{
  const char *infile=0;
  const char *outfile=0;
  bool scale=false;
  bool option=false;
  bool keepMinMax=false;
  int sizeX, sizeY;
  float scaleX,scaleY;
  int sampleFunc=0;
  {
    int opt;
    char options[] = "raknp";
    while ((opt = getopt (argc, argv, options)) != -1) {
      switch (opt) {
      case 'r':
        scale=true;
        option=true;
        break;
      case 'a':
        scale=false;
        option=true;
        break;
      case 'n':
        sampleFunc=1;
        break;
      case 'p':
        sampleFunc=2;
        break;
      case 'k':
        keepMinMax=true;
        break;
      }
    }
  }
  if (!option) {
    fprintf(stderr,"Either option \n"
                   "  -a (scale to absolute size)\n"
                   "or\n"
                   "  -r (scale to realtive size)\n"
                   "must be given\n\n");
    Usage(argv[0]);
  }
  if (argc-optind!=4) Usage(argv[0]);
  infile=argv[optind];
  outfile=argv[optind+1];
  if (scale) {
    sscanf(argv[optind+2],"%f",&scaleX);
    sscanf(argv[optind+3],"%f",&scaleY);
  }
  else {
    sscanf(argv[optind+2],"%d",&sizeX);
    sscanf(argv[optind+3],"%d",&sizeY);
  }
  Image im;
  if (!im.read(infile)) {
    fprintf(stderr,"Can't open file %s\n",infile);
    exit(1);
  }
  if (im.isEmpty()) {
    fprintf(stderr,"Image %s is empty\n",infile);
    exit(1);
  }
  printf("Read %s\n",infile);
  if (scale) {
    sizeX=int(im.sizeX()*scaleX);
    sizeY=int(im.sizeY()*scaleY);
  }
  printf("Scaling (%d x %d) -> (%d x %d) using %s\n",
         im.sizeX(),im.sizeY(),sizeX,sizeY,
         (sampleFunc ? (sampleFunc==1 ? "nearest neighbour"
                                      : "nearest neighbour plus")
                     : "linear interpolation"));
  float min=0;
  float max=0;
  if (keepMinMax && im.noChannels()==1) {
    im.storeMinValue();
    im.storeMaxValue();
    min=im.minValue();
    max=im.maxValue();
    printf("Orig  min/max: %f %f\n",min,max);
  }
  Image result;
  switch(sampleFunc) {
    case 0:
      result=im.resample(sizeX,sizeY);
      break;
    case 1:
      result=im.resampleNN(sizeX,sizeY);
      break;
    case 2:
      result=im.resampleNNplus(sizeX,sizeY);
      break;
  }
  if (keepMinMax && result.noChannels()==1) {
    result.storeMinValue();
    result.storeMaxValue();
    float newMin=result.minValue();
    float newMax=result.maxValue();
    printf("New   min/max: %f %f\n",newMin,newMax);
    float f=(max-min)/(newMax-newMin);
    void *ptr=result.begin();
    int size=result.sizeImage();
    for (int i=0; i<size; i++) {
      float v=result.getFloat(ptr);
      v=(v-newMin)*f+min;
      result.set(ptr,v);
      result.nextCol(ptr);
    }
    result.storeMinValue();
    result.storeMaxValue();
    newMin=result.minValue();
    newMax=result.maxValue();
    printf("Final min/max: %f %f\n",newMin,newMax);
  }
  result.write(outfile);
  return 0;
}

