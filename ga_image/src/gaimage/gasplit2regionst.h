#ifndef GaSplit2RegionsT_h
#define GaSplit2RegionsT_h

#ifdef linux
extern "C" {
int __isnanf(float);
	   }
#ifndef NAN
#define NAN sqrt(-1)
#endif
#endif

#include <vector>
#include <string>
#include <iostream>
#include <sstream>



/*
  RegDescT
     int setPixel(const DataPicT& dpic, LabelPicT& lpic, int x, int y, int val)
     int setId(int val);
     int size();
     int id()
     int llx()
     int lly()
     int urx()
     int ury()
  DataPicT:
     int sizeX()
     int sizeY()
     int|float getFloat(x,y)

  LabelPicT:
     sizeX()
     sizeY()
     set(x,y,value)
  TestClass:
     (const DataPicT&,const LabelPicT&,x_center,y_center,x_neighbour,y_neighbour)
     valid(const DataPicT,const LabelPicT,x,y)
 */


template <class RegDescT,
  class DataPicT,
  class LabelPicT,
  class TestClass>
std::vector<RegDescT > *splitIntoRegionsT(const DataPicT &hpic,
                                    LabelPicT &lpic,
                                    TestClass tclass,
                                    int minSize, int maxSize);


template <class RegDescT, class LabelPic>
void findNeighbours(LabelPic & lpic,
                    RegDescT &region,
                    int num_regions);





#include "gasplit2regionst.hpp"

#define USE_EXAMPLE_CLASSES
#ifdef USE_EXAMPLE_CLASSES

/*
  Class to provide the basic region description for one region.
  This class should be derived to get specified region descriptions
  e.g. when having operator depending attributes which should  be 
  included to the region description.

*/
template <class DataPicT, class LabelPicT>
class RegDescT
{
  public:
    RegDescT()
      {
        llx_=INT_MAX;
        ury_=INT_MAX;
        urx_=INT_MIN;
        lly_=INT_MIN,
        size_=0;
        sum_=0;
        numValidValues_=0;
        id_=0;
        class_ = "undefined";
      };
    int setPixel(const DataPicT& dpic, LabelPicT& lpic, int x, int y, int val)
    {
      lpic.set(x, y, val);
      size_++;
      float v=dpic.getFloat(x,y);
      if (!isnanf(v)) {
        sum_+=v;
        numValidValues_++;
      }
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
    int size() {return size_;}
    int id() {return id_;}
    int llx() {return llx_;}
    int lly() {return lly_;}
    int urx() {return urx_;}
    int ury() {return ury_;}
    
    std::string attributes2string()
        {            
            std::ostringstream out;
            out << "class=\"" << class_ << "\" ";
            out << "id=\"" << id_ << "\" ";
            out << "file=\"" << file_ << "\" ";
            out << "llx=\"" << llx_ << "\" ";
            out << "lly=\"" << lly_ << "\" ";
            out << "urx=\"" << urx_ << "\" ";
            out << "ury=\"" << ury_ << "\" ";
            if (!name_.empty())
                out << "name=\"" << name_ << "\" ";
            return out.str();
        }
    
    std::string toString(){
        std::ostringstream out = "<region ";
        out << attributes2string();
        out << "/>" << endl;
        return out;
    }

    std::string class_, file_, name_;    

protected:
    int llx_,lly_,urx_,ury_;
    int id_;
    int size_, numValidValues_;
    double sum_;

};


template <class DataPicT, class LabelPicT>
class DifferenceT
{
 public:
  DifferenceT(float level) { level_=level;}
  bool operator()(const DataPicT &hpic,
	     const LabelPicT &lpic,
	     int x_center, int y_center,
	     int x_neighbour, int y_neighbour)
    {
      if (!valid(hpic,lpic,x_neighbour,y_neighbour)) return false;
      float v=hpic.getFloat(x_center,y_center);
      float vn=hpic.getFloat(x_neighbour,y_neighbour);
      if (isnan(v) && isnan(vn)) return true;
      if (isnan(v) || isnan(vn)) return false;
      return fabs(v-vn)<level_;
    }
  bool valid(const DataPicT &hpic, const LabelPicT &lpic, int x, int y) {
    return true;
  }
 private:
  float level_;
};

#endif

#endif
