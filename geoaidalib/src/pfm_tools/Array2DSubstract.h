#ifndef Array2DSubstract_h
#define Array2DSubstract_h

#include "Array2D.h"

template < class Value > class Array2DSubstract:public Array2D < Value > {
public:
  Array2DSubstract(const Array2D < Value > &src,
                   const Array2D < Value > &matrix);
};

#include "Array2DSubstract.hpp"
#endif
