#ifndef Array2DFilter_h
#define Array2DFilter_h

#include "Array2D.h"

template < class Value > class Array2DFilter:public Array2D < Value > {
public:
  Array2DFilter(const Array2D < Value > &src,
                const Array2D < Value > &matrix);
private:
  void CalcPoint(int xd, int yd,
                 const Array2D < Value > &src,
                 const Array2D < Value > &matrix);
  bool getSrcVal(int x, int y, Value & val, const Array2D < Value > &src);
};

#include "Array2DFilter.hpp"
#endif
