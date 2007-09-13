/***************************************************************************
                            -  description
                             -------------------
    begin                : Tue Dec 12 2000
    copyright            : (C) 2000 by jürgen bückner
    email                : bueckner@tnt.uni-hannover.de
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "pfm.h"

void Usage(char *cmd)
{
  fprintf(stderr,
          "Write PFM data in (x, y, z)-format\n"
          "Usage:\n"
          "  %s <infile.pfm> <outfile.ascii> [<x-start> <y-start> <x-end> <y-end>]\n", cmd);
  exit(0);
}  

int main(int argc, char **argv)
{
  float* pic;
  int cols;
  int rows;
  float minval, maxval;
  int x,y;
  double x1, y1, x2, y2;
  double xdiff, ydiff;
  FILE *infile, *outfile;

  if (!((argc == 3)||(argc == 7))) Usage(argv[0]);
  
  infile=fopen(argv[1],"r");
  if (!infile) {
      fprintf(stderr,"Can't open file %s for reading!\n",argv[1]);
      exit(0);
  }
   
  pic=pfm_readpfm(infile,&cols,&rows,&minval,&maxval);
  fclose(infile);

  if (argc == 7) {
    sscanf(argv[3],"%lf",&x1);
    sscanf(argv[4],"%lf",&y1);
    sscanf(argv[5],"%lf",&x2);
    sscanf(argv[6],"%lf",&y2);
  } else {
    x1 = 0.0;
    y1 = 0.0;
    x2 = (double)cols-1;
    y2 = (double)rows-1;  
  }
  xdiff = (x2-x1)/(cols-1);  
  ydiff = (y2-y1)/(rows-1);  
  
  outfile=fopen(argv[2],"w");
  if (!outfile) {
    fprintf(stderr,"Can't open file %s for writing!\n",argv[2]);
    exit(0);
  }  

  fprintf(outfile,"# %s %d x %d\n# xrange %f %f yrange %f %f\n",argv[1],cols,rows,x1,x2,y1,y2);
  for (y=0;y<rows;y++)
    for (x=0;x<cols;x++)
      fprintf(outfile,"%f\t%f\t%f\n", x*xdiff+x1, y*ydiff+y1, pic[y*cols+x]);
  
  fclose(outfile);
  return 1;
}
