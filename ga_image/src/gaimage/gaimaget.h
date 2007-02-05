/***************************************************************************
                          GaImageT.h  -  geoAIDA template image class
                             -------------------
    begin                : Thu Jan 11 2001
    copyright            : (C) 2001 TNT, Uni Hannover
    authors              : Jürgen Bückner, Oliver Stahlhut, Martin Pahl
    email                : bueckner@tnt.uni-hannover.de
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
 * $Source: /data/cvs/ga_image/ga_image/gaimage/gaimaget.h,v $
 * $Revision: 1.14 $
 * $Date: 2003/05/28 06:35:02 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef __GA_IMAGET_H
#define __GA_IMAGET_H

#ifdef GCC29
#include <math.h>
#include <float.h>
#else
#include <cmath>
#endif
#if __GNUC__==2
#include <limits.h>
#else
#include <limits>
#endif
extern "C" {
#include <pnm.h>
#include <pfm.h>
}
#define GA_MAX_CHANNELS 10
#include <assert.h>
#include "gadefines.h"
#include "gaimagebase.h"
#include "gaarray2dt.h"

namespace Ga {

#ifndef NAN
#define NAN sqrt(-1)
#endif
#define GA_RED(p)  (p & 0xff)
#define GA_GREEN(p)  ((p & 0xff00) >> 8)
#define GA_BLUE(p)  ((p & 0xff0000) >> 16)

#define GA_RGB(r, g, b)  (r + g * 0x0100 + b * 0x010000)

/** \class ImageT
    \brief defines an template class for operations between matrices and vectors
    and for handling geo-related images
    */

  template <class PixTyp>
  class ImageT : public ImageBase
  {
  public:
    enum { red, green, blue };
    typedef PixTyp* Iterator;
    typedef const PixTyp* ConstIterator;
    
    //--------------------- constructors for images -----------------------------
    /** default constructor, defines a picture for later assignment */
    ImageT();
    /** copy constructor, definition and initialization from a picture;
	usage: \code ImageT A = B; ImageT A = B + 2 * C; return A; \endcode */
    ImageT(const ImageT<PixTyp>& rval);
    virtual ImageBase* newObject(int x_size, int y_size, int channels=1);
    virtual ImageBase* copyObject();
    
    /** create picture from matrix
	usage: \code ga_pix A( y ); \endcode */
    ImageT(const ImageBase& imgIn);
    
    /** size a picture and initialize it to zero; must then be assigned other values (for a picture of known dimension)
	usage: \code ImageT A( 4, 5 ); A( 1, 1 ) = 3; A( 1, 2 ) = 7; \endcode
	\param x x-size, number of columns
	\param y y-size, number of rows */
    ImageT(int x, int y, int noChannels=1);
    
    /** create a picture from an array
	usage: \code int w[]={1, 2, 3, 4, 5, 6}; ImageT A( 2, 3, w ); \endcode
	\param x x-size, number of columns
	\param y y-size, number of rows */
    ImageT (int x, int y, PixTyp *vec);
    
    /** create a picture from formatted File (ASCII)
	usage: \code ImageT A("MAT.DAT"); \endcode
	\param file filename */
    ImageT(char *file);
    
    /** create a subpicture with identical beginning (at [1,1])
	usage: \code ImageT B( 4, 5, A ); \endcode
	\param x new x-size, number of columns
	\param y new y-size, number of rows
	\param imgIn input picture */
    ImageT(int x, int y, const ImageT& imgIn);
    
    /** create subpicture starting at arg3, arg4 with size arg1 x arg2
	usage: \code ImageT B( m, n, i, j, A ); \endcode
	\param x new x-size, number of columns
	\param y new y-size, number of rows
	\param x0 starting offset x
	\param y0 starting offset y
	\param imgIn input picture */
    ImageT(int x, int y, int x0, int y0, const ImageT& imgIn);
    
    /** destructor */
    virtual ~ImageT(void);
    virtual void partCopy(const ImageBase &rvalue, int x0, int y0, int width, int height);
    virtual const class std::type_info& typeId() const;
    int noChannels() const;
    void rmChannel();
    void addChannel(const ImageBase& rval, int channel=0);
    
    //------------------------------ Access of matrix data ----------------------
    /** clear matrix */
    void clear();
    /** treats pointer this as if were a long integer */
    unsigned long hash(void) const;
    /** return pointer to the beginning of data
	usage: \code Iterator dp = M.begin(row); \endcode */
    Iterator begin(int row=0, int channel=0);
    /** return pointer to the beginning of data
	usage: \code PixTyp * dp = M.data(row); \endcode */
    PixTyp* data(int row =0, int channel = 0);
    PixTyp** dataArray(int channel=0);
    ConstIterator constBegin(int row=0, int channel=0) const;
    void* beginVoid(int row=0, int channel=0);
    ConstIterator end(int row, int channel=0) const;
    ConstIterator end() const;
    ConstIterator endChannel(int channel) const;
    const void* endVoid(int row, int channel=0) const;
    const void* endVoid() const;
    const void* constBeginVoid(int row=0, int channel=0) const;
    /** retrieve the value of an matrix element
	usage: \code PixTyp x = A.GetElement( 3, 5 ); \endcode */
    PixTyp get(int x, int y, int channel=0, PixTyp neutral=0) const;
    double getFloat(int x, int y, int channel, double neutral=0) const;
    virtual double getFloat(const void *it) const;
    virtual void setFloat(void* it, double val);
    virtual void* nextCol(const void*& ptr, int offset) const;
    virtual void* nextCol(const void*& ptr) const;
    virtual void* prevCol(const void*& ptr) const;
    virtual void* nextRow(const void*& ptr) const;
    virtual void* prevRow(const void*& ptr) const;
    /** assign and retrieve matrix values
	usage: \code PixTyp x = A( 1, 5 ); A( 3, 7 ) = 5; \endcode */
    PixTyp& operator () (int x, int y, int channel=0);
    /** assign and retrieve matrix values
	usage: \code PixTyp x = A( 5 ); A( 3 ) = 5; \endcode
	\param nr number of element
	\return reference to an element */
    PixTyp& operator () (unsigned int n);
    /** pointer to row y
	\param y row number */
    Iterator operator [] (int y);
    /** Assign value to matrix element, check for legal row and column index
	usage: \code  A.SetElement( 3, 4, 4444 );       \endcode
	\param x x of element (column)
	\param y y of element (row)
	\param val value to set element to */
    void set(int x, int y, PixTyp val, int channel=0, bool clip=false);
    void setFloat(int x, int y, double val, int channel=0, bool clip=false);
    /* set data without copying. Ther must be noChannels data-pointer in the ellipse */
    void setData(int x, int y, int noChannels, ...);
    void setData(int x, int y, void* initvalues);
    /** assignment of a matrix
	usage: \code  A=B; A=C+D; A=B=C+D; \endcode */
    ImageT<PixTyp>& operator = (const ImageT<PixTyp>& rval);
    /** assignment of matrix values (initialization) */
    ImageT& operator = (const PixTyp rval);
    //----------------------------------- ALGEBRAIC OPERATIONS ------------------
    class OpAdd {
    public:
      PixTyp operator() (PixTyp lval, PixTyp rval) {return lval+rval;}
    };
    
  class OpMult {
  public:
    PixTyp operator() (PixTyp lval, PixTyp rval) {return lval*rval;}
  };

  class OpSub {
  public:
    PixTyp operator() (PixTyp lval, PixTyp rval) {return lval-rval;}
  };

  class OpDiv {
  public:
    PixTyp operator() (PixTyp lval, PixTyp rval) {return lval/rval;}
  };

  class OpNeg {
  public:
    PixTyp operator() (PixTyp rval) {return -rval;}
  };

  class OpConstMult {
  public:
    OpConstMult(PixTyp f) {f_=f;}
    PixTyp operator() (PixTyp rval) {return rval*f_;}
    PixTyp f_;
  };

  class OpConstDiv {
  public:
    OpConstDiv(PixTyp f) {f_=f;}
    PixTyp operator() (PixTyp rval) {return f_/rval;}
    PixTyp f_;
  };

  template <class OperatorClass>
  void binaryPixelOp(const ImageT<PixTyp>& lval, const ImageT<PixTyp>& rval, OperatorClass opClass );
  template <class OperatorClass>
  void unaryPixelOp(const ImageT<PixTyp>& lval, OperatorClass opClass );
  virtual void add(const ImageBase& lval, const ImageBase& rval);
  virtual void multElements(const ImageBase& lval, const ImageBase& rval);
  virtual void sub(const ImageBase& lval, const ImageBase& rval);
  virtual void divElements(const ImageBase& lval, const ImageBase& rval);
  virtual void mult(const ImageBase& lval, double rval);
  virtual void mult(double rval, const ImageBase& lval);
  virtual void div(const ImageBase& lval, double rval);
  virtual void div(double lval, const ImageBase& rval);
  virtual void neg(const ImageBase& rval);
  virtual bool eq(const ImageBase& rval);
  virtual bool neq(const ImageBase& rval);
  virtual void mult(const ImageBase& lvalue, const ImageBase& rvalue);
  /** abbreviated form of sum
      usage: \code   A += B  => A = A + B; */
  ImageT &operator += (const ImageT& rval);
  /** abbreviated form of difference between matrices
      usage: \code   A -= B; // => A = A - B; \endcode */
  ImageT &operator -= (const ImageT& rval);
  /** Abbreviated form of product. Favour this alternative when possible!
      usage: \code   A *= B;  // =>  A = A * B; \endcode */
  ImageT& operator *= (const ImageT& rval);
  /** Multiplies and modifies a ImageT by a PixTyp.
      usage: \code  A *= 3.; \endcode */
  ImageT& operator *= (PixTyp rval);
  /** compare: If lval equals rval return 1, 0 otherwise
      usage: \code   if( A == B ) \endcode */
  template <class P>
  friend bool operator== (const ImageT<P>& lval, const ImageT<P>& rval);
  /** Purpose: If it is != return 1 otherwise 0
      usage: \code   if( A != B ) \endcode */
  template <class P>
  friend int  operator!= (const ImageT<P>& lval,const ImageT<P>& rval);
  /** addition of matrices
      usage: \code  A = B + C; \endcode */
  template <class P>
  friend ImageT<P>  operator+ (const ImageT<P>& lval,const ImageT<P>& rval);
  /** Difference between matrices
      usage: \code  A = B - C; \endcode */
  template <class P>
  friend ImageT<P>  operator- (const ImageT<P>& lval, const ImageT<P>& rval);
  /** unary minus
      usage: \code   A = -B; \endcode */
  template <class P>
  friend ImageT<P>  operator- (const ImageT<P>& rval);
  /** Product between matrices
      usage: \code C = A * B \endcode */
  template <class P>
  friend ImageT<P>  operator* (const ImageT<P>& lval, const ImageT<P>& rval);
  /**  Product of a PixTyp with a ImageT. Scalares Product
       usage: \code   A = 3 * B; \endcode */
  template <class P>
  friend ImageT<P>  operator* (double lval, ImageT<P>& rval);

  /** Dividing the coefficients of a matrix by a PixTyp
      usage: \code  A = B / 3.; \endcode */
  template <class P>
  friend ImageT<P>  operator/ (const ImageT<P>& lval, double rval);
  /** Dividing and modifying a ImageT by a PixTyp
      usage: \code  A /= 2.; \endcode */
  ImageT& operator /= (PixTyp rval);
  void concat(const ImageBase& im1, const ImageBase& im2, bool horizontal=true);
  void copyInto(const ImageBase& im1, int offsetX, int offsetY);
  //------------------------------------ Tool functions -----------------------

  /** Writes a ImageT in a formatted file (ASCII)
      usage: \code  A.Write( fp ); \endcode */
  virtual void debug(FILE *fp);                 // formatted
  /** store the current minimum value of the matrix (useful for iterated access to this value) */
  void storeMinValue();
  /** store the current minimum value of the matrix (useful for iterated access to this value) */
  void storeMaxValue();

  /** set the minimum value of the matrix to arg */
  void setMinValue(double v);
  /** set the maximum value of the matrix to arg */
  void setMaxValue(double v);

  double minValue() const;
  double maxValue() const;

  /** Maximum element value of a ImageT
      usage: \code  PixTyp xmax = A.Max( ); PixTyp xmax = A.Max( x, y ); \endcode */
  double matrixMax(int& x, int& y, int channel=0);
  double matrixMax(int channel=0);
  /** Absolute maximum of a ImageT
      usage: \code double xmax = A.MaxAbs( ); double xmax = A.MaxAbs( x, y ); \endcode */
  double matrixAbsMax(int& x, int& y, int channel=0);
  /** Minimum of a ImageT
      usage: \code double xmin = A.Min( ); \endcode */
  double matrixMin(int& x, int& y, int channel=0);
  double matrixMin(int channel=0);
  /** Absolute minimum of a ImageT
      usage: \code double xmin = A.MinAbs( ); double xmin = A.MinAbs( x, y ); \endcode */
  double matrixAbsMin(int& x, int& y, int channel=0);
  /** Returns the size of one element */
  int elemSize();
  void resize(int rx, int ry, int noChannels=1);

  // image methods
  /** set geocoordinates for this image */
  void setGeoCoordinates(double gW, double gN, double gE, double gS);

  /** get west/left geocoordinate */
  double geoWest();
  /** get north/top geocoordinate */
  double geoNorth();
  /** get east/right geocoordinate */
  double geoEast();
  /** get south/bottom geocoordinate */
  double geoSouth();

  /** set the resolution of this image unit [m/pixel] */
  void setGeoRes(double res);
  /** get the resolution of this image [m/pixel] */
  double geoRes();

  /** returns the x-image coordinate of a x-geocoordinate */
  int geo2ImageX(double geo);

  /** returns the x-image coordinate of a x-geocoordinate, caution: no range check */
  int geo2ImageX_f(double geo);

  /** returns the x-image coordinate of a x-geocoordinate, caution: no range check */
  double geo2ImageX_double(double geo);

  /** returns the y-image coordinate of a y-geocoordinate */
  int geo2ImageY(double geo);

  /** returns the y-image coordinate of a y-geocoordinate, caution: no range check */
  int geo2ImageY_f(double geo);

  /** returns the y-image coordinate of a y-geocoordinate, caution: no range check */
  double geo2ImageY_double(double geo);

  /** returns the geocoordinate of a x-image coordinate */
  double image2GeoX(double x);

  /** returns the geocoordinate of a y-image coordinate */
  double image2GeoY(double y);

  /** returns the number of image pixels covered by a geo-distance (only exact for 0 and 90 degree) */
  double geodist2pixel(double dist);

  /** returns the geo-distance for a number of pix image pixel (only exact for 0 and 90 degree) */
  double pixel2geodist(int pix);

  /** set a pixel using geocoordinates */
  void setGeoInt(double gx, double gy, int val, int channel=0);

  /** sget a pixel using geocoordinates */
  int getGeoInt(double gx, double gy, int channel=0);

  /** set a pixel */
  void setInt(void *ptr, int val);
  void setInt(int x, int y, int val, int channel=0);

  /** set a pixel */
  int getInt(const void *ptr) const;
  int getInt(int x, int y, int channel=0) const;
  int getInt(int x, int y, int channel, int neutral) const;


  /** merge 'this' image with 'img'. The labels 'img_label' of 'img'
  	* are insert (overlayed) in 'this' with the value 'new_label' */
  void merge(ImageBase& image, double img_label, double new_label);

 /** extracts one band from a rgb-color image to a new single-band image
      \param band 0=red 1=green 2=blue, enum type can be used (e.g. ImageT<GA_NOTYPE>::red) */
  void getChannel(ImageBase& pic, int channel=0);

  /**  converts a 3-band image (rgb-color) to a new luminance image */
  void convert2Luminance(ImageBase &pic);

  /** return a hsv 3 band image of an 3 band rgb image */
  ImageT<float> hsv();

  /** resample the current image to size (x, y) */
  void resample(ImageBase& result, int nx, int ny);

  /** resample the current image to size (x, y) using a nearest neighbour check */
  void resampleNN(ImageBase& result, int nx, int ny);

  /** resample the current image to size (x, y) using a nearest neighbour check;
      this version uses a sophisticated class determination algorithm, which is
      especially useful for subsampling/shrinking an image, thus for enlargement */
  void resampleNNplus(ImageBase& result, int nx, int ny);

  void calcHistogram(ImageT<int> &result, double min, double max, int n, int channel=0);
  void blow(ImageBase &in, int label, int iterations);
  void minValue(ImageBase &in, float value);
  void maxValue(ImageBase &in, float value);

  bool read(FILE *fp);
	bool read(const char *filename);
  bool write(FILE *fp, int channel=0, const char* comment=0);
	bool write(const char *filename, int channel=0, const char* comment=0);

protected:
  /** allocation of memory and initialization of pointers, internal use only */
  void alloc( int x, int y, int noChannels );
  /** initialization; common function for initialization; for internal use only
      usage: \code Initialize( x, y ) \endcode \endcode */
  void initialize( int x, int y, int noChannels );
  /** deinitialization; common function for deinitialization (of pointer only !!) */
  void deinitialize(void);
  /** prepare assignments; Preparing an assignment by adjusting "this" */
  void fill(double value);
  Array2DT<PixTyp> *pChannel_[GA_MAX_CHANNELS];
  int noChannels_;

private:
  PixTyp minval_, maxval_;
  void clearGeo();

  double geoWest_, geoNorth_, geoEast_, geoSouth_;
  double geoResolution_;

  int cursor_x_, cursor_y_;

};

} // namespace Ga

#include "gaimaget.hpp"

#endif        // __GA_IMAGET_H
