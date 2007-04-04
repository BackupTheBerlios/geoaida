/* -*- mode:c++ -*- */
//*******************************************************************
//  Filename:   garegression.cpp
//  Ort:        TNT, Uni. - Hannover, Germany
//
//  Info:	Segmentation by Linear Regression
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
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>

namespace Ga {


const int featureCount=2;
   
Image regression(const Image& imgIn,
		 const Image& maskIn,
		 const Image& learnfeatureIn1,
		 const Image& learnfeatureIn2
                    )
{
   Image result(typeid(int),imgIn.sizeX(),imgIn.sizeY()); 
   Image mask(typeid(int),maskIn.sizeX(),maskIn.sizeY()); 

   if ((learnfeatureIn1.sizeX() != learnfeatureIn2.sizeX())||
       (learnfeatureIn1.sizeY() != learnfeatureIn2.sizeY()))
     {
	exit(1);	
     }
   

   
   
   gsl_matrix * mf1 = gsl_matrix_alloc (learnfeatureIn1.sizeX(), learnfeatureIn1.sizeY());
   gsl_matrix * mf2 = gsl_matrix_alloc (learnfeatureIn2.sizeX(), learnfeatureIn2.sizeY());
   gsl_matrix * mf12 = gsl_matrix_alloc (learnfeatureIn2.sizeX(), learnfeatureIn2.sizeY());
   
   gsl_matrix * moments = gsl_matrix_alloc (featureCount+1, featureCount+1);
   gsl_matrix * mcoeff = gsl_matrix_calloc (featureCount+1, featureCount+1); // init with 0 in all places

   
   for (int x=0; x < learnfeatureIn1.sizeX(); x++)
     for (int y=0; y < learnfeatureIn1.sizeY(); y++)
       {
	  float f1=learnfeatureIn1.getFloat(x,y);
	  float f2=learnfeatureIn2.getFloat(x,y);
	  
	  gsl_matrix_set(mf1, x, y, f1);
	  gsl_matrix_set(mf2, x, y, f2);	 
       }
   gsl_matrix_memcpy(mf1, mf12);      
   gsl_matrix_mul_elements(mf12, mf2);
   
   
   double ef1, ef2, ef11, ef12, ef22;
   ef1=0;
   ef2=0;
   ef11=0;
   ef12=0;
   ef22=0;
   
   int s=learnfeatureIn1.sizeX()*learnfeatureIn1.sizeY();
   
   for (int x=0; x < learnfeatureIn1.sizeX(); x++)
     for (int y=0; y < learnfeatureIn1.sizeY(); y++)
       {
	  ef1+=gsl_matrix_get(mf1, x, y)/s;
	  ef11+=gsl_matrix_get(mf1, x, y)*gsl_matrix_get(mf1, x, y)/s;
	  ef2+=gsl_matrix_get(mf2, x, y)/s;
	  ef22+=gsl_matrix_get(mf2, x, y)*gsl_matrix_get(mf2, x, y)/s;	  
	  ef12+=gsl_matrix_get(mf12, x, y)/s;
       }
   
   
   
   //Momentenmatrix aufstellen	  
   gsl_matrix_set(moments, 0, 0, 1);
   gsl_matrix_set(moments, 0, 1, ef1);
   gsl_matrix_set(moments, 1, 0, ef1);
   gsl_matrix_set(moments, 0, 2, ef2);
   gsl_matrix_set(moments, 2, 0, ef2);
   gsl_matrix_set(moments, 1, 1, ef11);
   gsl_matrix_set(moments, 2, 2, ef22);
   gsl_matrix_set(moments, 1, 2, ef12);
   gsl_matrix_set(moments, 2, 1, ef12);
   

   int classCount=(int) mask.maxValue();
   
   // Streumatrix: Anzahl Merkmale x Anzahl Klassen
   gsl_matrix * mstreu = gsl_matrix_alloc (featureCount, classCount);
   gsl_matrix_set_zero(mstreu);
         
   gsl_vector* vcolchange =gsl_vector_alloc(featureCount);
   
   // Berechnung der Streumatrix
   for (int i=1; i <=classCount; i++)
     {	
	for (int x=0; x < mask.sizeX(); x++)
	  for (int y=0; y < mask.sizeY(); y++)
	    {
	       int classIndex=mask.getInt(x,y);
	       
	       if (classIndex) //else Background
		 {
		    gsl_vector_view colview = gsl_matrix_column(mstreu, classIndex);
		    gsl_vector_set(vcolchange, 1, gsl_matrix_get(mf1, x, y)/s);
		    gsl_vector_set(vcolchange, 2, gsl_matrix_get(mf2, x, y)/s);
		    
		    gsl_vector_add(&colview.vector, vcolchange);		    
		 }	       	       	       
	    }		    
     }
   
   
   // mcoeff += 1*momentsInv*mstreu;
   
   
   
   /// momentsInv = inverse der momentenmatrix muss noch bestimmt werden
   
   
   
   
   
  // gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1, momentsInv, mstreu, 1, mcoeff)
   
   
   
   
   

   
   
   
   
   
   return result;
}
   


} // namespace Ga

