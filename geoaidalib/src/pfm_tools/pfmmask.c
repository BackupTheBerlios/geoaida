/*
 * $RCSfile: pfmmask.c,v $
 * $Source: /data/cvs/geoaidalib/geoaida/pfm_tools/pfmmask.c,v $
 * $Revision: 1.2 $
 * $Date: 2001/03/23 13:01:07 $
 * $Author: pahl $
 * $Locker:  $
 */
#ifdef linux
int __isnanf(float);
#ifndef NAN
#define NAN sqrt(-1)
#endif
#endif


#include <stdio.h>
#include <stdlib.h>
#include "pfm.h"
#include <pbm.h>

void Usage() {
  fprintf(stderr,
	  "Masks a pfm file with a given pbm file\n"
	  "Usage:\n"
	  "  pfmmask <infile> <bitmap> <outfile> [<mask_val>=0]\n");
  exit(0);
}

void PfmMask(float *pic, bit **p_bit, int cols, int rows, int mask_val)
{
  int x,y;
  for (y=0; y<rows; y++)
    for (x=0; x<cols; x++) {
      	if (p_bit[y][x]==mask_val) pic[x+y*cols]=NAN;
   	}
}

int main(int argc, char **argv)
{
  FILE *infile, *outfile, *bitmap;
  int cols, rows, bcols, brows;
  int c;
  int mask_val;
  float *pic1, minval, maxval;
  double scale, offset;
 	bit **p_bit;

  if ( argc <4)
    {
      Usage();
    }
  if (argc > 4)
	  sscanf(argv[4],"%d",&mask_val);
 	else mask_val=0;
  infile=fopen(argv[1],"r");
  if (!infile)
    {
      fprintf(stderr,"Can't open file %s for reading!\n",argv[1]);
      exit(0);
    }
  bitmap=fopen(argv[2],"r");
  if (!bitmap)
    {
      fprintf(stderr,"Can't open bitmap %s for reading!\n",argv[1]);
      exit(0);
    }
  outfile=fopen(argv[3],"w");
  if (!outfile)
    {
      fprintf(stderr,"Can't open file %s for writing!\n",argv[2]);
      exit(0);
    }
  pic1=pfm_readpfm(infile,&cols,&rows,&minval,&maxval);
  pbm_init(&argc,argv);
  p_bit=pbm_readpbm(bitmap,&bcols,&brows);
  fclose(bitmap);
  if ((bcols!=cols) || (brows!=rows)) {
  	fprintf(stderr,"pfmfile (%dx%d) and bitmap (%dx%d) file do not have the same dimension\n",cols,rows,bcols,brows);
  	exit(0);
  }
  PfmMask(pic1,p_bit,cols,rows,mask_val);
  /*  pfm_writepfm(outfile,pic2,ncols,nrows,nminval,nmaxval); */
  pfm_writepfm(outfile,pic1,cols,rows,1,-1);
  fclose(outfile);
  return 1;
}
