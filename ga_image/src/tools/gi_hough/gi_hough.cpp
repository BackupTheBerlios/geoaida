/***************************************************************************
                          detectbuilding.cpp  -  description
                             -------------------
    begin                : Thu May 16 2002
    copyright            : (C) 2002 by Martin Pahl
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

#define N 360 

#include <stdio.h>
#include <getopt.h>
#include <gaimage.h>
#include <gahough.h>
#include <gadraw.h>

// #define DEBUG_PRG
// #define DEBUG_MSG

using namespace std;
using namespace Ga;

void Usage(char *prg)
{
  printf ("Usage:\n"
          "  %s [-d <debug-file> <level>] <infile> <outfile> <histogram_angle> <dft_image> <dft_values>[<min-angle> <max-angle> <step-angle>]\n\n",
          prg);
  exit(1);
}

int main(int argc, char **argv)
{
  const char *infile=0;
  const char *outfile=0;
  const char *debugfile=0;
  const char *histogram=0;
  const char *dft_image_name=0;
  const char *dft_values_name=0;  
  float threshhold=0;
  double angleMin=0;
  double angleMax=360;
  double angleStep=1;
  {
    int opt;
    char options[] = "d:";
    while ((opt = getopt (argc, argv, options)) != -1) {
      switch (opt) {
      case 'd':
        debugfile=optarg;
        break;
      }
    }
  }
  if (debugfile) {
    sscanf(argv[optind],"%f",&threshhold);
    optind++;
  }
  if (argc-optind<4) Usage(argv[0]);
  infile= argv[optind++];
  outfile= argv[optind++];
  histogram= argv[optind++];
  dft_image_name= argv[optind++];
  dft_values_name= argv[optind++];
  if (argc-optind>=3) {
    sscanf(argv[optind++],"%lf",&angleMin);
    sscanf(argv[optind++],"%lf",&angleMax);
    sscanf(argv[optind++],"%lf",&angleStep);
  }

  Image im;
  if (!im.read(infile)) {
    fprintf(stderr,"Can't open file %s\n",infile);
    exit(1);
  }
  if (im.isEmpty()) {
    fprintf(stderr,"Image %s is empty\n",infile);
    exit(1);
  }
  printf("Read %s\n",infile);

  Image result=houghMapLines(im,angleMin,angleMax,angleStep);

  printf("Write %s\n",outfile);
  result.write(outfile);
  
  //  sum hough-image over angle
  int slots= result.sizeY();
  float histcount[slots], histvalue[slots], peak=0.0, sum=0.0;
             
  for (int s = 0; s < slots; s++){
  histcount[s]=0.0;
  histvalue[s]=0.0;
  for (int x=0; x<result.sizeX(); x++){
  		histcount[s]+= result.getFloat(x,s);
  		histvalue[s]+= result.getFloat(x,s);
   		sum+= result.getFloat(x,s);    
      if (histcount[s] > peak)
        peak= histcount[s];
      }
      cout<<" histcount[="<<s<<"]="<<histcount[s]<<" max="<<peak<<" histvalue[="<<s<<"]="<<histvalue[s]<<endl;
//      if (histcount[s] < 3000.0)
//        histcount[s]= 0.0;
        
   }

  //  genarate hough angle histogram
  
  ImageT <unsigned char> histo_image (slots, 500, 1);

  bool swit=false;

  for (int x = 0; x < histo_image.sizeX(); x++){
   // scale histogram
   int histpart= (histcount[x]/peak)*histo_image.sizeY();
   for (int y = 0; y < histo_image.sizeY(); y++)
      if (histo_image.sizeY()-(int)histpart < y){
        if (swit)
          histo_image.set (x,y,0);
        else
          histo_image.set (x,y,125);
        }
      else
        histo_image.set (x,y,255);
      swit=!swit;
     }
  histo_image.write (histogram);
    
  //  calc autocorrelation of hough_histogram
/*
  float akfx[360], akf[360];

  for (int x=0; x < 360; x++)
    akfx[x]= histcount[180+x];
    
  //  initilise akf
  for (int s = 0; s < 360; s++)
    akf[s]= 0.0;
  //  shift over all slots
  for (int shift=0; shift < 360; shift++){
    //  sum in all slots akf function
    for (int s = 0; s < 360; s++)
        akf[shift]+= (akfx[s]*akfx[(s+shift)%360])/(peak*peak);
    }

  for (int s = 0; s < 360; s++)
     cout<<"akf["<<s<<"]="<< akf[s]<<endl;
*/    

    //  calc dft of hough array
    float xn[N];
    for (int x=0; x < N; x++)
      xn[x]= histvalue[x+180];
//        xn[x]= sin((2*M_PI/360)*x);

    for (int x=0; x < 90; x++)
      xn[x]+= histvalue[x+270];    

