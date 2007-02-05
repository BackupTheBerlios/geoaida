//#define DEBUG_MSG
//#define XFIG_DEBUG
//#define XFIG_DEBUG_MSG
#ifdef linux
extern "C"
{
  int __isnanf(float);
}
#ifndef NAN
#define NAN sqrt(-1)
#endif
#endif

#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <vector.h>
#include <list.h>

#ifdef XFIG_DEBUG
#include "xfig.h"
XFig xfig("dump.fig");
#endif

using namespace Ga;
using namespace std;

//#warning Bug in memory-management
//#define delete

template <class Container>
class StateContainer : public Container {
 public:
  StateContainer() : Container() {};
  template <class InputIterator>
    StateContainer(InputIterator f, InputIterator l) : Container(f,l) {};
  typename Container::value_type current()
    {
      assert(validCurrent_);
      if (current_==end()) return 0;
      return *current_;
    }
  typename Container::value_type next()
    {
      assert(validCurrent_);
      if (current_==end()) return 0;
      ++current_;
      if (current_==end()) return 0;
      return *current_;
    }
  typename Container::value_type prev()
    {
      assert(validCurrent_);
      if (current_==begin()) return 0;
      --current_;
      return *current_;
    }
  typename Container::value_type first()
    {
      validCurrent_=false;
      if (empty()) return 0;
      current_=begin();
      validCurrent_=true;
      return *current_;
    }
  typename Container::value_type getFirst()
    {
      return *begin();
    }
  typename Container::value_type last()
    {
      validCurrent_=false;
      if (empty()) return 0;
      current_=end();
      --current_;
      validCurrent_=true;
      return *current_;
    }
  typename Container::value_type getLast()
    {
      typename Container::iterator it=end();
      --it;
      return *it;
    }
  typename Container::iterator at()
    {
      assert(validCurrent_);
      return current_;
    }
  typename Container::value_type at(typename Container::iterator it)
    {
      validCurrent_=true;
      current_=it;
      return *current_;
    }
  typename Container::value_type findRef(typename Container::value_type value)
    {
      validCurrent_=true;
      current_=find(begin(),end(),value);
      if (current_==end()) return 0;
      validCurrent_=true;
      return *current_;
    }
  void remove()
    {
      validCurrent_=false;
      erase(current_);
      current_=0;
    }
  void removeRef(typename Container::value_type value)
    {
      findRef(value);
      if (current_==end()) return;
      validCurrent_=false;
      erase(current_);
      current_=0;
    }
 protected:
  typename Container::iterator current_;
  bool validCurrent_;

};

#define sqr(x) ((x)*(x))
#define checkBounds(x,y,xmin,ymin,xmax,ymax) \
  (((x)>=(xmin)) && ((y)>=(ymin)) && ((x)<=(xmax)) && ((y)<=(ymax)))


enum
{ DIR8_E, DIR8_NE, DIR8_N, DIR8_NW, DIR8_W, DIR8_SW, DIR8_S, DIR8_SE };
/********************* Generate Label picture **********/
template <class RegDescT>
static Image genLabelPic(const Ga::Image& lpic, const RegDescT &reg, int id)
{
  Image pic(typeid(signed char),reg.urx()-reg.llx()+1,reg.lly()-reg.ury()+1);
#ifdef DEBUG_MSG
  printf("genLabelPic src %d/%d (%d,%d)(%d,%d) -> (%d,%d)\n",
         reg.id(),id,reg.llx(),reg.lly(),reg.urx(),reg.lly(),pic.sizeX(),pic.sizeY());
#endif
  // Binärbild generieren
  for (int y=0; y<pic.sizeY(); y++)
    for (int x=0; x<pic.sizeX(); x++) {
#if 0
#ifdef DEBUG_MSG
      printf("%05d %05d %05d %05d %05d %05d x=%05d, y=%05d\n",
             reg.id(),
             reg.llx(),
             reg.lly(),
             reg.urx(), reg.ury(), reg.size(), x, y);
#endif
#endif
      int ox = x + reg.llx();
      int oy = y + reg.ury();
      if (lpic.getInt(ox, oy)!=id)
        pic.set(x, y, 0);
      else {
        int value = 0;
        //      if ((ox>0) && (oy>0) && (lpic.getInt(ox-1,oy-1)!=region))
        //        value=region;
        //      if ((ox<sizeX()-1) && (oy>0) && (lpic.getInt(ox+1,oy-1)!=region))
        //        value=region;
        //      if ((ox>0) && (oy<sizeY()-1) && (lpic.getInt(ox-1,oy+1)!=region))
        //        value=region;
        //      if ((ox<sizeX()-1) && (oy<sizeY()-1) && (lpic.getInt(ox+1,oy+1)!=region))
        //        value=region;
        if ((ox == 0) || (lpic.getInt(ox - 1, oy) != id))
          value = 1;
        if ((ox == lpic.sizeX() - 1) || (lpic.getInt(ox + 1, oy) != id))
          value = 1;
        if ((oy == 0) || (lpic.getInt(ox, oy - 1) != id))
          value = 1;
        if ((oy == lpic.sizeY() - 1) || (lpic.getInt(ox, oy + 1) != id))
          value = 1;
        pic.set(x, y, value);
      }
    }
  return pic;
}

