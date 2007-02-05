/***************************************************************************
                          gaimage.h  -  geoAIDA image class
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
 * $Source: /data/cvs/ga_image/ga_image/gaimage/gaimage.cpp,v $
 * $Revision: 1.33 $
 * $Date: 2006/07/12 09:01:30 $
 * $Author: mueller $
 * $Locker:  $
 */

// if defined asserts are deactivated
#define NDEBUG

#include <assert.h>
#include "gaimage.h"
#include "gaimaget.h"
#include "gaimagebase.h"

namespace Ga {

/** \class Image
    \brief class for handling geo-related images
    */
typedef const void* ConstIterator;
typedef void* Iterator;

//--------------------------------- constructors ----------------------------
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

  /** defines a template matrix class Image for operations between matrices and vectors.
     Vector is a friend of Image and Image is the base class for Image
     usage: \code Image A; A = B; \endcode */

Image::Image(void)
{
  pImage_=0;
}

Image::Image(const class std::type_info& t)
{
  pImage_=GenClasses(ImageT,t,ImageBase);
}

/** copy constructor: defining and initialize from a matrix
    usage: \code Image A = B; Image A = B + 2 * C; return A \endcode */
Image::Image(const Image& rval)
{
  if (rval.pImage_) pImage_=(ImageBase*)rval.pImage_->copyObject();
  else pImage_=0;
}

/** sizing a matrix and initialising it to zero, must be assigned other values afterwards (for a matrix of known dimension)
    usage: \code Image A( 4, 5 ); A( 1, 1 ) = 3; A( 1, 2 ) = 7; .... \endcode */
Image::Image(const class std::type_info& t, int x, int y, int noChannels)
{
  pImage_=GenClasses(ImageT,t,ImageBase);
  pImage_->initialize(x,y,noChannels);
}

void Image::init(const class std::type_info& t, int x, int y, int noChannels)
{
  pImage_=GenClasses(ImageT,t,ImageBase);
  pImage_->initialize(x,y,noChannels);
}

/** create a submatrix with identical beginning (at [0,0])
    usage: \code Image B( 4, 5, A ); \endcode
    \param width new x-size
    \param height new y-size
    \param rval input matrix */
Image::Image( int width, int height, const Image& rval )
{
  pImage_=(ImageBase*)rval.pImage_->newObject(width, height);
  pImage_->partCopy(*(rval.pImage_),0,0,width,height);
}

/** create submatrix starting at arg1, arg2 with size arg3 x arg4
    usage: \code Image B( m, n, i, j, A ); \endcode
    \param width new x-size
    \param height new y-size
    \param x0 x offset
    \param y0 y offset
    \param rval input matrix */
Image::Image( int x0, int y0, int width, int height, const Image& rval )
{
  pImage_=(ImageBase*)(rval.pImage_)->newObject(width,height,rval.noChannels());
  pImage_->partCopy(*(rval.pImage_),x0,y0,width,height);
}

/** destructor */
Image::~Image(void)
{
  if (pImage_)
    delete pImage_;
  pImage_=0;
}

const class std::type_info& Image::typeId() const {
  assert(pImage_!=0);
  return pImage_->typeId();
}

IMGTYPE Image::typeImage() const {
  assert(pImage_!=0);
  return pImage_->typeImage();
}

void Image::typeImage(IMGTYPE t) {
  assert(pImage_);
  pImage_->typeImage(t);
}

#undef GenClasses
#undef GenClassesIf

bool Image::isEmpty() const {
  if (pImage_==0) return true;
  if (sizeX()==0 || sizeY()==0) return true;
  return false;
}

bool Image::read(const char* filename) {
  FILE *fp=fopen(filename,"r");
  if (!fp) return(false);
  read(fp);
  fclose(fp);
	return(true);
}

IMGTYPE Image::readImageType(FILE *fp,int* cols, int* rows) {
	int pxmtype;
	IMGTYPE type=_UNKNOWN;
	int c, r;
	float minval,maxval;
	long pos=ftell(fp);
	if (pfm_readpfm_header(fp, &c, &r, &minval, &maxval, &pxmtype))
  type=(IMGTYPE)pxmtype;
	fseek(fp,pos,SEEK_SET);
	if (type==_UNKNOWN) {
		xelval max_x;
	  pnm_readpnminit(fp, &c, &r, &max_x, &pxmtype);
		fseek(fp,pos,SEEK_SET);
	  if (pxmtype == PBM_FORMAT || pxmtype == RPBM_FORMAT ||
	      pxmtype == PPM_FORMAT || pxmtype == RPPM_FORMAT ) {
	    switch(pxmtype) {
      case PBM_FORMAT:
      case RPBM_FORMAT:
        type=_PBM;
        break;
      case PGM_FORMAT:
      case RPGM_FORMAT:
        type=_PGM;
        break;
      case PPM_FORMAT:
      case RPPM_FORMAT:
        type=_PPM;
    	}
  	}
	}
	if (cols) *cols=c;
	if (rows) *rows=r;
	return type;
}

bool Image::read(FILE *fp) {
  if (!fp)
		return(false);
  if (!pImage_) {
    int cols,rows;
    IMGTYPE type=readImageType(fp,&cols,&rows);
    switch (type) {
    case _PFM_FLOAT:
      init(typeid(float),cols,rows,1);
      break;
    case _PFM_UINT16:
      init(typeid(unsigned short int),cols,rows,1);
      break;
    case _PFM_SINT16:
      init(typeid(signed short int),cols,rows,1);
      break;
    case _PFM_UINT:
      init(typeid(unsigned int),cols,rows,1);
      break;
    case _PFM_SINT:
      init(typeid(signed int),cols,rows,1);
      break;
    case _PGM:
      init(typeid(unsigned char),cols,rows,1);
      break;
    case _PPM:
      init(typeid(unsigned char),cols,rows,3);
      break;
    case _PBM:
      init(typeid(bool),cols,rows,1);
      break;
    default:
      return(false);
    }
  }
  assert(pImage_);
	return(pImage()->read(fp));
}

const char* Image::comment()
{
  return pfm_comment_get();
}

void Image::write(const char* filename, int channel, const char* comment) {
  FILE *fp=fopen(filename,"w");
  assert(fp);	
  write(fp, channel, comment);
  fclose(fp);
}

void Image::write(const char* filename, const char* comment) {
  write(filename,0,comment);
}

void Image::write(FILE *fp, const char* comment) {
  write(fp,0,comment);
}

