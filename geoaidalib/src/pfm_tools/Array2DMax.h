#ifndef Array2DMax_h
#define Array2DMax_h

#include "Array2D.h"

template < class Value > class Array2DMax:public Array2D < Value > {
public:
  Array2DMax(const Array2D < Value > &src, int fsizex, int fsizey);
private:
  void CalcPoint(int xd, int yd,
                 const Array2D < Value > &src, int fsizex, int fsizey);
  bool getSrcVal(int x, int y, Value & val, const Array2D < Value > &src);
};

#include "Array2DMax.hpp"
#endif
