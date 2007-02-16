/*
 * $RCSfile: ppmcutregion.c,v $
 * $Source: /data/cvs/geoaidalib/geoaida/pfm_tools/ppmcutregion.c,v $
 * $Revision: 1.1 $
 * $Date: 2000/12/13 12:45:53 $
 * $Author: bueckner $
 * $Locker:  $
 */
#include <stdio.h>
#include <ppm.h>

void Usage() {
  fprintf(stderr,
	  "Usage:\n"
	  "  ppmcutregion <infile> <outfile> <x1> <y1> <x2> <y2>\n");
  exit(0);
}


int main(int argc, char **argv)
{
  FILE *infile, *outfile;
  int cols, rows;
  pixval maxval;
  pixel **pic;
  int ncols, nrows;
  pixel **npic;
  int r,c;
  int x1,y1,x2,y2;

  if ( argc != 7) Usage();

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
  sscanf(argv[3],"%d",&x1);
  sscanf(argv[4],"%d",&y1);
  sscanf(argv[5],"%d",&x2);
  sscanf(argv[6],"%d",&y2);
  pic=ppm_readppm(infile,&cols,&rows,&maxval);
  printf("cols=%d, rows=%d, region=(%d,%d) (%d,%d)\n",cols,rows,x1,y1,x2,y2);
  ncols=x2-x1+1;
  nrows=y2-y1+1;
  npic=ppm_allocarray(ncols,nrows);
  for (r=0; r<nrows; r++) 
    for (c=0; c<ncols; c++)
      npic[r][c]=pic[y1+r][x1+c];
  printf("Cutting done\n");
  ppm_writeppm(outfile,npic,ncols,nrows,maxval,0);
  fclose(infile);
  fclose(outfile);
  return 1;
}
