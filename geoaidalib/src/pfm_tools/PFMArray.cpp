#if __GNUC__==2
#ifdef linux
extern "C" {
int __isnanf(float);
	   }
#ifndef NAN
#define NAN sqrt(-1)
#endif
#endif
#endif

#include "PFMArray.h"
#include <string.h>
#include <math.h>


PFMArray::PFMArray(const PFMArray& pic)
{
  init(pic.xsize(),pic.ysize());
  if (!data_) return;
  memcpy(data_,pic.data(),xsize_*ysize_*sizeof(float));
  minval_=pic.min();
  maxval_=pic.max();
}

PFMArray::PFMArray(int x_size, int y_size) 
{
  init(x_size,y_size);
}

PFMArray::PFMArray(FILE *fp)
{
  init(0,0);
  read(fp);
}

PFMArray::PFMArray(const char *fname)
{
  init(0,0);
  read(fname);
}

PFMArray::~PFMArray()
{
  if (data_) delete data_;
}

void PFMArray::init(int x_size, int y_size) 
{
  data_=0;
  xsize_=0;
  ysize_=0;
  if ((x_size!=0) && (y_size!=0)) {
    xsize_=x_size;
    ysize_=y_size;
    data_=new float[xsize_*ysize_];
  }
}


void PFMArray::read(FILE *fp)
{
  if (data_) delete data_;
  data_=pfm_readpfm(fp,&xsize_,&ysize_,&minval_,&maxval_);
}

void PFMArray::read(const char *fname)
{
  FILE *fp=fopen(fname,"r");
  if (!fp) return;
  read(fp);
  fclose(fp);
}

void PFMArray::write(FILE *fp) const
{
	 
  if (valid()) {
    printf("Writing (%dx%d) (%f<h<%f)\n",
	   xsize(),ysize(),min(),max());
    pfm_writepfm(fp, data_, xsize_, ysize_, 1, -1);
  }
}

void PFMArray::write(const char *fname) const
{
  FILE *fp=fopen(fname,"w");
  if (!fp) return;
  write(fp);
  fclose(fp);
}

bool PFMArray::valid() const
{
  if (data_) return true;
  else return false;
}

bool PFMArray::valid(int x, int y) const
{
  return ((x>=0) && (x<xsize_) && (y>=0) && (y<ysize_));
}

void PFMArray::calcMinMax()
{
  int i;
  minval_=1e37; maxval_=-1e37;
  for (i=0; i<xsize_*ysize_; i++) {
    if (isnan(data_[i])) continue;
    if (minval_>data_[i]) minval_=data_[i];
    if (maxval_<data_[i]) maxval_=data_[i];
  }
}

void PFMArray::scale(float factor, float offset)
{
  int i;
  for (i=0; i<xsize_*ysize_; i++) {
    if (!isnan(data_[i])) data_[i]=(data_[i]+offset)*factor;
    if (minval_>data_[i]) minval_=data_[i];
    if (maxval_<data_[i]) maxval_=data_[i];
  }
}

void PFMArray::abs()
{
  int i;
  minval_=1e37; maxval_=-1e37;
  for (i=0; i<xsize_*ysize_; i++) {
    if (!isnan(data_[i])) data_[i]=fabs(data_[i]);
    if (minval_>data_[i]) minval_=data_[i];
    if (maxval_<data_[i]) maxval_=data_[i];
  }
}

