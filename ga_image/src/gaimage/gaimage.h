/****************************************************************************
                          GaImage.h  -  geoAIDA template image class
                             -------------------
    begin                : Thu Jan 11 2001
    copyright            : (C) 2001 TNT, Uni Hannover
    authors              : Martin Pahl, Jürgen Bückner, Oliver Stahlhut
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
 * $Source: /data/cvs/ga_image/ga_image/gaimage/gaimage.h,v $
 * $Revision: 1.17 $
 * $Date: 2003/05/28 06:35:02 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef __GAIMAGE_H
#define __GAIMAGE_H

#include <typeinfo>
#include "gadefines.h"
#include "gaimagebase.h"

namespace Ga {

class ImageBase;

/** \class Image
    \brief class for handling geo-related images
    */
typedef const void* ConstIterator;
typedef void* Iterator;

class Image 
{
public:
  //--------------------------------- constructors ----------------------------

  /** defines a template matrix class Image for operations between matrices and vectors.
      Vector is a friend of Image and Image is the base class for ImageT
      usage: \code Image A; A = B; \endcode */
  Image(void);
  Image(const class std::type_info& t);

  /** copy constructor: defining and initialize from a matrix
      usage: \code Image A = B; Image A = B + 2 * C; return A \endcode */
  Image(const Image& rval);

  /** sizing a matrix and initialising it to zero, must be assigned other values afterwards (for a matrix of known dimension)
      usage: \code Image A( 4, 5 ); A( 1, 1 ) = 3; A( 1, 2 ) = 7; .... \endcode */
  Image(const class std::type_info& t, int x, int y, int noChannels=1);

  void init(const class std::type_info& t, int x, int y, int noChannels=1);

  /** create a submatrix with identical beginning (at [0,0])
	    usage: \code Image B( 4, 5, A ); \endcode
      \param width new x-size
      \param height new y-size
      \param rval input matrix */
  Image(int width, int height, const Image& rval );

  /** create submatrix starting at arg1, arg2 with size arg3 x arg4
      usage: \code Image B( m, n, i, j, A ); \endcode
      \param width new x-size
      \param height new y-size
      \param x0 x offset
      \param y0 y offset
      \param rval input matrix */
  Image( int x0, int y0, int width, int height, const Image& rval );

  /** destructor */
  ~Image(void);

  typedef const void* ConstIterator;
  typedef void* Iterator;
  bool isEmpty() const;
  const class std::type_info& typeId() const ;
  IMGTYPE typeImage() const ;
  void typeImage(IMGTYPE t);
  IMGTYPE readImageType(FILE *fp,int* cols, int* rows);
  bool read(const char* filename);
  bool read(FILE *fp);
  const char* comment();
  void write(const char* filename, int channel=0, const char* comment=0);
  void write(const char* filename, const char* comment);
  void write(FILE *fp, const char* comment);
  void write(FILE *fp, int channel=0, const char* comment=0);
	
  //------------------------------ Iterators ----------------------
  void* begin(int row=0, int channel=0);
  const void* constBegin(int row=0, int channel=0) const;
  const void* end(int row, int channel=0) const;
  const void* end() const;
  double getFloat(int x, int y, int channel, double neutral=0) const;
  void set(int x, int y, double val, int channel=0, bool clip=false);
  void fillRow(int row, int startX, int endX, double val, int channel=0, bool clip=false);
  double getFloat(const void *it) const;
  void set(void* it, double val);
  void nextCol(const void*& ptr) const;
  void nextCol(void*& ptr) const;
  void nextCol(const void*& ptr, int offset) const;
  void nextCol(void*& ptr, int offset) const;
  void prevCol(const void*& ptr) const;
  void prevCol(void*& ptr) const;
  void nextRow(const void*& ptr) const;
  void nextRow(void*& ptr) const;
  void prevRow(const void*& ptr) const;
  void prevRow(void*& ptr) const;

  //------------------------------ Access of matrix data ----------------------
  int sizeX() const;
  int sizeY() const;
  int sizeImage() const; 
  int noChannels() const;

  /** clear matrix */
  void clear();

  /** treats pointer this as if were a long integer */
  unsigned long hash(void) const;

  /** return pointer to the enclosed matrix */
  ImageBase* pImage() const;
  void pImage(ImageBase*);

