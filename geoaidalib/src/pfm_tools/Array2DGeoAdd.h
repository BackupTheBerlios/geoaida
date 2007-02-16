#ifndef Array2DGeoAdd_h
#define Array2DGeoAdd_h

#include "Array2D.h"

template < class Value > class Array2DGeoAdd:public Array2D < Value > {
public:
  Array2DGeoAdd(const Array2D < Value > &src,
                const Array2D < Value > &matrix);
};

#include "Array2DGeoAdd.hpp"
#endif
