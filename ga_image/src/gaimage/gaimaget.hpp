/***************************************************************************
                          GaImageT.hpp  -  geoAIDA template image class
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
 * $Source: /data/cvs/ga_image/ga_image/gaimage/gaimaget.hpp,v $
 * $Revision: 1.25 $
 * $Date: 2003/11/20 10:17:43 $
 * $Author: pahl $
 * $Locker:  $
 */

#include <stdarg.h>
namespace Ga {

  /** default constructor, defines a picture for later assignment */
  template <class PixTyp>
  inline ImageT<PixTyp>::ImageT() : ImageBase() {
    initialize( 0, 0, 1 );
    clearGeo();
  } 
  
  /** copy constructor, definition and initialization from a picture;
      usage: \code ImageT A = B; ImageT A = B + 2 * C; return A; \endcode */
  template <class PixTyp>
  inline ImageT<PixTyp>::ImageT (const ImageT<PixTyp>& rval) 
    : ImageBase(rval) {
    noChannels_=rval.noChannels_;
    for (int c=0; c<noChannels_; c++) {
      pChannel_[c]=rval.pChannel_[c]->shallowCopy();
    }
    typeImage(rval.typeImage());
    geoWest_=rval.geoWest_;
    geoNorth_=rval.geoNorth_;
    geoEast_=rval.geoEast_;
    geoSouth_=rval.geoSouth_;
    geoResolution_=rval.geoResolution_;
    cursor_x_=rval.cursor_x_;
    cursor_y_=rval.cursor_y_;
  }
  
template <class PixTyp>
inline ImageBase* ImageT<PixTyp>::newObject(int x_size, int y_size, int channels) {
  ImageT<PixTyp>* p=new ImageT<PixTyp>(x_size, y_size, channels);
  p->typeImage(typeImage());
  return p;
}
	
template <class PixTyp>
inline ImageBase* ImageT<PixTyp>::copyObject() {
  return new ImageT(*this);
}

template <class PixTyp>
inline ImageT<PixTyp>& ImageT<PixTyp>::operator= (const ImageT<PixTyp>& rval) {
  if (this==&rval) return *this;
  for (int c=0; c<noChannels_; c++) {
    assert(pChannel_[c]!=0);
    pChannel_[c]=pChannel_[c]->unlink();
  }
  (ImageBase&)*this=(ImageBase&)rval;
  noChannels_=rval.noChannels();
  for (int c=0; c<noChannels_; c++) {
    sizeX_=rval.sizeX_;
    sizeY_=rval.sizeY_;
    assert(rval.pChannel_[c]!=0);
    pChannel_[c]=rval.pChannel_[c]->shallowCopy();
  }	  	
  geoWest_=rval.geoWest_;
  geoNorth_=rval.geoNorth_;
  geoEast_=rval.geoEast_;
  geoSouth_=rval.geoSouth_;
  geoResolution_=rval.geoResolution_;
  cursor_x_=rval.cursor_x_;
  cursor_y_=rval.cursor_y_;
  return *this;
}

/** create picture from matrix
    usage: \code ga_pix A( y ); \endcode */
template <class PixTyp>
inline ImageT<PixTyp>::ImageT (const ImageBase& imgIn) : ImageBase(imgIn) {
  ImageT<PixTyp>& src=(ImageT<PixTyp>&)imgIn;
  noChannels_=src.noChannels_;
  for (int c=0; c<noChannels_; c++) {
    pChannel_[c]=src.pChannel_[c]->shallowCopy();
  }
  typeImage(src.typeImage());
  clearGeo();
}

/** size a picture and initialize it to zero; must then be assigned other values (for a picture of known dimension)
   usage: \code ImageT A( 4, 5 ); A( 1, 1 ) = 3; A( 1, 2 ) = 7; \endcode
   \param x x-size, number of columns
   \param y y-size, number of rows */
template <class PixTyp>
inline ImageT<PixTyp>::ImageT(int x, int y, int noChannels) : ImageBase() {
  initialize( x, y, noChannels );
  clearGeo();
}


/** create a picture from an array
    usage: \code int w[]={1, 2, 3, 4, 5, 6}; ImageT A( 2, 3, w ); \endcode
    \param x x-size, number of columns
    \param y y-size, number of rows */
template <class PixTyp>
inline ImageT<PixTyp>::ImageT (int x, int y, PixTyp *vec) : ImageBase()  {
  noChannels_=1;
  alloc( x, y, 1 );
  setData(x,y,vec);
  clearGeo();
}

/** create a picture from formatted File (ASCII)
    usage: \code ImageT A("MAT.DAT"); \endcode
    \param file filename */
template <class PixTyp>
inline ImageT<PixTyp>::ImageT (char *file) : ImageBase(file) {
  clearGeo();
}

/** create a subpicture with identical beginning (at [1,1])
    usage: \code ImageT B( 4, 5, A ); \endcode
    \param x new x-size, number of columns
    \param y new y-size, number of rows
    \param imgIn input picture */
template <class PixTyp>
inline ImageT<PixTyp>::ImageT (int x, int y, const ImageT& imgIn) : ImageBase() {
  assert(x >= 0);
  assert(x < imgIn.sizeX_);
  assert(y >= 0);
  assert(y < imgIn.sizeY_);

  typeImage(imgIn.typeImage());
  noChannels_=imgIn.noChannels_;
  initialize( x, y );

  for (int c=0; c<noChannels_; c++)
    for( int row = 0; row < sizeY_; row++ )
      memcpy( begin(row,c), imgIn.constBegin(row,c),sizeX_*sizeof(PixTyp) );
}

/** create subpicture starting at arg3, arg4 with size arg1 x arg2
    usage: \code ImageT B( m, n, i, j, A ); \endcode
    \param x new x-size, number of columns
    \param y new y-size, number of rows
    \param x0 starting offset x
    \param y0 starting offset y
    \param imgIn input picture */
template <class PixTyp>
inline ImageT<PixTyp>::ImageT (int x, int y, int x0, int y0, const ImageT& imgIn)
  : ImageBase() {
  	partCopy(imgIn,x,y,x0,y0);
}

/** destructor */
template <class PixTyp>
inline ImageT<PixTyp>::~ImageT(void)
{
  for (int c=0; c<noChannels_; c++)
    if (pChannel_[c]) pChannel_[c]=pChannel_[c]->unlink();
}

/* set data without copying. There must be noChannels data-pointer in the ellipse */
template <class PixTyp>
inline void ImageT<PixTyp>::setData(int x, int y, int noChannels, ...) {
  va_list argPtr;

  va_start(argPtr, noChannels);
  for (int c=0; c<noChannels_; c++)
    if (pChannel_[c]) pChannel_[c]=pChannel_[c]->unlink();
  noChannels_=noChannels;
  for (int c=0; c<noChannels_; c++) {
    if( x < 1 || y < 1 ) {
      sizeY_ = sizeX_ = nSize_ = 0;
    }
    else {
      sizeX_ = x;
      sizeY_ = y;
      nSize_ = (size_t) sizeX_ * sizeY_;
    }
    void* data=va_arg(argPtr,void*);
    pChannel_[c]=new Array2DT<PixTyp>(sizeX_, sizeY_,(PixTyp*)data);
  }

  va_end(argPtr);
}

/** create a matrix from an array
    usage: \code int w[]={1, 2, 3, 4, 5, 6}; ImageT A( 2, 3, w ); \endcode */
template <class PixTyp>
inline void ImageT<PixTyp>::setData(int x, int y, void* initvalues) {
  noChannels_=1;
  resize( x, y, 1 );

  PixTyp *w = begin();
  if( sizeY_ != 0 )
    memcpy( w, initvalues, ( nSize_ ) * sizeof( PixTyp ) );
}

template <class PixTyp>
inline void ImageT<PixTyp>::partCopy(const ImageBase &rvalue, int x0, int y0, int width, int height) {
  const ImageT& rval=(const ImageT&)rvalue;
  assert(width >= 0);
  assert(width < rval.sizeX_ - x0);
  assert(height >= 0);
  assert(height < rval.sizeY_ - y0);
  assert(x0 >= 0);
  assert(x0 < rval.sizeX_);
  assert(y0 >= 0);
  assert(y0 < rval.sizeY_);

  initialize( width, height, rval.noChannels_ );
  typeImage(rval.typeImage());
  for (int c=0; c<noChannels_; c++)
    for( int row = 0; row < sizeY_; row++ )
      memcpy( begin(row,c), rval.constBegin(row + y0,c) + x0, sizeX_ * sizeof(PixTyp) );
}

template <class PixTyp>
#if __GNUC__==2
inline const class type_info& ImageT<PixTyp>::typeId() const {
#else
inline const class std::type_info& ImageT<PixTyp>::typeId() const {
#endif
  return typeid(PixTyp);
}

template <class PixTyp>
inline int ImageT<PixTyp>::noChannels() const {
  return noChannels_;
}

template <class PixTyp>
inline void ImageT<PixTyp>::rmChannel() {
  if (noChannels()>0) {
    noChannels_--;
    assert(pChannel_[noChannels_]!=0);
    pChannel_[noChannels_]=pChannel_[noChannels_]->unlink();
  }
}

template <class PixTyp>
inline void ImageT<PixTyp>::addChannel(const ImageBase& rvalue, int channel) {
	const ImageT& rval=(const ImageT&)rvalue;
  assert(rval.sizeX()==sizeX());
  assert(rval.sizeY()==sizeY());
  pChannel_[noChannels_]=rval.pChannel_[channel]->shallowCopy();
  noChannels_++;
}

//------------------------------ Access of matrix data ----------------------
/** clear matrix */
template <class PixTyp>
inline void ImageT<PixTyp>::clear() {
  for (int c=0; c<noChannels_; c++) {
    assert(pChannel_[c]!=0);
    if( (nSize_ != 0) && (begin(0,c) != 0) )
      memset( begin(0,c), 0, nSize_*sizeof(PixTyp) );
  }	    	
}

/** treats pointer this as if were a long integer */
template <class PixTyp>
inline unsigned long ImageT<PixTyp>::hash(void) const {
  return (((unsigned long)this)/3 );
}

/** return pointer to the beginning of data
    usage: \code Iterator dp = M.begin(row); \endcode */
template <class PixTyp>
inline typename ImageT<PixTyp>::Iterator ImageT<PixTyp>::begin(int row, int channel) {
  assert(channel<noChannels_);
  assert(pChannel_[channel]!=0);
  return pChannel_[channel]->begin(pChannel_[channel],row);
}

/** return pointer to the beginning of data
    usage: \code PixTyp * dp = M.data(row); \endcode */
template <class PixTyp>
inline typename ImageT<PixTyp>::Iterator ImageT<PixTyp>::data(int row, int channel) {
  assert(channel<noChannels_);
  assert(pChannel_[channel]!=0);
  return pChannel_[channel]->begin(pChannel_[channel],row);
}

/** return pointer to the beginning of data
    usage: \code Iterator dp = M.dataArrayT(channel); \endcode */
template <class PixTyp>
inline PixTyp** ImageT<PixTyp>::dataArrayT(int channel) {
  assert(channel<noChannels_);
  assert(pChannel_[channel]!=0);
  return pChannel_[channel]->dataArray(pChannel_[channel]);
}

/** return pointer to the beginning of data
    usage: \code Iterator dp = M.dataArray(channel); \endcode */
template <class PixTyp>
inline void* ImageT<PixTyp>::dataArray(int channel) {
  assert(channel<noChannels_);
  assert(pChannel_[channel]!=0);
  return dataArrayT(channel);
}

template <class PixTyp>
inline typename ImageT<PixTyp>::ConstIterator ImageT<PixTyp>::constBegin(int row, int channel) const {
  assert(channel<noChannels_);
  assert(pChannel_[channel]!=0);
  return (pChannel_[channel]->constBegin(row));
}

template <class PixTyp>
inline void* ImageT<PixTyp>::beginVoid(int row, int channel) {
  return begin(row,channel);
}

template <class PixTyp>
inline typename ImageT<PixTyp>::ConstIterator ImageT<PixTyp>::end(int row, int channel) const {
  assert(channel<noChannels_);
  assert(pChannel_[channel]!=0);
  return (pChannel_[channel]->end(row));
}
	
template <class PixTyp>
inline typename ImageT<PixTyp>::ConstIterator ImageT<PixTyp>::end() const {
  assert(pChannel_[0]!=0);
  return (pChannel_[0]->end());
}
	
template <class PixTyp>
inline typename ImageT<PixTyp>::ConstIterator ImageT<PixTyp>::endChannel(int channel) const {
  assert(channel<noChannels_);
  assert(pChannel_[channel]!=0);
  return (pChannel_[channel]->end());
}

template <class PixTyp>
inline const void* ImageT<PixTyp>::endVoid(int row, int channel) const {
  return end(row,channel);
}
	
template <class PixTyp>
inline const void* ImageT<PixTyp>::endVoid() const {
  return end();
}
	
template <class PixTyp>
inline const void* ImageT<PixTyp>::constBeginVoid(int row, int channel) const {
  return constBegin(row,channel);
}
	
/** retrieve the value of an matrix element
    usage: \code PixTyp x = A.getFloat( 3, 5 ); \endcode */
template <class PixTyp>
inline PixTyp ImageT<PixTyp>::get(int x, int y, int channel, PixTyp neutral) const {
  assert(channel>=0);
  assert(channel<noChannels_);
  assert(pChannel_[channel]!=0);
  if ((x>=0) && (x<sizeX_) && (y>=0) && (y<sizeY_))
    return constBegin(y,channel)[x];
  else return neutral;
}

template <class PixTyp>
inline double ImageT<PixTyp>::getFloat(int x, int y, int channel, double neutral) const {
  return get(x,y,channel,PixTyp(neutral));
}
	

/** assign and retrieve matrix values
    usage: \code PixTyp x = A( 1, 5 ); A( 3, 7 ) = 5; \endcode */
template <class PixTyp>
inline PixTyp& ImageT<PixTyp>::operator () (int x, int y, int channel) {
  assert(channel>=0);
  assert(channel<noChannels_);
  assert(pChannel_[channel]!=0);
  assert(x>=0);
  assert(x<sizeX_);
  assert(y>=0);
  assert(y<sizeY_);
  return begin(y,channel)[x];
}

/** assign and retrieve matrix values
    usage: \code PixTyp x = A( 5 ); A( 3 ) = 5; \endcode
    \param nr number of element
    \return reference to an element */
template <class PixTyp>
inline PixTyp& ImageT<PixTyp>::operator () (unsigned int n) {
  assert(pChannel_[0]!=0);
  assert(n<nSize_);
  return begin()[n];
}

/** pointer to row y
    \param y row number */
template <class PixTyp>
inline typename ImageT<PixTyp>::Iterator ImageT<PixTyp>::operator [] (int y) {
  assert(pChannel_[0]!=0);
  assert(y>=0);
  assert(y<sizeY_);
  return begin(y);
}

/** Assign value to matrix element, check for legal row and column index
    usage: \code  A.SetElement( 3, 4, 4444 );       \endcode
    \param x x of element (column)
    \param y y of element (row)
    \param val value to set element to */
template <class PixTyp>
inline void ImageT<PixTyp>::set(int x, int y, PixTyp val, int channel, bool clip) {
  	assert(channel>=0);
  	assert(channel<noChannels_);
  	assert(pChannel_[channel]!=0);
  	if (clip)
  	{
  	  if (x >= 0 && x < sizeX_ && y >= 0 && y < sizeY_)
      	begin(y,channel)[x] = val;
      else
        return;
    }
    else
    {
    	assert(x>=0);
    	assert(x<sizeX_);
  	  assert(y>=0);
    	assert(y<sizeY_);
      begin(y,channel)[x] = val;
    }
}

template <class PixTyp>
inline void ImageT<PixTyp>::setFloat(int x, int y, double val, int channel, bool clip) {
  set(x,y,PixTyp(val),channel,clip);
}

template <class PixTyp>
inline double ImageT<PixTyp>::getFloat(const void *it) const {
	return (double)(*(PixTyp*)it);
}

template <class PixTyp>
inline void ImageT<PixTyp>::setFloat(void* it, double val) {
	*(PixTyp*)it=(PixTyp)val;
}

template <class PixTyp>
inline void ImageT<PixTyp>::fillRow(void *it, int startX, int endX, double val, int channel) {
  PixTyp* ptr=(PixTyp*)it;
  PixTyp* eptr=ptr+endX;
  ptr+=startX;
  PixTyp v=PixTyp(val);
  for (;ptr<=eptr;ptr++) {
    *ptr=v;
  }
}

template <class PixTyp>
inline void* ImageT<PixTyp>::nextCol(const void*& ptr, int offset) const {
  return ((PixTyp*)ptr)+offset;
}

template <class PixTyp>
inline void* ImageT<PixTyp>::nextCol(const void*& ptr) const {
  return ((PixTyp*)ptr)+1;
}

template <class PixTyp>
inline void* ImageT<PixTyp>::prevCol(const void*& ptr) const {
	return ((PixTyp*)ptr)-1;
}

template <class PixTyp>
inline void* ImageT<PixTyp>::nextRow(const void*& ptr) const {
	return ((PixTyp*)ptr)+sizeX();
}

template <class PixTyp>
inline void* ImageT<PixTyp>::prevRow(const void*& ptr) const {
	return ((PixTyp*)ptr)-sizeX();
}



/** assignment of matrix values (initialization) */
template <class PixTyp>
inline ImageT<PixTyp>& ImageT<PixTyp>::operator = (const PixTyp rval) {
  for (int c=0; c<noChannels_; c++) {
    assert(pChannel_[c]!=0);
    Iterator w=begin(0,c);
    for(unsigned int i = nSize_ - 1; i; i--, w ++)
      *w = rval;
    *w = rval;
  }	
  return *this;
}

	
template <class PixTyp>
template <class OperatorClass>
inline void ImageT<PixTyp>::binaryPixelOp(const ImageT<PixTyp>& lval, const ImageT<PixTyp>& rval, OperatorClass opClass ) {
  assert(lval.sizeX_==rval.sizeX_);
  assert(lval.sizeY_==rval.sizeY_);
  // init result matrix
  (ImageBase&)*this=(ImageBase&)lval;
  resize(lval.sizeX_, lval.sizeY_, lval.noChannels_);
  for (int c=0; c<noChannels_; c++) {
    // can not connect matrices of distinct dimension
	 assert(lval.pChannel_[c]!=0);
    assert(rval.pChannel_[c]!=0);

    // adjust data pointer
	 ConstIterator pLval = lval.constBegin(0,c);
    ConstIterator pRval = rval.constBegin(0,c);
    Iterator pResult = begin(0,c);
    for ( int i = rval.nSize_-1; i; i-- )  //????
					    *pResult++ =(PixTyp) opClass( *pLval++,*pRval++ );
  }
}

template <class PixTyp>
template <class OperatorClass>
inline void ImageT<PixTyp>::unaryPixelOp(const ImageT<PixTyp>& lval, OperatorClass opClass ) {
  (ImageBase&)*this=(ImageBase&)lval;
  resize(lval.sizeX_, lval.sizeY_,lval.noChannels_);
  for (int c=0; c<noChannels_; c++) {
    assert(lval.pChannel_[c]!=0);
    ConstIterator w=lval.constBegin(0,c);
	    Iterator r=begin(0,c);
	    for( int i = nSize_-1; i; i-- )
	      r[i] =PixTyp (opClass(w[i]));
  }	    	
}

template <class PixTyp>
inline void ImageT<PixTyp>::add(const ImageBase& lval, const ImageBase& rval) {
  assert(lval.typeId()==rval.typeId());
  binaryPixelOp((const ImageT<PixTyp>&) lval, (const ImageT<PixTyp>&) rval, OpAdd());
}
	
template <class PixTyp>
inline void ImageT<PixTyp>::multElements(const ImageBase& lval, const ImageBase& rval) {
  assert(lval.typeId()==rval.typeId());
  binaryPixelOp((const ImageT<PixTyp>&) lval, (const ImageT<PixTyp>&) rval, OpMult());
}
	
template <class PixTyp>
inline void ImageT<PixTyp>::sub(const ImageBase& lval, const ImageBase& rval) {
  assert(lval.typeId()==rval.typeId());
  binaryPixelOp((const ImageT<PixTyp>&) lval, (const ImageT<PixTyp>&) rval, OpSub());
}
	
template <class PixTyp>
inline void ImageT<PixTyp>::divElements(const ImageBase& lval, const ImageBase& rval) {
  assert(lval.typeId()==rval.typeId());
  binaryPixelOp((const ImageT<PixTyp>&) lval, (const ImageT<PixTyp>&) rval, OpDiv());
}
	
template <class PixTyp>
inline void ImageT<PixTyp>::mult(const ImageBase& lval, double rval) {
  assert(lval.typeId()==typeId());
  unaryPixelOp((const ImageT<PixTyp>&) lval, OpConstMult(PixTyp(rval)));
}
	
template <class PixTyp>
inline void ImageT<PixTyp>::mult(double rval, const ImageBase& lval) {
  assert(lval.typeId()==typeId());
  mult(lval,rval);
}
	
template <class PixTyp>
inline void ImageT<PixTyp>::div(const ImageBase& lval, double rval) {
  assert(lval.typeId()==typeId());
  unaryPixelOp((const ImageT<PixTyp>&) lval, OpConstMult(1/PixTyp(rval)));
}
	
template <class PixTyp>
inline void ImageT<PixTyp>::div(double lval, const ImageBase& rval) {
  assert(rval.typeId()==typeId());
  unaryPixelOp((const ImageT<PixTyp>&) rval, OpConstDiv(PixTyp(lval)));
}
	
template <class PixTyp>
inline void ImageT<PixTyp>::neg(const ImageBase& rval) {
  assert(rval.typeId()==typeId());
  unaryPixelOp((const ImageT<PixTyp>&) rval, OpNeg());
}
	
template <class PixTyp>
inline bool ImageT<PixTyp>::eq(const ImageBase& rval) {
  assert(rval.typeId()==typeId());
  return *this==(const ImageT<PixTyp>&)rval;
}

template <class PixTyp>
inline bool ImageT<PixTyp>::neq(const ImageBase& rval) {
  assert(rval.typeId()==typeId());
  return *this!=(const ImageT<PixTyp>&)rval;
}

template <class PixTyp>
inline void ImageT<PixTyp>::mult(const ImageBase& lvalue, const ImageBase& rvalue) {
  assert(lvalue.typeId()==rvalue.typeId());
  assert(typeId()==lvalue.typeId());
  const ImageT<PixTyp>& lval=(const ImageT<PixTyp>&) lvalue;
  const ImageT<PixTyp>& rval=(const ImageT<PixTyp>&) rvalue;
  (ImageBase&)*this=(ImageBase&)lval;
  assert(lval.sizeX_==rval.sizeY_);
  resize( lval.sizeX_, rval.sizeY_, lval.noChannels_ );
  for (int c=0; c<noChannels_; c++) {
    assert(lval.pChannel_[c]!=0);
    assert(rval.pChannel_[c]!=0);
    // cant multiplicate operands of distinct dimension
	
    Iterator pDataResult = begin(0,c);
    ConstIterator pDataRval   = rval.constBegin(0,c);
    int nxsRval = rval.sizeX();
    int nyLval = lval.sizeY();
    int nxsLval = lval.sizeX();
    for( int y = 0; y < nyLval; y++ ) {
      for( int x = 0; x < nxsRval; x++ ) {
				ConstIterator tmp = pDataRval + x;
				ConstIterator pDataLval = lval.constBegin(y,c);
				PixTyp sum = (PixTyp) 0.0;
				for( int i = 0; i < nxsLval; i++ ) {
				  sum += ( *++pDataLval ) * ( *tmp );
				  tmp += nxsRval;
				}
				*pDataResult = ( PixTyp )sum;
				pDataResult++;
      }
    }
  }
}

/** compare: If lval equals rval return 1, 0 otherwise
    usage: \code   if( A == B ) \endcode */
template <class PixTyp>
inline bool operator == (const ImageT<PixTyp>& lval, const ImageT<PixTyp>& rval) {
  if( lval.hash()==rval.hash() )
    return TRUE;
  if( lval.sizeY_!=rval.sizeY_ || lval.sizeX_!=rval.sizeX_ )
    return FALSE;
  for (int c=0;  c<lval.noChannels_; c++) {
    assert(lval.pChannel_[c]);
    assert(rval.pChannel_[c]);
    if( memcmp( lval.constBegin(0,c), rval.constBegin(0,c), (rval.nSize_-1)*sizeof(PixTyp) )==0 )//Gleich
      return TRUE;
    else
      return FALSE;
  }
  return (lval.noChannels_==rval.noChannels_);
}

/** Purpose: If it is != return 1 otherwise 0
    usage: \code   if( A != B ) \endcode */
template <class PixTyp>
inline int  operator != (const ImageT<PixTyp>& lval,const ImageT<PixTyp>& rval) {
  if( lval.hash()==rval.hash() )
    return FALSE;

  if( lval.sizeY_!=rval.sizeY_ || lval.sizeX_!=rval.sizeX_ )
    return TRUE;
  for (int c=0;  c<lval.noChannels_; c++) {
    assert(lval.pChannel_[c]);
    assert(rval.pChannel_[c]);
    if( memcmp( lval.constBegin(0,c), rval.constBegin(0,c), (rval.nSize_-1)*sizeof(PixTyp) )==0 ) //Gleich
      return FALSE;
    else
    	  return TRUE;
  }
  return (lval.noChannels_!=rval.noChannels_);
}

/** addition of matrices
    usage: \code  A = B + C; \endcode */
template <class PixTyp>
inline ImageT<PixTyp>  operator + (const ImageT<PixTyp>& lval,const ImageT<PixTyp>& rval) {
  // init result matrix
  ImageT<PixTyp> result( lval.sizeX_, lval.sizeY_, lval.noChannels_ );
  result.add(lval,rval);
  return result;
}


/** abbreviated form of sum
    usage: \code   A += B  => A = A + B; */
template <class PixTyp>
inline ImageT<PixTyp>& ImageT<PixTyp>::operator += (const ImageT& rval) {
  assert(sizeX_==rval.sizeX_);
  assert(sizeY_==rval.sizeY_);
  for (int c=0; c<noChannels_; c++) {
    assert(rval.pChannel_[c]);
    // cant add matrices of distinct dimensions

    Iterator pData = begin(0,c);

    // if a += a;
    if( this == &rval ) {
      for( int k = nSize_-1; k; k-- ) //????
	( *pData++ ) += ( *pData );
    }
    else { // a += b;
      ConstIterator pRval = rval.constBegin(0,c);
      for( int i = nSize_-1; i; i-- ) //????
	( *pData++ ) += ( *pRval++ );
    }
  }

  return *this;
}

/** Difference between matrices
    usage: \code  A = B - C; \endcode */
template <class PixTyp>
inline ImageT<PixTyp> operator - (const ImageT<PixTyp>& lval, const ImageT<PixTyp>& rval) {
  // cant subtract matrices of distinct dimension
  assert(lval.sizeX_==rval.sizeX_);
  assert(lval.sizeY_==rval.sizeY_);

  // init result matrix
  ImageT<PixTyp> result( lval.sizeX_, lval.sizeY_, lval.noChannels_ );
  result.add(lval,rval);
  return result;
}

/** abbreviated form of difference between matrices
    usage: \code   A -= B; // => A = A - B; \endcode */
template <class PixTyp>
inline ImageT<PixTyp>& ImageT<PixTyp>::operator -= (const ImageT& rval) {
  // cant subtract matrices of distinct dimensions
  assert(sizeX_==rval.sizeX_);
  assert(sizeY_==rval.sizeY_);
  assert(noChannels_==rval.noChannels_);
  for (int c=0; c<noChannels_; c++) {
    assert(rval.pChannel_[c]);

    Iterator pData = begin(0,c);

    // if a -= a;
    if( this == &rval ) {
      for ( int k = nSize_-1; k; k-- )
	( *pData++ ) = 0;
    }
    else  { // a -= b;
      ConstIterator pRval = rval.constBegin(0,c);
      for( int i = nSize_-1; i; i-- )
	( *pData++ ) -= ( *pRval++ );
    }
  }

  return *this;
}

/** unary minus
    usage: \code   A = -B; \endcode */
template <class PixTyp>
inline ImageT<PixTyp> operator- (const ImageT<PixTyp>& rval) {
  ImageT<PixTyp> result( rval.sizeX_, rval.sizeY_, rval.noChannels_ );
  for (int c=0; c<result.noChannels_; c++) {
    typename ImageT<PixTyp>::ConstIterator w=rval.constBegin(0,c);
    typename ImageT<PixTyp>::Iterator *r=result.begin(0,c);
    for( int i = result.nSize_-1; i; i-- )
      r[i] =PixTyp (-w[i]);
  }

  return result;
}


  /** logical product of matrices
      usage: \code   A = B & C; \endcode */
  /*
  friend ImageT  operator & ( const ImageT& lval, const ImageT& rval)
  {
    // cant AND matrices of distinct dimension
    assert(lval.sizeX_ == rval.sizeX_);
    assert(lval.sizeY_ == rval.sizeY_);

    //. init result matrix
    ImageT<PixTyp> result(lval.sizeX_, lval.sizeY_);

    //. adjust data pointer
    Iterator pLval = lval.pMatData[0];
    Iterator pRval = rval.pMatData[0];
    Iterator pResult = result.pMatData[0];
    for (int i = rval.nSize_-1; i; i--)
      *pResult++ = (*pLval++) & (*pRval++);
    return result;
  }
 */

  /** logical sum of matrices
      usage: \code   A = B | C; \endcode */
  /*
  friend ImageT  operator | ( const ImageT& lval, const ImageT& rval)
  {
    // cant OR matrices of distinct dimension
    assert(lval.sizeX_ == rval.sizeX_);
    assert(lval.sizeY_ == rval.sizeY_);

    // init result matrix
    ImageT result(lval.sizeX_, lval.sizeY_);

    // adjust data pointer
    Iterator pLval = lval.pMatData[1]+1;
    Iterator pRval = rval.pMatData[1]+1;
    Iterator pResult = result.pMatData[1]+1;
    for (int i = rval.nSize_-1; i; i--)
      *pResult++ = (PixTyp)((unsigned)(*pLval++) | (unsigned)(*pRval++));
    return result;
  }
  */

  /** logical xor of matrices
      usage: \code   A = B ^ C; \endcode */
  /*
  friend ImageT  operator ^ ( const ImageT& lval, const ImageT& rval)
  {
    // cant XOR matrices of distinct dimension
    assert(lval.sizeX_ == rval.sizeX_);
    assert(lval.sizeY_ == rval.sizeY_);

    // init result matrix
    ImageT result(lval.sizeX_, lval.sizeY_);

    // adjust data pointer
    Iterator pLval = lval.pMatData[1]+1;
    Iterator pRval = rval.pMatData[1]+1;
    Iterator pResult = result.pMatData[1]+1;
    for (int i = rval.nSize_-1; i; i--)
      *pResult++ = PixTyp((*pLval++) ^ (*pRval++));
    return result;
  }
  */

/** Product between matrices
    usage: \code C = A * B \endcode */
template <class PixTyp>
inline ImageT<PixTyp> operator * (const ImageT<PixTyp>& lval, const ImageT<PixTyp>& rval) {
  // cant multiplicate operands of distinct dimension
  assert(lval.sizeX_==rval.sizeY_ );
  assert(lval.noChannels_==rval.noChannels_);
  ImageT<PixTyp> result( lval.sizeX_, rval.sizeY_, rval.noChannels_ );
	result.mult(lval,rval);
  return result;
}

/** Abbreviated form of product. Favour this alternative when possible!
    usage: \code   A *= B;  // =>  A = A * B; \endcode */
 template <class PixTyp>
   inline ImageT<PixTyp>& ImageT<PixTyp>::operator *= (const ImageT& rval) {
   //.different dimensions of operands ??
   assert(sizeX_==rval.sizeY_);
   assert(noChannels_==rval.noChannels_);
   
   if ( this == &rval ) {
     ImageT C = ( *this ) * ( *this );
     Swap( this, &C );
   }
   else {
     // if rval is not square *this changes dimensions
     if( rval.sizeY_ == rval.sizeX_ ) {
       for (int c=0; c<noChannels_; c++) {
	 int nc = rval.sizeX_;                      // number of x in the right matrix
	 ConstIterator pDataRval= rval.begin(0,c);          // pointer to data of right matrix
	 Iterator pMove;                                // a moving pointer
	 Iterator pData = begin(0,c);                  // pointer to data of this
	 Iterator pArray = new PixTyp[sizeX_];// a temporary array
	 
	 for( int y = 0; y < sizeY_; y++ ) {
	   memcpy( pArray, begin(y,c), ( sizeX_ )*sizeof( PixTyp ) );
	   for( int x = 0; x < sizeX_; x++ ) {
	     pMove    = pDataRval + x;
	     //long sum = 0;
	     PixTyp sum = 0;
	     for( int i = 0; i < sizeX_; i++ ) {
	       sum  += pArray[i] * ( *pMove );
	       pMove += nc;
	     }
	     *++pData = ( PixTyp )sum;
	   }
	 }
	 delete pArray;
       }
     }
     else {
       ImageT result = ( *this ) * rval;
       Swap( this, &result );  // avoids copy
     }
   }
   return *this;
 }

/**  Product of a PixTyp with a ImageT. Scalares Product
     usage: \code   A = 3 * B; \endcode */
template <class PixTyp>
inline ImageT<PixTyp> operator* (double lval, ImageT<PixTyp>& rval) {
  ImageT<PixTyp> result( rval.sizeX_, rval.sizeY_, rval.noChannels_ );
  for (int c=0; c<result.noChannels_; c++) {
    typename ImageT<PixTyp>::Iterator pDataResult = result.begin(0,c);
    typename ImageT<PixTyp>::ConstIterator pDataRval   = rval.constBegin(0,c);
    for( int i = rval.nSize_-1; i; i-- )
      *pDataResult++ = PixTyp( lval * ( *pDataRval++ ) );
  }
  return result;
}

/** Multiplies and modifies a ImageT by a PixTyp.
    usage: \code  A *= 3.; \endcode */
template <class PixTyp>
inline ImageT<PixTyp>& ImageT<PixTyp>::operator *= (PixTyp rval) {
  for (int c=0; c<noChannels_; c++) {
    Iterator pData = begin(0,c);
    for( int i = nSize_-1; i; i-- ) {
      *pData = PixTyp ( rval * ( *pData ) );
      pData++;
    }
  }	
  return *this;
}

/** Dividing the coefficients of a matrix by a PixTyp
    usage: \code  A = B / 3.; \endcode */
template <class PixTyp>
inline ImageT<PixTyp>  operator / (const ImageT<PixTyp>& lval, PixTyp rval) {
	assert(rval!=0);

  ImageT<PixTyp> result( lval.sizeX_, lval.sizeY_, lval.noChannels_ );
  for (int c=0; c<result.noChannels_; c++) {
    typename ImageT<PixTyp>::Iterator pData = result.begin(0,c);
    typename ImageT<PixTyp>::ConstIterator pLval = lval.constBegin(0,c);
    for( int i = lval.nSize_-1; i; i-- )
      *pData++ = PixTyp ( ( *pLval++ )/rval );
  }

  return result;
}

/** Dividing and modifying a ImageT by a PixTyp
    usage: \code  A /= 2.; \endcode */
template <class PixTyp>
inline ImageT<PixTyp>& ImageT<PixTyp>::operator /= (PixTyp rval) {
  assert(rval!=0);

  for (int c=0; c<noChannels_; c++) {
    Iterator pData = begin(0,c);
    for( int i = nSize_-1; i; i-- ) {
      *pData = PixTyp (( *pData )/rval );
      pData++;
    }
  }	

  return *this;
}


//------------------------------------ Tool functions -----------------------

/** Writes a ImageT in a formatted file (ASCII)
    usage: \code  A.Write( fp ); \endcode */

template <class PixTyp>
inline void ImageT<PixTyp>::debug(FILE *fp) {               // formatted
  for (int c=0; c<noChannels_; c++) {
    fprintf( fp, "F0 %d # PFM Float ASCII\n",noChannels_-c);
    fprintf( fp, "# xsize ysize\n");
    fprintf( fp, " %d %d\n", sizeX_, sizeY_ );
    fprintf( fp, "# min max\n");
    fprintf( fp, "%f %f\n",(double)minValue(),(double)maxValue());
    for( int y = 0; y < sizeY_; y++ ) {
      for( int x = 0; x < sizeX_; x++ )
	fprintf( fp, " %f", (double)begin(y,c)[x] );
      fprintf( fp, "\n" );
    }
  }
}

template <class PixTyp>
void ImageT<PixTyp>::concat(const ImageBase& im1, const ImageBase& im2, bool horizontal)
{
  ImageT<PixTyp>& img1=(ImageT<PixTyp>&) im1;
  ImageT<PixTyp>& img2=(ImageT<PixTyp>&) im2;
  if (horizontal) {
    int sizeX=im1.sizeX()+im2.sizeX();
    int sizeY=(im1.sizeY()>im2.sizeY() ? im1.sizeY() : im2.sizeY());
    int noChannels=(im1.noChannels()>im2.noChannels() ? im1.noChannels() : im2.noChannels());
    resize(sizeX,sizeY,noChannels);
    for (int c=0; c<img1.noChannels(); c++)
      for (int y=0; y<img1.sizeY(); y++)
        memcpy(begin(y,c),img1.constBegin(y,c),img1.sizeX()*sizeof(PixTyp));
    for (int c=0; c<img2.noChannels(); c++)
      for (int y=0; y<img2.sizeY(); y++)
        memcpy(begin(y,c)+img1.sizeX(),img2.constBegin(y,c),img2.sizeX()*sizeof(PixTyp));
  }
  else {
    int sizeX=(im1.sizeX()>im2.sizeX() ? im1.sizeX() : im2.sizeX());
    int sizeY=im1.sizeY()+im2.sizeY();
    int noChannels=(im1.noChannels()>im2.noChannels() ? im1.noChannels() : im2.noChannels());
    resize(sizeX,sizeY,noChannels);
    for (int c=0; c<img1.noChannels(); c++)
      for (int y=0; y<img1.sizeY(); y++)
        memcpy(begin(y,c),img1.constBegin(y,c),img1.sizeX()*sizeof(PixTyp));
    for (int c=0; c<img2.noChannels(); c++)
      for (int y=0; y<img2.sizeY(); y++)
        memcpy(begin(y+img1.sizeY(),c),img2.constBegin(y,c),img2.sizeX()*sizeof(PixTyp));
  }
}

template <class PixTyp>
void ImageT<PixTyp>::copyInto(const ImageBase& im1, int offsetX, int offsetY)
{
  ImageT<PixTyp>& img1=(ImageT<PixTyp>&) im1;
  int sizey=im1.sizeY();
  if (sizey+offsetY>sizeY()) sizey=sizeY()-offsetY;
  int sizex=im1.sizeX();
  if (sizex+offsetX>sizeX()) sizex=sizeX()-offsetY;
  int noCh=(im1.noChannels()<noChannels() ? im1.noChannels() : noChannels());
  if (sizey<=0 || sizex<=0) return;
  for (int c=0; c<noCh; c++)
    for (int y=0; y<sizey; y++)
      memcpy(begin(y+offsetY,c)+offsetX,img1.constBegin(y,c),sizex*sizeof(PixTyp));
}

/** store the current minimum value of the matrix (useful for iterated access to this value) */
template <class PixTyp>
inline void ImageT<PixTyp>::storeMinValue() {
  minval_ = (PixTyp)matrixMin();
}

/** store the current minimum value of the matrix (useful for iterated access to this value) */
template <class PixTyp>
inline void ImageT<PixTyp>::storeMaxValue() {
  maxval_ = (PixTyp)matrixMax();
}

/** set the minimum value of the matrix to arg */
template <class PixTyp>
inline void ImageT<PixTyp>::setMinValue(double v) {
  minval_ = (PixTyp)v;
}

/** set the maximum value of the matrix to arg */
template <class PixTyp>
inline void ImageT<PixTyp>::setMaxValue(double v) {
  maxval_ = (PixTyp)v;
}

template <class PixTyp>
inline double ImageT<PixTyp>::minValue() const {
  return minval_;
}

template <class PixTyp>
inline double ImageT<PixTyp>::maxValue() const {
  return maxval_;
}


/** Maximum element value of a ImageT
    usage: \code  PixTyp xmax = A.Max( channel=0 ); PixTyp xmax = A.Max( x, y, channel=0 ); \endcode */
template <class PixTyp>
inline double ImageT<PixTyp>::matrixMax(int& x, int& y, int channel) {
#if 0
  if(nSize_ < 0)
    return *pMatData[0];
#endif

  ConstIterator index = constBegin(0, channel);
  PixTyp max = *index;
  unsigned int e = 0;

  for(unsigned int i = 1; i < nSize_; i++) {
    if (max < *index) {
      max = *index;
      e = i;
    }
    ++ index;
  }

  y = e / sizeX_;
  x = e % sizeX_;

  return max;
}

template <class PixTyp>
inline double ImageT<PixTyp>::matrixMax(int channel) {
  if(nSize_ < 0)
    return *(begin());

  ConstIterator index = constBegin(0, channel);
  PixTyp max = *index;
  unsigned int e = 0;

  for(unsigned int i = 1; i < nSize_; i++) {
    if (max < *index) {
      max = *index;
      e = i;
    }
    ++ index;
  }

  return max;
}

/** Absolute maximum of a ImageT
    usage: \code PixTyp xmax = A.MaxAbs( channel=0 ); PixTyp xmax = A.MaxAbs( x, y, channel=0 ); \endcode */
template <class PixTyp>
inline double ImageT<PixTyp>::matrixAbsMax(int& x, int& y, int channel) {
#if 0
  if(nSize_ < 0)
    return *pMatData[0];
#endif

  ConstIterator index = constBegin(0, channel);
  PixTyp max = *index;
  unsigned int e = 0;

  for(unsigned int i = 1; i < nSize_; i++) {
    if (max < *index) {
      max = *index;
      e = i;
    }
    ++ index;
  }
  
  y = e / sizeX_;
  x = e % sizeX_;

  return max;
}

/** Minimum of a ImageT
    usage: \code PixTyp xmin = A.Min( channel=0 ); \endcode */
template <class PixTyp>
inline double ImageT<PixTyp>::matrixMin(int& x, int& y, int channel) {
#if 0
  if(nSize_ < 0)
    return *(begin());
#endif

  ConstIterator index = constBegin(0, channel);
  PixTyp min = *index;
  unsigned int e = 0;

  for(unsigned int i = 1; i < nSize_; i++) {
    if (min > *index) {
      min = *index;
      e = i;
    }
    ++ index;
  }

  y = e / sizeX_;
  x = e % sizeX_;

  return min;
  }

template <class PixTyp>
inline double ImageT<PixTyp>::matrixMin(int channel) {
  if(nSize_ < 0)
    return *(begin());

  ConstIterator index = constBegin(0, channel);
  PixTyp min = *index;
  unsigned int e = 0;

  for(unsigned int i = 1; i < nSize_; i++) {
    if (min > *index) {
      min = *index;
      e = i;
    }
    ++ index;
  }

  return min;
}


/** Absolute minimum of a ImageT
    usage: \code PixTyp xmin = A.MinAbs( channel=0 ); PixTyp xmin = A.MinAbs( x, y, channel=0 ); \endcode */
template <class PixTyp>
inline double ImageT<PixTyp>::matrixAbsMin(int& x, int& y, int channel) {
  if (nSize_ < 0)
    return *(begin());

  ConstIterator index = begin(0, channel);
  PixTyp min = *index;
  unsigned int e = 0;

  for(unsigned int i = 1; i < nSize_; i++) {
    if (min > Abs(*index)) {
      min = Abs(*index);
      e = i;
    }
    ++ index;
  }

  y = e / sizeX_;
  x = e % sizeX_;

  return min;
}

  /** Frobenius norm( the square root of the sums of squares of all the coefficients ), uses the SqrtSumSqr from tools.cpp
      usage: \code double normF = A.NormF( ); \endcode */
  /*
  double NormF(void)
  {
    return SqrtSumSqr( nSize_-1, pMatData[0]+1 );
  }
  */

  /** eliminate a nonused matrix
	    usage: \code Delete( &A ); \endcode */
  /*
  friend void  Delete(ImageT* A)           // eliminate Matrix
  {
    result->ImageT::~ImageT( );
  }
  */

  /** changing the dimensions of a ImageT
    	usage: \code ChangeDimensions( x, y, &A ); \endcode */
  /*
  friend void  ChangeDimensions(int x, int y, ImageT *result,char zero = 0)
  {
    result->resize( x, y );

    if( zero == 0 && result->nSize_ != 0 )   // we want initialize to zero
    memset( result->pMatData[0], 0, result->nSize_*sizeof( PixTyp ) );
  }
  */

  /** reloading a matrix stored by a formatted Save( file name )
    	usage: \code ReLoad( &x, file name ); \endcode */
  /*
  friend void  ReLoad(ImageT* A, char* filevector)
  {
    zzFileIn=fopen( filematrix, "r" );

    int x, y;
    fscanf( zzFileIn, "%d %d", &x, &y );

    ChangeDimensions( x, y, A, 1 );

    PixTyp *w = A->pMatData[0];
    for( unsigned int i = 1;i < A->nSize_;i++ )
    fscanf( zzFileIn, "%d", ++w );

    fclose( zzFileIn );
  }
  */

  /** modifying a matrix into its transpose, if the matrix is square it replaces itself
	    usage: \code Transpose( &A ); \endcode */
  /*
  friend void  Transpose(ImageT* A)
  {
    int i, j;
    PixTyp **a=A->pMatData;
    if( A->sizeY_ == A->sizeX_ )                // squared matrix
    {
      for( i = 1;i <= A->sizeY_;i++ )
        for( j = i+1;j <= A->sizeX_;j++ )
        {
          // Swap values
          PixTyp tmp = a[i][j];
          a[i][j]   = a[j][i];
          a[j][i]   = tmp;
        }
    }
    else
    {
      ImageT B( '0', A->sizeX_, A->sizeY_ );  // only for internal use
      for( i = 1;i <= A->sizeY_;i++ )
        for( j = 1;j <= A->sizeX_;j++ )
          B[j][i] = a[i][j];
        Swap( A, &B ); // avoid copying
    }
  }
  */

  /** swapping the elements of any two Matrices, Provides an efficient method of copying when a ImageT remains in the purpose and another one leaves, they swap
	    usage: \code     Swap( &A, &B ); \endcode */
  /*
  friend void  Swap(ImageT* lval, ImageT* rval)
  {
    PixTyp **temp = lval->pMatData;
    lval->pMatData = rval->pMatData;
    rval->pMatData = temp;
    Swap( &lval->sizeX_, &rval->sizeX_ );  //in tools.cpp
    Swap( &lval->sizeY_, &rval->sizeY_ );        // %
    Swap( &lval->nSize_, &rval->nSize_ );            // %
  }
  */

/** allocation of memory and initialization of pointers, internal use only */
template <class PixTyp>
inline void ImageT<PixTyp>::alloc( int x, int y, int noChannels ) {
  for (int c=0; c<noChannels_; c++)
    if (pChannel_[c]) pChannel_[c]=pChannel_[c]->unlink();
  noChannels_=noChannels;
  for (int c=0; c<noChannels_; c++) {
    if( x < 1 || y < 1 ) {
      sizeY_ = sizeX_ = nSize_ = 0;
    }
    else {
      sizeX_ = x;
      sizeY_ = y;
      nSize_ = (size_t) sizeX_ * sizeY_;
    }
    pChannel_[c]=new Array2DT<PixTyp>(sizeX_, sizeY_);
  }
}

/** initialization; common function for initialization; for internal use only
    usage: \code Initialize( x, y ) \endcode \endcode */
template <class PixTyp>
inline void ImageT<PixTyp>::initialize( int x, int y, int noChannels ) {
  noChannels_=noChannels;
  for (int c=0; c<noChannels_; c++) pChannel_[c]=0;
  alloc( x, y, noChannels_ );
  imageType_ = _UNKNOWN;
}

/** deinitialization; common function for deinitialization (of pointer only !!) */
template <class PixTyp>
inline void ImageT<PixTyp>::deinitialize(void) {
  if (noChannels_==0) return;
  for (int c=0; c<noChannels_; c++) {
    if( pChannel_[c] == 0 ) continue;
    pChannel_[c]=pChannel_[c]->unlink();
  }
}

/** prepare assignments; Preparing an assignment by adjusting "this" */
template <class PixTyp>
inline void ImageT<PixTyp>::resize(int rx, int ry, int noChannels) {
  if( sizeY_ != ry || sizeX_ != rx || noChannels_ != noChannels ) {
    deinitialize();
    alloc( rx, ry, noChannels );
  }
}

template <class PixTyp>
inline void ImageT<PixTyp>::fill(double value) {
	*this=(PixTyp)value;
}

/** Returns the size of one element */
template <class PixTyp>
inline int ImageT<PixTyp>::elemSize()
{
  return sizeof(PixTyp);
}

/** set geocoordinates for this image */
template <class PixTyp>
inline void ImageT<PixTyp>::setGeoCoordinates(double gW, double gN, double gE, double gS) {
    geoWest_ = gW; geoNorth_ = gN; geoEast_ = gE; geoSouth_ = gS;
}

/** get west/left geocoordinate */
template <class PixTyp>
inline double ImageT<PixTyp>::geoWest() {
  return geoWest_;
}

/** get north/top geocoordinate */
template <class PixTyp>
inline double ImageT<PixTyp>::geoNorth() {
  return geoNorth_;
}

/** get east/right geocoordinate */
template <class PixTyp>
inline double ImageT<PixTyp>::geoEast() {
  return geoEast_;
}

/** get south/bottom geocoordinate */
template <class PixTyp>
inline double ImageT<PixTyp>::geoSouth() {
  return geoSouth_;
}

/** set the resolution of this image unit [m/pixel] */
template <class PixTyp>
inline void ImageT<PixTyp>::setGeoRes(double res) {
  geoResolution_ = res;
}

/** get the resolution of this image [m/pixel] */
template <class PixTyp>
inline double ImageT<PixTyp>::geoRes() {
  return geoResolution_;
}

/** returns the x-image coordinate of a x-geocoordinate */
template <class PixTyp>
inline int ImageT<PixTyp>::geo2ImageX(double geo) {
  double f1 = geoEast_ - geoWest_, f2 = geo - geoWest_;
  assert(geoEast_ > geoWest_);
  //assert(f2 <= f1);
  //assert(f2 >= 0);

//!MP10.07.01  return (int)(f2 / f1 * (double)sizeX_ + 0.5);
  return (int)(f2 / f1 * (double)sizeX_);
}

/** returns the x-image coordinate of a x-geocoordinate, caution: no range check */
template <class PixTyp>
inline int ImageT<PixTyp>::geo2ImageX_f(double geo) {
//!MP10.07.01  return (int)((geo - geoWest_) / (geoEast_ - geoWest_) * (double)sizeX_ + 0.5);
  return (int)((geo - geoWest_) / (geoEast_ - geoWest_) * (double)sizeX_);
}

/** returns the x-image coordinate of a x-geocoordinate, caution: no range check */
template <class PixTyp>
inline double ImageT<PixTyp>::geo2ImageX_double(double geo) {
//!MP10.07.01  return ((geo - geoWest_) / (geoEast_ - geoWest_) * (double)sizeX_);
  return ((geo - geoWest_) / (geoEast_ - geoWest_) * (double)sizeX_);
}

/** returns the y-image coordinate of a y-geocoordinate */
template <class PixTyp>
inline int ImageT<PixTyp>::geo2ImageY(double geo) {
//!MP10.07.01  double f1 = geoNorth_ - geoSouth_, f2 = geoNorth_ - geo;
  assert(geoNorth_ > geoSouth_);
  //assert(f2 <= f1);
  //assert(f2 >= 0);

//!MP10.07.01  return (int)(f2 / f1 * (double)sizeY_ + 0.5);
  return (int)(sizeY_/(geoSouth_-geoNorth_)*(geo-geoNorth_));
}

/** returns the y-image coordinate of a y-geocoordinate, caution: no range check */
template <class PixTyp>
inline int ImageT<PixTyp>::geo2ImageY_f(double geo) {
//!MP10.07.01  return (int)((geoNorth_ - geo) / (geoNorth_ - geoSouth_) * (double)sizeY_ + 0.5);
  return (int)(sizeY_/(geoSouth_-geoNorth_)*(geo-geoNorth_));
}

/** returns the y-image coordinate of a y-geocoordinate, caution: no range check */
template <class PixTyp>
inline double ImageT<PixTyp>::geo2ImageY_double(double geo) {
//!MP10.07.01  return ((geoNorth_ - geo) / (geoNorth_ - geoSouth_) * (double)sizeY_);
  return sizeY_/(geoSouth_-geoNorth_)*(geo-geoNorth_);
}

/** returns the geocoordinate of a x-image coordinate */
template <class PixTyp>
inline double ImageT<PixTyp>::image2GeoX(double x) {
//!MP11.07.01  assert(x >= 0);
//!MP11.07.01  assert(x < sizeX_);
  assert(sizeX_);

//!MP10.07.01  return ( geoWest_ + (geoEast_ - geoWest_) * (double)x / (double)sizeX_ );
  return (x+0.5)*(geoEast_-geoWest_)/sizeX_+geoWest_;
}

/** returns the geocoordinate of a y-image coordinate */
template <class PixTyp>
inline double ImageT<PixTyp>::image2GeoY(double y) {
//!MP11.07.01  assert(y >= 0);
//!MP11.07.01  assert(y < sizeY_);
  assert(sizeY_);

//!MP10.07.01  return ( geoNorth_ - (geoNorth_ - geoSouth_) * (double)(y) / (double)sizeY_ );
  return (geoSouth_-geoNorth_)/sizeY_*(y+0.5)+geoNorth_;
}

/** set a pixel using geocoordinates */
template <class PixTyp>
inline void ImageT<PixTyp>::setGeoInt(double gx, double gy, int val, int channel) {
  int x = geo2ImageX(gx);
  int y = geo2ImageY(gy);
  set(x,y,val,channel);
}

/** get a pixel using geocoordinates */
template <class PixTyp>
inline int ImageT<PixTyp>::getGeoInt(double gx, double gy, int channel) {
  int x = geo2ImageX(gx);
  int y = geo2ImageY(gy);
  return getInt(x,y,channel);
}

/** set a pixel  */
template <class PixTyp>
inline void ImageT<PixTyp>::setInt(void *ptr, int val) {
  PixTyp *p=(PixTyp*)ptr;
  *p=PixTyp(val);
}

/** set a pixel  */
template <class PixTyp>
inline void ImageT<PixTyp>::setInt(int x, int y, int val, int channel) {
  Iterator p=begin(y,channel)+x;
  setInt(p,val);
}

/** set a pixel  */
template <class PixTyp>
inline int ImageT<PixTyp>::getInt(const void *ptr) const {
  PixTyp *p=(PixTyp*)ptr;
  return int(*p);
}

/** set a pixel  */
template <class PixTyp>
inline int ImageT<PixTyp>::getInt(int x, int y, int channel) const {
  ConstIterator p=constBegin(y,channel)+x;
  return getInt(p);
}

template <class PixTyp>
inline int ImageT<PixTyp>::getInt(int x, int y, int channel, int neutral) const {
  return int(get(x,y,channel,neutral));
}
	

/** returns the number of image pixels covered by a geo-distance (only exact for 0 and 90 degree) */
template <class PixTyp>
inline double ImageT<PixTyp>::geodist2pixel(double dist) {
  assert(geoResolution_ > 0.0);

  return (dist / geoResolution_);
}

/** returns the geo-distance for a number of pix image pixel (only exact for 0 and 90 degree) */
template <class PixTyp>
inline double ImageT<PixTyp>::pixel2geodist(int pix) {
  return (pix * geoResolution_);
}

/** merge 'this' image with 'img'. The labels 'img_label' of 'img'
  * are insert (overlayed) in 'this' with the value 'new_label' */
template <class PixTyp>
inline void ImageT<PixTyp>::merge(ImageBase& image, double img_label, double new_label) {
	ImageT<PixTyp>& img=(ImageT<PixTyp>&)image;
	PixTyp label=(PixTyp)img_label;
	PixTyp newLabel=(PixTyp)new_label;
  PixTyp *p_img = img.begin();
  PixTyp *p_data = this->begin();
  int size = this->sizeImage();
  if (size == img.sizeImage())
    for (int i=0; i<=size; i++) {
      if (*p_img == label) *p_data = newLabel;
      p_img++;
      p_data++;
    }
}

/** extracts one band from a multi-band image to a new single-band image
     \param band 0=red 1=green 2=blue, enum type can be used (e.g. ImageT<GA_NOTYPE>::red) */

template <class PixTyp>
inline void ImageT<PixTyp>::getChannel(ImageBase& resultImg, int channel)
{
  ImageT<PixTyp>& pic=(ImageT<PixTyp>&)resultImg;
	pic.resize(sizeX(), sizeY());
  pic.setGeoCoordinates(geoWest(), geoNorth(), geoEast(), geoSouth());
  pic.setGeoRes(geoRes());

  pic.pChannel_[0] = pChannel_[channel]->shallowCopy();
}

/**  converts rgb-color image to a new luminance image */
 template <class PixTyp>
   inline void ImageT<PixTyp>::convert2Luminance(ImageBase& resultImg)
   {
     assert(noChannels()==3);
     
     ImageT<PixTyp> &pic = (ImageT<PixTyp> &)resultImg;
     
     pic.resize(sizeX(), sizeY());
     pic.setGeoCoordinates(geoWest(), geoNorth(), geoEast(), geoSouth());
     pic.setGeoRes(geoRes());
     
     if (typeImage()==_PPM)
       pic.typeImage(_PGM);
     Iterator elem = pic.begin();
     //    ConstIterator elem_rval = constBegin(0,channel);
     ConstIterator pChR=constBegin(0,0);
     ConstIterator pChG=constBegin(0,1);
     ConstIterator pChB=constBegin(0,2);
     for (int i = 0; i < sizeImage(); ++i, ++elem, ++pChR, ++pChG, ++pChB) {
       *elem = (PixTyp) ( (double)*pChR *0.3 +
			  (double)*pChG *0.59 +
			  (double)*pChB *0.11 + 0.5);
     }
   }
   
   /** converts a 3-band image (irrgb-color) to a new single band ndvi-image */   
 template <class PixTyp>
   inline void ImageT<PixTyp>::convert2ndvi(ImageBase& resultImg)
   {
     assert(noChannels()==3);
     
     ImageT<float> &pic = (ImageT<float> &)resultImg;
     
     pic.resize(sizeX(), sizeY());
     pic.setGeoCoordinates(geoWest(), geoNorth(), geoEast(), geoSouth());
     pic.setGeoRes(geoRes());
     
     if (typeImage()==_PPM)
       pic.typeImage(_PGM);
      ImageT<float>::Iterator elem = pic.begin();
     //    ConstIterator elem_rval = constBegin(0,channel);
     ConstIterator pChIR=constBegin(0,0);
     ConstIterator pChR=constBegin(0,1);
     for (int i = 0; i < sizeImage(); ++i, ++elem, ++pChIR, ++pChR) {
       *elem = (float) ( (double)((double) *pChIR - (double) *pChR)/
			  ((double) *pChIR + (double) * pChR) );

     }
   }

/** get the hue of an 3 band color image */
template <class PixTyp>
inline ImageT<float> ImageT<PixTyp>::hsv()
{
  assert(noChannels()==3);
  ImageT<float> pic(sizeX(),sizeY(),3);
  ImageT<float>::Iterator pChH = pic.begin(0,0);
  ImageT<float>::Iterator pChS = pic.begin(0,1);
  ImageT<float>::Iterator pChV = pic.begin(0,2);
  ConstIterator pChR=constBegin(0,0);
  ConstIterator pChG=constBegin(0,1);
  ConstIterator pChB=constBegin(0,2);
  for (int i = 0; i < sizeImage(); ++i, ++pChH, ++pChS, ++pChV, ++pChR, ++pChG, ++pChB) {
    double r = *pChR;
    double g = *pChG;
    double b = *pChB;
    double max = r;                               // maximum RGB component
    int whatmax = 0;                            // r=>0, g=>1, b=>2
    if (g>max) {
        max=g;
        whatmax=1;
    }
    if (b>max) {
        max=b;
        whatmax=2;
    }
    double min = r;                               // find minimum value
    if (g<min) min = g;
    if (b<min) min = b;
    double delta = max-min;
    float v = max;                                   // calc value
    float s = (max!=0) ? (510*delta+max)/(2*max) : 0;
    float h = 0;
    if ( s == 0 ) {
        h = -1;                                // undefined hue
    }
    else {
      switch ( whatmax ) {
        case 0:                             // red is max component
          if ( g >= b )
            h = (120*(g-b)+delta)/(2*delta);
          else
            h = (120*(g-b+delta)+delta)/(2*delta) + 300;
          break;
        case 1:                             // green is max component
          if ( b > r )
            h = 120 + (120*(b-r)+delta)/(2*delta);
          else
            h = 60 + (120*(b-r+delta)+delta)/(2*delta);
          break;
        case 2:                             // blue is max component
          if ( r > g )
            h = 240 + (120*(r-g)+delta)/(2*delta);
          else
            h = 180 + (120*(r-g+delta)+delta)/(2*delta);
          break;
        }
    }
    *pChH=h;
    *pChS=s;
    *pChV=v;
  }
  return pic;
}


/** resample the current image to size (x, y) */
template <class PixTyp>
inline void ImageT<PixTyp>::resample(ImageBase& resImg, int nx, int ny)
{
  ImageT<PixTyp>& result=(ImageT<PixTyp>&)resImg;
  result.resize(nx, ny, noChannels());
  result.typeImage(typeImage());

  int nc, nr;
  int x1, x2, y1, y2;
  double x, y;

  double cfac=1.0*(sizeX_ - 1) / (nx - 1);
  double rfac=1.0*(sizeY_ - 1) / (ny - 1);

  double p11, p12, p21, p22;
  for (int c=0; c<noChannels(); c++) {
    Iterator elem = result.begin(0,c);

    for (nr = 0; nr < ny; ++nr)
      for (nc = 0; nc < nx; ++nc) {
	x=nc*cfac; y=nr*rfac;
	x1 = (int)x; y1 = (int)y;
	x2 = x1 + 1; y2 = y1 + 1;
	
	// range checking is done in getFloat() !
	     p11 = getFloat(x1, y1, c); p12 = getFloat(x1, y2, c);
	p21 = getFloat(x2, y1, c); p22 = getFloat(x2, y2, c);
	
	if ((typeid(PixTyp)==typeid(float)) || (typeid(PixTyp)==typeid(float))) {		
	  int nan=0;
	  if (isnan(p11)) nan++;
	  if (isnan(p21)) nan++;
	  if (isnan(p12)) nan++;
	  if (isnan(p22)) nan++;
	  if (nan>=2) {
	    *elem=(PixTyp)NAN;
	  }
	  else if (nan==0)
	    *elem=(PixTyp)((y2-y)*((x2-x)*p11+(x-x1)*p21)
			   +(y-y1)*((x2-x)*p12+(x-x1)*p22));
	  else {
	    if (isnan(p11)) p11=p22+(p21-p22)+(p12-p22);
	    if (isnan(p12)) p12=p21+(p11-p21)+(p22-p21);
	    if (isnan(p21)) p21=p12+(p22-p12)+(p11-p12);
	    if (isnan(p22)) p22=p11+(p12-p11)+(p21-p11);
	    *elem=(PixTyp)((y2-y)*((x2-x)*p11+(x-x1)*p21)
			   +(y-y1)*((x2-x)*p12+(x-x1)*p22));
	  }
	}
	else { // typeid != float != double
		    *elem = (PixTyp)((y2-y)*((x2-x)*p11+(x-x1)*p21)+(y-y1)*((x2-x)*p12+(x-x1)*p22) + 0.5);
	}
	++elem;
      }
  }

}

/** resample the current image to size (x, y) using a nearest neighbour check */
template <class PixTyp>
inline void ImageT<PixTyp>::resampleNN(ImageBase& resImg, int nx, int ny)
{
  ImageT<PixTyp>& result=(ImageT<PixTyp>&)resImg;
  result.resize(nx, ny, noChannels());
  result.typeImage(typeImage());

  int x, y;

  double xfac = (double)sizeX_ / (double)nx;
  double yfac = (double)sizeY_ / (double)ny;

  for (int c=0; c<noChannels(); c++) {
    Iterator elem = result.begin(0,c);

    for (y = 0; y < ny; ++y)
      for (x = 0; x < nx; ++x) {
	*elem = get((int)((x + 0.5) * xfac), (int)((y + 0.5) * yfac),c);
	++elem;
      }
  }
}

/**
*/
template <class PixTyp>
inline void ImageT<PixTyp>::maxValue(ImageBase &inImg, float value) {
  ImageT<PixTyp>& in=(ImageT<PixTyp>&)inImg;
  PixTyp val = (PixTyp)value;
  Iterator elem = in.begin(0);
  int size = in.sizeX()*in.sizeY();

  for (int i = 0; i < size; i++) {
      if(*elem > val) *elem = val;
      elem++;
    }
}

/**
*/
template <class PixTyp>
inline void ImageT<PixTyp>::minValue(ImageBase &inImg, float value) {
  ImageT<PixTyp>& in=(ImageT<PixTyp>&)inImg;
  PixTyp val = (PixTyp)value;
  Iterator elem = in.begin(0);
  int size = in.sizeX()*in.sizeY();

  for (int i = 0; i < size; i++) {
     if(*elem < val) *elem = val;
      elem++;
    }
}

/**
blow the label 'label' in image 'in'. The number of blow steps are given
by 'iterations'.
*/
template <class PixTyp>
inline void ImageT<PixTyp>::blow(ImageBase &inImg, int la, int iterations) {

  ImageT<PixTyp>& in=(ImageT<PixTyp>&)inImg;
  ImageT<PixTyp> tmp = in;
  PixTyp label = (PixTyp) la;

  int xs = in.sizeX();
  int ys = in.sizeY();

  while (iterations --) {
    if (ys>3) {//Y - Rand bearbeiten
      for (int y = 1; y < ys-1; y++)
        if (in.get(1, y) == label) {
          tmp.set(1, y+1, label);
          tmp.set(1, y-1, label);
        }
      if(xs>1)
      for (int y = 1; y < ys-1; y++)
        if (in.get(xs, y) == label) {
          tmp.set(xs-1, y+1, label);
          tmp.set(xs-1, y-1, label);
        }
    }
    if (xs>3) {//X - Rand bearbeiten
      for (int x = 1; x < xs-1; x++)
        if (in.get(x, 1) == label) {
          tmp.set(x+1, 1, label);
          tmp.set(x-1, 1, label);
        }
      if(ys>1)
      for (int x = 1; x < xs-1; x++)
        if (in.get(x, ys) == label) {
          tmp.set(x+1, ys-1, label);
          tmp.set(x-1, ys-1, label);
        }
    }
    for (int y = 1; y < ys-1; y ++) //restbild
      for (int x = 1; x < xs-1; x ++)
        if (in.get(x, y) == label) {
          tmp.set(x-1, y, label);
          tmp.set(x+1, y, label);
          tmp.set(x, y+1, label);
          tmp.set(x, y-1, label);
       }
    in = tmp;
  }
}

/** resample the current image to size (x, y) using a nearest neighbour check;
    this version uses a sophisticated class determination algorithm, which is
    especially useful for subsampling/shrinking an image, thus for enlargement */
template <class PixTyp>
inline void ImageT<PixTyp>::resampleNNplus(ImageBase& resImg, int nx, int ny)
{
  ImageT<PixTyp>& result=(ImageT<PixTyp>&)resImg;
  // calculate the scaling factors
       float xfac = (float)sizeX_ / (float)nx;
  float yfac = (float)sizeY_ / (float)ny;

  if (xfac < 1.0 || yfac < 1.0) {
      resampleNN(result, nx, ny);
      return;
  }

  // allocate the resized image
  result.resize(nx, ny, noChannels());
  result.typeImage(typeImage());

  // allocate memory for xfac * yfac possible classes, 2.0 is for safety ;-)
  const int window_size=(int)(xfac * yfac * 2.0);
  PixTyp *window_val = (PixTyp *)new PixTyp[window_size];
  float *window_count = new float[window_size];

  for (int ch=0; ch<noChannels(); ch++) {
    // get a pointer to the the image data
	 Iterator elem = result.begin(0,ch);
    PixTyp ptmp;

    int classmax, c, bleft, bright, btop, bbottom;
    float max, xfrac, yfrac, fx, fy;

    // sweep over the new image ...
	 for (int y = 0; y < ny; ++y)
	   for (int x = 0; x < nx; ++x) {
	     // ... and calculate all it's pixel values

	          // clear the class determination arrays
	          memset(window_val, 0, window_size * sizeof(PixTyp));
	          memset(window_count, 0, window_size * sizeof(float));
	          classmax = 0; max = 0.0;

	          // get all pixels covered by the low-res window (only useful for downsampling ...)
	          for (float dy = 0.0; dy < yfac; dy += 1.0) {
	            // do we pick pixel at the y-upper/-lower border of the window ?
	            btop = (dy == 0.0 ? 1 : 0);
	            bbottom = ((dy + 1.0) >= yfac ? 1 : 0);
	
	            for (float dx = 0.0; dx < xfac; dx += 1.0) {
	              // do we pick pixel at the x-left/-right border of the window ?
	              bleft = (dx == 0.0 ? 1 : 0);
	              bright = ((dx + 1.0) >= xfac ? 1 : 0);

	              // this is the floating point representation of the current pixel within the window ...
	              fx = (x * xfac + dx);
	              fy = (y * yfac + dy);
	
	              // ... get its value from the original image!
	              ptmp = get((int)fx, (int)fy, ch);
	
	              // how much overlapping area does the current pixel contribute to the window ?
	              // default: 1.0 pixel
	              xfrac = yfrac = 1.0;
	              // if we travel along the border the area is smaller:
	              if (bleft)
	                xfrac -= (fx - floor(fx));
	              if (bright)
	                xfrac -= (ceil(x * xfac + xfac) - (x * xfac + xfac));
	              if (btop)
	                yfrac -= (fy - floor(fy));
	              if (bbottom)
	                yfrac -= (ceil(y * yfac + yfac) - (y * yfac + yfac));
	
	              // is the current pixel value(class) already registered ?
	              for (c = 0; c <= classmax; c ++) {
	                // if yes then
	                if (window_val[c] == ptmp) {
	                  // increase the class weight by the contributed pixel area
		                assert(c<window_size);
	                  window_count[c] += xfrac * yfrac;
	                  break;
	                }
	              }
	              // if the value(class) isn't yet registered
	     if (c == classmax + 1) {
	       // put it into the list
		    assert(classmax<window_size);
	       window_val[classmax] = ptmp;
	       window_count[classmax] += xfrac * yfrac;
	       classmax ++;
	     }
	   }
  }

  // after we have completed the whole window we have to decide to
       // which class this new pixel belongs: find the maximum class value
       for (c = 0, max = 0.0; c <= classmax; c ++) {
	 if (max < window_count[c]) {
	   max = window_count[c];
	   // set the pixel to the maximum value
		*elem = window_val[c];
	 }
       }
  // and move to the next pixel in the new image
       ++elem;
}
}

delete window_val;
delete window_count;
}

template <class PixTyp>
inline void ImageT<PixTyp>::calcHistogram(ImageT<int> &result, double min, double max, int n, int channel) {
  result.resize(n,1,1);
  result.typeImage(_PFM_SINT);
	const PixTyp* elem = (PixTyp*)constBegin(0,channel);
	ImageT<int>::Iterator pResult=result.begin();
	double f=1/(max-min);

	for (int i = 0; i < sizeImage(); ++i, ++elem) {
	 	int index=int((*elem-min)*f*n);
  	if (index<0 || index>=n) continue;
      pResult[index] ++;
  }
}


template <class PixTyp>
inline bool ImageT<PixTyp>::read(const char *filename) {
	FILE *fp=fopen(filename,"r");
  if (!fp) return(false);
  read(fp);
  fclose(fp);
	return(true);
}

template <class PixTyp>
inline bool ImageT<PixTyp>::read(FILE *fp) {
	int storageType=PFM_LAST;
	IMGTYPE type = _UNKNOWN;
 	if (typeid(PixTyp)==typeid(float)) {
		storageType=PFM_FLOAT;
		type=_PFM_FLOAT;
	}
	else if (typeid(PixTyp)==typeid(char)) {
		storageType=PFM_BYTE;
		type=_PGM;
	}
	else if (typeid(PixTyp)==typeid(unsigned char)) {
		storageType=PFM_BYTE;
		type=_PGM;
	}
	else if (typeid(PixTyp)==typeid(signed char)) {
		storageType=PFM_BYTE;
		type=_PGM;
	}
	else if (typeid(PixTyp)==typeid(short int)) {
		storageType=PFM_UINT16;
		type=_PFM_UINT16;
	}
	else if (typeid(PixTyp)==typeid(unsigned short int)) {
		storageType=PFM_UINT16;
		type=_PFM_UINT16;
	}
	else if (typeid(PixTyp)==typeid(signed short int)) {
		storageType=PFM_SINT16;
		type=_PFM_SINT16;
	}
	else if (typeid(PixTyp)==typeid(int)) {
		storageType=PFM_UINT;
		type=_PFM_UINT;
	}
	else if (typeid(PixTyp)==typeid(unsigned int)) {
		storageType=PFM_UINT;
		type=_PFM_UINT;
	}
	else if (typeid(PixTyp)==typeid(signed int)) {
		storageType=PFM_SINT;
		type=_PFM_SINT;
	}
	else if (typeid(PixTyp)==typeid(bool)) {
		storageType=PFM_BIT;
		type=_PBM;
	}
	else fprintf(stderr,"Warning: Unsupport data type\n");

  if (noChannels()==3 && storageType==PFM_BYTE) { // PPM
  	type=_PPM;
  }
	typeImage(type);
  switch (type) {
  case _PPM: {
  	int cols, rows;
  	float minval, maxval;
    PFM3Byte *ppmbuffer=(PFM3Byte*)pfm_readpfm_type(fp, &cols, &rows, &minval, &maxval,PFM_3BYTE,0);
    {
      double gW,gN,gE,gS;
      if (pfm_geo_get(&gW,&gN,&gE,&gS)) setGeoCoordinates(gW,gN,gE,gS);
    }

		setData(cols,rows,3,ppmbuffer->r,ppmbuffer->g,ppmbuffer->b);
		setMinValue(minval);
	  setMaxValue(maxval);
	  free(ppmbuffer);
  	break;
#if 0  		
 	  pixval maxval;
 	  int format;
 	  int cols, rows;
  	ppm_readppminit(fp, &cols, &rows, &maxval, &format);

   	resize(cols,rows,3);
   	
   	int sizeX_ = cols, sizeY_ = rows;
   	pixel *row = (pixel *)new pixel[sizeX_];
   		
    for (int y = 0; y < sizeY_; ++y) {
 		  Iterator pChR = begin(y,0);
 		  Iterator pChG = begin(y,1);
		  Iterator pChB = begin(y,2);
		  assert(pChR);
		  assert(pChG);
		  assert(pChB);
	   	pixel *elem_pix = row;
       		
   		ppm_readppmrow(fp, row, sizeX_, maxval, format);
       		
 		  for (int x = 0; x < sizeX_; ++x, ++elem_pix, ++pChR, ++pChG, ++pChB) {
 		  	*pChR=PPM_GETR(*elem_pix);
 		  	*pChG=PPM_GETG(*elem_pix);
 		  	*pChB=PPM_GETB(*elem_pix);
 		  }
 	  }
		delete row;				    	
 		break;
#endif
 		}
  case _PBM: {
 	  int format;
 	  int cols, rows;
  	pbm_readpbminit(fp, &cols, &rows, &format);

   	resize(cols,rows,3);
   	
   	int sizeX_ = cols, sizeY_ = rows;
   	bit *row = (bit*)new bit[sizeX_];
   		
    for (int y = 0; y < sizeY_; ++y) {
 		  Iterator pBit = begin(y,0);
		  assert(pBit);
	   	bit *elem_pix = row;
       		
   		pbm_readpbmrow(fp, row, sizeX_, format);
       		
 		  for (int x = 0; x < sizeX_; ++x, ++elem_pix, ++pBit) {
 		  	*pBit=(*elem_pix!=0);
 		  }
 	  }
		delete row;				    	
 		break;
 		}
 	case _UNKNOWN:
		fprintf(stderr,"##  (ERROR) unknown image type!");
 		break;  	
  default: {
  	int cols, rows;
  	float minval, maxval;
		void* data=pfm_readpfm_type(fp, &cols, &rows, &minval, &maxval,storageType,0);
    {
      double gW,gN,gE,gS;
      if (pfm_geo_get(&gW,&gN,&gE,&gS)) setGeoCoordinates(gW,gN,gE,gS);
    }
		setData(cols,rows,data);
		setMinValue(minval);
	  setMaxValue(maxval);
	  free(data);
  	break;
  	}
  }
  return true;
}

template <class PixTyp>
inline bool ImageT<PixTyp>::write(const char *filename, int channel, const char* comment) {
	FILE *fp=fopen(filename,"w");
  if (!fp) return(false);	
  write(fp, channel, comment);
  fclose(fp);
	return(true);
}

template <class PixTyp>
inline bool ImageT<PixTyp>::write(FILE *fp, int channel, const char* comment) {
	int storageType=PFM_LAST;
 	if (typeid(PixTyp)==typeid(float)) {
		storageType=PFM_FLOAT;
	}
	else if (typeid(PixTyp)==typeid(unsigned char)) {
		storageType=PFM_BYTE;
	}
	else if (typeid(PixTyp)==typeid(signed char)) {
		storageType=PFM_BYTE;
	}
	else if (typeid(PixTyp)==typeid(unsigned short int)) {
		storageType=PFM_UINT16;
	}
	else if (typeid(PixTyp)==typeid(signed short int)) {
		storageType=PFM_SINT16;
	}
	else if (typeid(PixTyp)==typeid(unsigned int)) {
		storageType=PFM_UINT;
	}
	else if (typeid(PixTyp)==typeid(signed int)) {
		storageType=PFM_SINT;
	}
	else if (typeid(PixTyp)==typeid(bool)) {
		storageType=PFM_BIT;
	}
	else fprintf(stderr,"Warning: Unsupport data type\n");
	
	storeMinValue();
	storeMaxValue();

	switch (typeImage()) {
		case _PPM: {
		  PFM3Byte ppmbuffer;
		  ppmbuffer.r=(unsigned char*)constBegin(0,0);
 	 		ppmbuffer.g=(unsigned char*)constBegin(0,1);
			ppmbuffer.b=(unsigned char*)constBegin(0,2);
      if (geoNorth()!=geoSouth() && geoWest()!=geoEast())
        pfm_geo_set(geoWest(),geoNorth(),geoEast(),geoSouth());
      if (comment)
        pfm_comment_set(comment);
  		pfm_writepfm_type(fp,&ppmbuffer,sizeX(),sizeY(),1,-1,PFM_3BYTE);
		
#if 0		
 	    int sizeX_ = sizeX(), sizeY_ = sizeY();
 		  pixel *row = (pixel *)new pixel[sizeX_], *elem_pix;
   		
   		ppm_writeppminit(fp, sizeX_, sizeY_, PPM_MAXMAXVAL, 0);
     		
   		for (int y = 0; y < sizeY_; ++y) {
   			ConstIterator pChR=constBegin(y,0);
 	 			ConstIterator pChG=constBegin(y,1);
				ConstIterator pChB=constBegin(y,2);
     		elem_pix = row;
       		
   		  for (int x = 0; x < sizeX_; ++x, ++elem_pix, ++pChR, ++pChG, ++pChB) {
           PPM_ASSIGN(*elem_pix, (pixval)*pChR, (pixval)*pChG, (pixval)*pChB);
   		  }
   		  ppm_writeppmrow(fp, row, sizeX_, PPM_MAXMAXVAL, 0);
  	  }
   	  delete row;
#endif
  		}
			break;
		case _PBM: {
 	    int sizeX_ = sizeX(), sizeY_ = sizeY();
 		  bit *row = (bit *)new bit[sizeX_];
   		
   		pbm_writepbminit(fp, sizeX_, sizeY_, 0);
     		
   		for (int y = 0; y < sizeY_; ++y) {
   			ConstIterator pBit=constBegin(y,channel);
     		bit* elem_pix = row;
       		
   		  for (int x = 0; x < sizeX_; ++x, ++elem_pix, ++pBit) {
           *elem_pix=(*pBit ? 1 : 0);
   		  }
   		  pbm_writepbmrow(fp, row, sizeX_, 0);
  	  }
   	  delete row;
  		}
			break;
		default:
//  		pfm_writepfm_type(fp,constBegin(),sizeX(),sizeY(),minValue(),maxValue(),storageType);
  		// Let the pfm library calculate min and max
      if (geoNorth()!=geoSouth() && geoWest()!=geoEast())
        pfm_geo_set(geoWest(),geoNorth(),geoEast(),geoSouth());
      if (comment)
        pfm_comment_set(comment);
  		pfm_writepfm_type(fp,constBegin(0,channel),sizeX(),sizeY(),1,-1,storageType);
	}
	return true;
}



template <class PixTyp>
inline void ImageT<PixTyp>::clearGeo() {
  geoWest_ = geoNorth_ = geoEast_ = geoSouth_ = geoResolution_ = 0.0;
  cursor_x_ = cursor_y_ = 0;
}

} // namespace Ga
