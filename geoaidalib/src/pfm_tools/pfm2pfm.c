/*
 * $RCSfile: pfm2pfm.c,v $
 * $Source: /data/cvs/geoaidalib/geoaida/pfm_tools/pfm2pfm.c,v $
 * $Revision: 1.6 $
 * $Date: 2004/07/09 11:12:42 $
 * $Author: mueller $
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

void Usage() {
  fprintf(stderr,
	  "Resamples a pfm file to the given dimension\n"
	  "Usage:\n"
	  "  pfmheader <infile> <outfile> <xdim> <ydim> [<min> <max>]\n\n"
    "  value between min and max are set to NaN\n");
  exit(0);
}

void Resample(float *pic1, float *pic2, 
	      int cols, int rows, 
	      int ncols, int nrows)
{
  int nan;
  int nc,nr;
  int x1,x2,y1,y2;
  double cfac, rfac;
  double p11,p12,p21,p22;
  double x,y;
  cfac=1.0*(cols-1)/(ncols-1);
  rfac=1.0*(rows-1)/(nrows-1);
  for (nr=0; nr<nrows; nr++)
    for (nc=0; nc<ncols; nc++)
      {
	x=nc*cfac; y=nr*rfac;
	x1=x; y1=y;
	x2=x1+1; y2=y1+1;
	if ((x2>=cols) && (y2>=rows)) {
	    p11=pic1[x1+y1*cols]; p12=0; 
	    p21=0;                p22=0; 
	  }
	else if (x2>=cols) {
	    p11=pic1[x1+y1*cols]; p12=pic1[x1+y2*cols];  
	    p21=0;                p22=0; 
	  }
	else if (y2>=rows) {
	    p11=pic1[x1+y1*cols]; p12=0;  
	    p21=pic1[x2+y1*cols]; p22=0; 
	  }
	else {
	    p11=pic1[x1+y1*cols]; p12=pic1[x1+y2*cols];  
	    p21=pic1[x2+y1*cols]; p22=pic1[x2+y2*cols];
	  }
	nan=0;
	if (isnan(p11)) nan++;
	if (isnan(p21)) nan++;
	if (isnan(p12)) nan++;
	if (isnan(p22)) nan++;
	if (nan>=2) {
	  pic2[nc+nr*ncols]=NAN;
	}
	else if (nan==0)
	  pic2[nc+nr*ncols]=(y2-y)*((x2-x)*p11+(x-x1)*p21)
	    +(y-y1)*((x2-x)*p12+(x-x1)*p22);
	else {
	  if (p11==NAN) p11=p22+(p21-p22)+(p12-p22);
	  if (p12==NAN) p12=p21+(p11-p21)+(p22-p21);
	  if (p21==NAN) p21=p12+(p22-p12)+(p11-p12);
	  if (p22==NAN) p22=p11+(p12-p11)+(p21-p11);
	  pic2[nc+nr*ncols]=(y2-y)*((x2-x)*p11+(x-x1)*p21)
	    +(y-y1)*((x2-x)*p12+(x-x1)*p22);
	}
#ifdef DEBUG
	printf("p1=(%4d,%4d) p2=(%4d,%4d) p=(%6.1f,%6.1f) "
	       "p11=%6.1f p21=%6.1f p12=%6.1f p22=%6.1f h(%3d,%3d)=%6.1f\n",
	       x1,y1,x2,y2,x,y,p11,p21,p12,p22,nc,nr,pic2[nc+nr*ncols]);
#endif
      }
}

int main(int argc, char **argv)
{
  FILE *infile, *outfile;
  int cols, rows, ncols, nrows;
  double gW,gN,gE,gS;
  float min,max;
  int minMaxSet;
  int c;
  float *pic1, *pic2, minval, maxval, nminval, nmaxval;
  double scale, offset;


  if ( argc < 5)
    {
      Usage();
    }
  sscanf(argv[3],"%d",&ncols);
  sscanf(argv[4],"%d",&nrows);
  minMaxSet=0;
  if (argc>6) {
    sscanf(argv[5],"%f",&min);
    sscanf(argv[6],"%f",&max);
    minMaxSet=1;
  }
  infile=fopen(argv[1],"r");
  if (!infile)
    {
      fprintf(stderr,"Can't open file %s for reading!\n",argv[1]);
      exit(0);
    }
  pic1=pfm_readpfm(infile,&cols,&rows,&minval,&maxval);
  if (pfm_geo_get(&gW,&gN,&gE,&gS))
    pfm_geo_set(gW,gN,gE,gS);
  minval=1/0.0; maxval=-1/0.0;
  if (minMaxSet)
    printf("Setting value %f - %f to NaN\n",min,max);
  printf("Finding min-max-vals:\n  pic1(%f - %f)\n",minval,maxval);
  for (c=0; c<cols*rows; c++)
    {
      if (minMaxSet & pic1[c]>=min && pic1[c]<=max) {
        pic1[c]=NAN;
        continue;
      }
      if (pic1[c]<minval) minval=pic1[c];
      if (pic1[c]>maxval) maxval=pic1[c];
    }
  if ((cols!=ncols) || (rows!=nrows)) 
    {
      printf("Resampling: pic1(%dx%d) -> pic2(%dx%d)\n",cols,rows,ncols,nrows);
      pic2=(float*)calloc(ncols*nrows,sizeof(float));
      Resample(pic1,pic2,cols,rows,ncols,nrows);
    }
  else
    pic2=pic1;
  printf("Detecting new min-maxvals:\n  pic1(%f - %f)\n",minval,maxval);
  nminval=1/0.0; nmaxval=-1/0.0;
  for (c=0; c<ncols*nrows; c++)
    {
      if (isnan(pic2[c])) {continue;}
      if (pic2[c]<nminval) nminval=pic2[c];
      if (pic2[c]>nmaxval) nmaxval=pic2[c];
    }
  printf("  pic2(%f - %f)\n",nminval,nmaxval);
  if (pic1!=pic2) 
    {
      scale=(maxval-minval)/(nmaxval-nminval);
      offset=nminval;
      nminval=1e6; nmaxval=-1e6;
      printf("Rescaling\n");
      for (c=0; c<ncols*nrows; c++)
	{
	  pic2[c]=(pic2[c]-offset)*scale+minval;
	  if (isnan(pic2[c])) continue;
	  if (pic2[c]<nminval) nminval=pic2[c];
	  if (pic2[c]>nmaxval) nmaxval=pic2[c];
	}
      printf("  pic2(%f - %f)\n",nminval,nmaxval);
    }

  /*  pfm_writepfm(outfile,pic2,ncols,nrows,nminval,nmaxval); */
  outfile=fopen(argv[2],"w");
  if (!outfile)
    {
      fprintf(stderr,"Can't open file %s for writing!\n",argv[2]);
      exit(0);
    }
  pfm_writepfm(outfile,pic2,ncols,nrows,1,-1);
  fclose(outfile);
  return 1;
}
