#ifndef PFMArray_h
#define PFMArray_h
#include "pfm.h"
#include <assert.h>
// #define NDEBUG

class PFMArray {
 public:
  PFMArray();
  PFMArray(const PFMArray& pic);
  PFMArray(int x_size=0, int y_size=0);
  PFMArray(FILE *fp);
  PFMArray(const char* fname);
  virtual ~PFMArray();
  void read(FILE *fp);
  void read(const char* fname);
  void write(FILE *fp) const;
  void write(const char* fname) const;
  float *data() const;
  int xsize() const;
  int ysize() const;
  float min() const;
  float max() const;
  float getPixel(int index) const;
  float getPixel(int x, int y) const;
  float setPixel(int x, int y, float val);
  bool valid() const;
  bool valid(int x, int y) const;
  void calcMinMax();
  void scale(float factor, float offset=0.0);
  void abs();
  protected:
  float* data_;
  int xsize_;
  int ysize_;
  float minval_;
  float maxval_;
 private: 
  void init(int x_size=0, int y_size=0);
};

inline float* PFMArray::data() const
{ 
  return data_;
}

inline int PFMArray::xsize() const 
{ 
  return xsize_;
}

inline int PFMArray::ysize() const
{ 
  return ysize_;
}

inline float PFMArray::min() const
{
  return minval_;
}

inline float PFMArray::max() const
{
  return maxval_;
}

inline float PFMArray::getPixel(int x, int y) const
{ 
  return data_[y*xsize_+x];
}

inline float PFMArray::getPixel(int index) const
{ 
  return data_[index];
}

inline float PFMArray::setPixel(int x, int y, float val) 
{
  assert(x>=0);
  assert(x<xsize_);
  assert(y>=0);
  assert(y<ysize_);
  data_[y*xsize_+x]=val;
  return val;
}

#endif