  void Image::write(FILE *fp, int channel, const char* comment) {
  assert(pImage_);
  assert(fp);
  pImage()->write(fp, channel, comment);
}

//------------------------------ Iterators ----------------------
void* Image::begin(int row, int channel)  {
  assert(pImage_);
  return pImage_->beginVoid(row,channel);
};

const void* Image::constBegin(int row, int channel) const {
  assert(pImage_);
  return pImage_->constBeginVoid(row,channel);
};

const void* Image::end(int row, int channel) const {
  assert(pImage_);
  return pImage_->endVoid(row,channel);
};


const void* Image::end() const {
  assert(pImage_);
  return pImage_->endVoid();
};

double Image::getFloat(int x, int y, int channel, double neutral) const {
  assert(pImage_);
  return pImage_->getFloat(x,y,channel,neutral);
};

void Image::set(int x, int y, double val, int channel, bool clip) {
  assert(pImage_);
  return pImage_->setFloat(x,y,val,channel,clip);
};


double Image::getFloat(const void *it) const {
  assert(pImage_);
  return pImage_->getFloat(it);
};


void Image::set(void* it, double val) {
  assert(pImage_);
  return pImage_->setFloat(it,val);
};


void Image::nextCol(const void*& ptr) const {
  assert(pImage_);
  pImage_->nextCol(ptr);
}

void Image::nextCol(void*& ptr) const {
  assert(pImage_);
  pImage_->nextCol((const void*&)ptr);
}

void Image::nextCol(const void*& ptr, int offset) const {
  assert(pImage_);
  pImage_->nextCol(ptr,offset);
}

void Image::nextCol(void*& ptr, int offset) const {
  assert(pImage_);
  pImage_->nextCol((const void*&)ptr,offset);
}

void Image::prevCol(const void*& ptr) const {
  assert(pImage_);
  pImage_->prevCol(ptr);
}

void Image::prevCol(void*& ptr) const {
  assert(pImage_);
  pImage_->prevCol((const void*&)ptr);
}

void Image::nextRow(const void*& ptr) const {
  assert(pImage_);
  pImage_->nextRow(ptr);
};

void Image::nextRow(void*& ptr) const {
  assert(pImage_);
  pImage_->nextRow((const void*&)ptr);
};

void Image::prevRow(const void*& ptr) const {
  assert(pImage_);
  pImage_->prevRow(ptr);
};

void Image::prevRow(void*& ptr) const {
  assert(pImage_);
  pImage_->prevRow((const void*&)ptr);
};

//------------------------------ Access of matrix data ----------------------
int Image::sizeX() const {
  if (pImage_) return pImage_->sizeX();
  else return 0;
}

int Image::sizeY() const {
  if (pImage_) return pImage_->sizeY();
  else return 0;
}

int Image::sizeImage() const {
  if (pImage_) return pImage_->sizeImage();
  return 0;
}
	
int Image::noChannels() const {
  if (pImage_) return pImage_->noChannels();
  else return 0;
}

/** clear matrix */
void Image::clear()
{
  assert(pImage_!=0);
  pImage_->clear();
}

/** treats pointer this as if were a long integer */
unsigned long Image::hash(void) const { return (((unsigned long)this->pImage_)/3 ); }

/** return pointer to the beginning of data
    usage: \code PixTyp* dp = M.Data(); \endcode */
void* Image::data(int row, int channel) {
  assert(pImage_!=0);
  return pImage_->beginVoid(row,channel);
}

/** return pointer to the beginning of data array
    usage: \code PixTyp* dp = M.Data(); \endcode */
void* Image::dataArray(int channel) {
  assert(pImage_!=0);
  return pImage_->dataArray(channel);
}

/** retrieve the value of an matrix element
    usage: \code PixTyp x = A.GetElement( 3, 5 ); \endcode */
double Image::getFloat(int x, int y) const
{
  assert(pImage_!=0);
  return pImage_->getFloat(x,y);
}

#if 0
/** Assign value to matrix element, check for legal row and column index
    usage: \code  A.SetElement( 3, 4, 4444 );       \endcode
    \param x x of element (column)
    \param y y of element (row)
    \param val value to set element to */
void Image::setElement(int x, int y, double val)
{
  assert(pImage_!=0);
  pImage_->setFloat(x,y,val);
}
#endif

/** assignment of a matrix
    usage: \code  A=B; A=C+D; A=B=C+D; \endcode */
Image& Image::operator = (const Image& rval)
{
  if (this==&rval) return *this;
  if (pImage_) delete pImage_;
  if (rval.pImage_) pImage_=(ImageBase*)(rval.pImage_)->copyObject();
  else pImage_=0;
  return *this;
}

/** assignment of matrix values (initialization) */
Image&  Image::operator = (double rval)
{
  assert(pImage_!=0);
  pImage_->fill(rval);
  return *this;
}

//----------------------------------- ALGEBRAIC OPERATIONS ------------------

Image& Image::add(const Image& lval, const Image& rval) {
  assert(rval.pImage_!=0);
  assert(lval.pImage_!=0);
  if (pImage_) delete pImage_;
  pImage_=(ImageBase*)lval.pImage_->newObject(lval.sizeX(),lval.sizeY(),lval.noChannels());
  pImage_->add(*(lval.pImage_),*(rval.pImage_));
  return *this;
}

Image& Image::multElements(const Image& lval, const Image& rval) {
  assert(rval.pImage_!=0);
  assert(lval.pImage_!=0);
  if (pImage_) delete pImage_;
  pImage_=(ImageBase*)lval.pImage_->newObject(lval.sizeX(),lval.sizeY(),lval.noChannels());
  pImage_->multElements(*(lval.pImage_),*(rval.pImage_));
  return *this;
}

Image& Image::mult(const Image& lval, const Image& rval) {
  assert(rval.pImage_!=0);
  assert(lval.pImage_!=0);
  if (pImage_) delete pImage_;
  pImage_=(ImageBase*)lval.pImage_->newObject(lval.sizeX(),rval.sizeY(),lval.noChannels());
  pImage_->mult(*(lval.pImage_),*(rval.pImage_));
  return *this;
}

/** compare: If lval equals rval return 1, 0 otherwise
    usage: \code   if( A == B ) \endcode */
bool operator == (const Image& lval, const Image& rval)
{

  return lval.pImage_->eq(*rval.pImage_);
}

/** Purpose: If it is != return 1 otherwise 0
    usage: \code   if( A != B ) \endcode */
bool operator != (const Image& lval, const Image& rval)
{

  return lval.pImage_->neq(*rval.pImage_);
}

/** addition of matrices
    usage: \code  A = B + C; \endcode */
Image  operator + (const Image& lval,const Image& rval)
{
  assert(lval.pImage_!=0);
  assert(rval.pImage_!=0);
  Image result;
  result.pImage_=(ImageBase*)(lval.pImage_)->newObject(lval.sizeX(),lval.sizeY(),lval.noChannels());
  result.pImage_->add(*(lval.pImage_),*(rval.pImage_));
  return result;
}


/** abbreviated form of sum
    usage: \code   A += B  => A = A + B; */
Image &Image::operator += (const Image& rval)
{
  assert(rval.pImage_!=0);
  assert(pImage_!=0);
  pImage_->add(*pImage_,*(rval.pImage_));
  return *this;
}

/** Difference between matrices
    usage: \code  A = B - C; \endcode */
Image operator - (const Image& lval, const Image& rval)
{
  assert(lval.pImage_!=0);
  assert(rval.pImage_!=0);
  Image result;
  result.pImage_=(ImageBase*)(lval.pImage_)->newObject(lval.sizeX(),lval.sizeY(),lval.noChannels());
  result.pImage_->sub(*(lval.pImage_),*(rval.pImage_));
  return result;
}

/** abbreviated form of difference between matrices
    usage: \code   A -= B; // => A = A - B; \endcode */
Image &Image::operator -= (const Image& rval)
{
  assert(rval.pImage_!=0);
  assert(pImage_!=0);
  pImage_->sub(*pImage_,*(rval.pImage_));
  return *this;
}

/** unary minus
    usage: \code   A = -B; \endcode */
Image  operator - (const Image& rval)
{
  Image result;
  result.pImage_=(ImageBase*)rval.pImage_->newObject(rval.sizeX(), rval.sizeY());
  result.pImage_->neg(*(rval.pImage_));

  return result;
}


/** logical product of matrices
    usage: \code   A = B & C; \endcode */
/*
  friend Image  operator & ( const Image& lval, const Image& rval)
  {
    // cant AND matrices of distinct dimension
    assert(lval.size_x == rval.size_x);
    assert(lval.size_y == rval.size_y);

    //. init result matrix
    Image<PixTyp> result(lval.size_x, lval.size_y);

    //. adjust data pointer
    PixTyp* pLval = lval.pMatData[0];
    PixTyp* pRval = rval.pMatData[0];
    PixTyp* pResult = result.pMatData[0];
    for (int i = rval.nSize-1; i; i--)
      *pResult++ = (*pLval++) & (*pRval++);
    return result;
  }
 */

