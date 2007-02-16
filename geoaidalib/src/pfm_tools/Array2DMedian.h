#ifndef Array2DMedian_h
#define Array2DMedian_h

#include "Array2D.h"

template < class Value > class Array2DMedian:public Array2D < Value > {
public:
  Array2DMedian(const Array2D < Value > &src, int fsizex, int fsizey,
                int pick = -1);
private:
  void CalcPoint(int xd, int yd,
                 const Array2D < Value > &src,
                 int fsizex, int fsizey, int pick = -1);
  bool getSrcVal(int x, int y, Value & val, const Array2D < Value > &src);
};

#include "Array2DMedian.hpp"
#endif
