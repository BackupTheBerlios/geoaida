/***************************************************************************
                          ga_threshold_segment.h  -  description
                             -------------------
    begin                : Fri Aug 24 2007
    copyright            : (C) 2007 Christian Becker
    email                : becker@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef GA_THRESHOLD_SEGMENT_H
#define GA_THRESHOLD_SEGMENT_H

#include <garegionsplittert.h>
#include <garegionsplitter.h>
#include <gaimage.h>
using namespace Ga;

  class RegDescThres : public RegDesc
  {
  public: 
      RegDescThres(){
          average_=0;
          regsize_=0;
      }
      double average_;
      int regsize_;

      virtual string attributes2string()
          {            
              string s = RegDesc::attributes2string();
              ostringstream out;

              // clog << "Old attributes " << s << endl;
              // clog << "New attribute Average: " << average_ << endl;

              out << s << " average=\"" << average_ << "\" ";
              out << s << " reg_size=\"" << regsize_ << "\" ";

              return out.str();
          }
  };

class RegionFinderThres : public RegionFinder{
public:
    RegionFinderThres(const Image& mask, const Image& data, float level)
        : RegionFinder(mask, data, level){};

    bool valid(const Image &lpic, int x, int y){
        bool retval=true;
        retval &= (mask_.getInt(x, y)>0);
        retval &= (data_.getFloat(x, y) > 1);

        return retval;
    }
    
protected:
    
};


#endif