  /** logical sum of matrices
      usage: \code   A = B | C; \endcode */
  /*
  friend Image  operator | ( const Image& lval, const Image& rval)
  {
    // cant OR matrices of distinct dimension
    assert(lval.size_x == rval.size_x);
    assert(lval.size_y == rval.size_y);

    // init result matrix
    Image result(lval.size_x, lval.size_y);

    // adjust data pointer
    PixTyp* pLval = lval.pMatData[1]+1;
    PixTyp* pRval = rval.pMatData[1]+1;
    PixTyp* pResult = result.pMatData[1]+1;
    for (int i = rval.nSize-1; i; i--)
      *pResult++ = (PixTyp)((unsigned)(*pLval++) | (unsigned)(*pRval++));
    return result;
  }
  */

  /** logical xor of matrices
      usage: \code   A = B ^ C; \endcode */
  /*
  friend Image  operator ^ ( const Image& lval, const Image& rval)
  {
    // cant XOR matrices of distinct dimension
    assert(lval.size_x == rval.size_x);
    assert(lval.size_y != rval.size_y);

    // init result matrix
    Image result(lval.size_x, lval.size_y);

    // adjust data pointer
    PixTyp* pLval = lval.pMatData[1]+1;
    PixTyp* pRval = rval.pMatData[1]+1;
    PixTyp* pResult = result.pMatData[1]+1;
    for (int i = rval.nSize-1; i; i--)
      *pResult++ = PixTyp((*pLval++) ^ (*pRval++));
    return result;
  }
  */

/** Product between matrices
    usage: \code C = A * B \endcode */
Image operator * (const Image& lval, const Image& rval)
{
  assert(lval.pImage_!=0);
  assert(rval.pImage_!=0);
  Image result;
  result.pImage_=(ImageBase*)(lval.pImage_)->newObject(lval.sizeX(),lval.sizeY(),lval.noChannels());
  result.pImage_->mult(*(lval.pImage_),*(rval.pImage_));
  return result;
}

/** Abbreviated form of product. Favour this alternative when possible!
    usage: \code   A *= B;  // =>  A = A * B; \endcode */
Image& Image::operator *= (const Image& rval)
{
  assert(rval.pImage_!=0);
  assert(pImage_!=0);
  pImage_->mult(*pImage_,*(rval.pImage_));
  return *this;
}

/**  Product of a PixTyp with a Image. Scalares Product
     usage: \code   A = 3 * B; \endcode */
Image operator * (double lval, const Image& rval)
{
  assert(rval.pImage_!=0);
  Image result;
  result.pImage_=(ImageBase*)(rval.pImage_)->newObject(rval.sizeX(),rval.sizeY(),rval.noChannels());
  result.pImage_->mult(lval,*(rval.pImage_));
  return result;
}

/**  Product of a PixTyp with a Image. Scalares Product
     usage: \code   A = 3 * B; \endcode */
Image operator * (const Image& rval, double lval)
{
  assert(rval.pImage_!=0);
  Image result;
  result.pImage_=(ImageBase*)(rval.pImage_)->newObject(rval.sizeX(),rval.sizeY(),rval.noChannels());
  result.pImage_->mult(lval,*(rval.pImage_));
  return result;
}

/** Multiplies and modifies a Image by a PixTyp.
    usage: \code  A *= 3.; \endcode */
Image& Image::operator *= (double rval)
{
  assert(pImage_!=0);
  pImage_->mult(rval,*(pImage_));
  return *this;
}

/** Dividing the coefficients of a matrix by a PixTyp
    usage: \code  A = B / 3.; \endcode */
Image operator / (const Image& lval, double rval)
{
  assert(lval.pImage_!=0);
  Image result;
  result.pImage_=(ImageBase*)(lval.pImage_)->newObject(lval.sizeX(),lval.sizeY(),lval.noChannels());
  result.pImage_->div(*(lval.pImage_),rval);
  return result;
}

/** Dividing and modifying a Image by a PixTyp
    usage: \code  A /= 2.; \endcode */
Image& Image::operator /= (double rval)
{
  assert(pImage_!=0);
  pImage_->div(*(pImage_),rval);
  return *this;
}


Image& Image::concat(const Image im1, const Image im2, bool horizontal)
{
  assert(im1.pImage_!=0);
  assert(im2.pImage_!=0);
  if (pImage_) delete pImage_;
  pImage_=(ImageBase*)im1.pImage_->newObject(0,0,0);
  pImage_->concat(*(im1.pImage_),*(im2.pImage_),horizontal);
  return *this;
}

Image& Image::copyInto(const Image& im1, int offsetX, int offsetY)
{
  assert(im1.pImage_!=0);
  pImage_->copyInto(*(im1.pImage_),offsetX,offsetY);
  return *this;
}


//------------------------------------ Tool functions -----------------------

/** Writes a Image in a formatted file (ASCII)
    usage: \code  A.write( fp ); \endcode */
void Image::debug(FILE *fp) {
  assert(pImage_!=0);
  pImage_->debug(fp);
}

/** store the current minimum value of the matrix (useful for iterated access to this value) */
void Image::storeMinValue() { pImage_->storeMinValue();}
/** store the current minimum value of the matrix (useful for iterated access to this value) */
void Image::storeMaxValue() { pImage_->storeMaxValue();}

/** set the minimum value of the matrix to arg */
void Image::setMinValue(double v) { pImage_->setMinValue(v); }
/** set the maximum value of the matrix to arg */
void Image::setMaxValue(double v) { pImage_->setMaxValue(v); }

double Image::minValue() { return pImage_->minValue(); }
double Image::maxValue() { return pImage_->maxValue(); }


/** Maximum element value of a Image
    usage: \code  PixTyp xmax = A.Max( channel=0 ); PixTyp xmax = A.Max( x, y, channel=0 ); \endcode */
double Image::matrixMax(int& x, int& y, int channel)
{
  return pImage_->matrixMax(x,y,channel);
}

double Image::matrixMax(int channel)
{
  return pImage_->matrixMax(channel);
}

/** Absolute maximum of a Image
    usage: \code double xmax = A.MaxAbs( channel=0 ); double xmax = A.MaxAbs( x, y, channel=0 ); \endcode */
double Image::matrixAbsMax(int& x, int& y, int channel)
{
  return pImage_->matrixAbsMax(x,y,channel);
}

/** Minimum of a Image
    usage: \code double xmin = A.Min( channel=0 ); \endcode */
double Image::matrixMin(int& x, int& y, int channel)
{
  return pImage_->matrixMin(x,y,channel);
}

double Image::matrixMin(int channel)
{
  return pImage_->matrixMin(channel);
}


/** Absolute minimum of a Image
    usage: \code double xmin = A.MinAbs( ); double xmin = A.MinAbs( x, y ); \endcode */
double Image::matrixAbsMin(int& x, int& y, int channel)
{
  return pImage_->matrixAbsMin(x,y,channel);
}

#ifdef TODO
/** Returns a const pointer to the data */
const void* Image::constData() const {
  assert(pImage_!=0);
  return pImage_->dataVoid();
}

/** Frobenius norm( the square root of the sums of squares of all the coefficients ), uses the SqrtSumSqr from tools.cpp
    usage: \code double normF = A.NormF( ); \endcode */
/*
  double NormF(void)
  {
  return SqrtSumSqr( nSize-1, pMatData[0]+1 );
  }
*/

/** eliminate a nonused matrix
    usage: \code Delete( &A ); \endcode */
  /*
    friend void  Delete(Image* A)           // eliminate Matrix
    {
    result->Image::~Image( );
    }
  */

