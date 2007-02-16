#include "Array2DAbs.h"
#include <typeinfo>
#include "pfm.h"
#include <vector>
#include <algorithm>

template < class Value >
  Array2DAbs < Value >::Array2DAbs(const Array2D < Value > &src)
:Array2D < Value > (src.xsize(), src.ysize())
{
  int x, y;
  for (y = 0; y < ysize(); y++) {
#if 0
#ifdef DEBUG
    printf("(%d,%d) %f \n", 0, y, src.getPixel(0, y));
#endif
#endif
    for (x = 0; x < xsize(); x++) {
      set(x, y, fabs(src.get(x, y)));
    }
  }
}
