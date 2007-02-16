#include "Array2DMinPic.h"
#include <typeinfo>
#include "pfm.h"

template < class Value >
  Array2DMinPic < Value >::Array2DMinPic(const Array2D < Value > &src1,
                                         const Array2D < Value > &src2)
  :
  Array2D <
Value > (src1.xsize(), src1.ysize())
{
  if ((src1.xsize() != src2.xsize()) || (src1.ysize() != src2.ysize())) {
    fprintf(stderr,
            "Array2DMinPic: dimension of src1 and src2 must be the same!\n");
    return;
  }

  int x, y;
  for (y = 0; y < Array2D<Value>::ysize(); y++) {

    for (x = 0; x < Array2D<Value>::xsize(); x++) {
      Value p1 = src1.get(x, y);
      Value p2 = src2.get(x, y);
      set(x, y, (p1 < p2 ? p1 : p2));
    }
  }
}
