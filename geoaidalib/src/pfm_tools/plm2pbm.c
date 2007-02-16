/*
 * $RCSfile: plm2pbm.c,v $
 * $Source: /data/cvs/geoaidalib/geoaida/pfm_tools/plm2pbm.c,v $
 * $Revision: 1.1 $
 * $Date: 2001/02/13 12:27:23 $
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
#include <pbm.h>
#include "pfm.h"

void Usage()
{
  fprintf(stderr,
	  "Usage:\n"
	  "  plm2pbm <infile> <outfile>\n");
  exit(0);
}

bit **IntToBit(int *pic, int cols, int rows)
{
  bit **p_bit;
  int val;
  int x, y;

  p_bit=pbm_allocarray(cols,rows);
  for (y=0; y<rows; y++)
    {
      for (x=0; x<cols; x++) {
	val=pic[y*cols+x];
	switch (val) {
	case -1:
	case 0:
	  p_bit[y][x]=0;
	  break;
	default:
	  p_bit[y][x]=1;
	  break;
	}
      }
    }
  return p_bit;
}

int main(int argc, char **argv)
{
  FILE *infile, *outfile;
  int cols, rows;
  float minval,maxval;
  int *pic;
  bit **p_bit;

  if ( argc < 3)  Usage();

  infile=fopen(argv[1],"r");
  if (!infile) {
      fprintf(stderr,"Can't open file %s for reading!\n",argv[1]);
      exit(0);
    }
  outfile=fopen(argv[2],"w");
  if (!outfile) {
      fprintf(stderr,"Can't open file %s for writing!\n",argv[2]);
      exit(0);
    }
  pbm_init(&argc,argv);
  pic=(int*)pfm_readpfm_type(infile,&cols,&rows,&minval,&maxval,PFM_SINT,0);
  printf("Size (%dx%d) values: %f - %f (read)\n",cols,rows,minval,maxval);
  fclose(infile);
  printf("minval=%f, maxval=%f\n",minval,maxval);
  p_bit=IntToBit(pic,cols,rows);
  pbm_writepbm(outfile,p_bit,cols,rows,0);
  fclose(outfile);
  return 1;
}
