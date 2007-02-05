/* -*- mode:c++ -*- */
//*******************************************************************
//  Filename:   hough.h
//  Datum:      1997
//  Autoren:    J"urgen B"uckner
//  Ort:        TNT, Uni. - Hannover, Germany
//  Projekt:    Bilddatenstruktur - TROPIC
//
//  Info:	Provides the Hough transformation
//		for all pixel having a value less than
//		a certain threshold t
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

#include "gaimage.h"
#include "gaimaget.h"

namespace Ga {

#define HESSE_NF

template <class PixTyp>
void houghMapLines(ImageT<int>& result, const ImageBase& img,
                   double angleMin, double angleMax, double angleStep)
{
  ImageT<int>& imgOut=result;
  const ImageT<PixTyp>& imgIn=(const ImageT<PixTyp>&)img;
//  int r,c,ra;
  int numAngles=int((angleMax-angleMin)/angleStep);
  double sarr[numAngles],carr[numAngles];

  int nx=imgIn.sizeX();
  int ny=imgIn.sizeY();
  const double conv=3.1415926535/180.0;
#ifndef HESSE_NF
  int xc=nx/2;
  int yc=ny/2;
#endif
  int rmax=(int)(sqrt((double)ny*ny+(double)nx*nx)/2.0);
  int rmax2=2*rmax;

  //allocate the Hough image: 180 degrees by twice the maximum
  //possible distance found in image
  imgOut.resize(rmax2,numAngles);

  for (int h = 0; h < numAngles; h++)
    for (int k = 0; k < rmax2; k++)
      imgOut[h][k] = 0;

  //Initialize a table of sine and cosine values
  for(int alpha=0; alpha<numAngles; alpha++ ) {
    sarr[alpha] = sin((double)(alpha*angleStep+angleMin)*conv);
    carr[alpha] = cos((double)(alpha*angleStep+angleMin)*conv);
//    printf("%f -> cos: %f sin: %f\n",(alpha*angleStep+angleMin),carr[alpha],sarr[alpha]);
  }

  //Transform each pixel into Hough coordinate (r,alpha) if
  //it has a value <=T, increment all Hough pixels that correspond
  for(int r=0; r<ny; r++ ) {
    const PixTyp* row=imgIn.constBegin(r);
    for(int c=0; c<nx; c++ ) {
      if (row[c]) {
#ifdef HESSE_NF
        int ra0=int(c*carr[0]+r*sarr[0]);
        if (ra0<0 || ra0>=rmax2) {
//          printf("Warning: alhpa %f -> p %d out of range\n",0*angleStep+angleMin,ra0);
        }
#else
        double rr=(c-yc)*sarr[0]-(r-xc)*carr[0];
        int ra0=(rr<0.0?int(rr):int(rr+1));
#endif
        imgOut[0][rmax+ra0]++;
        for(int alpha=1; alpha<numAngles; alpha++ ) {
#ifdef HESSE_NF
          int ra1=int(c*carr[alpha]+r*sarr[alpha]);
          if (ra1<0 || ra1>=rmax2) {
//            printf("Warning: alhpa %f -> p %d out of range\n",alpha*angleStep+angleMin,ra1);
            continue;
          }
          int rahalf=(ra1+ra0)/2;
          if (ra1<ra0) {
            for (int rx=ra1; rx<=rahalf; rx++) {
              if (rx>=rmax2) continue;
              imgOut[alpha][rx]++;
            }
            for (int rx=rahalf; rx<ra0; rx++) {
              if (rx>=rmax2) continue;
              imgOut[alpha-1][rx]++;
            }
          }
          else {
            for (int rx=ra0+1; rx<=rahalf; rx++) {
              if (rx>=rmax2) continue;
              imgOut[alpha-1][rx]++;
            }
            for (int rx=rahalf; rx<=ra1; rx++) {
              if (rx>=rmax2) continue;
              imgOut[alpha][rx]++;
            }
          }
#else
          rr=(c-yc)*sarr[alpha]-(r-xc)*carr[alpha];
          int ra1=(rr<0.0?int(rr):int(rr+1));
          int rahalf=(ra1+ra0)/2;
          if (ra1<ra0) {
            int x,y;
            for (int rx=ra1; rx<=rahalf; rx++)
              imgOut[alpha][rmax+rx]++;
            for (int rx=rahalf; rx<ra0; rx++) {
              imgOut[alpha-1][rmax+rx]++;
          }
          else {
            for (int rx=ra0+1; rx<=rahalf; rx++) {
              imgOut[alpha-1][rmax+rx]++;
            for (int rx=rahalf; rx<=ra1; rx++) {
              imgOut[alpha][rmax+rx]++;
          }
#endif
          ra0=ra1;
        }
      }
    }
  }
}

Image houghMapLines(const Image& imgIn,
                    double angleMin, double angleMax, double angleStep)
{
  Image result(typeid(int));
  ForEachTypeDo(imgIn.typeId(),houghMapLines,
                (((ImageT<int>&)*(result.pImage())),
                 *(imgIn.pImage()),
                 angleMin,angleMax,angleStep));
  return result;
}


} // namespace Ga

