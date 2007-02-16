#ifndef Array2DAbs_h
#define Array2DAbs_h

#include "Array2D.h"

template < class Value > class Array2DAbs:public Array2D < Value > {
public:
  Array2DAbs(const Array2D < Value > &src);
};

#include "Array2DAbs.hpp"
#endif
