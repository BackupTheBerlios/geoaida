/***************************************************************************
                          GaImageBase.h  -  geoAIDA template image class
                             -------------------
    begin                : Thu Jan 11 2001
    copyright            : (C) 2001 TNT, Uni Hannover
    authors              : Jürgen Bückner, Oliver Stahlhut, Martin Pahl
    email                : pahl@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 * $Source: /data/cvs/ga_image/ga_image/gaimage/gaimagebase.h,v $
 * $Revision: 1.11 $
 * $Date: 2003/05/28 06:35:02 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef __GAIMAGEBASE_H
#define __GAIMAGEBASE_H

#include <typeinfo>
#include "gadefines.h"
#include <assert.h>

namespace Ga {

//... preventive statements

enum IMGTYPE {
    _PFM_FLOAT=0,
    _PFM_SINT,
    _PFM_UINT,
    _PFM_SINT16,
    _PFM_UINT16,
		_PGM,
		_PBM,
		_PPM,	
		_UNKNOWN
};

template <class PixType> class ImageT;

/** \class ImageBase
    \brief defines an template class ImageBase for operations between matrices
    */

class ImageBase
{
public:
  ImageBase();
  ImageBase(const ImageBase& rval);
  virtual ~ImageBase(void){}
  virtual void partCopy(const ImageBase &rvalue, int x0, int y0, int width, int height)=0;
  virtual ImageBase* newObject(int x_size, int y_size, int channels=1)=0;
  virtual ImageBase* copyObject()=0;
  /** sets the image x-size to c
    \param c x-size of image */
  void sizeX(int c) { sizeX_ = c; }
  /** sets the image y-size to c
    \param c y-size of image */
  void sizeY(int c) { sizeY_ = c; }

  /** returns the x-size of the picture (number of columns)
      usage: \code int x = M.sizeX() \endcode
      \return x-size */
  int sizeX(void) const { return sizeX_; }

  /** returns the y-size of the picture (number of rows)
      usage: \code int y = M.sizeY() \endcode
      \return y-size */
  int sizeY(void) const { return sizeY_; }

  /** returns the number of elements (x_size * y_size)
      usage: \code int x = M.size(); \endcode
      \return number of elements */
  int sizeImage(void) const { return nSize_; }

	IMGTYPE typeImage (void) const { return imageType_; }
	void typeImage (IMGTYPE t) { imageType_ = t;}

  /** return number of bands */
  int getNoBands() { return( typeImage() == _PPM ? 3 : 1 ); }

  virtual const class std::type_info& typeId() const =0;
  virtual int noChannels() const=0;
  virtual void rmChannel()=0;
  virtual void addChannel(const ImageBase& rval, int channel=0)=0;
  virtual void clear()=0;
  // Iterators
  virtual void* beginVoid(int row=0, int channel=0)=0;
  virtual const void* constBeginVoid(int row=0, int channel=0) const =0;
  virtual const void* endVoid(int row, int channel=0) const =0;
  virtual const void* endVoid() const =0;
  virtual void* dataArray(int channel=0)=0;
  virtual double getFloat(int x, int y, int channel=0, double neutral=0) const=0;
  virtual void setFloat(int x, int y, double val, int channel=0, bool clip=false)=0;
  virtual double getFloat(const void *it) const=0;
  virtual void setFloat(void* it, double val)=0;
  virtual void* nextCol(const void*& ptr) const=0;
  virtual void* nextCol(const void*& ptr, int offset) const=0;
  virtual void* prevCol(const void*& ptr) const=0;
  virtual void* nextRow(const void*& ptr) const=0;
  virtual void* prevRow(const void*& ptr) const=0;
  virtual void* operator [] (int y)=0;
	virtual void setData(int x, int y, void* initvalues)=0;
  virtual bool eq(const ImageBase& rval)=0;
  virtual bool neq(const ImageBase& rval)=0;
  virtual void add(const ImageBase& lval, const ImageBase& rval)=0;
  virtual void multElements(const ImageBase& lval, const ImageBase& rval)=0;
  virtual void mult(const ImageBase& lval, const ImageBase& rval)=0;
  virtual void sub(const ImageBase& lval, const ImageBase& rval)=0;
  virtual void divElements(const ImageBase& lval, const ImageBase& rval)=0;
  virtual void div(const ImageBase& lval, double rval)=0;
  virtual void div(double lval, const ImageBase& rval)=0;
  virtual void mult(const ImageBase& lval, double rval)=0;
  virtual void mult(double lval, const ImageBase& lval)=0;
  virtual void neg(const ImageBase& rval)=0;
  virtual void concat(const ImageBase& im1, const ImageBase& im2, bool horizontal=true)=0;
  virtual void copyInto(const ImageBase& im1, int offsetX, int offsetY)=0;
  virtual void debug(FILE *fp)=0;

