/***************************************************************************
                          gadowindow.h  -  do a window function on image
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
 * $Source: /data/cvs/ga_image/ga_image/gaimage/gadowindowt.h,v $
 * $Revision: 1.3 $
 * $Date: 2002/07/04 08:02:02 $
 * $Author: pahl $
 * $Locker:  $
 */

#ifndef __GA_DOWINDOWT_H
#define __GA_DOWINDOWT_H

#include "gaimaget.h"

namespace Ga {

template <class ResultPixTyp, class PixTyp, class WindowFunction>
void doWindow(ImageT<ResultPixTyp>& result,
              ImageT<PixTyp>& imgIn,
              WindowFunction winFunc,
              int winSize)
{
  result.resize(imgIn.sizeX(), imgIn.sizeY());
//  result.typeImage(imgIn.typeImage());

  //erzeugen eines Vektors mit Pointern zu den Bilddaten:
  //PixTyp* in_data = imgIn.Data();
  int vecSize=winSize*winSize;
  PixTyp* vec[vecSize];
  int offset=winSize/2;

  for (int y=0; y<imgIn.sizeY(); y++) {
    for (int i=0; i<vecSize; i++) vec[i]=0;
    for (int yy=0; yy<winSize; yy++) {
      if ((y+yy-offset>=0) && (y+yy-offset<imgIn.sizeY()))
        for (int xx=offset; xx<winSize; xx++) {
          vec[xx+(yy*winSize)] = &imgIn(xx-offset,y+yy-offset);
      }
    }
  	for (int x=0; x<imgIn.sizeX(); x++) {
#if 0
      printf("x=%d, y=%d\n",x,y);
      for (int yy=0; yy<winSize; yy++)
        for (int xx=0; xx<winSize; xx++)
          printf("vec[%d,%d]=%p\n",xx,yy,vec[xx+(yy*winSize)]);

#endif
	  	result(x,y) = winFunc(vec);
#if 0	  	
	  	printf(" (%04d,%04d)=%10.3f\n",
	  	       x,y,result(x+offset,y+offset));
#endif	  	
	
      //Alle weitersetzen
      for (int yy=0; yy<winSize; yy++) {
        PixTyp** p=&vec[yy*winSize];
        int xx;
        for (xx=0; xx<winSize-1; xx++)
	        p[xx]=p[xx+1];
	      if (x+xx>imgIn.sizeX())
	        p[xx]=0;
	      else
	        if (p[xx-1])
  	        p[xx]=p[xx-1]+1;
      }
    }
  }

}

/********************************************************************************************/
template <class PixTyp1, class PixTyp2, class WindowFunction>
void calcCoWinValT(const ImageT<PixTyp1>&  img1,int channel1,
                   int offset1X, int offset1Y,
                   const ImageT<PixTyp2>& img2,int channel2,
                   int offset2X, int offset2Y,
                   int winSizeX, int winSizeY,
                   WindowFunction winFunc,
                   double &val)
{
  int start1X=offset1X;
  int end1X=offset1X+winSizeX;
  int start1Y=offset1Y;
  int end1Y=offset1Y+winSizeY;
  int start2X=offset2X;
  int end2X=offset2X+winSizeX;
  int start2Y=offset2Y;
  int end2Y=offset2Y+winSizeY;
  if (start1X<0) { start2X-=start1X; start1X-=start1X; }
  if (start2X<0) { start1X-=start2X; start2X-=start2X; }
  if (start1Y<0) { start2Y-=start1Y; start1Y-=start1Y; }
  if (start2Y<0) { start1Y-=start2Y; start2Y-=start2Y; }
  if (end1X>img1.sizeX()) { end2X-=end1X-img1.sizeX(); end1X-=img1.sizeX();}
  if (end2X>img2.sizeX()) { end1X-=end2X-img2.sizeX(); end2X-=img2.sizeX();}
  if (end1Y>img1.sizeY()) { end2Y-=end1Y-img1.sizeY(); end1Y-=img1.sizeY();}
  if (end2Y>img2.sizeY()) { end1Y-=end2Y-img2.sizeY(); end2Y-=img2.sizeY();}
  const PixTyp1* start1ptr=img1.constBegin(start1Y,channel1)+start1X;
  const PixTyp2* start2ptr=img2.constBegin(start2Y,channel2)+start2X;
  winFunc.init(img1,channel1,start1X,start1Y,end1X,end1Y,
               img2,channel2,start2X,start2Y,end2X,end2Y);
  int y1,y2;
  for (y1=start1Y, y2=start2Y;
       y1<end1Y;
       start1ptr+=img1.sizeX(), start2ptr+=img2.sizeX(), y1++,y2++) {
    const PixTyp1* ptr1=start1ptr;
    const PixTyp2* ptr2=start2ptr;
    int x1=start1X;
    int x2=start1X;
    for (;
         x1<=end1X;
         ptr1++, ptr2++, x1++,x2++) {
       winFunc(x1,y1,*ptr1,x2,y2,*ptr2);
    }
  }
  val=winFunc.result();
}


} // namespace Ga
#endif
