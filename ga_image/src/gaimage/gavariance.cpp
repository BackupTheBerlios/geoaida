/***************************************************************************
                          gavariancet.h  -  variance of image
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
 * $Source: /data/cvs/ga_image/ga_image/gaimage/gavariance.cpp,v $
 * $Revision: 1.5 $
 * $Date: 2003/01/10 13:36:44 $
 * $Author: pahl $
 * $Locker:  $
 */


#ifndef __GA_VARIANCET_H
#define __GA_VARIANCET_H

#include "gaimage.h"
#include "gaimaget.h"
#include "gadowindowt.h"
#ifdef GCC29
#include <math.h>
#endif

namespace Ga {

template <class ResultPixTyp, class PixTyp>
class Variance {
public:
  Variance(int winSize) : winSize_(winSize) {
    vecSize_=winSize_*winSize_;
  }
  ResultPixTyp operator()(PixTyp* vec[]) {
    double sum=0;
    double qsum=0;
    if (typeid(PixTyp)==typeid(float)) {
      int n=vecSize_;
      for (int i=0; i<vecSize_; i++) {
#if 0
        if (vec[i])
          printf("%3d ",*vec[i]);
        else printf("nul ");
#endif
        if (vec[i] && !isnan(*vec[i])) {
          sum+=*vec[i];
          qsum+=*vec[i]* *vec[i];
        }
        else n--;
      }
#if 0
      printf("sum=%f qsum=%f",sum,qsum);
#endif
      switch (n) {
      case 0:
      case 1:
        return 0.0;
      default:
        double xm=sum/n;
        return (ResultPixTyp)sqrt((qsum-n*xm*xm)/(n-1));
      }
    }
    else {
      int n=vecSize_;
      for (int i=0; i<vecSize_; i++) {
        if (vec[i]) {
#if 0
          printf("%3d ",*vec[i]);
#endif
          sum+=*vec[i];
          qsum+=*vec[i]* *vec[i];
        }
        else n--;
      }
#if 0
      printf("sum=%f qsum=%f",sum,qsum);
#endif
      if (n<2) return 0.0;
      double xm=sum/vecSize_;
      return (ResultPixTyp)sqrt((qsum-vecSize_*xm*xm)/(n-1));
    }
  }
private:
  int winSize_,vecSize_;
};


template <class PixTyp>
static void variance(ImageBase& result, ImageBase& imgIn, int winSize)
{
  ImageT<float>& im1=(ImageT<float>&)result;
  ImageT<PixTyp>& im2=(ImageT<PixTyp>&)imgIn;
  variance(im1,im2,winSize);
}

Image variance(Image& imgIn, int winSize)
{
  Image result(typeid(float));
  ForEachTypeDo(imgIn.typeId(),variance,(*(result.pImage()),*(imgIn.pImage()),winSize));
  return result;
}

template <class PixTyp>
static void variance(ImageT<float>& result,
                     ImageT<PixTyp>& imgIn,
                     int winSize)
{
  doWindow(result,imgIn,Variance<float,PixTyp>(winSize),winSize);
}


/********************************************************************************************/
// This class is use if you want to calculate the variance of a circular scale e.g. degrees
template <class ResultPixTyp, class PixTyp>
class CircularVariance {
public:
  CircularVariance(int winSize, float scaleSize)
    : winSize_(winSize),scaleSize_(scaleSize) {
    vecSize_=winSize_*winSize_;
  }
  ResultPixTyp operator()(PixTyp* vec[]) {
    double sum=0;
    double qsum=0;
    double halfScale=scaleSize_/2;
    if (typeid(PixTyp)==typeid(float)) {
      int n=vecSize_;
      for (int i=0; i<vecSize_; i++) {
#if 0
        if (vec[i])
          printf("%3d ",*vec[i]);
        else printf("nul ");
#endif
        if (vec[i] && !isnan(*vec[i])) {
          sum+=*vec[i];
        }
        else n--;
      }
      if (n<2) return 0.0;
      double xm=sum/n;
      for (int i=0; i<vecSize_; i++) {
#if 0
        if (vec[i])
          printf("%3d ",*vec[i]);
        else printf("nul ");
#endif
        if (vec[i] && !isnan(*vec[i])) {
          double v=*vec[i]-xm;
          if (v>halfScale) v-=scaleSize_;
          else
            if (v<-halfScale) v+=scaleSize_;
          qsum+=v*v;
        }
      }
#if 0
      printf("sum=%f qsum=%f",sum,qsum);
#endif
      return (ResultPixTyp)sqrt(qsum/(n-1));
    }
    else {
      int n=vecSize_;
      for (int i=0; i<vecSize_; i++) {
#if 0
        printf("%3d ",*vec[i]);
#endif
        if (vec[i])
          sum+=*vec[i];
        else n--;
      }
      if (n<2) return 0.0;
      double xm=sum/vecSize_;
      for (int i=0; i<vecSize_; i++) {
#if 0
        printf("%3d ",*vec[i]);
#endif
        if (vec[i]) {
          double v=*vec[i]-xm;
          if (v>halfScale) v-=scaleSize_;
          else
            if (v<-halfScale) v+=scaleSize_;
          qsum+=v*v;
        }
      }
#if 0
      printf("sum=%f qsum=%f",sum,qsum);
#endif
      return (ResultPixTyp)sqrt((qsum-vecSize_*xm*xm)/(n-1));
    }
  }
private:
  int winSize_,vecSize_;
  float offset_, scaleSize_;
};


template <class PixTyp>
static void circVariance(ImageBase& result, ImageBase& imgIn, int winSize, double scaleSize)
{
  ImageT<float>& im1=(ImageT<float>&)result;
  ImageT<PixTyp>& im2=(ImageT<PixTyp>&)imgIn;
  circVariance(im1,im2,winSize,scaleSize);
}

Image variance(Image& imgIn, int winSize, double scaleSize)
{
  Image result(typeid(float));
  ForEachTypeDo(imgIn.typeId(),circVariance,(*(result.pImage()),*(imgIn.pImage()),winSize,scaleSize));
  return result;
}

template <class PixTyp>
static void circVariance(ImageT<float>& result,
                         ImageT<PixTyp>& imgIn,
                         int winSize,
                         double scaleSize)
{
  doWindow(result,imgIn,CircularVariance<float,PixTyp>(winSize,scaleSize),winSize);
}


/********************************************************************************************/
// This class calculates the quad-error using a window in two different images
template <class PixTyp1, class PixTyp2>
class QuadErrorT {
public:
  QuadErrorT() {}
  void init(const ImageT<PixTyp1>& img1, int channel1,
            int start1X, int start1Y, int end1X, int end1Y,
            const ImageT<PixTyp2>& img2, int channel2,
            int start2X, int start2Y, int end2X, int end2Y) {
    winSize=(end1X-start1X)*(end1Y-start1Y);
    sum=0.0;
  }
  void operator()(int x1, int y1, PixTyp1 v1,
                  int x2, int y2, PixTyp2 v2) {
    double v=double(v1)-double(v2);
    v=v*v;
    sum+=v;
  }
  double result() {
    return sum/winSize;
  }
protected:
  double sum;
  int winSize;
};

template <class PixTyp>
static void calcQuadErrorT(const ImageBase&  image1, int channel1,
                    int offset1X, int offset1Y,
                    const ImageBase& image2, int channel2,
                    int offset2X, int offset2Y,
                    int winSizeX, int winSizeY,
                    double &sqrError)
{
  calcCoWinValT((const ImageT<PixTyp>&)image1,channel1,
                                          offset1X, offset1Y,
                                          (const ImageT<PixTyp>&)image2,channel2,
                                          offset2X, offset2Y,
                                          winSizeX, winSizeY,
                                          QuadErrorT<PixTyp,PixTyp>(),
                                          sqrError);
}

double sqrError(const Image& img1, int channel1,
                 int offset1X, int offset1Y,
                 const Image& img2, int channel2,
                 int offset2X, int offset2Y,
                 int winSizeX, int winSizeY)
{
  double sqrError;
  ForEachTypeDo(img1.typeId(),calcQuadErrorT,
                (*(img1.pImage()),channel1,offset1X,offset1Y,
                 *(img2.pImage()),channel2,offset2X,offset2Y,
                 winSizeX,winSizeY,sqrError));
  return sqrError;
}

/********************************************************************************************/
// This class calculates the covariance using a window in two different images
template <class PixTyp1, class PixTyp2>
class CoVarianceT {
public:
  CoVarianceT() {}
  void init(const ImageT<PixTyp1>& img1, int channel1,
            int start1X, int start1Y, int end1X, int end1Y,
            const ImageT<PixTyp2>& img2, int channel2,
            int start2X, int start2Y, int end2X, int end2Y) {
    winSize=(end1X-start1X)*(end1Y-start1Y);
    sum1=sum2=sumProd=0.0;
  }
  void operator()(int x1, int y1, PixTyp1 v1,
                  int x2, int y2, PixTyp2 v2) {
    sum1+=v1;
    sum2+=v2;
    sumProd+=v1*v2;
  }
  double result() {
    double mean1=sum1/winSize;
    double mean2=sum2/winSize;
    return (sumProd-mean1*sum2-mean2*sum1+winSize*mean1*mean2)/(winSize-1);
  }
protected:
  double sum1,sum2;
  double sumProd;
  int winSize;
};

template <class PixTyp>
static void calcCoVarianceT(const ImageBase&  image1, int channel1,
                    int offset1X, int offset1Y,
                    const ImageBase& image2, int channel2,
                    int offset2X, int offset2Y,
                    int winSizeX, int winSizeY,
                    double &coVariance)
{
  calcCoWinValT((const ImageT<PixTyp>&)image1,channel1,
                                          offset1X, offset1Y,
                                          (const ImageT<PixTyp>&)image2,channel2,
                                          offset2X, offset2Y,
                                          winSizeX, winSizeY,
                                          CoVarianceT<PixTyp,PixTyp>(),
                                          coVariance);
}

double covariance(const Image& img1, int channel1,
                 int offset1X, int offset1Y,
                 const Image& img2, int channel2,
                 int offset2X, int offset2Y,
                 int winSizeX, int winSizeY)
{
  double coVariance;
  ForEachTypeDo(img1.typeId(),calcCoVarianceT,
                (*(img1.pImage()),channel1,offset1X,offset1Y,
                 *(img2.pImage()),channel2,offset2X,offset2Y,
                 winSizeX,winSizeY,coVariance));
  return coVariance;
}

/********************************************************************************************/
// This class calculates the covariance using a window in two different images
template <class PixTyp1, class PixTyp2>
class CorrelationT {
public:
  CorrelationT() {}
  void init(const ImageT<PixTyp1>& img1, int channel1,
            int start1X, int start1Y, int end1X, int end1Y,
            const ImageT<PixTyp2>& img2, int channel2,
            int start2X, int start2Y, int end2X, int end2Y) {
    winSize=(end1X-start1X)*(end1Y-start1Y);
    sum1=sum2=sumQuad1=sumQuad2=sumProd=0.0;
  }
  void operator()(int x1, int y1, PixTyp1 v1,
                  int x2, int y2, PixTyp2 v2) {
    sum1+=v1;
    sum2+=v2;
    sumQuad1+=v1*v1;
    sumQuad2+=v2*v2;
    sumProd+=v1*v2;
  }
  double result() {
    double mean1=sum1/winSize;
    double mean2=sum2/winSize;
    double cov1=sumQuad1-2*mean1*sum1+winSize*mean1*mean1;
    double cov2=sumQuad2-2*mean2*sum2+winSize*mean2*mean2;
    return (sumProd-mean1*sum2-mean2*sum1+winSize*mean1*mean2)/sqrt(cov1*cov2);
  }
protected:
  double sum1,sum2;
  double sumQuad1,sumQuad2;
  double sumProd;
  int winSize;
};

template <class PixTyp>
static void calcCorrelationT(const ImageBase&  image1, int channel1,
                    int offset1X, int offset1Y,
                    const ImageBase& image2, int channel2,
                    int offset2X, int offset2Y,
                    int winSizeX, int winSizeY,
                    double &correlation)
{
  calcCoWinValT((const ImageT<PixTyp>&)image1,channel1,
                                          offset1X, offset1Y,
                                          (const ImageT<PixTyp>&)image2,channel2,
                                          offset2X, offset2Y,
                                          winSizeX, winSizeY,
                                          CorrelationT<PixTyp,PixTyp>(),
                                          correlation);
}

double correlation(const Image& img1, int channel1,
                 int offset1X, int offset1Y,
                 const Image& img2, int channel2,
                 int offset2X, int offset2Y,
                 int winSizeX, int winSizeY)
{
  double correlation;
  ForEachTypeDo(img1.typeId(),calcCorrelationT,
                (*(img1.pImage()),channel1,offset1X,offset1Y,
                 *(img2.pImage()),channel2,offset2X,offset2Y,
                 winSizeX,winSizeY,correlation));
  return correlation;
}







} // namespace Ga
#endif



#if 0
template <class PixTyp>
void calcSqrErrorT(const ImageBase&  image1, const ImageBase& image2,
                     int winSizeX, int winSizeY,
                     int offset1X, int offset1Y,
                     int offset2X, int offset2Y,
                     double &correlation)
{
  const ImageT<PixTyp>& img1=(const ImageT<PixTyp>&)image1;
  const ImageT<PixTyp>& img2=(const ImageT<PixTyp>&)image2;

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
  const PixTyp* start1ptr=img1.constBegin(start1Y)+start1X;
  const PixTyp* start2ptr=img2.constBegin(start2Y)+start2X;
  double sum=0;
  for (int y=start1Y;
       y<end1Y;
       start1ptr+=img1.sizeX(), start2ptr+=img2.sizeX(), y++) {
    const PixTyp* ptr1=start1ptr;
    const PixTyp* ptr2=start2ptr;
    int x=start1X;
    for (;
         x<=end1X;
         ptr1++, ptr2++, x++) {
       double v=double(*ptr1)-double(*ptr2);
       v=v*v;
       sum+=v;
    }
  }
  correlation=sum/(end1X-start1X)/(end1Y-start1Y);
}
#endif
