/*
 * $RCSfile: pgm2pfm.c,v $
 * $Source: /data/cvs/geoaidalib/geoaida/pfm_tools/pgm2pfm.c,v $
 * $Revision: 1.2 $
 * $Date: 2001/03/05 14:16:30 $
 * $Author: bueckner $
 * $Locker:  $
 */
#include <stdio.h>
#include <pgm.h>
#include "pfm.h"

void Usage()
{
  fprintf(stderr,
	  "Usage:\n"
	  "  pgmtopfm <infile> <outfile>\n");
  exit(0);
}

float *GrayToFloat(gray **pic, int cols, int rows)
{
  float *p_float;
  int x, y;

  p_float=calloc(cols*rows,sizeof(float));
  for (y=0; y<rows; y++)
    {
      for (x=0; x<cols; x++)
	p_float[y*cols+x]=pic[y][x];
    }
  return p_float;
}

void WritePFM(FILE *fp, float *pic,
	 int maxx, int maxy, 
	 int x1, int y1, int x2, int y2, 
	 float min, float max)
{
  int y;

  fprintf(fp,"F4\n");
#ifdef WORDS_BIGENDIAN
  fprintf(fp,"B\n");
#else
  fprintf(fp,"L\n");
#endif
  fprintf(fp,"%d\t%d\n",x2-x1+1,y2-y1+1);
  fprintf(fp,"%f\t%f\n",min,max);
  for (y=y1; y<=y2; y++)
    {
      fwrite(&pic[y*maxx],sizeof(float),x2-x1+1,fp);
    }
  /*  
      for (x=x1; x<=x2; x++)
      for (y=y1; y<=y2; y++)
      {
      fwrite(&pic[y*maxx+x],sizeof(float),1,fp);
      }
  */
}

int main(int argc, char **argv)
{
  FILE *infile, *outfile;
  int cols, rows;
  gray maxval;
  gray **pic;
  float *p_float;

  if ( argc != 3) Usage();

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
  pic=pgm_readpgm(infile,&cols,&rows,&maxval);
  fclose(infile);
  p_float=GrayToFloat(pic,cols,rows);
  pfm_writepfm(outfile,p_float,cols,rows,0,maxval);
  fclose(outfile);
  return 1;
}
