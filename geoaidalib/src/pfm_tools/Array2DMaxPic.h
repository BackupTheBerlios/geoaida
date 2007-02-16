#ifndef Array2DMaxPic_h
#define Array2DMaxPic_h

#include "Array2D.h"

template < class Value > class Array2DMaxPic:public Array2D < Value > {
public:
  Array2DMaxPic(const Array2D < Value > &src,
                const Array2D < Value > &matrix);
};

#include "Array2DMaxPic.hpp"
#endif