  /** return pointer to the beginning of data
      usage: \code PixTyp* dp = M.Data(); \endcode */
  void* data(int row=0, int channel=0);

  /** return pointer to the beginning of data array
      usage: \code PixTyp* dp = M.Data(); \endcode */
  void* dataArray(int channel=0);

  /** retrieve the value of an matrix element
      usage: \code PixTyp x = A.GetElement( 3, 5 ); \endcode */
  double getFloat(int x, int y) const;

#if 0
  /** Assign value to matrix element, check for legal row and column index
      usage: \code  A.SetElement( 3, 4, 4444 );       \endcode
      \param x x of element (column)
      \param y y of element (row)
      \param val value to set element to */
  void setElement(int x, int y, double val);
#endif

  /** assignment of a matrix
      usage: \code  A=B; A=C+D; A=B=C+D; \endcode */
  Image& operator = (const Image& rval);

  /** assignment of matrix values (initialization) */
  Image& operator = (double rval);

  //----------------------------------- ALGEBRAIC OPERATIONS ------------------

  Image& add(const Image& lval, const Image& rval);
  Image& multElements(const Image& lval, const Image& rval);
  Image& mult(const Image& lval, const Image& rval);

  /** compare: If lval equals rval return 1, 0 otherwise
      usage: \code   if( A == B ) \endcode */
  friend bool operator == (const Image& lval, const Image& rval);

  /** Purpose: If it is != return 1 otherwise 0
      usage: \code   if( A != B ) \endcode */
  friend bool operator != (const Image& lval, const Image& rval);

  /** addition of matrices
      usage: \code  A = B + C; \endcode */
  friend Image  operator + (const Image& lval,const Image& rval);

  /** abbreviated form of sum
      usage: \code   A += B  => A = A + B; */
  Image &operator += (const Image& rval);

  /** Difference between matrices
      usage: \code  A = B - C; \endcode */
  friend Image  operator - (const Image& lval, const Image& rval);

  /** abbreviated form of difference between matrices
      usage: \code   A -= B; // => A = A - B; \endcode */
  Image &operator -= (const Image& rval);

  /** unary minus
      usage: \code   A = -B; \endcode */
  friend Image  operator - (const Image& rval);


  /** logical product of matrices
      usage: \code   A = B & C; \endcode */
  /*
  friend Image  operator & ( const Image& lval, const Image& rval);
 */

  /** logical sum of matrices
      usage: \code   A = B | C; \endcode */
  /*
  friend Image  operator | ( const Image& lval, const Image& rval);
  */

  /** logical xor of matrices
      usage: \code   A = B ^ C; \endcode */
  /*
  friend Image  operator ^ ( const Image& lval, const Image& rval);
  */

  /** Product between matrices
      usage: \code C = A * B \endcode */
  friend Image  operator * (const Image& lval, const Image& rval);

  /** Abbreviated form of product. Favour this alternative when possible!
      usage: \code   A *= B;  // =>  A = A * B; \endcode */
  Image& operator *= (const Image& rval);

  /**  Product of a PixTyp with a Image. Scalares Product
       usage: \code   A = 3 * B; \endcode */
  friend Image  operator * (double lval, const Image& rval);

  /**  Product of a PixTyp with a Image. Scalares Product
       usage: \code   A = 3 * B; \endcode */
  friend Image  operator * (const Image& rval, double lval);

  /** Multiplies and modifies a Image by a PixTyp.
      usage: \code  A *= 3.; \endcode */
  Image& operator *= (double rval);

  /** Dividing the coefficients of a matrix by a PixTyp
      usage: \code  A = B / 3.; \endcode */
  friend Image  operator / (const Image& lval, double rval);

  /** Dividing and modifying a Image by a PixTyp
      usage: \code  A /= 2.; \endcode */
  Image& operator /= (double rval);

  Image& concat(const Image im1, const Image im2, bool horizontal=true);
  Image& copyInto(const Image& im1, int offsetX, int offsetY);


  //------------------------------------ Tool functions -----------------------

  /** Writes a Image in a formatted file (ASCII)
      usage: \code  A.write( fp ); \endcode */
  void debug(FILE *fp); 

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


  /** Maximum element value of a Image
      usage: \code  double xmax = A.Max( channel=0 ); double xmax = A.Max( x, y, channel=0 ); \endcode */
  double matrixMax(int& x, int& y, int channel=0);
  double matrixMax(int channel=0);

