#include "Array2DClose.h"
#include <typeinfo>
#include "pfm.h"
#include <vector>
#include <algorithm>
// #define DEBUG
template <class Value>
Array2DClose<Value>::Array2DClose(const Array2D<Value>& src, int label)
  : Array2D<Value>(src) {
	label_ = label;
  close();
}

template <class Value>
Array2DClose<Value>::Array2DClose(FILE *fp, int label) 
  : Array2D<Value>(fp) {
	label_ = label;
  close();
}

template <class Value>
Array2DClose<Value>::Array2DClose(const char* fname, int label)
  : Array2D<Value>(fname) {
	label_ = label;
  close();
}

template <class Value>
void Array2DClose<Value>::close() {
  int x,y;
  bool found=true;
  if (typeid(Value)==typeid(float)) {
    while (found) {
      found=false;
      for (y=ysize()-1; y>=0; y--)
        for (x=0; x<xsize(); x++)
	        if (isnan(getInt(x,y))) {
            interpolate(x,y);
  	        if (!isnan(getInt(x,y)))
  	          fillHole(x,y,0);
            else
              found=true;
          }
    }
  }
}

#define checkBounds(x,y,xmin,ymin,xmax,ymax) \
  (((x)>=(xmin)) && ((y)>=(ymin)) && ((x)<=(xmax)) && ((y)<=(ymax)))

template <class Value>
void Array2DClose<Value>::interpolate(int x, int y) {
  int num_points=0;
  double sum=0.0;

	int c, cmax = 0, c_count[8], c_val[8];
	memset(c_count, 0, sizeof(int) * 8);
	memset(c_val, 0, sizeof(int) * 8);

  int offset_x[8]={+1,+1, 0,-1,-1,-1, 0,+1};
  int offset_y[8]={ 0,-1,-1,-1, 0,+1,+1,+1};
  int dir;
  for (dir=0; dir<8; dir++) {
      int nx=x+offset_x[dir];
      int ny=y+offset_y[dir];
      
      if (checkBounds(nx,ny,0,0,xsize()-1,ysize()-1))
	      if (!isnan(getInt(nx,ny)))
				{
					if (label_)
					{
						int tmpc = (int)getInt(nx,ny);
						for (c = 0; c < cmax; c ++) 
            	if (c_val[c] == tmpc)
							{
								c_count[c] ++;
								break;
							}
						if (c == cmax)
						{
							c_val[cmax] = tmpc;
							c_count[cmax] = 1;
							cmax ++;
						}
					}
					else
						sum+=get(nx,ny);
	        num_points++;
	      }
    }
  if (num_points)
	{
		if (label_)
		{
			int tmpc = 0;
			for (c = 0; c < cmax; c ++)
				if (c_count[c] > tmpc)
				{
					tmpc = c_count[c];
					set(x, y, c_val[c]);
				}
		}
		else
		{
	    sum/=num_points;
  	  set(x,y,sum);
		}
  }
}

template <class Value>
bool Array2DClose<Value>::nextPoint(int& x, 
				    int& y, 
				    int& dir) {
#ifdef DEBUG
  printf("nextPoint: Closing hole at %d, %d, %d\n",x,y,dir);
#endif
  interpolate(x,y);
  dir=(dir+5)%8; 
  int offset_x[8]={+1,+1,0,-1,-1,-1,0,+1};
  int offset_y[8]={0,-1,-1,-1,0,+1,+1,+1};
  int olddir=dir;
  do {
    assert(x<xsize());
    assert(y<ysize());
    int nx=x+offset_x[dir];
    int ny=y+offset_y[dir];
    if (checkBounds(nx,ny,0,0,xsize()-1,ysize()-1))
      if (isnan(getInt(nx,ny))) {
	      x=nx;
	      y=ny;
	      return true;;
      }
    dir=(dir+1)%8;
  } while (dir!=olddir);
  return false;
}

template <class Value>
void Array2DClose<Value>::fillHole(int x_start, 
				   int y_start, 
				   int start_dir) {
#ifdef DEBUG
  printf("Closing hole at %d, %d, %d\n",x_start,y_start,start_dir);
#endif
  while (nextPoint(x_start,y_start,start_dir));
}


#undef checkBounds
