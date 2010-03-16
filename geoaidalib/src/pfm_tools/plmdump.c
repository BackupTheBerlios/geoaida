/*
 * $RCSfile: plmdump.c,v $
 * $Source: /data/cvs/geoaidalib/geoaida/pfm_tools/plmdump.c,v $
 * $Revision: 1.2 $
 * $Date: 2001/02/13 12:27:23 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifdef linux
int __isnanf(float);
#ifndef NAN
#define NAN sqrt(-1)
#endif
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "pfm.h"

void Usage()
{
  fprintf(stderr,
	  "Usage:\n"
	  "  plmdump [-d 32|16] [-i] <infile> <outfile>\n\n"
	  "Description:\n"
	  "  dumps a label image to raw int32 data\n"
	  "  -d [32|16]: depth of dumped data\n"
	  "  -i: print info for image magick convert\n");
  exit(1);
}

int main(int argc, char **argv)
{
  FILE *infile, *outfile, *info;
  int cols, rows;
  float minval,maxval;
  int *pic;
  int opt;
  int depth=PFM_SINT;
  char infomode=0;
  int type;
  while ((opt=getopt(argc,argv,"id:")) != -1) {
    switch (opt) {
    case 'i':
      infomode=1;
      break;
    case 'd':
      depth=atoi(optarg);
      switch (depth) {
      case 16: 
	depth=PFM_UINT16;
	break;
      case 32:
	depth=PFM_UINT;
	break;
      default:
	Usage();
	break;
      }
    }
  }
  if ( optind>=argc)  Usage();

  infile=fopen(argv[optind],"r");
  if (!infile) {
      fprintf(stderr,"Can't open file %s for reading!\n",argv[optind]);
      exit(0);
    }
  if (infomode) {
    pfm_readpfm_header(infile,&cols,&rows,&minval,&maxval,&type);
    printf("-size %dx%d -depth %d",
	   cols,rows,(depth==PFM_SINT ? 32 : 16));
    fclose(infile);
    return 0;
  }
  else {
    if ( optind>=argc)  Usage();
    printf("%s\n",argv[optind+1]);
    if (strcmp(argv[optind+1],"-")==0) {
      printf("to stdout\n");
      outfile=stdout;
    }
    else
      outfile=fopen(argv[optind+1],"w");
    if (!outfile) {
      fprintf(stderr,"Can't open file %s for writing!\n",argv[optind+1]);
      exit(0);
    }
    info=stderr;
    pic=(int*)pfm_readpfm_type(infile,&cols,&rows,&minval,&maxval,depth,0);
    fprintf(info,"Size (%dx%d) values: %f - %f (read)\n",cols,rows,minval,maxval);
    fclose(infile);
    fprintf(info,"minval=%f, maxval=%f\n",minval,maxval);
    fwrite(pic,(depth==PFM_SINT ? 4 : 2),cols*rows,outfile);
    fclose(outfile);
    return 0;
  }
}
