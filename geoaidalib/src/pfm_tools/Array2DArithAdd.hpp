#include "Array2DArithAdd.h"
#include <typeinfo>
#include "pfm.h"

template < class Value >
  Array2DArithAdd < Value >::Array2DArithAdd(const Array2D < Value > &src1,
                                             const Array2D < Value > &src2)
  :
  Array2D <
Value > (src1.xsize(), src1.ysize())
{
  if ((src1.xsize() != src2.xsize()) || (src1.ysize() != src2.ysize())) {
    fprintf(stderr,
            "Array2DArithAdd: dimension of src1 and src2 must be the same!\n");
    return;
  }

  int x, y;
  for (y = 0; y < ysize(); y++) {

    for (x = 0; x < xsize(); x++) {
      set(x, y, src1.get(x, y) + src2.get(x, y));
    }
  }
}
