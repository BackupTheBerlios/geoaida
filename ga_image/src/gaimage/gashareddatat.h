/***************************************************************************
                          gashareddatat.h  -  description
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

#ifndef GASHAREDDATAT_H
#define GASHAREDDATAT_H


/**template to create a shared data obejct
  *@author Oliver Stahlhut
  */
#include "gashareddata.h"

namespace Ga {

template <class T, class Base>
class SharedDataT : public Base {
public: 
  SharedDataT();
  ~SharedDataT();
  T* uniqueData();
  T* shallowCopy();
};

template <class T, class Base>
inline SharedDataT<T,Base>::SharedDataT() {
}


template <class T, class Base>
inline SharedDataT<T,Base>::~SharedDataT() {
}

template <class T, class Base>
inline T* SharedDataT<T,Base>::uniqueData() {
  if (Base::count()>1) {
    Base::deref();
    return ((T*)this)->deepCopy();
  } else return (T*)this;
}

template <class T, class Base>
inline T* SharedDataT<T,Base>::shallowCopy() {
  Base::ref();
  return (T*)this;
}

} // namespace Ga
#endif
