/*
 * $RCSfile: pxminfo.c,v $
 * $Source: /data/cvs/geoaidalib/geoaida/pfm_tools/pxminfo.c,v $
 * $Revision: 1.1 $
 * $Date: 2000/12/13 12:45:53 $
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
#include "pfm.h"
#include <pnm.h>


void Usage()
{
  fprintf(stderr,
	  "Prints the header information of a pfm or pnm file\n"
	  "Usage:\n"
	  "  pxminfo <infile>\n");
  exit(0);
}


int main(int argc, char **argv)
{
  FILE *infile;
  int cols, rows;
  int type;
  float minval, maxval;
  xelval maxvalP;

  if ( argc != 2) Usage();

  infile=fopen(argv[1],"r");
  if (!infile) {
    fprintf(stderr,"Can't open file %s for reading!\n",argv[1]);
    exit(0);
  }
  if (pfm_readpfm_header(infile,&cols,&rows,&minval,&maxval,&type))
    printf("cols=%d rows=%d minval=%f maxval=%f datatype=%d filetyp=pfm\n",
	   cols,rows,minval,maxval,type);
  else {
    /*void pnm_readpnminit( FILE* fp, int* colsP, int* rowsP, xelval* maxvalP, int* formatP)*/
    pnm_readpnminit( infile,&cols,&rows,&maxvalP,&type);
    printf("cols=%d rows=%d maxval=%d filetype=%d\n",
	   cols,rows,maxvalP,type);
  }
  /*else printf("Not a pfm or pnm file\n");*/
  fclose(infile);
  return 1;
}
