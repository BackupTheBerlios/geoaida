#include "Array2D.h"
#include <typeinfo>
#include <string>
#include "pfm.h"

template <class Value>
Array2D<Value>::Array2D(FILE *fp)
{
  init(0,0);
  read(fp);
}

template <class Value>
Array2D<Value>::Array2D(const char *fname)
{
  init(0,0);
  read(fname);
}

template <class Value>
Array2D<Value>::Array2D(int x_size, int y_size)
{
  init(x_size,y_size);
}

template <class Value>
Array2D<Value>::Array2D(const Array2D<Value> &pic)
{
  init(pic.xsize(),pic.ysize());
  if (!data_) return;
  memcpy(data_,pic.data(),xsize_*ysize_*sizeof(Value));
}

template <class Value>
Array2D<Value>::~Array2D()
{
  if (!data_) return;
  delete data_;
}

template <class Value>
void Array2D<Value>::init(int x_size, int y_size)
{
  data_=0;
  xsize_=0;
  ysize_=0;  
  if ((x_size!=0) && (y_size!=0)) {
    xsize_=x_size;
    ysize_=y_size;
    data_=new Value[xsize_*ysize_];
  }
  gW_=gN_=gE_=gS_=0.0;
}


template <class Value>
void Array2D<Value>::read(FILE *fp)
{
  int storageType;
  if (typeid(Value)==typeid(int))
    storageType=PFM_SINT;
  else if (typeid(Value)==typeid(float))
    storageType=PFM_FLOAT;
  else if (typeid(Value)==typeid(unsigned short int))
    storageType=PFM_UINT16;
  else {
    fprintf(stderr,"Reading of %s data not implemented!\n",typeid(Value).name());
    return;
  }
  if (data_) delete data_;
  float minval, maxval;
  data_=(Value*)pfm_readpfm_type(fp,&xsize_,&ysize_,
				 &minval,&maxval,storageType,0);
  pfm_geo_get(&gW_,&gN_,&gE_,&gS_);
  minval_=Value(minval);
  maxval_=Value(maxval);
}

template <class Value>
void Array2D<Value>::read(const char *fname)
{
  FILE *fp=fopen(fname,"r");
  if (!fp) {
    fprintf(stderr,"Array2D::read(%s): file not found",fname);
    return;
  }
  read(fp);
  fclose(fp);
}

template <class Value>
void Array2D<Value>::write(FILE *fp) const
{
  if (!valid()) return;
  int storageType;
  if (typeid(Value)==typeid(int))
    storageType=PFM_SINT;
  else if (typeid(Value)==typeid(float))
    storageType=PFM_FLOAT;
  else if (typeid(Value)==typeid(unsigned short int))
    storageType=PFM_UINT16;
  else {
    fprintf(stderr,"Reading of %s data not implemented!\n",typeid(Value).name());
    return;
  }
  if (gW_==gE_ && gN_==gS_) pfm_geo_set(gW_,gN_,gE_,gS_);
  pfm_writepfm_type(fp, data_, xsize_, ysize_, 1,-1, storageType);
#if 0	 
  if (valid()) {
    printf("Writing (%dx%d) (%f<h<%f)\n",
	   xsize(),ysize(),min(),max());
    fprintf(fp,"P5\n%d %d\n%d\n",xsize(),ysize(),255);
    int y,x;
    unsigned char buf[xsize()];
    for (y=0; y<ysize(); y++) {
      for (x=0; x<xsize(); x++) {
        int val=getInt(x,y);
        switch (val) {
      	case -1:
      	case 0:
      	  buf[x]=0;
      	  break;
      	default:
      	  buf[x]=(unsigned char)((val % 255)+1);
      	  break;
      	}
      }
      fwrite(buf,sizeof(unsigned char),xsize(),fp);
    }
  }
#endif
}

template <class Value>
void Array2D<Value>::write(const char *fname) const
{
  FILE *fp=fopen(fname,"w");
  if (!fp) return;
  write(fp);
  fclose(fp);
}

