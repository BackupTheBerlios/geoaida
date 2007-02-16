#include "Array2DMaxLocal.h"
#include <typeinfo>
#include "pfm.h"
#include <vector>
#include <algorithm>

template < class Value >
  Array2DMaxLocal < Value >::Array2DMaxLocal(const Array2D < Value > &src,
                                         int fsizex, int fsizey, int pick)
  :
  Array2D <
Value > (src.xsize(), src.ysize())
{
  int x, y;
  for (y = 0; y < ysize(); y++) {
#if 0
#ifdef DEBUG
    printf("(%d,%d) %f \n", 0, y, src.getPixel(0, y));
#endif
#endif
    for (x = 0; x < xsize(); x++) {
      CalcPoint(x, y, src, fsizex, fsizey, pick);
    }
  }
}

template < class Value >
  bool Array2DMaxLocal < Value >::getSrcVal(int x, int y, Value & val,
                                          const Array2D < Value > &src)
{
  if ((x < 0) || (y < 0) || (x >= xsize()) || (y >= ysize()))
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
  void Array2DMaxLocal < Value >::CalcPoint(int xd, int yd,
                                          const Array2D < Value > &src,
                                          int fsizex, int fsizey, int pick)
{
  Value sumval;
  Value count;
  int mx = fsizex / 2;
  int my = fsizey / 2;
  sumval = 0;
  count = 0;
  int x, y;
  vector < Value > vec;
  for (y = -my; y <= my; y++)
    for (x = -mx; x <= mx; x++) {
      Value val;
      if (!getSrcVal(xd + x, yd + y, val, src))
        continue;
      vec.push_back(val);
    }
  if (pick == -1)
    pick = vec.size() / 2;
  if (pick >= vec.size())
    pick = vec.size() - 1;
  if (vec.size() < 1) {
#if 0
#ifdef DEBUG
    printf("(%d,%d) novec\n", xd, yd);
#endif
#endif
    if (typeid(Value) == typeid(float))
        set(xd, yd, sqrt(-1));
    else
      set(xd, yd, 0);
  }
  else {
    sort(vec.begin(), vec.end());
    set(xd, yd, vec[pick]);
  }
}
