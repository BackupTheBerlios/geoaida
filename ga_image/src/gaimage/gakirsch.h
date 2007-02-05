/* -*- mode:c++ -*- */
//*******************************************************************
//  Filename:   kirsch.h
//  Datum:      1997
//  Autoren:    J"urgen B"uckner
//  Ort:        TNT, Uni. - Hannover, Germany
//  Projekt:    Bilddatenstruktur - TROPIC
//
//  Info:	Kirsch filter; provides an emboss effect
//
//           --------      
//          /     ^ /      
//         /  ^^   /--     
//        / ^ ° ^^/  /
//        --------  /--
//         /  °  * /+ /
//         --------  /
//          /   ~~  /
//          --------
//
//*******************************************************************

#ifndef __GA_KIRSCH_H
#define __GA_KIRSCH_H

#include "gaimaget.h"

namespace Ga {

template <class PixTyp> ImageT<PixTyp>* Kirsch(ImageT<PixTyp>& imgIn)
{
  int k, l;
  //PixTyp rmax, rmin;
  double rmax, rmin;
  double rng, k1, k2, k3, k4, kk;
  int nr = imgIn.sizeY();
  int nc = imgIn.sizeX();
  ImageT<PixTyp>* imgOut = new ImageT<PixTyp>(imgIn);
        imgOut->typeImage(imgIn.typeImage());
  ImageT<double> tmp1(imgIn.sizeX(), imgIn.sizeY()) ;

  //.locate MAX and MIN for rescaling
  rmax = (double)imgIn(1,1);
  rmin = (double)rmax;

  for(k=2; k<nr-1; k++) {
    for(l=2; l<nc-1; l++) {
      k1=(double)imgIn[k][l+1]+(double)imgIn[k-1][l+1]+(double)imgIn[k+1][l+1]
        -((double)imgIn[k][l-1]+(double)imgIn[k-1][l-1]+(double)imgIn[k+1][l-1]);
      k2=(double)imgIn[k-1][l-1]+(double)imgIn[k-1][l]+(double)imgIn[k-1][l+1]
        -((double)imgIn[k+1][l-1]+(double)imgIn[k+1][l]+(double)imgIn[k+1][l+1]);
      k3=(double)imgIn[k-1][l]+(double)imgIn[k-1][l+1]+(double)imgIn[k][l+1]
        -((double)imgIn[k][l-1]+(double)imgIn[k+1][l-1]+(double)imgIn[k+1][l]);
      k4=(double)imgIn[k][l-1]+(double)imgIn[k-1][l-1]+(double)imgIn[k-1][l]
        -((double)imgIn[k][l+1]+(double)imgIn[k+1][l+1]+(double)imgIn[k+1][l]);
      kk=(k1+k2+k3+k4)/4;
      //      imgOut[k][l]=(PixTyp)kk;
      tmp1[k][l] = kk;
      if(rmax<kk) rmax=kk;
      if(rmin>kk) rmin=kk;
    }
  }
  rng = rmax-rmin;
  //.now rescaling
  for(k=2; k<nr-1; k++)
    for(l=2; l<nc-1; l++)
      (*imgOut)[k][l]=(PixTyp)
        (((double)(tmp1[k][l]-rmin)/rng) *
         ((double)(MAX_GREY_VALUE+1)));

  return imgOut;
}

/*
//template <class PixTyp>
g_basic*
Kirsch2(g_basic& imgIn)
{
  G_FKT_NAME( "ImageT Kirsch" );

  int k, l;
  //PixTyp rmax, rmin;
  double rmax, rmin;
  double rng, k1, k2, k3, k4, kk;
  int nr = imgIn.y_Size();
  int nc = imgIn.x_Size();
  ImageT<PixTyp>* imgOut = new ImageT<PixTyp>(imgIn);
  ImageT<double> tmp1(imgIn.Rows(), imgIn.Columns()) ;

  //.locate MAX and MIN for rescaling
  rmax = (double)imgIn(1,1);
  rmin = (double)rmax;

  for(k=2; k<nr-1; k++) {
    for(l=2; l<nc-1; l++) {
      k1=(double)imgIn[k][l+1]+(double)imgIn[k-1][l+1]+(double)imgIn[k+1][l+1]
        -((double)imgIn[k][l-1]+(double)imgIn[k-1][l-1]+(double)imgIn[k+1][l-1]);
      k2=(double)imgIn[k-1][l-1]+(double)imgIn[k-1][l]+(double)imgIn[k-1][l+1]
        -((double)imgIn[k+1][l-1]+(double)imgIn[k+1][l]+(double)imgIn[k+1][l+1]);
      k3=(double)imgIn[k-1][l]+(double)imgIn[k-1][l+1]+(double)imgIn[k][l+1]
        -((double)imgIn[k][l-1]+(double)imgIn[k+1][l-1]+(double)imgIn[k+1][l]);
      k4=(double)imgIn[k][l-1]+(double)imgIn[k-1][l-1]+(double)imgIn[k-1][l]
        -((double)imgIn[k][l+1]+(double)imgIn[k+1][l+1]+(double)imgIn[k+1][l]);
      kk=(k1+k2+k3+k4)/4;
      //      imgOut[k][l]=(PixTyp)kk;
      tmp1[k][l] = kk;
      if(rmax<kk) rmax=kk;
      if(rmin>kk) rmin=kk;
    }
  }
  rng = rmax-rmin;
  //.now rescaling
  for(k=2; k<nr-1; k++)
    for(l=2; l<nc-1; l++)
      (*imgOut)[k][l]=(PixTyp)
        (((double)(tmp1[k][l]-rmin)/rng) *
         ((double)(MAX_GREY_VALUE+1)));

  return imgOut;
}
*/

// namespace Ga
#endif