  /** Absolute maximum of a Image
      usage: \code double xmax = A.MaxAbs( channel=0 ); double xmax = A.MaxAbs( x, y, channel=0 ); \endcode */
  double matrixAbsMax(int& x, int& y, int channel=0);

  /** Minimum of a Image
      usage: \code double xmin = A.Min( channel=0 ); \endcode */
  double matrixMin(int& x, int& y, int channel=0);

  double matrixMin(int channel=0);


  /** Absolute minimum of a Image
      usage: \code double xmin = A.MinAbs( ); double xmin = A.MinAbs( x, y ); \endcode */
  double matrixAbsMin(int& x, int& y, int channel=0);

#ifdef TODO
  /** Returns a const pointer to the data */
  const void* constData() const; 

  /** Frobenius norm( the square root of the sums of squares of all the coefficients ), uses the SqrtSumSqr from tools.cpp
    	usage: \code double normF = A.NormF( ); \endcode */
  /*
  double NormF(void);
  */

  /** eliminate a nonused matrix
	    usage: \code Delete( &A ); \endcode */
  /*
  friend void  Delete(Image* A);           // eliminate Matrix
  */

  /** changing the dimensions of a Image
    	usage: \code ChangeDimensions( x, y, &A ); \endcode */
  /*
  friend void  ChangeDimensions(int x, int y, Image *result,char zero = 0);
  */

  /** reloading a matrix stored by a formatted Save( file name )
    	usage: \code ReLoad( &x, file name ); \endcode */
  /*
  friend void  ReLoad(Image* A, char* filevector);
  */

  /** modifying a matrix into its transpose, if the matrix is square it replaces itself
	    usage: \code Transpose( &A ); \endcode */
  /*
  friend void  Transpose(Image* A);
  */

  /** swapping the elements of any two Matrices, Provides an efficient method of copying when a Image remains in the purpose and another one leaves, they swap
	    usage: \code     Swap( &A, &B ); \endcode */
  /*
  friend void  Swap(Image* lval, Image* rval);
  */

#endif
  // Image functions

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
  double geoRes() const;
  /** returns the x-image coordinate of a x-geocoordinate */
  int geo2ImageX(double geo) const;
  /** returns the x-image coordinate of a x-geocoordinate, caution: no range check */
  int geo2ImageX_f(double geo) const;
  /** returns the x-image coordinate of a x-geocoordinate, caution: no range check */
  double geo2ImageX_double(double geo) const;
  /** returns the y-image coordinate of a y-geocoordinate */
  int geo2ImageY(double geo) const;
  /** returns the y-image coordinate of a y-geocoordinate, caution: no range check */
  int geo2ImageY_f(double geo) const;
  /** returns the y-image coordinate of a y-geocoordinate, caution: no range check */
  double geo2ImageY_double(double geo) const;
  /** returns the geocoordinate of a x-image coordinate */
  double image2GeoX(double x) const;
  /** returns the geocoordinate of a y-image coordinate */
  double image2GeoY(double y) const;
  /** Calculate the bounding box in pixeln from given bounding box in geocoordinates */
  void imageBBox(double gW, double gN, double gE, double gS, int &llx,
                           int &lly, int &urx, int &ury) const;
  /** Calculate the bounding box in geocoordinates from given bounding box in pixeln */
  void geoBBox(int llx, int lly, int urx, int ury, double &gW, double &gN,
                    double &gE, double &gS) const;
  /** returns the number of image pixels covered by a geo-distance (only exact for 0 and 90 degree) */
  double geodist2pixel(double dist) const;
  /** returns the geo-distance for a number of pix image pixel (only exact for 0 and 90 degree) */
  double pixel2geodist(int pix);
  /** set a pixel using geocoordinates */
  void setGeo(double gx, double gy, int val, int channel=0);
  /** get a pixel using geocoordinates */
  int getGeoInt(double gx, double gy, int channel=0);
  /** set a pixel */
  void set(int x, int y, int val, int channel=0);
  /** set a pixel */
  void set(void *ptr, int val);
  /** get a pixel */
  int getInt(int x, int y, int channel=0) const;
  int getInt(int x, int y, int channel, int neutral) const;
  /** get a pixel */
  int getInt(const void *ptr) const;
  /** merge 'this' image with 'img'. The labels 'img_label' of 'img'
  	* are insert (overlayed) in 'this' with the value 'new_label' */
  void merge(const Image& img, double img_label, double new_label);
  /** merge 'this' image with 'img'. The labels 'img_label' of 'img'
  	* are insert (overlayed) in 'this' with the value 'new_label' */
  void merge(const Image& img, double img_label, double new_label,
	     int llx, int lly, int urx, int ury);
  /** merge 'this' image with 'img'. The labels 'img_label' of 'img'
  	* are insert (overlayed) in 'this' with the value 'new_label' */
  void geoMerge(const Image& img, int img_label, int new_label,
       int llx=-1, int lly=-1, int urx=-1, int ury=-1,
       int* nllx=0, int* nlly=0, int* nurx=0, int* nury=0,
       int* changeVec=0);

