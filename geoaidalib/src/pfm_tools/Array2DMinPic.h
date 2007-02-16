#ifndef Array2DMinPic_h
#define Array2DMinPic_h

#include "Array2D.h"

template < class Value > class Array2DMinPic:public Array2D < Value > {
public:
  Array2DMinPic(const Array2D < Value > &src,
                const Array2D < Value > &matrix);
};

#include "Array2DMinPic.hpp"
#endif
