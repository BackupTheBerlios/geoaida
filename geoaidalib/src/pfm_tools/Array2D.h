#ifndef Array2D_h
#define Array2D_h
#include <stdio.h>
#include <assert.h>

#ifdef GCC29
#ifdef linux
extern "C"
{
  int __isnanf(float);
}
#ifndef NAN
#define NAN sqrt(-1)
#endif
#endif
#endif

template < class Value > class Array2D
{
public:
  Array2D(FILE * fp);
  Array2D(const char *fname);
  Array2D(int xsize, int ysize);
  Array2D(const Array2D < Value > &labelPic);
  virtual ~Array2D();
  Value set(int x, int y, Value val);
  Value getInt(int x, int y) const;
  Value get(int x, int y) const;
  int xsize() const
  {
    return xsize_;
  }
  int ysize() const
  {
    return ysize_;
  }
  Value *data() const
  {
    return data_;
  }
  virtual void read(FILE * fp);
  void read(const char *fname);
  virtual void write(FILE * fp) const;
  void write(const char *fname) const;
  bool valid() const;
protected:
  void init(int x_size, int y_size);
  int xsize_;
  int ysize_;
  Value minval_;
  Value maxval_;
  Value *data_;
  double gW_,gN_,gE_,gS_;
};

template < class Value >
  inline Value Array2D < Value >::set(int x, int y, Value val)
{
  assert(x >= 0);
  assert(x < xsize_);
  assert(y >= 0);
  assert(y < ysize_);
  data_[y * xsize_ + x] = val;
  return val;
}

template < class Value > inline Value Array2D < Value >::getInt(int x, int y) const
{
  assert(x >= 0);
  assert(x < xsize_);
  assert(y >= 0);
  assert(y < ysize_);
  return data_[y * xsize_ + x];
}

template < class Value > inline Value Array2D < Value >::get(int x, int y) const
{
  assert(x >= 0);
  assert(x < xsize_);
  assert(y >= 0);
  assert(y < ysize_);
  return data_[y * xsize_ + x];
}

template < class Value > inline bool Array2D < Value >::valid() const
{
  return (data_ != 0);
}

#include "Array2D.hpp"

#endif