#if 0
/******************** Generate full point list ************/
static void setPointIfNull(Ga::Image &pic, int x, int y, int val)
{
  if (checkBounds(x, y, 0, 0, pic.sizeX()-1, pic.sizeY()-1))
    if (!pic.getInt(x, y))
      pic.set(x, y, val);
}
#endif

static void nextPoint(Ga::Image &pic, int &x, int &y, int &dir)
{
  dir = (dir + 5) % 8;
  int offset_x[8] = { +1, +1, 0, -1, -1, -1, 0, +1 };
  int offset_y[8] = { 0, -1, -1, -1, 0, +1, +1, +1 };
  int olddir = dir;
  do {
    assert(x < pic.sizeX());
    assert(y < pic.sizeY());
    int nx = x + offset_x[dir];
    int ny = y + offset_y[dir];
    if (checkBounds(nx, ny, 0, 0, pic.sizeX() - 1, pic.sizeY() - 1)) {
      int pixVal = pic.getInt(nx, ny);
      if (pixVal != 0) {
        x = nx;
        y = ny;
        if (pixVal > 0)
          pic.set(x, y, -pixVal);
        return;
      }
    }
    dir = (dir + 1) % 8;
  } while (dir != olddir);
}

template < class PointT >
  list < vector < const PointT *>*>*fullPointList(Ga::Image &pic)
{
  list < vector < const PointT *>*>*polygonList =
    new list < vector < const PointT * >*>;
  int x, y;
  int dir = DIR8_E;
  bool inner_loop=false;

  for (y = pic.sizeY() - 1; y >= 0; y--) {
    for (x = 0; x < pic.sizeX(); x++) {
      if (pic.getInt(x, y) > 0) {
#ifdef DEBUG_MSG
        printf("Start point (%d,%d) pic(%d,%d)\n",
               x,y,pic.sizeX(),pic.sizeY());
#endif
        vector < const PointT *>*pl = new vector < const PointT * >;
        int xp = x;
        int yp = y;
        assert(xp < pic.sizeX());
        PointT *p = new PointT(xp, yp);
        pl->push_back(p);
        nextPoint(pic, xp, yp, dir);
        while ((xp != p->x()) || (yp != p->y())) {
          if (yp > p->y() && !inner_loop) {
            fprintf(stderr, "Warning: (%d, %d) not in border (%d, %d)\n",
                    xp, yp, p->x(), p->y());
          }
#ifdef DEBUG_MSG
          printf("new point (%3d,%3d)\n",xp,yp);
#endif
          PointT *np=new PointT(xp,yp);
          pl->push_back(np);
          nextPoint(pic, xp, yp, dir);
        }
        pl->push_back(p);
        polygonList->push_back(pl);
        dir = DIR8_S;
        inner_loop=true;
      }
    }
  }
  return polygonList;
}

/******************** Init Polygon ************/

template < class PointT >
  static list < const PointT *>*initPolygon(const vector <const PointT *>&pFullList,
                                            bool inner_loop)
{
  const PointT *pxmin = pFullList[0];
  const PointT *pymin = pFullList[0];
  const PointT *pxmax = pFullList[0];
  const PointT *pymax = pFullList[0];
  for (unsigned int i = 1; i < pFullList.size(); i++) {
    assert(pFullList[i]->y() <= pymax->y());    // pymax=pFullList[i];
    if (pFullList[i]->x() > pxmax->x()) {
      pxmax = pFullList[i];
      pymin = pxmax;
      pxmin = pxmax;
    }
    if (pFullList[i]->y() < pymin->y()) {
      pymin = pFullList[i];
      pxmin = pymin;
    }
    if (pFullList[i]->x() < pxmin->x()) {
      pxmin = pFullList[i];
    }
  }

  list < const PointT *>*pList = new list < const PointT * >;
  pList->push_back(pymax);
  if (pxmax != pymax)
    pList->push_back(pxmax);
  if (pymin != pxmax)
    pList->push_back(pymin);
  if (pxmin != pymin)
    pList->push_back(pxmin);
  if (pymax != pxmin)
    pList->push_back(pymax);

  return pList;
}

