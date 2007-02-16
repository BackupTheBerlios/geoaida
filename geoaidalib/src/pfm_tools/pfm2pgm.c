/*
 * $RCSfile: pfm2pgm.c,v $
 * $Source: /data/cvs/geoaidalib/geoaida/pfm_tools/pfm2pgm.c,v $
 * $Revision: 1.4 $
 * $Date: 2003/03/18 12:38:44 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifdef linux
int __isnanf(float);
#ifndef NAN
#define NAN sqrt(-1)
#endif
#endif

#include <stdio.h>
#include <pgm.h>
#include "pfm.h"


void Usage()
{
  fprintf(stderr,
          "Usage:\n"
          "  pfm2pgm [-m] <infile> <outfile> [<min> <max>]\n"
          "    -m: modulo\n"
          "  convert PFM to PGM, optional: use range min-max for scaling (0,255 uses absolute values)\n\n");
  exit(0);
}

gray **FloatToGray(float *pic, int cols, int rows, float minval, float maxval,
                   int modulo)
{
  gray **p_gray;
  float factor;
  int x, y;

  factor = maxval - minval;
  p_gray = pgm_allocarray(cols, rows);
  if (modulo) {
    for (y = 0; y < rows; y++) {
      for (x = 0; x < cols; x++) {
        if (isnan(pic[y * cols + x]))
          p_gray[y][x] = 0;
        else
          p_gray[y][x] = (int)((pic[y * cols + x] - minval) / factor * 254 ) % 255 + 1;
      }
    }
  }
  else {
    for (y = 0; y < rows; y++) {
      for (x = 0; x < cols; x++) {
        if (isnan(pic[y * cols + x]))
          p_gray[y][x] = 0;
        else if (pic[y * cols + x] < minval)
          p_gray[y][x] = 1;
        else if (pic[y * cols + x] > maxval)
          p_gray[y][x] = 255;
        else
          p_gray[y][x] = (pic[y * cols + x] - minval) / factor * 254 + 1;
      }
    }
  }
  return p_gray;
}

int main(int argc, char **argv)
{
  FILE *infile, *outfile;
  int cols, rows;
  int c;
  float minval, maxval;
  float *pic;
  gray **p_gray;
  int modulo = 0;

  if (argc < 3)
    Usage();

  if (strcmp(argv[1], "-m") == 0) {
    modulo = 1;
    optind++;
  }

  infile = fopen(argv[optind++], "r");
  if (!infile) {
    fprintf(stderr, "Can't open file %s for reading!\n", argv[1]);
    exit(0);
  }
  outfile = fopen(argv[optind++], "w");
  if (!outfile) {
    fprintf(stderr, "Can't open file %s for writing!\n", argv[2]);
    exit(0);
  }
  pgm_init(&argc, argv);
  pic = pfm_readpfm(infile, &cols, &rows, &minval, &maxval);
  printf("Size (%dx%d) values: %f - %f (read)\n", cols, rows, minval, maxval);
  minval = 1e38;
  maxval = -1e38;
  for (c = 0; c < cols * rows; c++) {
    if (isnan(pic[c]))
      continue;
    if (pic[c] < minval)
      minval = pic[c];
    if (pic[c] > maxval)
      maxval = pic[c];
  }
  printf("Size (%dx%d) values: %f - %f (calculated)\n",
         cols, rows, minval, maxval);
  if (argc > optind) {
    sscanf(argv[optind++], "%f", &minval);
  }
  if (argc > optind) {
    sscanf(argv[optind++], "%f", &maxval);
  }
  fclose(infile);
  printf("minval=%f, maxval=%f\n", minval, maxval);
  p_gray = FloatToGray(pic, cols, rows, minval, maxval, modulo);
  pgm_writepgm(outfile, p_gray, cols, rows, 255, 0);
  fclose(outfile);
  return 1;
}
