/***************************************************************************
                          defines.h  -  general definitions for
                                        geoAIDA template image class
                             -------------------
    begin                : Thu Jan 11 2001
    copyright            : (C) 2001 by Juergen Bueckner
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

#ifndef  __GA_DEFINES_H
#define  __GA_DEFINES_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#ifdef GCC29
#include <math.h>
#else
#include <cmath>
#endif
#include <time.h>
#if __GNUC__==2
#include <limits.h>
#else
#include <limits>
#endif
#include <errno.h>

#if !defined( __GNUG__ )
# if !defined( _DOS_ )
#   error Either  __GNUG__ or _DOS_ has to be defined
# endif
#endif

//.....for image class
#define  MAX_GREY_VALUE 255
#define  MIN_GREY_VALUE 0
#define  BACKGROUND 255

/*-------------------------------------------------------------------------*/
/* Utility macros                                                          */
/*-------------------------------------------------------------------------*/

#ifndef TRUE
# define TRUE 1
#endif
#ifndef FALSE
# define FALSE 0
#endif

#ifndef GA_NOTYPE
 #define GA_NOTYPE    unsigned char
#endif

#ifdef abs
# define Abs(x) abs
#endif

#ifndef Abs
# define Abs(x)                 (x>=0 ? x : -(x))
#endif

#define GA_RGB(r,g,b)   (r + g * 0x0100 + b * 0x010000)

namespace Ga {
// --------------------------------------------------------------------------
//... Swap data of type T
//... Purpose:  Having a unique function for Swap
//... Use    :  Swap(&x,&y);
// --------------------------------------------------------------------------
template <class T> void swap(T *x,T *y)
{
  T temp = *x;
  *x = *y;
  *y = temp;
}

}

#endif  //...__GA_DEFINES_H

