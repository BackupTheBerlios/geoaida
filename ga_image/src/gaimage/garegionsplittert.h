#ifndef GaRegionSplitterT_h
#define GaRegionSplitterT_h

#ifdef linux
extern "C" {
int __isnanf(float);
	   }
#ifndef NAN
#define NAN sqrt(-1)
#endif
#endif

#include <vector>
using namespace std;

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
     (const Image&,x_center,y_center,x_neighbour,y_neighbour)
     valid(const LabelPicT,x,y)
 */

//#include <gaimage.h>

template <class RegDescT, class TestClassT>
class RegionSplitterT{
public:
  RegionSplitterT(vector<RegDescT>& regList, Ga::Image& labelImg, TestClassT& testClass, int minSize=1, int maxSize=0, int llx=-1, int lly=-1, int urx=-1, int ury=-1);
  /** split the data into regions and returns the next free id */
  int split();
  int startId(int id=-1);
  void setBBox(int llx, int lly, int urx, int ury);
  void setBgId(int id);
  void setSmallRegionsId(int id);
protected:
  void goWest(int x, int y, int value, int def_val);
  void goNorth(int x, int y, int value, int def_val);
  void goEast(int x, int y, int value, int def_val);
  void goSouth(int x, int y, int value, int def_val);
  vector<RegDescT>* classify(int minSize, int maxSize);
  int setPixel(int x, int y, int val);
protected:
  vector<RegDescT>& regionList_;
  Ga::Image& lpic_;
  TestClassT& tclass_;
  int minSize_;
  int maxSize_;
  int llx_,lly_,urx_,ury_;
  int startId_;
  /* Pixel in the labelImg with this value are not bound to a region */
  int bgId_;
  /* Set regions smalle then minsize to this value */
  int smallRegionsId_;
private:
  int rl_;
  RegDescT region_;
};


template <class RegDescT, class TestClassT>
vector<RegDescT>* splitIntoRegions(Ga::Image& lpic, TestClassT tclass, int minSize, int maxSize);


#if 0
template <class RegDescT, class LabelPic>
void findNeighbours(LabelPic & lpic,
                    RegDescT &region,
                    int num_regions);
#endif

#include "garegionsplittert.hpp"


#ifdef USE_EXAMPLE_CLASSES

template <class DataPicT, class LabelPicT>
class RegDesc
{
  public:
    RegDesc()
      {
        llx_=INT_MAX;
        ury_=INT_MAX;
        urx_=INT_MIN;
        lly_=INT_MIN,
        size_=0;
        sum_=0;
        numValidValues_=0;
        id_=0;
      };
    int setPixel(LabelPicT& lpic, int x, int y, int val)
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
    int size() {return size_;}
    int id() {return id_;}
    int llx() {return llx_;}
    int lly() {return lly_;}
    int urx() {return urx_;}
    int ury() {return ury_;}
  protected:
    int llx_,lly_,urx_,ury_;
    int id_;
    int size_, numValidValues_;
    double sum_;
};

class Difference
{
 public:
  Difference(const Ga::Image& dpic, float level) : dpic_(dpic) { level_=level;}
  bool operator()(const Ga::Image& lpic,
	     int x_center, int y_center,
	     int x_neighbour, int y_neighbour)
    {
      if (!valid(lpic,x_neighbour,y_neighbour)) return false;
      float v=dpic_.getFloat(x_center,y_center);
      float vn=dpic_.getFloat(x_neighbour,y_neighbour);
      if (isnan(v) && isnan(vn)) return true;
      if (isnan(v) || isnan(vn)) return false;
      return fabs(v-vn)<level_;
    }
  bool valid(const Ga::Image& lpic, int x, int y) {
    return true;
  }
 private:
  const Ga::Image& dpic_;
  float level_;
};

#endif

#endif
