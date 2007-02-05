/* -*- mode:c++ -*- */
//*******************************************************************
//  Filename:   gradient.h
//  Datum:      1997
//  Autoren:    J"urgen B"uckner
//  Ort:        TNT, Uni. - Hannover, Germany
//  Projekt:    Bilddatenstruktur - TROPIC
//
//  Info:	calculate the gradient of the image [3x3] Sobel
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

#ifndef __GA_GRADIENTT_H
#define __GA_GRADIENTT_H

#include "gaimaget.h"

namespace Ga {

template <class PixTyp> void gradient(ImageT<PixTyp>& result, ImageT<PixTyp>& imgIn)
{
  result.resize(imgIn.sizeX(), imgIn.sizeY());
  result.typeImage(imgIn.typeImage());

  int i, j ,k ,l, n, m;

  //erzeugen eines Vektors mit Pointern zu den Bilddaten:
  //PixTyp* in_data = imgIn.Data();
  PixTyp* vec[9];
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      vec[(i*3) + j] = &imgIn(i+1, j+1 );

  for(k = 0; k < imgIn.sizeY()-3; k++) {
    for(l = 0; l < imgIn.sizeX()-3; l++) {
    double hor = 0;
    double ver = 0;

    hor = + (double) (*vec[0])  + (double) (*vec[1])*2
      + (double) (*vec[2]) - (double) (*vec[6])
      - (double) (*vec[7])*2 - (double) (*vec[8]);
    ver = + (double) (*vec[0]) + (double) (*vec[3])*2
      + (double) (*vec[6]) - (double) (*vec[2])
      - (double) (*vec[5])*2 - (double) (*vec[8]);
    result(l+1,k+1) = (PixTyp) sqrt(sqrt(ver*ver + hor*hor));

    //Alle weitersetzen
    for (m = 0; m < 9; m++)
      vec[m]++;
  }
  for (n = 0; n < 3; n++)
    for (m = 0; m < 9; m++)
      vec[m]++;
 }

}

} // namespace Ga
#endif
