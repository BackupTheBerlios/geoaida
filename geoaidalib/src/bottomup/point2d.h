/***************************************************************************
                          point2d.h  -  description
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

#ifndef POINT2D_H
#define POINT2D_H

#include <math.h>
#include <iostream>

/**class for 2D points
  *@author juergen bueckner
  */

class Point2D {
public: 
	Point2D();
	Point2D(int x, int y):x_(x), y_(y){};
	~Point2D();
	
	/** return X value of the point */
	int x(void) const {return x_;}
	/** return Y value of the point */
	int y(void) const {return y_;}
	/** set X value of the point */
	void x(int x) {x_ = x;}
	/** set Y value of the point */
	void y(int y) {y_ = y;}
	/** returns the Euclidean distance*/
  double distance (const Point2D&) const;
  /** returns the square of the Euclidean distance */
  double  sqr_dist(const Point2D&) const;
	/** returns the Euclidean distance to 0,0 */
  double distance (void) const;
  /**  returns the horizontal distance */
  double xdist(Point2D&) const;
	/** returns the vertical distance */
  double ydist(Point2D&) const;
	/** returns the direction between two Point2Ds */
  double direction (const Point2D &p) const;
  /** add point - vector addition*/
  void operator+(const Point2D& p)  { x_+=p.x(); y_+=p.y(); }
  /** sub point - vector substraction*/
  void operator-(const Point2D& p)  { x_-=p.x(); y_-=p.y(); }
  /** compate vector - identical*/
  bool operator==(const Point2D& p) const {return (x_==p.x() && y_==p.y());}
  /** compate vector - unequal*/
  bool operator!=(const Point2D& p) const { return !operator==(p);}
  /** assignment */
  void operator=(const Point2D& p) { x_=p.x(); y_=p.y();}


  int x_,y_;
};

#endif
