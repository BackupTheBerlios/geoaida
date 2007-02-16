#if __GNUC__==2
#ifdef linux
extern "C" {
int __isnanf(float);
	   }
#ifndef NAN
#define NAN sqrt(-1)
#endif
#endif
#endif

#include "PFMGradAbs.h"
#include <math.h>

#define sqr(x) ((x)*(x))

PFMGradAbs::PFMGradAbs(PFMArray &pic) 
  : PFMArray(pic.xsize(), pic.ysize())
{
  int x, y;

  for (x=0; x<xsize_-1; x++)
    for (y=0; y<ysize_-1; y++) {
      if (isnan(pic.getPixel(x,y)) 
	  || isnan(pic.getPixel(x+1,y))
	  || isnan(pic.getPixel(x,y+1)) 
	  || isnan(pic.getPixel(x+1,y+1))) {
	setPixel(x,y,NAN);
	continue;
      }
      setPixel(x,y,hypot(pic.getPixel(x+1,y+1)-pic.getPixel(x,y),
			  pic.getPixel(x+1,y)-pic.getPixel(x,y+1)));
    }
  for (x=0; x<xsize_; x++) setPixel(x,ysize_-1,NAN);
  for (y=0; y<ysize_; y++) setPixel(xsize_-1,y,NAN);
}
		

