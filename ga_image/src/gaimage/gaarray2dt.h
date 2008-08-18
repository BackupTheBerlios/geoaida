/***************************************************************************
                          gaarray2d.h  -  description
                             -------------------
    begin                : Fri Feb 23 2001
    copyright            : (C) 2001 by Martin Pahl
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

#ifndef GAARRAY2DT_H
#define GAARRAY2DT_H

#include <assert.h>
#include "gashareddatat.h"

namespace Ga {
/**template for shared 2D arrays
  *@author Martin Pahl
  */

template <class T>
class Array2DT : public SharedDataT<Array2DT<T>,SharedData> {
public:
	Array2DT();
	Array2DT(int sizeX, int sizeY);
	Array2DT(int sizeX, int sizeY, T* data);
	virtual ~Array2DT();
	Array2DT* unlink();
	Array2DT* deepCopy();
	void alloc(int x, int y, T* data=0);
	T* begin(Array2DT*& ptr, int row=0);
	const T* end(int row);
	const T* end();
	const T* constBegin(int row=0) const;
  T** dataArray(Array2DT*& ptr);
private:
	T** data_;
	int sizeX_;
	int sizeY_;
};

template <class T>
inline Array2DT<T>::Array2DT() {
	sizeX_=0;
	sizeY_=0;
	data_=0;
}

template <class T>
inline Array2DT<T>::Array2DT(int sizeX, int sizeY) {
	data_=0;
	alloc(sizeX,sizeY);
}

template <class T>
inline Array2DT<T>::Array2DT(int sizeX, int sizeY, T* data) {
	data_=0;
	alloc(sizeX,sizeY,data);
}

template <class T>
inline Array2DT<T>::~Array2DT() {
	if (data_) {
		delete[] data_[0];
		delete[] data_;
	}
}

template <class T>
inline Array2DT<T>* Array2DT<T>::unlink() {
  if (SharedDataT<Array2DT<T>,SharedData>::deref()) {
    delete this;
  }
  return 0;
}

template <class T>
inline Array2DT<T>* Array2DT<T>::deepCopy() {
		Array2DT* ptr=new Array2DT(sizeX_,sizeY_);
		ptr->sizeX_=sizeX_;
		ptr->sizeY_=sizeY_;
    if( sizeY_*sizeX_ != 0 ) {
    	memcpy( ptr->data_[0], data_[0], sizeX_*sizeY_*sizeof(T) );
    }
		return ptr;
}

template <class T>
inline void Array2DT<T>::alloc(int sizeX, int sizeY, T* data) {
	if (data_) {
		delete data_[0];
		delete data_;
		data_=0;
	}
	sizeX_=sizeX;
	sizeY_=sizeY;
	const int size=sizeX_*sizeY_;
	if (size>0) {
		data_=new T*[sizeY_+1];
		T* ptr=data;
		if (!ptr) {
		  ptr=new T[size];
  		assert(ptr!=0);
  		memset(ptr,0,sizeof(T)*size);
    }
		for (int y=0; y<=sizeY_; y++) {
			data_[y]=ptr;
			ptr+=sizeX_;
		}
	}
}

template <class T>
inline T** Array2DT<T>::dataArray(Array2DT*& ptr) {
  ptr=SharedDataT<Array2DT<T>,SharedData>::uniqueData();
	
  return ptr->data_;
}

template <class T>
inline T* Array2DT<T>::begin(Array2DT*& ptr, int row) {
	assert(row>=0);
	assert(row<sizeY_);
	ptr=SharedDataT<Array2DT<T>,SharedData>::uniqueData();
//	return ptr->data_[row];
	
	return (data_ ? ptr->data_[row] : 0);
}

template <class T>
inline const T* Array2DT<T>::constBegin(int row) const {
	assert(row>=0);
	assert(row<sizeY_);
	return (data_ ? data_[row] : 0);
}

template <class T>
inline const T* Array2DT<T>::end(int row) {
	assert(row>=0);
	assert(row<sizeY_);
	return (data_ ? data_[row+1] : 0);
}

template <class T>
inline const T* Array2DT<T>::end() {
	return (data_ ? data_[sizeY_] : 0);
}

#if 0
template <class T>
inline Array2DT<T>::
#endif

} // namespace Ga

#endif
