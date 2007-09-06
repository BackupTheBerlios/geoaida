/* -*- mode:c++ -*- */
//*******************************************************************
//  Filename:   garegionsplittert.hpp
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

#include "garegionsplittert.h"

#define SMALL_REGIONS_TO_1 1

template <class RegDescT, class RegionFinderClassT>
RegionSplitterT<RegDescT,RegionFinderClassT>::RegionSplitterT(vector<RegDescT>& regList, 
                                                              Ga::Image& labelImg, 
                                                              RegionFinderClassT& testClass, 
                                                              int minSize, int maxSize, 
                                                              int llx, int lly, int urx, int ury)
  : regionList_(regList), lpic_(labelImg), tclass_(testClass), minSize_(minSize), maxSize_(maxSize), llx_(llx), lly_(lly), urx_(urx), ury_(ury)
{
  if (maxSize==-1) maxSize_=lpic_.sizeImage();
  if (llx==-1) llx_=0;
  if (lly==-1) lly_=lpic_.sizeY()-1;
  if (urx==-1) urx_=lpic_.sizeX()-1;
  if (ury==-1) ury_=0;
  smallRegionsId_=1;
  bgId_=0;
#ifdef SMALL_REGIONS_TO_1
  startId_=2;
#else
  startId_=1;
#endif
}

template <class RegDescT, class RegionFinderClassT>
int RegionSplitterT<RegDescT,RegionFinderClassT>::startId(int id) {
  if (id!=-1) startId_=id;
  return startId_;
}

template <class RegDescT, class RegionFinderClassT>
void RegionSplitterT<RegDescT,RegionFinderClassT>::setBgId(int id) {
  bgId_=id;
}

template <class RegDescT, class RegionFinderClassT>
void RegionSplitterT<RegDescT,RegionFinderClassT>::setRegionClass(string regionClass){
    regionClass_ = regionClass;
}

template <class RegDescT, class RegionFinderClassT>
void RegionSplitterT<RegDescT,RegionFinderClassT>::setLabelFile(string labelfilename){
    labelfilename_=labelfilename;
}

template <class RegDescT, class RegionFinderClassT>
void RegionSplitterT<RegDescT,RegionFinderClassT>::setSmallRegionsId(int id) {
  smallRegionsId_=id;
}

template <class RegDescT, class RegionFinderClassT>
void RegionSplitterT<RegDescT,RegionFinderClassT>::setBBox(int llx, int lly, int urx, int ury)
{
  if (urx>llx) {
    llx_=llx;
    urx_=urx;
  }
  else {
    llx_=urx;
    urx_=llx;
  }
  if (ury>lly) {
    lly_=ury;
    ury_=lly;
  }
  else {
    lly_=lly;
    ury_=ury;
  }

}

#include <math.h>
#include <limits.h>
#include <stdio.h>

#define sqr(x) ((x)*(x))

template <class RegDescT, class RegionFinderClassT>
inline int RegionSplitterT<RegDescT,RegionFinderClassT>::setPixel(int x, int y, int val)
{
    return region_.setPixel(lpic_,x,y,val);
    
}

// #define DBG_MSG
template <class RegDescT, class RegionFinderClassT>
void RegionSplitterT<RegDescT,RegionFinderClassT>::goWest(int x, int y, int value, int def_val)
{
  rl_++;
#ifdef DBG_MSG
  //  {int i; for (i=0; i<rl_; i++) fprintf(stderr," ");}
  fprintf(stderr, "goWest (%05d,%05d,%05d,%05d)\n", x, y, value, rl_);
#endif
  if (x > llx_) {
    if (lpic_.getInt(x - 1, y) != def_val) {
      rl_--;
      return;
    }
    if ((def_val!=bgId_) || tclass_(lpic_, x, y, x - 1, y)) {
      setPixel(x - 1, y, value);
    }
    else {
      rl_--;
      return;
    }
    x--;
  }
#ifdef DBG_MSG
  fprintf(stderr, "goWest Start\n");
#endif

  int xstart = x;
  int xend = llx_;
  for (x = xstart; x > llx_; x--) {
    if (lpic_.getInt(x - 1, y) != def_val) {
      xend = x;
      break;
    }
    if ((def_val!=bgId_) || tclass_(lpic_, x, y, x - 1, y)) {
      setPixel(x - 1, y, value);
    }
    else {
      xend = x;
      break;
    }
  }
  for (x = xstart; x > xend; x--) {
    goNorth(x, y, value, def_val);
    goSouth(x, y, value, def_val);
  }
  rl_--;
#ifdef DBG_MSG
  fprintf(stderr, "goWest done\n");
#endif
}

