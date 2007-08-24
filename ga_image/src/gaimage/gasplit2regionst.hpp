#include "gasplit2regionst.h"

#define SMALL_REGIONS_TO_1 1

template < class RegDescT,
  class DataPicT,
  class LabelPicT, class TestClass > class SIRprivate {
public:
  SIRprivate(const DataPicT & hpic,
             LabelPicT & lpic, TestClass & tclass);
  void goWest(int x, int y, int value, int def_val);
  void goNorth(int x, int y, int value, int def_val);
  void goEast(int x, int y, int value, int def_val);
  void goSouth(int x, int y, int value, int def_val);
  vector < RegDescT>*classify(int minSize, int maxSize);
  int setPixel(int x, int y, int val);
protected:
  const DataPicT *hpic_;
  LabelPicT *lpic_;
  TestClass *tclass_;
  int rl;
  RegDescT region_;
  vector < RegDescT> *regionList_;
};


#include <math.h>
#include <limits.h>
#include <cstdio>

#define sqr(x) ((x)*(x))


template <class RegDescT,
          class DataPicT,
          class LabelPicT,
          class TestClass >
  SIRprivate<RegDescT, DataPicT, LabelPicT, TestClass >
    ::SIRprivate(const DataPicT& hpic, LabelPicT & lpic, TestClass & tclass)
{
  hpic_ = &hpic;
  lpic_ = &lpic;
  tclass_ = &tclass;
}


template < class RegDescT,
           class DataPicT,
           class LabelPicT,
           class TestClass >
  inline int SIRprivate < RegDescT, DataPicT, LabelPicT,TestClass >
     ::setPixel(int x, int y, int val)
{
  return region_.setPixel(*hpic_,*lpic_,x,y,val);
#if 0
  lpic_->set(x, y, val);
  region_.size++;
  float v=hpic_->getFloat(x,y);
  if (!isnanf(v)) {
    region_.sum_+=v;
    region_.numValidValues_++;
  }
  if (x < region_.llx)
    region_.llx = x;
  if (y < region_.ury)
    region_.ury = y;
  if (x > region_.urx)
    region_.urx = x;
  if (y > region_.lly)
    region_.lly = y;
  return val;
#endif
}