  /*
  virtual void Save(char *filematrix)=0;                 // formatted
  */

  virtual void storeMinValue()=0;
  virtual void storeMaxValue()=0;
  virtual void setMinValue(double v)=0;
  virtual void setMaxValue(double v)=0;

  virtual double minValue() const=0;
  virtual double maxValue() const=0;
  virtual double matrixMax(int& x, int& y, int ch=0) =0;
  virtual double matrixMax(int ch=0) =0;
  virtual double matrixAbsMax(int& x, int& y, int ch=0) =0;
  virtual double matrixMin(int& x, int& y, int ch=0) =0;
  virtual double matrixMin(int ch=0) =0;
  virtual double matrixAbsMin(int& x, int& y, int ch=0) =0;

  virtual void alloc( int x, int y, int noChannels )=0;
  virtual void initialize( int x, int y, int noChannels )=0;
  virtual void deinitialize(void)=0;
  virtual void resize(int rx, int ry, int noChannels=1)=0;
  virtual void fill(double value)=0;
  virtual void setGeoCoordinates(double gW, double gN, double gE, double gS){assert(1<0);}
  virtual double geoWest(){assert(1<0); return 0;}
  virtual double geoNorth(){assert(1<0); return 0;}
  virtual double geoEast(){assert(1<0); return 0;}
  virtual double geoSouth(){assert(1<0); return 0;}
  virtual void setGeoRes(double res) {return;}
  virtual double geoRes() {return 0.0;}
  virtual int geo2ImageX(double geo) {return 0;}
  virtual int geo2ImageX_f(double geo) {return 0;}
  virtual double geo2ImageX_double(double geo) {return 0.0;}
  virtual int geo2ImageY(double geo) {return 0;}
  virtual int geo2ImageY_f(double geo) {return 0;}
  virtual double geo2ImageY_double(double geo) {return 0.0;}
  virtual double image2GeoX(double x) {return 0.0;}
  virtual double image2GeoY(double y) {return 0.0;}
  virtual double geodist2pixel(double dist) {return 0.0;}
  virtual double pixel2geodist(int pix) {return 0.0;}
  virtual void setGeoInt(double gx, double gy, int val, int channel=0) { return;}
  virtual void setInt(void *ptr, int val) {return;}
  virtual void setInt(int x, int y, int val, int channel=0) {return;}
  virtual int getInt(const void *ptr) const {return 0;}
  virtual int getInt(int x, int y, int channel=0) const {return 0;}
  virtual int getInt(int x, int y, int channel, int neutral) const {return 0;}
  virtual int getGeoInt(double gx, double gy, int channel=0) {return 0;}

  virtual void merge(ImageBase& img, double img_label, double new_label) {}
  virtual void getChannel(ImageBase& pic, int channel=0)=0;
  virtual void convert2Luminance(ImageBase& pic) {}
   virtual ImageT<float> hsv()=0;
   virtual void convert2ndvi(ImageBase& pic) {}
   virtual void resample(ImageBase& result, int nx, int ny)=0;
   virtual void resampleNN(ImageBase& result, int nx, int ny)=0;
   virtual void resampleNNplus(ImageBase& result, int nx, int ny)=0;
  virtual void calcHistogram(ImageT<int>& hist , double min, double max, int n, int channel=0)=0;
  virtual void blow(ImageBase &in, int label, int iterations)=0;
  virtual void minValue(ImageBase &in, float value)=0;
  virtual void maxValue(ImageBase &in, float value)=0;
  virtual bool read(FILE *fp)=0;
  virtual bool write(FILE *fp, int channel=0, const char* comment=0)=0;
  bool write(const char *filename, int channel=0);
  bool read(const char* filename);


protected:

	IMGTYPE imageType_;	
  int sizeX_, sizeY_;
  size_t nSize_;

};

inline ImageBase::ImageBase()
{
  sizeX_=0;
  sizeY_=0;
  nSize_=0;
  imageType_=_UNKNOWN;
}

inline ImageBase::ImageBase(const ImageBase& rval)
{
  sizeX_=rval.sizeX_;
  sizeY_=rval.sizeY_;
  nSize_=rval.nSize_;
  imageType_=rval.imageType_;
}

inline bool ImageBase::write(const char* filename, int channel)
{
  FILE *fp;
  fp=fopen(filename,"w");
  if (!fp) return false;
  return write(fp,channel);
}

inline bool ImageBase::read(const char* filename)
{
  FILE *fp;
  fp=fopen(filename,"r");
  if (!fp) return false;
  return read(fp);
}
} // namespace Ga

#endif        // __GA_IMAGEBASE_H
