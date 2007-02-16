/*
 * $RCSfile: ppmgentiles.c,v $
 * $Source: /data/cvs/geoaidalib/geoaida/pfm_tools/ppmgentiles.c,v $
 * $Revision: 1.2 $
 * $Date: 2002/01/21 12:28:17 $
 * $Author: pahl $
 * $Locker:  $
 */
#include <stdio.h>
#include <ppm.h>

void Usage()
{
  fprintf(stderr,
          "Usage:\n"
          "  ppmgentiles <infile> <outfile-prefix> <tile-size> [offset] \n");
  exit(0);
}


int main(int argc, char **argv)
{
  FILE *infile, *outfile;
  int cols, rows;
  pixval maxval;
  pixel **pic;
  pixel **npic;
  int r, c;
  int x1, y1, x2, y2;
  int tsize, xcount, ycount, xdiff, ydiff;
  int x, y;
  char filename[255];

  if (argc < 4)
    Usage();

  infile = fopen(argv[1], "r");
  if (!infile) {
    fprintf(stderr, "Can't open file %s for reading!\n", argv[1]);
    exit(0);
  }
  sscanf(argv[3], "%d", &tsize);
  pic = ppm_readppm(infile, &cols, &rows, &maxval);
  xcount = ((cols - 1) / tsize) + 1;
  ycount = ((rows - 1) / tsize) + 1;
  if (argc>4) {
    sscanf(argv[4], "%d", &xdiff);
    ydiff=xdiff;
  }
  else {
    xdiff = (cols - (tsize * xcount)) / 2;
    ydiff = (rows - (tsize * ycount)) / 2;
  }
  fclose(infile);
  printf
    ("cols=%d, rows=%d, tsize=%d, xcount=%d, ycount=%d, xdiff=%d, ydiff=%d\n",
     cols, rows, tsize, xcount, ycount, xdiff, ydiff);
  npic = ppm_allocarray(tsize, tsize);
  for (x = 0; x < xcount; x++)
    for (y = 0; y < ycount; y++) {
      sprintf(filename, "%s.%02d.%02d.ppm", argv[2], x, y);
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
      /*$x1=x*tsize; x2=x1+tsize; if (x2>=cols) x2=cols-1;
         y1=y*tsize; y2=y1+tsize; if (y2>=rows) y2=rows-1;

         printf("region(%d,%d)=(%d,%d) (%d,%d)\n",
         x,y,x1,y1,x2,y2);$ */
      for (r = 0; r < tsize; r++)
        for (c = 0; c < tsize; c++)
          PPM_ASSIGN(npic[r][c], maxval, maxval, maxval);
      for (r = 0; r < y2 - y1+1; r++)
        for (c = 0; c < x2 - x1+1; c++)
          if ((y1 + r) >= 0 && (y1 + r) < rows && (x1 + c) >= 0
              && (x1 + c) < cols)
            npic[r][c] = pic[y1 + r][x1 + c];
      printf("Cutting done\n");
      ppm_writeppm(outfile, npic, tsize, tsize, maxval, 0);
      fclose(outfile);
    }
  return 1;
}
