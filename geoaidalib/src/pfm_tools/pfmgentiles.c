/*
 * $RCSfile: pfmgentiles.c,v $
 * $Source: /data/cvs/geoaidalib/geoaida/pfm_tools/pfmgentiles.c,v $
 * $Revision: 1.2 $
 * $Date: 2002/01/21 12:28:17 $
 * $Author: pahl $
 * $Locker:  $
 */
#include <stdio.h>
#include <stdlib.h>
#include "pfm.h"

void Usage()
{
  fprintf(stderr,
          "Usage:\n"
          "  pfmgentiles <infile> <outfile-prefix> <tile-size> \n");
  exit(0);
}


int main(int argc, char **argv)
{
  FILE *infile, *outfile;
  int cols, rows;
  float minval, maxval;
  float *pic;
  int x1, y1, x2, y2;
  int tsize, xcount, ycount, xdiff, ydiff;
  int x, y;
  char filename[255];

  if (argc != 4)
    Usage();

  infile = fopen(argv[1], "r");
  if (!infile) {
    fprintf(stderr, "Can't open file %s for reading!\n", argv[1]);
    exit(0);
  }
  sscanf(argv[3], "%d", &tsize);
  pic = pfm_readpfm(infile, &cols, &rows, &minval, &maxval);
  xcount = ((cols - 1) / tsize) + 1;
  ycount = ((rows - 1) / tsize) + 1;
  xdiff = (cols - (tsize * xcount)) / 2;
  ydiff = (rows - (tsize * ycount)) / 2;
  fclose(infile);
  printf
    ("cols=%d, rows=%d, tsize=%d, xcount=%d, ycount=%d, xdiff=%d, ydiff=%d\n",
     cols, rows, tsize, xcount, ycount, xdiff, ydiff);
  for (x = 0; x < xcount; x++)
    for (y = 0; y < ycount; y++) {
      sprintf(filename, "%s.%02d.%02d.pfm", argv[2], x, y);
      outfile = fopen(filename, "w");
      if (!outfile) {
        fprintf(stderr, "Can't open file %s for writing!\n", filename);
        exit(0);
      }
      x1 = x * tsize + xdiff;
      x2 = x1 + tsize - 1;
      y1 = y * tsize + ydiff;
      y2 = y1 + tsize - 1;
      printf("region(%d,%d)=(%d,%d) (%d,%d)\n", x, y, x1, y1, x2, y2);
      pfm_writepfm_region(outfile, pic, cols, rows, 0, maxval, x1, y1, x2,
                          y2);
      fclose(outfile);
    }
  return 1;
}
