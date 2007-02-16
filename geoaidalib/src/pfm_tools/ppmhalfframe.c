/*
 * $RCSfile: ppmhalfframe.c,v $
 * $Source: /data/cvs/geoaidalib/geoaida/pfm_tools/ppmhalfframe.c,v $
 * $Revision: 1.1 $
 * $Date: 2000/12/13 12:45:53 $
 * $Author: bueckner $
 * $Locker:  $
 */
#include <stdio.h>
#include <ppm.h>

void Usage()
{
  fprintf(stderr,
	  "Usage:\n"
	  "  ppmhalfframe <infile> <outfile> <0|1>\n"
          "            0 - even\n"
          "            1 - odd\n");
  exit(0);
}


int main(int argc, char **argv)
{
  FILE *infile, *outfile;
  int odd;
  int cols, rows;
  pixval maxval;
  pixel **pic;
  int r;
  int x1,y1,x2,y2;

  if ( argc != 4) Usage();

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
  sscanf(argv[3],"%d",&odd);
  pic=ppm_readppm(infile,&cols,&rows,&maxval);
  printf("cols=%d, rows=%d, region=(%d,%d) (%d,%d)\n",cols,rows,x1,y1,x2,y2);
  if (odd)
    for (r=1; r<rows; r+=2)
      pic[r-1]=pic[r];
  else
    for (r=1; r<rows; r+=2)
      pic[r]=pic[r-1];
  printf("Converting done\n");
  ppm_writeppm(outfile,pic,cols,rows,maxval,0);
  fclose(infile);
  fclose(outfile);
  return 1;
}
