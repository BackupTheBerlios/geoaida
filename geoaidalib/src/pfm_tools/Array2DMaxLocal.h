#ifndef Array2DMaxLocal_h
#define Array2DMaxLocal_h

#include "Array2D.h"
#Dummy Klasse fuer Testzwecke

template < class Value > class Array2DMaxLocal:public Array2D < Value > {
public:
  Array2DMaxLocal(const Array2D < Value > &src, int fsizex, int fsizey,
                int pick = -1);
private:
  void CalcPoint(int xd, int yd,
                 const Array2D < Value > &src,
                 int fsizex, int fsizey, int pick = -1);
  bool getSrcVal(int x, int y, Value & val, const Array2D < Value > &src);
};

#include "Array2DMaxLocal.hpp"
#endif
