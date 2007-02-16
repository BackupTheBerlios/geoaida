/*
 * $RCSfile: pgmcutregion.c,v $
 * $Source: /data/cvs/geoaidalib/geoaida/pfm_tools/pgmcutregion.c,v $
 * $Revision: 1.1 $
 * $Date: 2000/12/13 12:45:53 $
 * $Author: bueckner $
 * $Locker:  $
 */
#include <stdio.h>
#include <pgm.h>

void Usage() {
  fprintf(stderr,
	  "Usage:\n"
	  "  pgmcutregion <infile> <outfile> <x1> <y1> <x2> <y2>\n");
  exit(0);
}


int main(int argc, char **argv)
{
  FILE *infile, *outfile;
  int cols, rows;
  gray maxval;
  gray **pic;
  gray **npic;
  int x1,y1,x2,y2;
  int r,c;
  int ncols,nrows;
  
  if ( argc != 7) Usage();
  
  infile=fopen(argv[1],"r");
  if (!infile)  {
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
  pgm_init(&argc,argv);
  pic=pgm_readpgm(infile,&cols,&rows,&maxval);
  printf("cols=%d, rows=%d, region=(%d,%d) (%d,%d)\n",cols,rows,x1,y1,x2,y2);
  ncols=x2-x1+1;
  nrows=y2-y1+1;
  npic=pgm_allocarray(ncols,nrows);
  for (r=0; r<nrows; r++) 
    for (c=0; c<ncols; c++)
      npic[r][c]=pic[y1+r][x1+c];
  printf("Cutting done\n");
  pgm_writepgm(outfile,npic,ncols,nrows,maxval,0);
  fclose(infile);
  fclose(outfile);
  return 1;
}
