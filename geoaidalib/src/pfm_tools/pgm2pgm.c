/*
 * $RCSfile: pgm2pgm.c,v $
 * $Source: /data/cvs/geoaidalib/geoaida/pfm_tools/pgm2pgm.c,v $
 * $Revision: 1.1 $
 * $Date: 2000/12/13 12:45:53 $
 * $Author: bueckner $
 * $Locker:  $
 */
#include <stdio.h>
#include <pgm.h>

void Usage() {
  fprintf(stderr,
	  "Triangle points to PFM\n"
	  "Usage:\n"
	  "  pgm2pgm <infile> <outfile> <xdim> <ydim>\n");
  exit(0);
}

void Resample(gray **pic1, gray **pic2, 
	      int cols, int rows, 
	      int ncols, int nrows)
{
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
	x2=x+1; y2=y+1;
	if ((x2>=cols) && (y2>=rows)) {
	    p11=pic1[y1][x1]; p12=0; 
	    p21=0;            p22=0; 
	  }
	else if (x2>=cols) {
	    p11=pic1[y1][x1]; p12=pic1[y2][x1];  
	    p21=0;                p22=0; 
	  }
	else if (y2>=rows) {
	    p11=pic1[y1][x1]; p12=0;  
	    p21=pic1[y1][x2]; p22=0; 
	  }
	else {
	    p11=pic1[y1][x1]; p12=pic1[y2][x1];  
	    p21=pic1[y1][x2]; p22=pic1[y2][x2];
	  }
	pic2[nr][nc]=(y2-y)*((x2-x)*p11+(x-x1)*p21)
	                 +(y-y1)*((x2-x)*p12+(x-x1)*p22);
#ifdef DEBUG
	printf("p1=(%4d,%4d) p2=(%4d,%4d) p=(%6.1f,%6.1f) "
	       "p11=%6.1f p21=%6.1f p12=%6.1f p22=%6.1f h(%3d,%3d)=%d\n",
	       x1,y1,x2,y2,x,y,p11,p21,p12,p22,nc,nr,pic2[nr][nc]);
#endif
      }
}


int main(int argc, char **argv)
{
  FILE *infile, *outfile;
  int cols, rows, ncols, nrows;
  gray **pic1, **pic2, maxval, nmaxval;

  pgm_init(&argc,argv);
  if ( argc != 5) Usage();

  sscanf(argv[3],"%d",&ncols);
  sscanf(argv[4],"%d",&nrows);
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
  pic1=pgm_readpgm(infile,&cols,&rows,&maxval);
  pic2=pgm_allocarray(ncols,nrows);
  printf("Resampling: pic1(%dx%d) -> pic2(%dx%d)\n",cols,rows,ncols,nrows);
  Resample(pic1,pic2,cols,rows,ncols,nrows);
  
  printf("Detecting new maxvals:\n  pic1(%d)\n",maxval);
#ifdef TMP
  nminval=1e6; nmaxval=-1e6;
  for (c=0; c<ncols*nrows; c++)  {
      if (pic2[c]<nminval) nminval=pic2[c];
      if (pic2[c]>nmaxval) nmaxval=pic2[c];
    }
  printf("  pic2(%f - %f)\n",nminval,nmaxval);
  scale=(maxval-minval)/(nmaxval-nminval);
  offset=nminval;
  nminval=1e6; nmaxval=-1e6;
  printf("Rescaling\n");
  for (c=0; c<ncols*nrows; c++) {
      pic2[c]=(pic2[c]-offset)*scale+minval;
      if (pic2[c]<nminval) nminval=pic2[c];
      if (pic2[c]>nmaxval) nmaxval=pic2[c];
    }
  printf("  pic2(%f - %f)\n",nminval,nmaxval);
#else
  nmaxval=maxval;
#endif

  pgm_writepgm(outfile,pic2,ncols,nrows,nmaxval,0);
  fclose(outfile);
  return 1;
}
