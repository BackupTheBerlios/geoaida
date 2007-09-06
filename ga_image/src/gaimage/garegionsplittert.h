/* -*- mode:c++ -*- */
//*******************************************************************
//  Filename:   garegionsplittert.h
//  Ort:        TNT, Leibniz Uni. - Hannover, Germany
//
//  Info:	Region Finding and Exporting Interface (Template Version)
//
// Copyright (C) 2002 Martin Pahl
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
#include <string>
#include <sstream>

using namespace std;


template <class RegDescT, class RegionFinderClassT>
class RegionSplitterT{
public:
  RegionSplitterT(vector<RegDescT>& regList, 
                  Ga::Image& labelImg, 
                  RegionFinderClassT& testClass, 
                  int minSize=1, 
                  int maxSize=0, 
                  int llx=-1, int lly=-1, int urx=-1, int ury=-1);

    /** split the data into regions and returns the next free id */
    int split();
    int startId(int id=-1);
    void setBBox(int llx, int lly, int urx, int ury);
    void setBgId(int id);
    void setSmallRegionsId(int id);
    void setRegionClass(string classname);
    void setLabelFile(string labelfilename);
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
    RegionFinderClassT& tclass_;
    int minSize_;
    int maxSize_;
    int llx_,lly_,urx_,ury_;
    int startId_;
    string regionClass_;
    string labelfilename_;

    /* Pixel in the labelImg with this value are not bound to a region */
    int bgId_;
    /* Set regions smalle then minsize to this value */
    int smallRegionsId_;
private:
    int rl_;
    RegDescT region_;
};


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
        file_="";
      };
    int setPixel(LabelPicT& lpic, int x, int y, int val)
    {
      lpic.set(x, y, val);
      size_++;
/* 
      float v=dpic.getFloat(x,y);
      if (!isnanf(v)) {
        sum_+=v;
        numValidValues_++;
      }
*/
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
    
    virtual string attributes2string()
        {            
            ostringstream out;
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
    
    string toString(){
        ostringstream out("<region ");
        out << "<region ";
        out << attributes2string();
        out << "/>";
        return out.str();
    }

    string class_, file_, name_;    

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

/*
template <class RegDescT, class RegionFinderClassT>
vector<RegDescT>* splitIntoRegionsT(Ga::Image& lpic, RegionFinderClassT tclass, int minSize, int maxSize);
*/


#include "garegionsplittert.hpp"



#endif
