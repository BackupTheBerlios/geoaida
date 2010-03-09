/*
 * $RCSfile: pfmheader.c,v $
 * $Source: /data/cvs/geoaidalib/geoaida/pfm_tools/pfmheader.c,v $
 * $Revision: 1.2 $
 * $Date: 2002/07/02 06:07:26 $
 * $Author: pahl $
 * $Locker:  $
 */
#ifdef linux
int __isnanf(float);
#ifndef NAN
#define NAN sqrt(-1)
#endif
#endif


#include "unistd.h"
#include <stdio.h>
#include <stdlib.h>
#include "pfm.h"

void Usage()
{
  fprintf(stderr,
	  "Prints the header information of a pfm-file\n"
	  "Usage:\n"
	  "  pfmheader <infile1> [<infile2> ...]\n"
	  " -t: info in tfw-format\n");
  exit(1);
}


int main(int argc, char **argv)
{
  FILE *infile;
  int cols, rows;
  int type;
  float minval, maxval;
  double gW,gN,gE,gS;
  int i;
  char tfwmode=0;
  int opt;
  while ((opt=getopt(argc,argv,"t")) != -1) {
    switch (opt) {
    case 't':
      tfwmode=1;
      break;
    }
  }
  
  if ( optind>=argc)  Usage();

  for (i=optind; i<argc; i++) {
    infile=fopen(argv[i],"r");
    if (!infile) {
      fprintf(stderr,"Can't open file %s for reading!\n",argv[i]);
      exit(0);
    }
    if (pfm_readpfm_header(infile,&cols,&rows,&minval,&maxval,&type)) {
      if (tfwmode) {
	if (pfm_geo_get(&gW,&gN,&gE,&gS)) {
	  printf("%f\n%f\n%f\n%f\n%f\n%f\n",
		 (gE-gW)/cols,0.0,0.0,(gS-gN)/rows,
		 gW,gN);
	  return 0;
	}
	else {
	  fprintf(stderr,"no geo header\n");
	  return 1;
	}
      }
      else {
	if (argc>2) printf("==> %s <==\n",argv[i]);
	printf("cols=%d rows=%d minval=%f maxval=%f type=%d\n",
	       cols,rows,minval,maxval,type);
	if (pfm_geo_get(&gW,&gN,&gE,&gS)) {
	  printf("geo: %f %f %f %f\n",gW,gN,gE,gS);
	  printf("geo: size (%f x %f) pixsize: (%f x %f)\n\n",
		 gE-gW,gN-gS,(gE-gW)/cols,(gN-gS)/rows);
	}
      }
    }
    else printf("Not a pfmfile\n\n");
    fclose(infile);
  }
  return 0;
}