 /** extracts one band from a rgb-color image to a new single-band image
      \param band 0=red 1=green 2=blue, enum type can be used (e.g. Image<GA_NOTYPE>::red) */
  Image getChannel(int channel=0);

  /**  converts a 3-band image (rgb-color) to a luminance image */
  Image convert2Luminance();

  Image hsv();
   
   /** converts a 3-band image (irrgb-color) to single band ndvi-image */   
   Image convert2ndvi();
    

  /** resample the current image to size (x, y) */
  Image resample(int nx, int ny) const;

  /** resample the current image to size (x, y) using a nearest neighbour check */
  Image resampleNN(int nx, int ny) const;

  /** resample the current image to size (x, y) using a nearest neighbour check;
      this version uses a sophisticated class determination algorithm, which is
      especially useful for subsampling/shrinking an image, thus for enlargement */
  Image resampleNNplus(int nx, int ny) const;

  Image calcHistogram(double min, double max, int n, int channel=0);
  //blow 'iterations' * 'label' value
  void blow(int label, int iterations=1);
  //reduce all entries > 'value' to 'value'
  void maxValue(float value);
  //increase all entries < 'value' to 'value'
  void minValue(float value);
protected:

  /** allocation of memory and initialization of pointers, internal use only */
  void alloc( int x, int y, int noChannels=1 );

  /** initialization; common function for initialization; for internal use only
      usage: \code Initialize( x, y ) \endcode \endcode */
  void initialize( int x, int y, int noChannels=1 );

  /** deinitialization; common function for deinitialization (of pointer only !!) */
  void deinitialize(void);
	
  /** prepare assignments; Preparing an assignment by resizeing "this" */
  void resize(int rx, int ry, int noChannels=1);

  void fill(double value);

  ImageBase *pImage_;

};

#define ForTypeDo(PixTyp,function,args) \
  if (gaMatrixType==typeid(PixTyp)) {   \
    function<PixTyp>args;       \
  }
#define ForEachTypeDo(type_id,function,args) \
{ \
 const class std::type_info& gaMatrixType=type_id; \
 \
  ForTypeDo(bool, function, args)  \
  ForTypeDo(char, function, args)  \
  ForTypeDo(signed char, function, args)  \
  ForTypeDo(unsigned char, function, args)  \
  ForTypeDo(short, function, args)   \
  ForTypeDo(signed short, function, args)   \
  ForTypeDo(unsigned short, function, args) \
  ForTypeDo(int, function, args)     \
  ForTypeDo(signed int, function, args)     \
  ForTypeDo(unsigned int, function, args)   \
  ForTypeDo(float, function, args)          \
}

#define GenClassesIf(classname,t,type,elsepart) \
( t==typeid(type)            \
  ? new classname<type>     \
  : elsepart \
  )
#define GenClasses(classname,t,baseclass) \
  GenClassesIf(classname, t, bool,  \
  GenClassesIf(classname, t, char,  \
  GenClassesIf(classname, t, signed char,  \
  GenClassesIf(classname, t, unsigned char,  \
  GenClassesIf(classname, t, short,   \
  GenClassesIf(classname, t, signed short,   \
  GenClassesIf(classname, t, unsigned short, \
  GenClassesIf(classname, t, int,     \
  GenClassesIf(classname, t, signed int,     \
  GenClassesIf(classname, t, unsigned int,   \
  GenClassesIf(classname, t, float,          \
  (baseclass*)0 )))))))))))

} // namespace Ga

#endif        // __GA_IMAGE_H
