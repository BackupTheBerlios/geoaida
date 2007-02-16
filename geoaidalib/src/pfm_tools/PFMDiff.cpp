#ifdef linux
extern "C" {
int __isnanf(float);
	   }
#ifndef NAN
#define NAN sqrt(-1)
#endif
#endif

#include "PFMDiff.h"
#include <math.h>

PFMDiff::PFMDiff(PFMArray &pic, int direction) 
  : PFMArray(pic.xsize(), pic.ysize())
{
  int x, y;

  switch (direction) {
  case X_DIR:
    for (x=0; x<xsize_-1; x++)
      for (y=0; y<ysize_; y++)
	if ((isnan(pic.getPixel(x,y))) || (isnan(pic.getPixel(x+1,y))))
	  setPixel(x,y,NAN);
	else
	  setPixel(x,y,pic.getPixel(x+1,y)-pic.getPixel(x,y));
    break;
  case Y_DIR:
    for (x=0; x<xsize_; x++)
      for (y=0; y<ysize_-1; y++)
	if ((isnan(pic.getPixel(x,y))) || (isnan(pic.getPixel(x,y+1))))
	  setPixel(x,y,NAN);
	else
	  setPixel(x,y,pic.getPixel(x,y+1)-pic.getPixel(x,y));
    break;
  }
}