template <class RegDescT, class RegionFinderClassT>
void RegionSplitterT<RegDescT,RegionFinderClassT>::goNorth(int x, int y, int value, int def_val)
{
  rl_++;
#ifdef DBG_MSG
  //  {int i; for (i=0; i<rl_; i++) fprintf(stderr," ");}
  fprintf(stderr, "goNorth(%05d,%05d,%05d,%05d)\n", x, y, value, rl_);
#endif
  if (y > ury_) {
    if (lpic_.getInt(x, y - 1) != def_val) {
      rl_--;
      return;
    }
    if ((def_val!=bgId_) || tclass_(lpic_, x, y, x, y - 1)) {
      setPixel(x, y - 1, value);
    }
    else {
      rl_--;
      return;
    }
    y--;
  }
  int ystart = y;
  int yend = ury_;
  for (y = ystart; y > ury_; y--) {
    if (lpic_.getInt(x, y - 1) != def_val) {
      yend = y;
      break;
    }
    if ((def_val!=bgId_) || tclass_(lpic_, x, y, x, y - 1)) {
      setPixel(x, y - 1, value);
    }
    else {
      yend = y;
      break;
    }
  }
#ifdef DBG_MSG
  fprintf(stderr, "goNorth(start=%d, end=%d\n", ystart, yend);
#endif
  for (y = ystart; y > yend; y--) {
    goEast(x, y, value, def_val);
    goWest(x, y, value, def_val);
  }
  rl_--;
}

template <class RegDescT, class RegionFinderClassT>
void RegionSplitterT<RegDescT,RegionFinderClassT>::goEast(int x, int y, int value, int def_val)
{
  rl_++;
#ifdef DBG_MSG
  //  {int i; for (i=0; i<rl_; i++) fprintf(stderr," ");}
  fprintf(stderr, "goEast (%05d,%05d,%05d,%05d)\n", x, y, value, rl_);
#endif
  if (x < urx_) {
    if (lpic_.getInt(x + 1, y) != def_val) {
      rl_--;
      return;
    }
    if ((def_val!=bgId_) || tclass_(lpic_, x, y, x + 1, y)) {
      setPixel(x + 1, y, value);
    }
    else {
      rl_--;
      return;
    }
    x++;
  }
  int xstart = x;
  int xend = urx_;
  for (x = xstart; x < urx_; x++) {
    if (lpic_.getInt(x + 1, y) != def_val) {
      xend = x;
      break;
    }
    if ((def_val!=bgId_) || tclass_(lpic_, x, y, x + 1, y)) {
      setPixel(x + 1, y, value);
    }
    else {
      xend = x;
      break;
    }
  }
  for (x = xstart; x < xend; x++) {
    goSouth(x, y, value, def_val);
    goNorth(x, y, value, def_val);
  }
  rl_--;
}

template <class RegDescT, class RegionFinderClassT>
void RegionSplitterT<RegDescT,RegionFinderClassT>::goSouth(int x, int y, int value, int def_val)
{
  rl_++;
#ifdef DBG_MSG
  //  {int i; for (i=0; i<rl_; i++) fprintf(stderr," ");}
  fprintf(stderr, "goSouth(%05d,%05d,%05d,%05d)\n", x, y, value, rl_);
#endif
  if (y < lly_) {
    if (lpic_.getInt(x, y + 1) != def_val) {
      rl_--;
      return;
    }
    if ((def_val!=bgId_) || tclass_(lpic_, x, y, x, y + 1)) {
      setPixel(x, y + 1, value);
    }
    else {
      rl_--;
      return;
    }
    y++;
  }
  int ystart = y;
  int yend = lly_;
  for (y = ystart; y < lly_; y++) {
    if (lpic_.getInt(x, y + 1) != def_val) {
      yend = y;
      break;
    }
    if ((def_val!=bgId_) || tclass_(lpic_, x, y, x, y + 1)) {
      setPixel(x, y + 1, value);
    }
    else {
      yend = y;
      break;
    }
  }
  for (y = ystart; y < yend; y++) {
    goWest(x, y, value, def_val);
    goEast(x, y, value, def_val);
  }
  rl_--;
}