template < class PointT >
  static list < list < const PointT *>*>*initPolygonList(list < vector <
                                                        const PointT *
                                                        >*>&pFullPolygonList)
{
  list < list < const PointT *>*>*pInitList =
    new list < list < const PointT * >*>;
  typename list < vector < const PointT *>*>::const_iterator l;
  bool inner_loop=false;
  for (l = pFullPolygonList.begin(); l != pFullPolygonList.end(); ++l) {
    pInitList->push_back(initPolygon(**l,inner_loop));
    inner_loop=true;
#warning break muss weg, dafür inner_loop richtig behandel werden!
    break;
  }
  return pInitList;
}

/************************************************************/
template < class PointT >
  static double diffVec(const PointT & p1, const PointT & p2, const PointT pc,
                        double &dx, double &dy, double &len)
{
  PointT p = p2 - p1;
  double pabs = sqrt(double(p.x() * p.x() + p.y() * p.y()));
  PointT d = pc - p1;
  double evx = p.x() / pabs;
  double evy = p.y() / pabs;
  dx = d.x() - (d.x() * evx + d.y() * evy) * evx;
  dy = d.y() - (d.x() * evx + d.y() * evy) * evy;
  len = dx * dx + dy * dy;
  return len;
}

template < class PointT >
  static double distanceP(const PointT & p1, const PointT & p2, const PointT pc)
{
  double dx;
  double dy;
  double len;
  diffVec(p1, p2, pc, dx, dy, len);
  return len;
}

#if 0
inline double sqr(double x)
{
  return x * x;
}
#endif

template < class PointT, class InputIterator >
  const PointT *maxDistance(const PointT * p1, const PointT * p2,
                           InputIterator p_full, double tolerance)
{
  double maxDist;
  const PointT *p_new = 0;
  for (maxDist = -1.0; (*p_full != p2); ++p_full) {
    double dist = distanceP(*p1, *p2, **p_full);
    if (dist < maxDist)
      continue;
    if (dist <= sqr(tolerance))
      continue;
    p_new = *p_full;
    maxDist = dist;
  }
  return p_new;
}

template < class InputIterator, class PointT >
  static void findPoint(InputIterator & pList,
                        InputIterator first,
                        InputIterator last, const PointT * p)
{
  while (pList != last) {
    if (*pList == p)
      return;
    ++pList;
  }
  pList = first;
  while (pList != last) {
    if (*pList == p)
      return;
    ++pList;
  }
  assert(pList != last);
  assert(pList == last);
}

template < class PointT >
  static typename list < const PointT *>::iterator
adaptLine(const vector < const PointT * >&pFullList,
          list < const PointT * >&pList,
          typename list < const PointT * >::iterator index, double tolerance)
{
  typename list < const PointT *>::iterator p_begin = index;
  ++index;
  typename list < const PointT *>::iterator p_end = index;
  index = p_begin;
  if (p_end == pList.end())
    return p_end;
  //  list<const PointT*>::iterator endindex=p_end;;
  if (**p_begin == **p_end) {
    if (*p_begin == *p_end)
      return pList.end();
    pList.remove(*p_end);
    return index;
  }

  const PointT *p_new = 0;

  typename vector < const PointT *>::const_iterator p_begin_full = pFullList.begin();
  findPoint(p_begin_full, pFullList.begin(), pFullList.end(), *p_begin);
  do {
    p_new = maxDistance(*p_begin, *p_end, p_begin_full, tolerance);
    if (p_new) {
      pList.insert(p_end, p_new);
      --p_end;
    }
  } while (p_new);
  return p_end;

}


