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

#include "PFMSqrDiff.h"
#include <math.h>

PFMSqrDiff::PFMSqrDiff(PFMArray &pic1, PFMArray &pic2) 
  : PFMArray(pic1.xsize(), pic1.ysize())
{
  int i;

  for (i=0; i<xsize_*ysize_; i++) {
    if (isnan(pic1.getPixel(i))) {
      data_[i]=pic2.getPixel(i);
      continue;
    }
    if (isnan(pic2.getPixel(i))) {
      data_[i]=pic1.getPixel(i);
      continue;
    }
    data_[i]=sqrt(pic1.getPixel(i)*pic1.getPixel(i)
		 +pic2.getPixel(i)*pic2.getPixel(i));
  }
}
