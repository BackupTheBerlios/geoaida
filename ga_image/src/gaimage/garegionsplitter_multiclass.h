#ifndef REGIONSPLITTER_MULTICLASS
#define REGIONSPLITTER_MULTICLASS

#include "garegionsplitter.h"
#include <map>
#include <string>


typedef std::map<int, std::string> ClassnameMapper;


class MulticlassRegionFinder : public Ga::RegionFinder
{

public:
 MulticlassRegionFinder(const Ga::Image& mask, const Ga::Image& data, float level)
   : Ga::RegionFinder(mask, data, level) {};
  


  /*
    Can be used to give the regions names, depending
    on the value in the input image.
    
    Return empty string when the region name
    in RegionSplitterT should be used;
  */
  
  virtual std::string getRegionClass(int x, int y, int channel=0){	  	  
    return classnamemapper_[data_.getInt(x, y, channel)];
  }
  
  void setClassnameMapper(const ClassnameMapper classnamemapper){
    classnamemapper_=classnamemapper;
  }

  /*
    lpig: meaningless in this implementation
    x_center, y_center: coordinate1 in data image
    x_neighbour, y_neighbour: coordinate2 in data image
    
    Should return true iff the pixels at the two coordinates be in the same region.
    
    returns true when mask at coordinate1 equals mask at coordinate2
    AND coordinate1 in data differs from coordinate2 in data no more than level
    else false 
  */
  virtual bool operator()(const Ga::Image &lpic,
			  int x_center, int y_center,
			  int x_neighbour, int y_neighbour)
  {
    /*
      if (mask_.getInt(x_center, y_center) 
	!= mask_.getInt(x_neighbour, y_neighbour))
      return false; // outmasked
    */
    
    return (fabs(data_.getFloat(x_center, y_center) - 
		 data_.getFloat(x_neighbour, y_neighbour)) 
	    <= level_);
  }
  /*
    Can be used to mask out pixels which should not 
    be in any region (eg background pixels).
    
    In this implementation it returns true iff the value in data image is greater than 0 (not background)
  */
  virtual bool valid(const Ga::Image &lpic,
		     int x, int y)
  {
    return true;
  }	

 private:
	std::map<int, std::string> classnamemapper_;

};

int relabelOutput(const Ga::Image& imageToRelabel,
		   ClassnameMapper classnamemapper,
		   const std::string& labelimagefilename, 
		   const std::string& regiondescriptionfilename, 
		  int minsize=0, int maxsize=INT_MAX, int greyLevelDifference=0);

struct BoundingBox 
{
   int llx;
   int lly;
   int urx;
   int ury;
   
   BoundingBox()
     {
	llx=INT_MAX;
	lly=0;
	urx=0;
	ury=INT_MAX;
     }
   
};


/*
 * Converts a label image into a GeoAIDA operator output: saves label file and generates xml-region-description
 * In this implementation only so many regions are created as there are Label Values
 * Thus regions can have many parts and can become quite large.
 * 
 */
int simpleOutput(const Ga::Image& imageToRelabel,
		       ClassnameMapper classnamemapper,
		       const std::string& labelimagefilename, 
		       const std::string& regiondescriptionfilename);
#endif