// #define DBG_MSG
template < class RegDescT,
  class DataPicT,
  class LabelPicT,
  class TestClass >
  void SIRprivate < RegDescT, DataPicT, LabelPicT,
  TestClass >::goWest(int x, int y, int value, int def_val)
{
  rl++;
#ifdef DBG_MSG
  //  {int i; for (i=0; i<rl; i++) fprintf(stderr," ");}
  fprintf(stderr, "goWest (%05d,%05d,%05d,%05d)\n", x, y, value, rl);
#endif
  if (x > 0) {
    if (lpic_->getInt(x - 1, y) != def_val) {
      rl--;
      return;
    }
    if ((*tclass_) (*hpic_, *lpic_, x, y, x - 1, y)) {
      //(fabs(hpic_->getFloat(x,y)-hpic_->getFloat(x-1,y))<level)
      setPixel(x - 1, y, value);
    }
    else {
      rl--;
      return;
    }
    x--;
  }
#if 1
#ifdef DBG_MSG
  fprintf(stderr, "goWest Start\n");
#endif
  int xstart = x;
  int xend = 0;
  for (x = xstart; x > 0; x--) {
    if (lpic_->getInt(x - 1, y) != def_val) {
      xend = x;
      break;
    }
    if ((*tclass_) (*hpic_, *lpic_, x, y, x - 1, y)) {
      // (fabs(hpic_->getFloat(x,y)-hpic_->getFloat(x-1,y))<level) {
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
  rl--;
#ifdef DBG_MSG
  fprintf(stderr, "goWest done\n");
#endif
#else
  while (x > 0) {
    if (lpic_->getInt(x - 1, y) != def_val) {
      goNorth(x, y, value, def_val);
      goSouth(x, y, value, def_val);
      rl--;
      return;
    }
    if ((*tclass_) (*hpic_, *lpic_, x, y, x - 1, y)) {
      //    if (fabs(hpic_->getFloat(x,y)-hpic_->getFloat(x-1,y))<level) {
      setPixel(x - 1, y, value);
      goNorth(x, y, value, def_val);
      goSouth(x, y, value, def_val);
    }
    else {
      rl--;
      return;
    }
    x--;
  }
  rl--;
#endif
}

template < class RegDescT,
  class DataPicT,
  class LabelPicT,
  class TestClass >
  void SIRprivate < RegDescT, DataPicT, LabelPicT,
  TestClass >::goNorth(int x, int y, int value, int def_val)
{
  rl++;
#ifdef DBG_MSG
  //  {int i; for (i=0; i<rl; i++) fprintf(stderr," ");}
  fprintf(stderr, "goNorth(%05d,%05d,%05d,%05d)\n", x, y, value, rl);
#endif
  if (y > 0) {
    if (lpic_->getInt(x, y - 1) != def_val) {
      rl--;
      return;
    }
    if ((*tclass_) (*hpic_, *lpic_, x, y, x, y - 1)) {
      //    if (fabs(hpic_->getFloat(x,y)-hpic_->getFloat(x,y-1))<level) {
      setPixel(x, y - 1, value);
    }
    else {
      rl--;
      return;
    }
    y--;
  }
#if 1
  int ystart = y;
  int yend = 0;
  for (y = ystart; y > 0; y--) {
    if (lpic_->getInt(x, y - 1) != def_val) {
      yend = y;
      break;
    }
    if ((*tclass_) (*hpic_, *lpic_, x, y, x, y - 1)) {
      //    if (fabs(hpic_->getFloat(x,y)-hpic_->getFloat(x,y-1))<level) {
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
  rl--;
#else
  while (x > 0) {
    if (lpic_->getInt(x, y - 1) != def_val) {
      goEast(x, y, value, def_val);
      goSouth(x, y, value, def_val);
      goWest(x, y, value, def_val);
      rl--;
      return;
    }
    if ((*tclass_) (*hpic_, *lpic_, x, y, x, y - 1)) {
      //    if (fabs(hpic_->getFloat(x,y)-hpic_->getFloat(x,y-1))<level) {
      setPixel(x, y - 1, value);
      goEast(x, y, value, def_val);
      //      goSouth(x,y,level,value,def_val);
      goWest(x, y, value, def_val);
    }
    else {
      rl--;
      return;
    }
    y--;
  }
  rl--;
#endif
}

template < class RegDescT,
  class DataPicT,
  class LabelPicT,
  class TestClass >
  void SIRprivate < RegDescT, DataPicT, LabelPicT,
  TestClass >::goEast(int x, int y, int value, int def_val)
{
  rl++;
#ifdef DBG_MSG
  //  {int i; for (i=0; i<rl; i++) fprintf(stderr," ");}
  fprintf(stderr, "goEast (%05d,%05d,%05d,%05d)\n", x, y, value, rl);
#endif
  if (x < lpic_->sizeX() - 1) {
    if (lpic_->getInt(x + 1, y) != def_val) {
      rl--;
      return;
    }
    if ((*tclass_) (*hpic_, *lpic_, x, y, x + 1, y)) {
      //    if (fabs(hpic_->getFloat(x,y)-hpic_->getFloat(x+1,y))<level) {
      setPixel(x + 1, y, value);
    }
    else {
      rl--;
      return;
    }
    x++;
  }
#if 1
  int xstart = x;
  int xend = lpic_->sizeX() - 1;
  for (x = xstart; x < lpic_->sizeX() - 1; x++) {
    if (lpic_->getInt(x + 1, y) != def_val) {
      xend = x;
      break;
    }
    if ((*tclass_) (*hpic_, *lpic_, x, y, x + 1, y)) {
      //    if (fabs(hpic_->getFloat(x,y)-hpic_->getFloat(x+1,y))<level) {
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
  rl--;
#else
  while (x < lpic_->sizeX() - 1) {
    if (lpic_->getInt(x + 1, y) != def_val) {
      goSouth(x, y, value, def_val);
      goWest(x, y, value, def_val);
      goNorth(x, y, value, def_val);
      rl--;
      return;
    }
    if ((*tclass_) (*hpic_, *lpic_, x, y, x + 1, y)) {
      //    if (fabs(hpic_->getFloat(x,y)-hpic_->getFloat(x+1,y))<level) {
      setPixel(x + 1, y, value);
      goSouth(x, y, value, def_val);
      goWest(x, y, value, def_val);
      goNorth(x, y, value, def_val);
    }
    else {
      rl--;
      return;
    }
    x++;
  }
  rl--;
#endif
}

template < class RegDescT,
  class DataPicT,
  class LabelPicT,
  class TestClass >
  void SIRprivate < RegDescT, DataPicT, LabelPicT,
  TestClass >::goSouth(int x, int y, int value, int def_val)
{
  rl++;
#ifdef DBG_MSG
  //  {int i; for (i=0; i<rl; i++) fprintf(stderr," ");}
  fprintf(stderr, "goSouth(%05d,%05d,%05d,%05d)\n", x, y, value, rl);
#endif
  if (y < lpic_->sizeY() - 1) {
    if (lpic_->getInt(x, y + 1) != def_val) {
      rl--;
      return;
    }
    if ((*tclass_) (*hpic_, *lpic_, x, y, x, y + 1)) {
      //    if (fabs(hpic_->getFloat(x,y)-hpic_->getFloat(x,y+1))<level) {
      setPixel(x, y + 1, value);
    }
    else {
      rl--;
      return;
    }
    y++;
  }
#if 1
  int ystart = y;
  int yend = lpic_->sizeY() - 1;
  for (y = ystart; y < lpic_->sizeY() - 1; y++) {
    if (lpic_->getInt(x, y + 1) != def_val) {
      yend = y;
      break;
    }
    if ((*tclass_) (*hpic_, *lpic_, x, y, x, y + 1)) {
      //    if (fabs(hpic_->getFloat(x,y)-hpic_->getFloat(x,y+1))<level) {
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
  rl--;
#else
  while (y < lpic_->sizeY() - 1) {
    if (lpic_->getInt(x, y + 1) != def_val) {
      goWest(x, y, value, def_val);
      goNorth(x, y, value, def_val);
      goEast(x, y - 1, value);
      rl--;
      return;
    }
    if ((*tclass_) (*hpic_, *lpic_, x, y, x, y + 1)) {
      //    if (fabs(hpic_->getFloat(x,y)-hpic_->getFloat(x,y+1))<level) {
      setPixel(x, y, value);
      goWest(x, y + 1, value);
      goNorth(x, y, value, def_val);
      goEast(x, y, value, def_val);
    }
    else {
      rl--;
      return;
    }
    y++;
  }
  rl--;
#endif
}

template < class RegDescT,
  class DataPicT,
  class LabelPicT,
  class TestClass >
  vector < RegDescT>*SIRprivate < RegDescT, DataPicT,
  LabelPicT, TestClass >::classify(int minSize, int maxSize)
{
  // Regionen segmentieren
  int x, y, value;
  regionList_ = new vector < RegDescT>;
  region_=RegDescT();
  region_.setId(0);
  regionList_->push_back(region_);
#ifdef SMALL_REGIONS_TO_1
  region_=RegDescT();
  region_.setId(1);
#endif
  regionList_->push_back(region_);
  if (maxSize == 0)
    maxSize = INT_MAX;
  rl = 0;
#ifdef SMALL_REGIONS_TO_1
  value=1;
#else
  value=0;
#endif
  for (y = 0; y < lpic_->sizeY(); y++) {
    //    printf("%05d %05d %05d\n",x,y,value);
    for (x = 0; x < lpic_->sizeX(); x++) {
      if (lpic_->getInt(x, y) == 0 && tclass_->valid(*hpic_,*lpic_,x,y)) {
        value++;
        region_=RegDescT();
        region_.setId(value);
        setPixel(x, y, value);
        goEast(x, y, value, 0);
        goSouth(x, y, value, 0);
// #define DO_MIN_EQUAL_MAX
#ifdef DO_MIN_EQUAL_MAX
        if ((region_.size < minSize) || (region_.size > maxSize)) {
#else
        if (region_.size() < minSize) {
#ifdef DBG_MSG
          printf("%05d %05d %05d size=%d\n", x, y, value, region_.size);
#endif
          int v;
#ifdef SMALL_REGIONS_TO_1
          v = 1;
#else
          if ((x > 0) && (lpic_->getInt(x - 1, y) != value)) {
            v = lpic_->getInt(x - 1, y);
            //      printf("%05d %05d classified (%d)\n",x,y,v);
          }
          else {
            v = -1;
            //      printf("%05d %05d classified (%d)\n",x,y,v);
          }
#endif
          if (v == -1)
            region_ = (*regionList_)[0];
          else
            region_ = (*regionList_)[v];
          setPixel(x, y, v);
          goEast(x, y, v, value);
          goSouth(x, y, v, value);
          if (v == -1)
            (*regionList_)[0] = region_;
          else {
#ifdef DBG_MSG
            printf("store %d %d(%d)\n", v, regionList_->size(),
                   regionList_->capacity());
#endif
            if (v < int(regionList_->size()))
              (*regionList_)[v] = region_;
            else
              regionList_->push_back(region_);
          }
          value--;
        }
        else if (region_.size() > maxSize) {
#endif // DO_MIN_EQUAL_MAX
#ifdef DBG_MSG
          printf("%05d %05d %05d size=%d\n", x, y, value, region_.size);
          printf("MaxSize %d\n", value);
#endif
          int v;
          v = -1;
          region_ = (*regionList_)[0];
          setPixel(x, y, v);
          goEast(x, y, v, value);
          goSouth(x, y, v, value);
          (*regionList_)[0] = region_;
          value--;
        }
        else {
#ifdef DBG_MSG
          printf("%05d %05d %05d %05d %05d %05d\n",
                 region_.id,
                 region_.llx,
                 region_.ury, region_.urx, region_.lly, region_.size);
          printf("push_back start %d(%d)\n", regionList_->size(),
                 regionList_->capacity());
#endif
          regionList_->push_back(region_);
#ifdef DBG_MSG
          printf("push_back end   %d(%d)\n", regionList_->size(),
                 regionList_->capacity());
#endif

          //      (*regionList_)[value]=region_;
        }
      }
    }
  }
  for (y = 0; y < lpic_->sizeY(); y++)
    for (x = 0; x < lpic_->sizeX(); x++)
      if (lpic_->getInt(x, y) == -1)
        lpic_->set(x, y, 0);
#ifdef DBG_MSG
  printf("%05d %05d %05d %05d %05d %05d\n",
         (*regionList_)[0].id,
         (*regionList_)[0].llx,
         (*regionList_)[0].ury,
         (*regionList_)[0].urx,
         (*regionList_)[0].lly, (*regionList_)[0].size);
#endif
#ifdef DBG_MSG
  printf("noRegions=%d\n", value);
#endif
  return regionList_;
}

/*
template < class RegDescT,
  class DataPicT,
  class LabelPicT,
  class TestClass >
int splitIntoRegions(const DataPicT &hpic,
                     char* labelfilename,
                     char* regionfilename,
                     TestClass tclass,
                     int minSize, int maxSize){

    SIRprivate < RegDescT, DataPicT, LabelPicT, TestClass > pic(hpic,
                                                                lpic,
                                                                tclass);
    vector <RegDescT>* regiond_ = pic.classify(minSize, maxSize);
    
    
}
*/

template < class RegDescT,
  class DataPicT,
  class LabelPicT,
  class TestClass >
  vector <RegDescT>* splitIntoRegionsT(const DataPicT & hpic, LabelPicT & lpic,
                     TestClass tclass, int minSize, int maxSize)
{
  SIRprivate < RegDescT, DataPicT, LabelPicT, TestClass > pic(hpic,
                                                                   lpic,
                                                                   tclass);
  return pic.classify(minSize, maxSize);
}

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


//splitIntoRegions(hpic,lpic,DiffHeigtValues(level,minsize,maxsize));
