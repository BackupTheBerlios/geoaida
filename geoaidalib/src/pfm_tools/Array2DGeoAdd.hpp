#include "Array2DGeoAdd.h"
#include <typeinfo>
#include "pfm.h"

template < class Value >
  Array2DGeoAdd < Value >::Array2DGeoAdd(const Array2D < Value > &src1,
                                         const Array2D < Value > &src2)
  :
  Array2D <
Value > (src1.xsize(), src1.ysize())
{
  if ((src1.xsize() != src2.xsize()) || (src1.ysize() != src2.ysize())) {
    fprintf(stderr,
            "Array2DGeoAdd: dimension of src1 and src2 must be the same!\n");
    return;
  }

  int x, y;
  for (y = 0; y < Array2D<Value>::ysize(); y++) {

    for (x = 0; x < Array2D<Value>::xsize(); x++) {
      set(x, y, hypot(src1.get(x, y), src2.get(x, y)));
    }
  }
}
