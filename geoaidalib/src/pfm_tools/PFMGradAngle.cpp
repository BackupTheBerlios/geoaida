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

#include "PFMGradAngle.h"
#include <math.h>

#define sqr(x) ((x)*(x))

PFMGradAngle::PFMGradAngle(PFMArray &pic) 
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
      double dy=pic.getPixel(x+1,y+1)-pic.getPixel(x,y);
      double dx=pic.getPixel(x+1,y)-pic.getPixel(x,y+1);
      double angle=atan2(dy,dx);
      angle-=M_PI_4;
      if (angle<-M_PI) angle+=2*M_PI;
      setPixel(x,y,angle);
    }
}
		

