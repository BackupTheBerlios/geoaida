/*
 * $RCSfile: points2pfm.c,v $
 * $Source: /data/cvs/geoaidalib/geoaida/pfm_tools/points2pfm.c,v $
 * $Revision: 1.1 $
 * $Date: 2000/12/13 12:45:53 $
 * $Author: bueckner $
 * $Locker:  $
 */
#include <stdio.h>
#include <stdlib.h>
#include "pfm.h"

void Usage()
{
  fprintf(stderr,
	  "Triangle points to PFM\n"
	  "Usage:\n"
	  "  tritopfm <infile> <outfile> <xfactor> <yfactor>  <heightoffset> <heigtfactor>\n");
  exit(0);
}



int main(int argc, char **argv)
{
  FILE *infile, *outfile;
  int cols, rows;
  int cfactor, rfactor;
  int coffset,roffset;
  int cmin, cmax, rmin, rmax;
  float hfactor,hoffset;
  float h;
  int c,r;
  float *p_float, minval, maxval;

  if ( argc != 7)
    {
      Usage();
    }
  sscanf(argv[3],"%d",&cfactor);
  sscanf(argv[4],"%d",&rfactor);
  sscanf(argv[5],"%f",&hoffset);
  sscanf(argv[6],"%f",&hfactor);
  infile=fopen(argv[1],"r");
  if (!infile)
    {
      fprintf(stderr,"Can't open file %s for reading!\n",argv[1]);
      exit(0);
    }
  outfile=fopen(argv[2],"w");
  if (!outfile)
    {
      fprintf(stderr,"Can't open file %s for writing!\n",argv[2]);
      exit(0);
    }

  cmin=1e6; cmax=-1e6; rmin=1e6, rmax=-1e6;
  while (!feof(infile))
    {
      if (fscanf(infile,"%d %d %f\n",&c,&r,&h)==3)
	{
	  c=c/cfactor;
	  r=r/rfactor;
	  if (c>cmax) cmax=c;
	  if (c<cmin) cmin=c;
	  if (r>rmax) rmax=r;
	  if (r<rmin) rmin=r;
	}
     }
  fclose(infile);
  coffset=-cmin;
  roffset=-rmin;
  cols=cmax+coffset+1;
  rows=rmax+roffset+1;
  fprintf(stderr,
	  "min=(%d,%d), max=(%d,%d), offset=(%d,%d), dim=(%d,%d) hoffs=%f hfac=%f %s\n",
	 cmin,rmin,cmax,rmax,coffset,roffset,cols,rows,hoffset,hfactor,argv[6]);
  infile=fopen(argv[1],"r");
  p_float=(float*)calloc(cols*rows,sizeof(float));
  while (!feof(infile))
    {
      if (fscanf(infile,"%d %d %f\n",&c,&r,&h)==3)
	{
#ifdef DEBUG
	  printf("%4d %4d %6.0f (%3d %3d %6.0f)\n",
		 c,r,h,c/cfactor+coffset,
		 r/rfactor+roffset,
		 (h+hoffset)/hfactor);
#endif
	  c=c/cfactor+coffset;
	  r=r/rfactor+roffset;
	  if ((c>=cols) || (r>=rows) || (c<0) || (r<0))
	    {
	      fprintf(stderr,"(%d,%d) out of range (%d,%d)\n",c,r,rows,cols);
	    }
	  else 
	    p_float[c+r*cols]=(h+hoffset)/hfactor;
	}
     }
  fclose(infile);
  maxval=-1e6; minval=1e6;
  for (c=0; c<rows*cols; c++)
    {
      if (p_float[c]>maxval)
	maxval=p_float[c];
      if (p_float[c]<minval)
	minval=p_float[c];
    }
#ifdef DEBUG
  for (r=0; r<rows; r++)
    {
      printf("%3d: ",r);
      for (c=0; c<cols; c++)
	printf("%6.0f ",p_float[c+r*cols]);
      printf("\n");
    }
#endif
  pfm_writepfm(outfile,p_float,cols,rows,minval,maxval);
  fclose(outfile);
  return 1;
}
