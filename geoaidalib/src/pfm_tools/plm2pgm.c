/*
 * $RCSfile: plm2pgm.c,v $
 * $Source: /data/cvs/geoaidalib/geoaida/pfm_tools/plm2pgm.c,v $
 * $Revision: 1.2 $
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
#include <pgm.h>
#include "pfm.h"

void Usage()
{
  fprintf(stderr,
	  "Usage:\n"
	  "  plm2pgm <infile> <outfile>\n");
  exit(0);
}

gray **IntToGray(int *pic, int cols, int rows)
{
  gray **p_gray;
  int val;
  int x, y;

  p_gray=pgm_allocarray(cols,rows);
  for (y=0; y<rows; y++)
    {
      for (x=0; x<cols; x++) {
	val=pic[y*cols+x];
	switch (val) {
	case -1:
	case 0:
	  p_gray[y][x]=0;
	  break;
	default:
	  p_gray[y][x]=(unsigned char)((val % 255)+1);
	  break;
	}
      }
    }
  return p_gray;
}

int main(int argc, char **argv)
{
  FILE *infile, *outfile;
  int cols, rows;
  float minval,maxval;
  int *pic;
  gray **p_gray;

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
  pgm_init(&argc,argv);
  pic=(int*)pfm_readpfm_type(infile,&cols,&rows,&minval,&maxval,PFM_SINT,0);
  printf("Size (%dx%d) values: %f - %f (read)\n",cols,rows,minval,maxval);
  fclose(infile);
  printf("minval=%f, maxval=%f\n",minval,maxval);
  p_gray=IntToGray(pic,cols,rows);
  pgm_writepgm(outfile,p_gray,cols,rows,255,0);
  fclose(outfile);
  return 1;
}
