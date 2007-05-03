/***************************************************************************
                          gi_pick_color.cpp  -  description
                             -------------------
    begin                : 2004
    copyright            : (C) 2004 by Soenke Mueller
    email                : mueller@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#undef DEBUG
//  #define DEBUG
#define CLASSES 3

#include <stdlib.h>
#include <stdio.h>

#include <gaimage.h>
#include <gaimaget.h>
#include <gadraw.h>
#include <garegiont.h>
//#include <unistd.h>

using namespace std;
using namespace Ga;

int main(int argc, char *argv[])
{

   /****************************************************************************
   * Get the command line arguments.
   ****************************************************************************/
   
   if(argc == 1){
      cout<<"gi_pick_color - picks a color in an image"<<endl;
      cout<<"options:  input image,"<<endl;
      cout<<"          x,"<<endl;
      cout<<"          y,"<<endl;
      cout<<"          hypothesis_class."<<endl;      
      exit(1);
                }
     
  char *input_name= argv[1];
  int x= atoi(argv[2]);
  int y= atoi(argv[3]);
  int hclass= atoi(argv[4]);
  char *output_file= argv[5];
 	float temp_matrix[CLASSES][CLASSES]={  {1.0,  0.3,		0.3},
																				 {0.0,	0.65,		0.05},
																				 {0.0,	0.05,		0.65}};

   // read input image ---------------------------------------------------------------------------
  
  Image im;
  im.read(input_name);
  x= im.sizeX();
  y= im.sizeY();  
  int old_class_1= im.getInt(5,5);
  int old_class_2, old_class_3, old_class_4;
  if ((75<x) && (75<y)){
    old_class_2= im.getInt(75,5);
    old_class_3= im.getInt(5,75);  
    old_class_4= im.getInt(75,75);
  }
  else{
    old_class_2= old_class_1;
    old_class_3= old_class_1;
    old_class_4= old_class_1;
    }
  
  FILE *fp=fopen(output_file, "w");
  fprintf (fp,"OLD=%f;",(temp_matrix[hclass-1][old_class_1-1]+temp_matrix[hclass-1][old_class_2-1]+
  temp_matrix[hclass-1][old_class_3-1]+temp_matrix[hclass-1][old_class_4-1])/4.0);

  fclose(fp);

  #ifdef DEBUG
  cout << "programm correct passed through" << endl;
  #endif

  return EXIT_SUCCESS;
}
