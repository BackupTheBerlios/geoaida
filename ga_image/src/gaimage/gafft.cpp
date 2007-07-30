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

#include "gafft.h"

using namespace std;
using namespace Ga;

const int verbose=0;


// Spatial -> Frequency
// Real -> Complex
int Ga::fft_real2compl(const Image& real,
                       const bool polar, 
                       Image& realout, Image& imagout) {  
    const Image imag(real.typeId(), real.sizeX(), real.sizeY());
    return fft_compl2compl(real, imag, polar, realout, imagout);
}



// Spatial -> Frequency
// Complex -> Complex
int Ga::fft_compl2compl(const Image& real, const Image& imag, 
                        const bool polar, 
                        Image& realout, Image& imagout) {  
    if (verbose) clog << "fft2D, " << endl;
    Image inreal = real;    
    Image inimag = imag;
    const double PI = acos(0)*2;                 

    if ((inreal.sizeX()!=inimag.sizeX()) ||     
        (inreal.sizeY()!=inimag.sizeY()))
        return -1;
    
    
    if (inreal.noChannels() != 1) {
        inreal = inreal.convert2Luminance();
    }  
    if (inimag.noChannels() != 1) {
        inimag = inimag.convert2Luminance();
    }  
            
    const int sizeX=inreal.sizeX();
    const int sizeY=inreal.sizeY();
    Image outreal(typeid(float), sizeX, sizeY); //Image contains two images   
   
    fftw_complex *data = (fftw_complex *)fftw_malloc(sizeX * sizeY * sizeof(fftw_complex));
    fftw_complex *dataout = (fftw_complex *)fftw_malloc(sizeX* sizeY * sizeof(fftw_complex));
            
    for (int y = 0; y < sizeY; y ++)
        for (int x = 0; x < sizeX; x ++){
            double * s = (double *) &data[y*sizeX + x];
            *(s++) = inreal.getFloat(x, y, 0);
            *(s) = inimag.getFloat(x, y, 0);

        }
                  
    fftw_plan plan = fftw_plan_dft_2d(sizeY, //According to Docu this is wrong, but it just works this way
                                        sizeX, //According to Docu this is wrong, but it just works this way
                                        data,
                                        dataout,
                                        FFTW_FORWARD,
                                        FFTW_ESTIMATE);
      
    if (!plan)
        return 1;

    fftw_execute(plan);

      

    for (int y = 0; y < sizeY; y ++)
        for (int x = 0; x < sizeX; x ++){
            double * val= (double *)&dataout[y * sizeX + x];
            const double realVal = val[0]/sizeX/sizeY;// FFTW calculates non-normalized fft 
            const double imVal = val[1]/sizeX/sizeY;  
            if (polar){
                double r=sqrt(pow(realVal,2)+pow(imVal, 2));
                realout.set(x, y, r);
                double phi=0;
                
                phi=atan2(realVal,imVal);
                                
                imagout.set(x, y,
                        phi,0);                  
            }
            else{
                
                realout.set(x, y, realVal, 0);
                imagout.set(x, y, imVal, 0);
            }
        }
              
    fftw_free(data);
    fftw_free(dataout);
    fftw_destroy_plan(plan);
        
    return 0;
}

/* 
   Inverse fft
   Frequency -> Spatial
   Complex -> Complex
   when polar=true real=r imag=phase
*/
int Ga::ifft_compl2compl(const Image& real, const Image& imag, 
                             const bool polar,
                             Image& realout, Image& imagout) {  

    if (verbose) clog << "ifft2D, " << endl;
    Image inreal = real;
    Image inimag = imag;

    const double PI = acos(0)*2;               
  
    const int sizeX=inreal.sizeX();
    const int sizeY=inreal.sizeY();
    Image outreal(typeid(float), sizeX, sizeY);
    Image outimag(typeid(float), sizeX, sizeY);

    
    if ((inreal.sizeX()!=inimag.sizeX()) ||     
        (inreal.sizeY()!=inimag.sizeY()))
        return -1;
    

    
    if (inreal.noChannels() != 1) {
        inreal = inreal.convert2Luminance();
    }  
    if (inimag.noChannels() != 1) {
        inimag = inimag.convert2Luminance();
    }  



    
    fftw_complex *data = (fftw_complex *)fftw_malloc(sizeX * sizeY * sizeof(fftw_complex));
    fftw_complex *dataout = (fftw_complex *)fftw_malloc(sizeX * sizeY * sizeof(fftw_complex));
        
    
    for (int y = 0; y < sizeY; y ++)
        for (int x = 0; x < sizeX; x ++){
            double * s = (double *) &data[y*sizeX + x];
            if (polar){
                const double PI = acos(0)*2;
                const double r=inreal.getFloat(x, y,0);
                const double phi=inimag.getFloat(x, y,0);
                *(s++) = r*cos(phi);
                *(s) = r*sin(phi);
                
            }
            else{
                *(s++) = inreal.getFloat(x, y, 0);
                *(s) = inimag.getFloat(x, y, 0);
            }
        }
    
    fftw_plan plan = fftw_plan_dft_2d(sizeY, //According to Docu this is wrong, but it just works this way
                                      sizeX, //According to Docu this is wrong, but it just works this way
                                      data,
                                      dataout,
                                      FFTW_BACKWARD,
                                      FFTW_ESTIMATE);
    

    if (!plan)
        return 1;

    fftw_execute(plan);

    
    for (int y = 0; y < sizeY; y ++)
        for (int x = 0; x < sizeX; x ++){
            double * val= (double *)&dataout[y * sizeX + x];
            const double realVal = val[0]/sizeX/sizeY;// FFTW calculates non-normalized fft 
            const double imVal = val[1]/sizeX/sizeY;  
            realout.set(x, y, val[0], 0);
            imagout.set(x, y, val[1], 0);
        }
    fftw_free(data);
    fftw_free(dataout);
    fftw_destroy_plan(plan);
    
    return 0;
}    

