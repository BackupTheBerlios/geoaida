#ifndef Array2DClose_h
#define Array2DClose_h

#include "Array2D.h"

template < class Value > class Array2DClose:public Array2D < Value > {
public:
  Array2DClose(const Array2D < Value > &src, int label=0);
  Array2DClose(FILE * fp, int label = 0);
  Array2DClose(const char *fname, int label = 0);
  void close();
private:
  void interpolate(int x, int y);

  bool nextPoint(int &x, int &y, int &dir);
  void fillHole(int x_start, int y_start, int start_dir);

	int label_;
};

#include "Array2DClose.hpp"
#endif