template <class RegDescT, class RegionFinderClassT>
int RegionSplitterT<RegDescT,RegionFinderClassT>::split()
{
  // Regionen segmentieren
  int x, y, value;
  if (regionList_.size()==0) {
    region_=RegDescT();
    region_.setId(0);
    regionList_.push_back(region_);
  }
#ifdef SMALL_REGIONS_TO_1
  if (regionList_.size()==1) {
    region_=RegDescT();
    region_.setId(1);
    regionList_.push_back(region_);
  }
#endif
  rl_ = 0;
  if (int(regionList_.size())<startId_) {
    int id=regionList_.size();
    regionList_.reserve(startId_);
    RegDescT reg;   
    while (int(regionList_.size())!=startId_) {
      reg.setId(id++);
      regionList_.push_back(reg);
    }
  }
  value=startId_-1;
  for (y = ury_; y <= lly_; y++) {
    //    printf("%05d %05d %05d\n",x,y,value);
    for (x = llx_; x <= urx_; x++) {
      if (lpic_.getInt(x, y) == bgId_ && tclass_.valid(lpic_,x,y)) {
        value++;
        region_=RegDescT();
        region_.class_=regionClass_;
        region_.file_=labelfilename_;
        region_.setId(value);
        setPixel(x, y, value);
        goEast(x, y, value, bgId_);
        goSouth(x, y, value, bgId_);
// #define DO_MIN_EQUAL_MAX
#ifdef DO_MIN_EQUAL_MAX
        if ((region_.size < minSize_) || (region_.size > maxSize_)) {
#else
        if (region_.size() < minSize_) {
#ifdef DBG_MSG
          printf("%05d %05d %05d size=%d\n", x, y, value, region_.size);
#endif
          int v;
#ifdef SMALL_REGIONS_TO_1
          v = smallRegionsId_;
#else
          if ((x > llx_) && (lpic_.getInt(x - 1, y) != value)) {
            v = lpic_.getInt(x - 1, y);
            //      printf("%05d %05d classified (%d)\n",x,y,v);
          }
          else {
            v = -1;
            //      printf("%05d %05d classified (%d)\n",x,y,v);
          }
#endif
          if (v == -1)
            region_ = regionList_[0];
          else
            region_ = regionList_[v];
          setPixel(x, y, v);
          goEast(x, y, v, value);
          goSouth(x, y, v, value);
          if (v == -1)
            regionList_[0] = region_;
          else {
#ifdef DBG_MSG
            printf("store %d %d(%d)\n", v, regionList_.size(),
                   regionList_.capacity());
#endif
            if (v < int(regionList_.size()))
              regionList_[v] = region_;
            else
              regionList_.push_back(region_);
          }
          value--;
        }
        else if (region_.size() > maxSize_) {
#endif // DO_MIN_EQUAL_MAX
#ifdef DBG_MSG
          printf("%05d %05d %05d size=%d\n", x, y, value, region_.size);
          printf("MaxSize %d\n", value);
#endif
          int v;
          v = -1;
          region_ = regionList_[0];
          setPixel(x, y, v);
          goEast(x, y, v, value);
          goSouth(x, y, v, value);
          regionList_[0] = region_;
          value--;
        }
        else {
#ifdef DBG_MSG
          printf("%05d %05d %05d %05d %05d %05d\n",
                 region_.id(),
                 region_.llx(),
                 region_.ury(), region_.urx(), region_.lly(), region_.size());
          printf("push_back start %d(%d)\n", regionList_.size(),
                 regionList_.capacity());
#endif
          regionList_.push_back(region_);
#ifdef DBG_MSG
          printf("push_back end   %d(%d)\n", regionList_.size(),
                 regionList_.capacity());
#endif

          //      (*regionList_)[value]=region_;
        }
      }
    }
  }
  for (y = ury_; y <= lly_; y++)
    for (x = llx_; x <= urx_; x++)
      if (lpic_.getInt(x, y) == -1)
        lpic_.set(x, y, 0);
#ifdef DBG_MSG
  printf("%05d %05d %05d %05d %05d %05d\n",
         regionList_[0].id(),
         regionList_[0].llx(),
         regionList_[0].ury(),
         regionList_[0].urx(),
         regionList_[0].lly(), regionList_[0].size());
#endif
#ifdef DBG_MSG
  printf("noRegions=%d\n", value);
#endif
  startId_=value+1;
  return startId_;
}

/*
template <class RegDescT,class RegionFinderClassT>
vector<RegDescT>* splitIntoRegionsT(Ga::Image& lpic,
                                    RegionFinderClassT tclass, 
                                    int minSize, int maxSize)
{
  vector<RegDescT>* regionList=new vector<RegDescT>();
  RegionSplitterT<RegDescT, RegionFinderClassT> splitter(*regionList,lpic,tclass,minSize,maxSize);
  splitter.split();
  return regionList;
}
*/

#if 0
template <class RegDescT, class LabelPic>
void findNeighbours(LabelPic & lpic,
                    RegDescT &region,
                    int num_regions)
{
//  printf("%4d:(%4d,%4d)=%4d\n",region.id,x,y,lpic.getInt(x,y));
#define setneighbour(x,y) {\
  assert(lpic.getInt(x,y)>=0); \
  assert(lpic.getInt(x,y)<num_regions); \
  neighbour[lpic.getInt(x,y)]=true; \
  }
//  printf("findNeighbours: num_regions=%d\n",num_regions);
  bool neighbour[num_regions+1];
  for (int i=0; i<num_regions; i++) {
    neighbour[i]=false;
  }
  for (int y = region.ury; y<=region.lly; y++)
    for (int x = region.llx; x<=region.urx; x++) {
#ifdef DBG_MSG
      printf("%05d %05d %05d %05d %05d %05d x=%05d, y=%05d\n",
             region.id,
             region.llx,
             region.ury,
             region.urx, region.lly, region.size, x, y);
#endif
      if (lpic.getInt(x, y) != region.id)
        continue;
      //      if ((ox>0) && (oy>0) && (lpic.getPixel(ox-1,oy-1)!=region.id))
      //        value=region.id;
      //      if ((ox<sizeX()-1) && (oy>0) && (lpic.getPixel(ox+1,oy-1)!=region.id))
      //        value=region.id;
      //      if ((ox>0) && (oy<sizeY()-1) && (lpic.getPixel(ox-1,oy+1)!=region.id))
      //        value=region.id;
      //      if ((ox<sizeX()-1) && (oy<sizeY()-1) && (lpic.getPixel(ox+1,oy+1)!=region.id))
      //        value=region.id;
      if ((x==0) || (y==0) || (x==lpic.sizeX()-1) || (y==lpic.sizeY()-1)) neighbour[num_regions]=true;
      if (x+1<lpic.sizeX())
        setneighbour(x+1,y  );
      if (x+1<lpic.sizeX() && (y+1<lpic.sizeY()))
        setneighbour(x+1,y+1);
      if (y+1<lpic.sizeY())
        setneighbour(x  ,y+1);
      if (x>0 && (y+1<lpic.sizeY()))
        setneighbour(x-1,y+1);
      if (x>0)
        setneighbour(x-1,y  );
      if ((x>0) && (y>0))
        setneighbour(x-1,y-1);
      if (y>0)
        setneighbour(x  ,y-1);
      if (x+1<lpic.sizeX() && (y>0))
        setneighbour(x+1,y-1);
    }
  int nn=0;
  neighbour[region.id]=false;
  for (int id=0; id<=num_regions; id++)
    if (neighbour[id]) nn++;
  region.numNeighbours_=nn;
  region.neighbour_=new int[nn];
  {
    int i=0;
    if (neighbour[num_regions]) {
      region.neighbour_[0]=-1;
      i++;
    }
    for (int id=0; id<num_regions; id++)
      if (neighbour[id]) {
        region.neighbour_[i]=id;
        i++;
      }
  }
#undef setneighbour
}
#endif

