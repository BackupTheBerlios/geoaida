#ifndef GaVectorizeT_h
#define GaVectorizeT_h

#include <gaimage.h>

/*
  PointT
  RegDescT
     int setPixel(const DataPicT& dpic, LabelPicT& lpic, int x, int y, int val)
     int setId(int val);
     int size();
     int id()
     int llx()
     int lly()
     int urx()
     int ury()
     vector<const Point*> *border;
  LabelPicT:
     sizeX()
     sizeY()
     set(x,y,value)
 */

template < class RegDescT, class PointT  >
void vectorize(const Ga::Image& lpic,
               RegDescT &reg, double tolerance);

#include "gavectorizet.hpp"

#ifdef USE_EXAMPLE_CLASSES

class Point
{
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

class RegBorderDesc
{
  public:
    RegBorderDesc()
      {
        llx_=INT_MAX;
        ury_=INT_MAX;
        urx_=INT_MIN;
        lly_=INT_MIN,
        size_=0;
        sum_=0;
        id_=0;
      }
    int setPixel(Ga::Image& lpic, int x, int y, int val)
    {
      lpic.set(x, y, val);
      size_++;
      if (x < llx_)
        llx_ = x;
      if (y < ury_)
        ury_ = y;
      if (x > urx_)
        urx_ = x;
      if (y > lly_)
        lly_ = y;
      return val;
    }
    int setId(int val) {id_=val;return val;}
    int size() const {return size_;}
    int id() const {return id_;}
    int llx() const {return llx_;}
    int lly() const {return lly_;}
    int urx() const {return urx_;}
    int ury() const {return ury_;}
    vector<const Point*> *border(vector<const Point*> *border=0) {
        if (border) border_=border;
        return border_;
      }
  protected:
    int llx_,lly_,urx_,ury_;
    int id_;
    int size_;
    vector<const Point*> *border_;
    double sum_;
};

#endif

#endif
