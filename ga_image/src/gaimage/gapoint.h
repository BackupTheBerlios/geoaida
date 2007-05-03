#ifndef GA_POINT_H
#define GA_POINT_H

#include <typeinfo>
#include <gaimage.h>

namespace Ga {
  
  class Point {
  public:
    Point(int x, int y): x_(x),y_(y) {}
      Point(const Point& p) {x_=p.x_; y_=p.y_;}
      int x() const {return x_;}
      int y() const {return y_;}
      bool operator==(const Point& p2) const { return (x_==p2.x_ && y_==p2.y_);}
      Point operator-(const Point& p2) const {Point p(*this); p.x_-=p2.x_; p.y_-=p2.y_; return p; }
      int x(int x) {x_=x; return x;}
      int y(int y) {x_=y; return y;}
  protected:
      int x_,y_;
  };
}

  
#endif