#ifdef XFIG_DEBUG
template <class RegDescT, class InputIterator>
  void dumpPolygon(XFig & xfig,
                   const RegDescT& reg,
                   InputIterator first, InputIterator last)
{
  InputIterator p;

  int count = 0;
  for (p = first; p != last; ++p)
    count++;

#ifdef XFIG_DEBUG_MSG
  fprintf(stderr, "dumpPolygon: %d count=%d\n", reg.id(), count);
#endif
  if (count<=2) return;
  double *x = new double[count];
  double *y = new double[count];

  xfig.comment(QString().sprintf("dumpPolygon: %d count=%d (%d,%d)",
                                 reg.id(), count,
                                 reg.llx(), reg.ury()));
  int i;
  for (i = 0, p = first; p != last; ++p, i++) {
    x[i] = double (reg.llx() + (*p)->x());
    y[i] = double (reg.ury() + (*p)->y());
  }
  xfig.drawPolyLine(x, y, count - 1, true);
  delete x;
  delete y;
}

template < class RegDescT, class ListContainer >
  void dumpPolygonList(XFig & xfig,
                       const RegDescT &reg,
                       ListContainer * pList)
{
  typename ListContainer::const_iterator l;
  for (l = pList->begin(); l != pList->end(); ++l) {
    dumpPolygon(xfig, reg, (*l)->begin(), (*l)->end());
    xfig.flush();
  }
}
#endif

template < class RegDescT, class PointT  >
void vectorize(const Ga::Image& lpic,
               RegDescT &reg, double tolerance)
{
  fprintf(stderr, "findPolygon: %d\n", reg.id());
  Ga::Image pic=genLabelPic(lpic, reg, reg.id());
  pic.write("labelpic.plm");
  list < vector < const PointT *>*>*pFullList = fullPointList < PointT > (pic);
  fprintf(stderr, "findPolygon: fullPointList done %d\n", reg.id());
#ifdef XFIG_DEBUG
  xfig.setAttrib("depth", "51");
  xfig.setAttrib("pen_color", "0");
  dumpPolygonList(xfig, reg, pFullList);
#endif
  list < list < const PointT *>*>*pList = initPolygonList(*pFullList);
#ifdef XFIG_DEBUG
  xfig.setAttrib("depth", "50");
  xfig.setAttrib("pen_color", "3");
  dumpPolygonList(xfig, reg, pList);
  xfig.flush();
#endif
  {
    typename list < list < const PointT *>*>::iterator itInitList;
    typename list < vector < const PointT *>*>::iterator itFullList;
    for (itInitList = pList->begin(), itFullList = pFullList->begin();
         (itInitList != pList->end()) && (itFullList != pFullList->end());
         ++itInitList, ++itFullList) {
      typename list < const PointT *>::iterator index = (*itInitList)->begin();
      while ((index = adaptLine(**itFullList,
                                **itInitList, index,
                                tolerance)) != (*itInitList)->end());
    }
  }
#ifdef XFIG_DEBUG
  xfig.setAttrib("depth", "49");
  xfig.setAttrib("pen_color", "4");
  dumpPolygonList(xfig, reg, pList);
  xfig.flush();
#endif
  // Clean up
  StateContainer < vector < const PointT *> >*pResult =
    new StateContainer < vector < const PointT * > >;
  pResult->reserve(pList->size());
  {
    typename list < list < const PointT *>*>::const_iterator itList;
    for (itList = pList->begin(); itList != pList->end(); ++itList) {
      typename list < const PointT *>::const_iterator p;
      for (p = (*itList)->begin(); p != (*itList)->end(); ++p) {
        pResult->push_back(new PointT(**p));
      }
    }
  }
  {
    typename list < vector < const PointT *>*>::const_iterator list;
    for (list = pFullList->begin(); list != pFullList->end(); ++list) {
      typename vector < const PointT *>::const_iterator p;
      (*list)->pop_back();
      for (p = (*list)->begin(); p != (*list)->end(); ++p) {
        delete(*p);
      }
      delete(*list);
    }
  }
  delete pFullList;
  delete pList;
  reg.border(pResult);
}


#if 0
template < class PointT, class LabelPicture, class InputIterator >
  void findPolygon(const LabelPicture & lpic,
                   InputIterator regBegin,
                   InputIterator regEnd, double tolerance)
{
  for (; regBegin != regEnd; ++regBegin)
    vector < const PointT *>*pList = findOnePolygon < PointT > (lpic,
                                                              *regBegin,
                                                              tolerance);

}
#endif

#if 0
template < class LabelPic, class PointT >
findRegionBorder(const LabelPic & lpic,
                 const vector < RegionDescription > &regionList)
{
  for (i = 1; i < noRegions; i++)
    findPolygon(i, tolerance);
}

#endif

#undef sqr
#undef checkBounds

#undef DEBUG_MSG