  /** changing the dimensions of a Image
    	usage: \code ChangeDimensions( x, y, &A ); \endcode */
  /*
  friend void  ChangeDimensions(int x, int y, Image *result,char zero = 0)
  {
    result->resize( x, y );

    if( zero == 0 && result->nSize != 0 )   // we want initialize to zero
    memset( result->pMatData[0], 0, result->nSize*sizeof( PixTyp ) );
  }
  */

  /** reloading a matrix stored by a formatted Save( file name )
    	usage: \code ReLoad( &x, file name ); \endcode */
  /*
  friend void  ReLoad(Image* A, char* filevector)
  {
    zzFileIn=fopen( filematrix, "r" );

    int x, y;
    fscanf( zzFileIn, "%d %d", &x, &y );

    ChangeDimensions( x, y, A, 1 );

    PixTyp *w = A->pMatData[0];
    for( unsigned int i = 1;i < A->nSize;i++ )
    fscanf( zzFileIn, "%d", ++w );

    fclose( zzFileIn );
  }
  */

  /** modifying a matrix into its transpose, if the matrix is square it replaces itself
	    usage: \code Transpose( &A ); \endcode */
  /*
  friend void  Transpose(Image* A)
  {
    int i, j;
    PixTyp **a=A->pMatData;
    if( A->size_y == A->size_x )                // squared matrix
    {
      for( i = 1;i <= A->size_y;i++ )
        for( j = i+1;j <= A->size_x;j++ )
        {
          // Swap values
          PixTyp tmp = a[i][j];
          a[i][j]   = a[j][i];
          a[j][i]   = tmp;
        }
    }
    else
    {
      Image B( '0', A->size_x, A->size_y );  // only for internal use
      for( i = 1;i <= A->size_y;i++ )
        for( j = 1;j <= A->size_x;j++ )
          B[j][i] = a[i][j];
        Swap( A, &B ); // avoid copying
    }
  }
  */

