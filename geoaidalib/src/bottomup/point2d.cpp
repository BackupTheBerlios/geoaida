/***************************************************************************
                          point2d.cpp  -  description
                             -------------------
    begin                : Mon Aug 6 2001
    copyright            : (C) 2001 by jrgen bckner
    email                : bueckner@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//	#define DEBUGMSG

#include "point2d.h"

#ifdef WIN32
#include <math.h>
#endif

Point2D::Point2D():x_(0), y_(0) {
}

Point2D::~Point2D(){
}

double Point2D::distance (const Point2D &p) const {
  return sqrt(double((x_-p.x())*(x_-p.x())+(y_-p.y())*(y_-p.y())));
}

double Point2D::direction (const Point2D &p) const {

float angle=0.0;
	
	int dx= x_-p.x(), dy= y_-p.y();
	float quotient= float(dy) / float (dx);
	if (quotient < 0.0)
		quotient= -1.0*quotient;

	if ((dx == 0) && (dy == 0))
		angle= 0.0;
	else if ((dx == 0) && (dy<0))
		angle= (3.0*M_PI)/2.0;
	else if ((dx == 0) && (dy>0))
		angle= M_PI/2.0;
	else if ((dx>0) && (dy == 0))
		angle= M_PI;
	else if ((dx<0) && (dy == 0))
		angle= 0.0;
	//	1. Quadrant
	else if ((dx>0) && (dy<0))
		angle= M_PI + atan (quotient);
	//	2. Quadrant
	else if ((dx<0) && (dy<0))
		angle= 2.0*M_PI - atan (quotient);
	//	3. Quadrant
	else if ((dx<0) && (dy>0))
		angle= atan (quotient);
	//	4. Quadrant
	else if ((dx>0) && (dy>0))
		angle= M_PI - atan (quotient);													

	// Umrechnung
	angle= (angle * 180.0) / M_PI;

	angle= (double) angle;
#ifdef DEBUGMSG
cout<<"dx="<<dx<<" dy="<<dy<<" angle="<<angle<<endl;		
#endif
	
return (angle);
}	

double Point2D::sqr_dist(const Point2D &p) const {
  return double((x_-p.x())*(x_-p.x())+(y_-p.y())*(y_-p.y()));
}

double Point2D::distance (void) const {
  return sqrt(double((x_*x_)+(y_*y_)));
}

double Point2D::xdist(Point2D& p) const {
  int dist = x_-p.x();
  return (dist>0) ? dist : -dist;
}

double Point2D::ydist(Point2D& p) const{
  int dist = y_-p.y();
  return (dist>0) ? dist : -dist;
}
