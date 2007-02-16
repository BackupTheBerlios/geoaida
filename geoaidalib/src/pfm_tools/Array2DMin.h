#ifndef Array2DMin_h
#define Array2DMin_h

#include "Array2D.h"

template < class Value > class Array2DMin:public Array2D < Value > {
public:
  Array2DMin(const Array2D < Value > &src, int fsizex, int fsizey);
private:
  void CalcPoint(int xd, int yd,
                 const Array2D < Value > &src, int fsizex, int fsizey);
  bool getSrcVal(int x, int y, Value & val, const Array2D < Value > &src);
};

#include "Array2DMin.hpp"
#endif