//  cut small values      
/*    for (int x=0; x < N; x++)
      if (xn[x]< 500.0)
        xn[x]=0.0;      */
      
    float xk_re[N], xk_im[N];

    for (int k=0; k<N; k++)
        xk_re[k]=0.0; xk_im[k]=0.0;

    for (int k=0; k<N; k++)
      for (int n=0; n<N; n++){          
        xk_re[k]+= xn[n]* cos( (2*M_PI*n*k)/N);
        xk_im[k]+= xn[n]* sin( (2*M_PI*n*k)/N);
        }      

    float F[N];
    peak=0.0;
    for (int k=0; k<N; k++){
      F[k]= sqrt(xk_re[k]*xk_re[k]+xk_im[k]*xk_im[k]);
      if (F[k]> peak)
        peak=F[k];
      }

  //  write result
    FILE *fp;
    if (!(fp = fopen(dft_values_name, "a")))
    {
      fprintf(stderr, "Can't open outputfile %s\n", dft_values_name);
      Usage(argv[0]);
      exit(-1);
    }
    
    for (int k=0; k<10; k++)
    	printf("F[%d]= %10f\n", k, F[k]);

   	fprintf(fp, "%f;", F[2]/(2.5*histo_image.sizeX()*histo_image.sizeY()));
                          
    fclose(fp);

  //  genarate dft angle histogram
  ImageT <unsigned char> dft_image (N, 500, 1);
  swit=false;

  for (int x = 0; x < dft_image.sizeX(); x++){
   // scale histogram
   int histpart= int((F[k]/peak)*dft_image.sizeY());
   for (int y = 0; y < dft_image.sizeY(); y++)
      if (dft_image.sizeY()-(int)histpart < y){
        if (swit)
          dft_image.set (x,y,0);
        else
          dft_image.set (x,y,125);
        }
      else
        dft_image.set (x,y,255);
      swit=!swit;
     }

  dft_image.write (dft_image_name);
  
  //  calculate debug image  
  if (!debugfile) return 0;
  if (threshhold<1) {
    int max=0;
    const void *p=result.constBegin();
    for (int i=0; i<result.sizeImage(); i++) {
      if (result.getInt(p)>max) max=result.getInt(p);
      result.nextCol(p);
    }
    threshhold*=max;
    printf("Threshhold: %f\n",threshhold);
  }
  printf("Threshhold: %f\n",threshhold);
  Image debug(typeid(unsigned char),im.sizeX(),im.sizeY());
  float tmin=sqrt(float(im.sizeX()*im.sizeX()+im.sizeY()*im.sizeY()));
  int sizeX2=im.sizeX()/2;
  int sizeY2=im.sizeY()/2;
  for (int y=0; y<result.sizeY(); y++)
     for (int x=0; x<result.sizeX(); x++) {
       if (result.getInt(x,y)>=threshhold) {
         double alpha=((y*angleStep)+angleMin)/180*M_PI;
         float r0=x;
         int xmin, ymin, xmax, ymax;
         if (fabs(cos(alpha))>fabs(sin(alpha))) {
           ymin=0; ymax=im.sizeY()-1;
           xmin=int((r0-(ymin)*sin(alpha))/cos(alpha));
           xmax=int((r0-(ymax)*sin(alpha))/cos(alpha));
         }
         else {
           xmin=0; xmax=im.sizeX()-1;
           ymin=int((r0-(xmin)*cos(alpha))/sin(alpha));
           ymax=int((r0-(xmax)*cos(alpha))/sin(alpha));
         }
#if 0
         xmin+=sizeX2;
         xmax+=sizeX2;
         ymin+=sizeY2;
         ymax+=sizeY2;
#endif
//       printf("Draw Line (%5d %5d)->(%5d %5d) (r0=%f, alpha=%f)\n", xmin,ymin,xmax,ymax,r0,alpha/M_PI*180);
#if 0
         float p0x=cos(alpha/180*M_PI)*r0;
         float p0y=sin(alpha/180*M_PI)*r0;
         int xmin=int(cos(alpha/180*M_PI)*(-tmin)+p0x)+sizeX2;
         int xmax=int(cos(alpha/180*M_PI)*(tmin)+p0x)+sizeX2;
         int ymin=int(sin(alpha/180*M_PI)*(-tmin)+p0y)+sizeY2;
         int ymax=int(sin(alpha/180*M_PI)*(tmin)+p0y)+sizeY2;
//       printf("Draw Line (%5d %5d)->(%5d %5d) (p0=(%f,%f)) (r0=%f, alpha=%f)\n", xmin,ymin,xmax,ymax,p0x,p0y,r0,alpha);
#endif
         drawLine(debug,xmin,ymin,xmax,ymax,1);
       }
  }
  debug.write(debugfile);
  return 0;
}

