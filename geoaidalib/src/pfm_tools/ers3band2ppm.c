/***************************************************************************
                            -  description
                             -------------------
    begin                : Tue Dec 12 2000
    copyright            : (C) 2000 by jürgen bückner
    email                : bueckner@tnt.uni-hannover.de
 ***************************************************************************/
/*
 * $RCSfile: ers3band2ppm.c,v $
 * $Source: /data/cvs/geoaidalib/geoaida/pfm_tools/ers3band2ppm.c,v $
 * $Revision: 1.2 $
 * $Date: 2001/03/05 14:16:30 $
 * $Author: bueckner $
 * $Locker:  $
 */
#include <stdio.h>
#include <ppm.h>

void Usage()
{
  fprintf(stderr,
	  "Usage:\n"
	  "  ers3band2ppm <infile> <outfile> <xdim> <ydim>\n",
	  "    convert ers 3band image (ERMapper) to PPM\n\n");
  exit(0);
}

pixel **read3band(FILE *infile, int *xdim, int *ydim, pixval *maxval)
{
  int x, y;
  pixel **pic;
  unsigned char *pline;
  pline=(unsigned char*)calloc(*xdim,sizeof(char));
  pic=ppm_allocarray(*xdim,*ydim);
  *maxval=255;
  printf("Reading RED/GREEN/BLUE\n");
  for (y=0; y<*ydim; y++)
    {
      fread(pline,sizeof(char),*xdim,infile);
      for (x=0; x<*xdim; x++)
	PPM_PUTR(pic[y][x],(long)pline[x]);
      fread(pline,sizeof(char),*xdim,infile);
      for (x=0; x<*xdim; x++)
	PPM_PUTG(pic[y][x],(long)pline[x]);
      fread(pline,sizeof(char),*xdim,infile);
      for (x=0; x<*xdim; x++)
	PPM_PUTB(pic[y][x],(long)pline[x]);
    }
  free(pline);
  return pic;
}

int main(int argc, char **argv)
{
  FILE *infile, *outfile;
  int cols, rows;
  pixval maxval;
  pixel **pic;

  if ( argc != 5)
    {
      Usage();
    }
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
  sscanf(argv[3],"%d",&cols);
  sscanf(argv[4],"%d",&rows);
  printf("cols=%d, rows=%d\n",cols,rows);
  pic=read3band(infile,&cols,&rows,&maxval);
  ppm_writeppm(outfile,pic,cols,rows,maxval,0);
  fclose(infile);
  fclose(outfile);
  return 1;
}
