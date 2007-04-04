#ifndef GA_POINT_H
#define GA_POINT_H

#include <typeinfo>
#include <gaimage.h>

namespace Ga {
  class Value {
    Value(bool v) 
      : type_(typeid(bool))
      {
	value_.bool_=v;
      }
    Value(char v) 
      : type_(typeid(char))
      {
	value_.char_=v;
      }
    Value(signed char v) 
      : type_(typeid(signed char))
      {
	value_.signedChar_=v;
      }
    Value(unsigned char v) 
      : type_(typeid(unsigned char))
      {
	value_.unsignedChar_=v;
      }
    Value(signed short v) 
      : type_(typeid(signed short))
      {
	value_.signedShort_=v;
      }
    Value(unsigned short v) 
      : type_(typeid(unsigned short))
      {
	value_.unsignedShort_=v;
      }
    Value(signed int v) 
      : type_(typeid(signed int))
      {
	value_.signedInt_=v;
      }
    Value(unsigned int v) 
      : type_(typeid(unsigned int))
      {
	value_.unsignedInt_=v;
      }
    Value(float v) 
      : type_(typeid(float))
      {
	value_.float_=v;
      }
  protected:
    const class std::type_info& type_;
    union {
      bool bool_;		
      char char_;		
      signed char signedChar_;  
      unsigned char unsignedChar_;	
      short short_;		
      signed short signedShort_;		
      unsigned short unsignedShort_;	
      int int_;			
      signed int signedInt_;		
      unsigned int unsignedInt_;		
      float float_;		
    } value_;
  };
  
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
