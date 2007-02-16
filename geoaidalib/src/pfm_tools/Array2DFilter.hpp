#include "Array2DFilter.h"
#include <typeinfo>
#include "pfm.h"

template < class Value >
  Array2DFilter < Value >::Array2DFilter(const Array2D < Value > &src,
                                         const Array2D < Value > &matrix)
  :
  Array2D <
Value > (src)
{
  if (((matrix.xsize() % 2) == 0) || ((matrix.ysize() % 2) == 0)) {
    fprintf(stderr, "matrix-dimension must be odd in each direction\n");
    return;
  }

  int x, y;
  for (y = 0; y < Array2D<Value>::ysize(); y++) {
#if 0
#ifdef DEBUG
    printf("(%d,%d) %f \n", 0, y, src.getPixel(0, y));
#endif
#endif

    for (x = 0; x < Array2D<Value>::xsize(); x++) {
      CalcPoint(x, y, src, matrix);
    }
  }
}

template < class Value >
  bool Array2DFilter < Value >::getSrcVal(int x, int y, Value & val,
                                          const Array2D < Value > &src)
{
  if ((x < 0) || (y < 0) 
      || (x >= Array2D<Value>::xsize()) || (y >= Array2D<Value>::ysize()))
    return false;
  val = src.get(x, y);

  if ((typeid(Value) == typeid(float)) && isnan(val))
      return false;
#if 0
#ifdef DEBUG
  printf("                (%d,%d) %f\n", x, y, val);
#endif
#endif
  return true;
}

template < class Value >
  void Array2DFilter < Value >::CalcPoint(int xd, int yd,
                                          const Array2D < Value > &src,
                                          const Array2D < Value > &matrix)
{
#define abs(v) (((v)<0) ? (-(v)) : (v))
  Value sumval;
  Value count;
  int mx = matrix.xsize() / 2;
  int my = matrix.ysize() / 2;
  sumval = 0;
  count = 0;
  int x, y;
  for (y = 1; y <= my; y++)
    for (x = -mx; x <= mx; x++) {
      Value val1;
      Value val2;
      if (!getSrcVal(xd - x, yd - y, val1, src))
        continue;
      if (!getSrcVal(xd + x, yd + y, val2, src))
        continue;
      sumval += val1 * matrix.get(mx - x, my - y)
        + val2 * matrix.get(mx + x, my + y);
      count += abs(matrix.get(mx - x, my - y))
        + abs(matrix.get(mx + x, my + y));
    }
  //  y=0;
  for (x = 1; x <= mx; x++) {
    Value val1;
    Value val2;
    if (!getSrcVal(xd - x, yd, val1, src))
      continue;
    if (!getSrcVal(xd + x, yd, val2, src))
      continue;
    sumval += val1 * matrix.get(mx - x, my)
      + val2 * matrix.get(mx + x, my);
    count += abs(matrix.get(mx - x, my))
      + abs(matrix.get(mx + x, my));
  }
  // y=0; x=0;
  {
    Value val1;
    if (getSrcVal(xd, yd, val1, src)) {
      sumval += val1 * matrix.get(mx, my);
      count += abs(matrix.get(mx, my));
    }
  }
  set(xd, yd, sumval / count);
#if 0
#ifdef DEBUG
  printf("                (%d,%d) %f\n", xd, yd, sumval / count);
#endif
#endif
#undef abs
}
