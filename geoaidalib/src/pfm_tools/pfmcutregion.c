/*
 * $RCSfile: pfmcutregion.c,v $
 * $Source: /data/cvs/geoaidalib/geoaida/pfm_tools/pfmcutregion.c,v $
 * $Revision: 1.3 $
 * $Date: 2002/01/18 13:55:50 $
 * $Author: pahl $
 * $Locker:  $
 */
#include <stdio.h>
#include <stdlib.h>
#include "pfm.h"

void Usage() {
  fprintf(stderr,
	  "Usage:\n"
	  "  pfmcutregion <infile> <outfile> <x1> <y1> <x2> <y2>\n"
	  "  cut a region from 'infile' to 'outfile'\n"
	  "  <x1> <y1> <x2> <y2> are image coordinates");
  exit(0);
}
 

int main(int argc, char **argv)
{
  FILE *infile, *outfile;
  int cols, rows;
  float minval,maxval;
  float *pic;
  int x1,y1,x2,y2;
  
  if ( argc != 7)  Usage();
  
  infile=fopen(argv[1],"r");
  if (!infile) {
    fprintf(stderr,"Can't open file %s for reading!\n",argv[1]);
    exit(0);
  }
  outfile=fopen(argv[2],"w");
  if (!outfile)  {
    fprintf(stderr,"Can't open file %s for writing!\n",argv[2]);
    exit(0);
  }
  sscanf(argv[3],"%d",&x1);
  sscanf(argv[4],"%d",&y1);
  sscanf(argv[5],"%d",&x2);
  sscanf(argv[6],"%d",&y2);
  pic=pfm_readpfm(infile,&cols,&rows,&minval,&maxval);
  printf("cols=%d, rows=%d, region=(%d,%d) (%d,%d)\n",cols,rows,x1,y1,x2,y2);
  pfm_writepfm_region(outfile,pic,cols,rows,maxval,minval,x1,y1,x2,y2);
  fclose(infile);
  fclose(outfile);
  return 1;
}
