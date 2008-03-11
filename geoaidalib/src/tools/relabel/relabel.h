/***************************************************************************
                          combine.h  -  description
                             -------------------
    begin                : Wed Jan 22 2003
    copyright            : (C) 2003 by Oliver Stahlhut
    email                : stahlhut@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <cstdio>
#include <iostream>
#include <gaimage.h>
#include <gaimaget.h>
#include <garegiont.h>

//	MAX_LEVEL includes original resolution
#define MAX_LEVEL 4
//	number of classes in image
#define CLASSES 4
//	outputs unlabeled and extra outputs in all resolution levels
//  #define DEBUG_MSG



int combine (Ga::Image *in, Ga::Image &mask, char* eval_name, char *path);
int compose_result (Ga::Image *in, char* eval_name, char* regdes_name, char *outimg_name);
Ga::ImageT<int> *relabelOutput (Ga::Image &Img, char *labelname, char *descr);