  /** swapping the elements of any two Matrices, Provides an efficient method of copying when a Image remains in the purpose and another one leaves, they swap
	    usage: \code     Swap( &A, &B ); \endcode */
  /*
  friend void  Swap(Image* lval, Image* rval)
  {
    PixTyp **temp = lval->pMatData;
    lval->pMatData = rval->pMatData;
    rval->pMatData = temp;
    Swap( &lval->size_x, &rval->size_x );  //in tools.cpp
    Swap( &lval->size_y, &rval->size_y );        // %
    Swap( &lval->nSize, &rval->nSize );            // %
  }
  */

#endif

/** allocation of memory and initialization of pointers, internal use only */
void Image::alloc( int x, int y, int noChannels)
{
  assert(pImage_!=0);
  pImage_->alloc(x,y,noChannels);
}

/** initialization; common function for initialization; for internal use only
    usage: \code Initialize( x, y ) \endcode \endcode */
void Image::initialize( int x, int y, int noChannels)
{
  assert(pImage_!=0);
  pImage_->initialize(x,y,noChannels);
}

/** deinitialization; common function for deinitialization (of pointer only !!) */
void Image::deinitialize(void)
{
  assert(pImage_!=0);
  pImage_->deinitialize();
}

/** prepare assignments; Preparing an assignment by resizeing "this" */
void Image::resize(int rx, int ry, int noChannels)
{
  assert(pImage_!=0);
  if (sizeX()!=rx || sizeY()!=ry) {
    pImage_->resize(rx,ry,1);
  }
}

void Image::fill(double value) {
  assert(pImage_!=0);
  pImage_->fill(value);
}

/** set geocoordinates for this image */
void Image::setGeoCoordinates(double gW, double gN, double gE, double gS)
{
  assert(pImage_!=0);
  pImage()->setGeoCoordinates(gW, gN, gE, gS);
}

/** get west/left geocoordinate */
double Image::geoWest() {
  assert(pImage_!=0);
  return pImage()->geoWest();
}

/** get north/top geocoordinate */
double Image::geoNorth() {
  assert(pImage_!=0);
  return pImage()->geoNorth();
}

/** get east/right geocoordinate */
double Image::geoEast() {
  assert(pImage_!=0);
  return pImage()->geoEast();
}

/** get south/bottom geocoordinate */
double Image::geoSouth() {
  assert(pImage_!=0);
  return pImage()->geoSouth();
}

/** set the resolution of this image unit [m/pixel] */
void Image::setGeoRes(double res) {
  assert(pImage_!=0);
  pImage()->setGeoRes(res);
}

/** get the resolution of this image [m/pixel] */
double Image::geoRes() const {
  assert(pImage_!=0);
  return pImage()->geoRes();
}

/** returns the x-image coordinate of a x-geocoordinate */
int Image::geo2ImageX(double geo) const {
  assert(pImage_!=0);
  return pImage()->geo2ImageX(geo);
}

/** returns the x-image coordinate of a x-geocoordinate, caution: no range check */
int Image::geo2ImageX_f(double geo) const {
  assert(pImage_!=0);
  return pImage()->geo2ImageX_f(geo);
}

/** returns the x-image coordinate of a x-geocoordinate, caution: no range check */
double Image::geo2ImageX_double(double geo) const {
  assert(pImage_!=0);
  return pImage()->geo2ImageX_double(geo);
}

/** returns the y-image coordinate of a y-geocoordinate */
int Image::geo2ImageY(double geo) const {
  assert(pImage_!=0);
  return pImage()->geo2ImageY(geo);
}

/** returns the y-image coordinate of a y-geocoordinate, caution: no range check */
int Image::geo2ImageY_f(double geo) const {
  assert(pImage_!=0);
  return pImage()->geo2ImageY_f(geo);
}

/** returns the y-image coordinate of a y-geocoordinate, caution: no range check */
double Image::geo2ImageY_double(double geo) const{
  assert(pImage_!=0);
  return pImage()->geo2ImageY_double(geo);
}

/** returns the geocoordinate of a x-image coordinate */
double Image::image2GeoX(double x) const {
  assert(pImage_!=0);
  return pImage()->image2GeoX(x);
}

/** returns the geocoordinate of a y-image coordinate */
double Image::image2GeoY(double y) const {
  assert(pImage_!=0);
  return pImage()->image2GeoY(y);
}

/** returns the number of image pixels covered by a geo-distance (only exact for 0 and 90 degree) */
double Image::geodist2pixel(double dist) const{
  assert(pImage_!=0);
  return pImage()->geodist2pixel(dist);
}

/** Calculate the bounding box in pixeln from given bounding box in geocoordinates */
void Image::imageBBox(double gW, double gN, double gE, double gS, int &llx,
                         int &lly, int &urx, int &ury) const
{
  llx = int (floor(geo2ImageX_double(gW) + 0.5));
  urx = int (floor(geo2ImageX_double(gE) - 0.5));
  lly = int (floor(geo2ImageY_double(gS) - 0.5));
  ury = int (floor(geo2ImageY_double(gN) + 0.5));
}

/** Calculate the bounding box in geocoordinates from given bounding box in pixeln */
void Image::geoBBox(int llx, int lly, int urx, int ury, double &gW, double &gN,
                    double &gE, double &gS) const
{
  gW = image2GeoX(llx - 0.5);
  gE = image2GeoX(urx + 0.5);
  gS = image2GeoY(lly + 0.5);
  gN = image2GeoY(ury - 0.5);
}

/** returns the geo-distance for a number of pix image pixel (only exact for 0 and 90 degree) */
double Image::pixel2geodist(int pix)
{
  assert(pImage_!=0);
  return pImage()->pixel2geodist(pix);
}

/** set a pixel using geocoordinates */
void Image::setGeo(double gx, double gy, int val, int channel) {
  assert(pImage_!=0);
  pImage()->setGeoInt(gx,gy,val,channel);
}

/** get a pixel using geocoordinates */
int Image::getGeoInt(double gx, double gy, int channel) {
  assert(pImage_!=0);
  return pImage()->getGeoInt(gx,gy,channel);
}

/** set a pixel */
void Image::set(int x, int y, int val, int channel) {
  assert(pImage_!=0);
  pImage()->setInt(x,y,val,channel);
}

/** set a pixel */
void Image::set(void *ptr, int val) {
  assert(pImage_!=0);
  pImage()->setInt(ptr,val);
}

/** get a pixel */
int Image::getInt(int x, int y, int channel) const {
  assert(pImage_!=0);
  return pImage()->getInt(x,y,channel);
}

/** get a pixel */
int Image::getInt(int x, int y, int channel, int neutral) const {
  assert(pImage_!=0);
  return pImage()->getInt(x,y,channel,neutral);
}

/** get a pixel */
int Image::getInt(const void *ptr) const {
  assert(pImage_!=0);
  return pImage()->getInt(ptr);
}

/** merge 'this' image with 'img'. The labels 'img_label' of 'img'
  * are insert (overlayed) in 'this' with the value 'new_label' */
void Image::merge(const Image& img, double img_label, double new_label) {
  assert(pImage_!=0);
  assert(img.pImage_!=0);
  if (img.typeId()==typeId())
    pImage()->merge(*(img.pImage_),img_label, new_label);
  else {
    assert(img.sizeX()==sizeX());
    assert(img.sizeY()==sizeY());
    void *dest=begin();
    const void *src=img.constBegin();
    const void *dest_end=end();
    for (;dest!=dest_end; nextCol(dest), img.nextCol(src)) {
      set(dest,img.getFloat(src));
    }
  }

}

/** merge 'this' image with 'img'. The labels 'img_label' of 'img'
  	* are insert (overlayed) in 'this' with the value 'new_label' */
void Image::geoMerge(const Image& img, int img_label, int new_label,
  int sllx, int slly, int surx, int sury,
  int* nllx, int* nlly, int* nurx, int* nury, int* changeVec)
{
#if 0
  int llx,lly,urx,ury;
#endif
  assert(pImage_!=0);
  assert(img.pImage_!=0);
  if (sllx==-1) {
    sllx=0;
    surx=img.sizeX()-1;
    slly=img.sizeY()-1;
    sury=0;
  }
  fprintf(stderr,"sllx... %4d %4d %4d %4d\n",sllx,slly,surx,sury);
#if 0
  llx=geo2ImageX(img.image2GeoX(sllx));
  lly=geo2ImageY(img.image2GeoY(slly));
  urx=geo2ImageX(img.image2GeoX(surx));
  ury=geo2ImageY(img.image2GeoY(sury));
  //folgende kann eigentlich weg - altes problem
  if (llx<0) llx=0;
  if (lly>sizeY()) lly=sizeY();
  if (urx>sizeX()) urx=sizeX();
  if (urx<0) urx=0;
  if (ury<0) ury=0;
  if (lly<0) lly=0;
#endif

  int dllx=sllx, dlly=slly, durx=surx, dury=sury;
  if ((img.sizeX()==sizeX()) && (img.sizeY()==sizeY()))
    merge(img,img_label,new_label);
  else {
#if 0
    float dx = float(img.sizeX())/float(geo2ImageX(img.image2GeoX(img.sizeX()))-geo2ImageX(img.image2GeoX(0)));
    float dy = float(img.sizeY())/float(geo2ImageY(img.image2GeoY(img.sizeY()))-geo2ImageY(img.image2GeoY(0)));
#endif
    double gW,gN,gE,gS;
    img.geoBBox(sllx,slly,surx,sury,gW,gN,gE,gS);
    imageBBox(gW,gN,gE,gS,dllx,dlly,durx,dury);
    if (dllx<0) dllx=0;
    if (dlly>sizeY()) dlly=sizeY();
    if (durx>sizeX()) durx=sizeX();
    if (dury<0) dury=0;
    float dx = float(surx-sllx+1)/float(durx-dllx+1);
    float dy = float(slly-sury+1)/float(dlly-dury+1);
fprintf(stderr,"%f %f %f %f\n",gW,gN,gE,gS);
fprintf(stderr,"dllx,...                    %4d %4d %4d %4d\n", dllx,dlly,durx,dury);
fprintf(stderr,"img-size %d %d, urx %d, llx %d, lly %d, ury %d, dx %f, dy %f\n",img.sizeX(),img.sizeY(),durx,dllx,dlly,dury,dx,dy);
//fprintf(stderr,"img-size %d %d, urx %d, llx %d, lly %d, ury %d, dx %f, dy %f\n",img.sizeX(),img.sizeY(),urx,llx,lly,ury,dx,dy);
//fprintf(stderr,"img.image2GeoX(0)=%f; geo2ImageX=%d\n",img.image2GeoX(0),geo2ImageX(img.image2GeoY(0)));
//fprintf(stderr,"img.image2GeoY(0)=%f; geo2ImageY=%d\n",img.image2GeoY(0),geo2ImageY(img.image2GeoY(0)));
    for (int y=dury; y<=dlly; y++) {
      int py = int((y-dury)*dy)+sury;
      for (int x=dllx; x<=durx; x++) {
        int px = int((x-dllx)*dx)+sllx;
//        fprintf(stderr,"%3d %3d %3d %3d\n",x,y,px,py);
          if (img.getInt(px,py) == img_label) {
            if (changeVec) { //aenderungen protokollieren
              if(getInt(x,y)) changeVec[getInt(x,y)]++; //
            }
            set(x,y,new_label);
          }
      }
    }
  }

  if(nllx) *nllx=dllx;
  if(nlly) *nlly=dlly;
  if(nurx) *nurx=durx;
  if(nury) *nury=dury;
}

/** extracts one band from a rgb-color image to a new single-band image
    \param band 0=red 1=green 2=blue, enum type can be used (e.g. Image<GA_NOTYPE>::red) */
Image Image::getChannel(int channel)
{
  Image result(typeId());
  pImage()->getChannel(*result.pImage(),channel);
  return result;
}

/**  converts a 3-band image (rgb-color) to a luminance image */
Image Image::convert2Luminance()
{
	Image result(typeId());
	pImage()->convert2Luminance(*result.pImage());
	return result;
}

Image Image::hsv()
{
  assert(pImage_!=0);
  Image result;
  result.pImage_=new ImageT<float>(pImage_->hsv());
  return result;
}

/** resample the current image to size (x, y) */
Image Image::resample(int nx, int ny) const
{
  assert(pImage_!=0);
  if (sizeX()==nx && sizeY()==ny) return *this;
  Image result(typeId());
  pImage()->resample(*result.pImage(),nx,ny);
  return result;
}

/** resample the current image to size (x, y) using a nearest neighbour check */
Image Image::resampleNN(int nx, int ny) const
{
  assert(pImage_!=0);
  if (sizeX()==nx && sizeY()==ny) return *this;
  Image result(typeId());
  pImage()->resampleNN(*result.pImage(),nx,ny);
  return result;
}

/** resample the current image to size (x, y) using a nearest neighbour check;
      this version uses a sophisticated class determination algorithm, which is
      especially useful for subsampling/shrinking an image, thus for enlargement */
Image Image::resampleNNplus(int nx, int ny) const
{
  assert(pImage_!=0);
  if (sizeX()==nx && sizeY()==ny) return *this;
  Image result(typeId());
  pImage()->resampleNNplus(*result.pImage(),nx,ny);
  return result;
}

Image Image::calcHistogram(double min, double max, int n, int channel)
{
  Image im;
  ImageT<int> *hist=new ImageT<int>;
  pImage()->calcHistogram(*hist,min,max,n,channel);
  im.pImage_=hist;
  return im;
}

void Image::blow(int label, int iterations)
{
  assert(pImage_!=0);
  pImage()->blow(*pImage(),label,iterations);
}

void Image::maxValue(float value)
{
  assert(pImage_!=0);
  pImage()->maxValue(*pImage(),value);
}

void Image::minValue(float value)
{
  assert(pImage_!=0);
  pImage()->minValue(*pImage(),value);
}

/** return pointer to the enclosed matrix */
ImageBase* Image::pImage() const {
  return (ImageBase*)pImage_;
}

void Image::pImage(ImageBase* p)
{
  pImage_=p;
}

} // namespace Ga


