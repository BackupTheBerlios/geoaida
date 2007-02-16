/*
 * $RCSfile: pbmblow.c,v $
 * $Source: /data/cvs/geoaidalib/geoaida/pfm_tools/pbmblow.c,v $
 * $Revision: 1.2 $
 * $Date: 2001/03/05 14:16:30 $
 * $Author: bueckner $
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
	  "  pbmblow <infile> <outfile> [[fg-color] no-loops]\n",
	  "   make 'no-loops' * blow of the 'fg-color' in the 'infile'\n",
	  "   and write the result to 'outfile'.\n"
	  "   for PFM and PBM format\n\n");
  exit(0);
}

#define checkBounds(x,y,xmin,ymin,xmax,ymax) \
  (((x)>=(xmin)) && ((y)>=(ymin)) && ((x)<=(xmax)) && ((y)<=(ymax)))

bit **BlowBit(bit **pic, int cols, int rows, int fg_val)
{
  bit **p_bit;
  int val;
  int x, y;

  p_bit=pbm_allocarray(cols,rows);
  for (y=0; y<rows; y++)
    {
      for (x=0; x<cols; x++) {
				val=pic[y][x];
				if (checkBounds(x-1,y  ,0,0,cols-1,rows-1) && (pic[y  ][x-1]==fg_val)) val=fg_val;
				if (checkBounds(x+1,y  ,0,0,cols-1,rows-1) && (pic[y  ][x+1]==fg_val)) val=fg_val;
				if (checkBounds(x  ,y-1,0,0,cols-1,rows-1) && (pic[y-1][x  ]==fg_val)) val=fg_val;
				if (checkBounds(x  ,y+1,0,0,cols-1,rows-1) && (pic[y+1][x  ]==fg_val)) val=fg_val;
				p_bit[y][x]=val;
      }
    }
  pbm_freearray(pic,rows);
  return p_bit;

}

int main(int argc, char **argv)
{
  FILE *infile, *outfile;
  int cols, rows;
  bit **p_bit;
  int fg_color, loops;

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
  if (argc > 3)
	  sscanf(argv[3],"%d",&fg_color);
	else fg_color=1;
	if (argc > 4)
		sscanf(argv[4],"%d",&loops);
	else
	  loops=1;
  pbm_init(&argc,argv);
  p_bit=pbm_readpbm(infile,&cols,&rows);
  fclose(infile);
  while (loops) {
  	p_bit=BlowBit(p_bit,cols,rows,fg_color);
  	loops--;
  }
  pbm_writepbm(outfile,p_bit,cols,rows,0);
  fclose(outfile);
  return 1;
}
