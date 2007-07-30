/* -*- mode:c++ -*- */
//*******************************************************************
//  Filename:   gacorrelation.cpp
//  Ort:    TNT, Uni. - Hannover, Germany
//
//  Info:	Correlation of two input images
//
// Copyright (C) 2007 Christian Becker
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//*******************************************************************


#include <math.h>
#include <fftw3.h>

#include <string>
#include <iostream>
#include <stdlib.h>

#include <gaimage.h>
#include <garegion.h>
#include <unistd.h>

#include "gafft.h";

using namespace std;
using namespace Ga;

const int verbose=0;

Ga::Image Ga::fft(const Image& img, bool polar=false) {  
    if (verbose) clog << "fft2D, " << endl;
    Image in = img;

    if (in.noChannels() != 1) {
        in = in.convert2Luminance();
    }  
           
    const int sizeX=in.sizeX();
    const int sizeY=in.sizeY();
    Image out(typeid(float), sizeX*2, sizeY); //Image contains two images, left:Re right:Im    
   
    fftw_complex *data = (fftw_complex *)fftw_malloc(sizeX * sizeY * sizeof(fftw_complex));
    fftw_complex *dataout = (fftw_complex *)fftw_malloc(sizeX* sizeY * sizeof(fftw_complex));
    
    
    
    for (int y = 0; y < sizeY; y ++)
        for (int x = 0; x < sizeX; x ++){
            double * s = (double *) &data[y*sizeX + x];
            *(s++) = in.getFloat(x, y, 0);
            *(s) = 0;
        }
                  
    fftw_plan plan = fftw_plan_dft_2d(sizeY, 
                                        sizeX, 
                                        data,
                                        dataout,
                                        FFTW_FORWARD,
                                        FFTW_ESTIMATE);
      
    assert(plan);
    fftw_execute(plan);

      

    for (int y = 0; y < sizeY; y ++)
        for (int x = 0; x < sizeX; x ++){
            double * val= (double *)&dataout[y * sizeX + x];
            const double realVal = val[0]/sizeX/sizeY;// FFTW calculates non-normalized fft 
            const double imVal = val[1]/sizeX/sizeY;  
            if (polar){
                double r=sqrt(pow(realVal,2)+pow(imVal, 2));
                out.set(x, y, r, 0);
                double phi=0;
                const double PI = acos(0)*2;                 
                phi=atan2(realVal,imVal);
                
                
                out.set(x + sizeX, y,
                        phi,0);                  
            }
            else{
                
                out.set(x, y, realVal, 0);
                out.set(x + sizeX, y, imVal, 0);
            }
        }
              
    fftw_free(data);
    fftw_free(dataout);
    fftw_destroy_plan(plan);
        
    return out;
}

// Inverse fft
Ga::Image Ga::ifft(const Image& img, bool polar) {  
    if (verbose) clog << "ifft2D, " << endl;
    Image in = img;

    if (in.noChannels() != 1) {
        in = in.convert2Luminance();
    }  
    
    fftw_complex *data = (fftw_complex *)fftw_malloc(in.sizeX() * in.sizeY()* sizeof(fftw_complex));
    fftw_complex *dataout = (fftw_complex *)fftw_malloc(in.sizeX()* in.sizeY()* sizeof(fftw_complex));
    
    assert(in.sizeX()%2==0);
    const int sizeX=in.sizeX()/2;
    const int sizeY=in.sizeY();
    
    Image out(typeid(float), sizeX, sizeY);
    
    for (int y = 0; y < sizeY; y ++)
        for (int x = 0; x < sizeX; x ++){
            double * s = (double *) &data[y*sizeX + x];
            if (polar){
                const double PI = acos(0)*2;
                const double r=in.getFloat(x, y,0);
                const double phi=in.getFloat(x+sizeX, y,0);
                *(s++) = r*cos(phi);
                *(s) = r*sin(phi);
                
            }
            else{
                *(s++) = in.getFloat(x, y, 0);
                *(s) = in.getFloat(x+sizeX, y, 0);
            }
        }
    
    fftw_plan plan = fftw_plan_dft_2d(sizeY, 
                                      sizeX, 
                                      data,
                                      dataout,
                                      FFTW_BACKWARD,
                                      FFTW_ESTIMATE);
    


    fftw_execute(plan);

    
    for (int y = 0; y < sizeY; y ++)
        for (int x = 0; x < sizeX; x ++){
            double * val= (double *)&dataout[y * sizeX + x];
            const double realVal = val[0]/sizeX/sizeY;// FFTW calculates non-normalized fft 
            const double imVal = val[1]/sizeX/sizeY;  
            out.set(x, y, val[0], 0);
        }
    fftw_free(data);
    fftw_free(dataout);
    fftw_destroy_plan(plan);
    
    return out;
}    

