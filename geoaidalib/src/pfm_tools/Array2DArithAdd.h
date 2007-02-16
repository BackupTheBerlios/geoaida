#ifndef Array2DArithAdd_h
#define Array2DArithAdd_h

#include "Array2D.h"

template < class Value > class Array2DArithAdd:public Array2D < Value > {
public:
  Array2DArithAdd(const Array2D < Value > &src,
                  const Array2D < Value > &matrix);
};

#include "Array2DArithAdd.hpp"
#endif
