/***************************************************************************
                          ga_region.h  -  region oriented functions for
                                          geoAIDA template image class
                             -------------------
    begin                : Wed Jan 24 2001
    copyright            : (C) 2001 by Oliver Stahlhut
    email                : stahlhut@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _GAREGIONT_H
#define _GAREGIONT_H

#include "gaimaget.h"

namespace Ga {

/** relabel the image: all pixel with label 'label_val' become the value '0'
    and the rest (background) become the value '1'.
    preparation for function 'labelImage' */
/** this is the cool and fasted version than possible ;) */
template<class PixTyp> void labelBin(ImageT<PixTyp> &img, PixTyp label_val)
{ //J.B. 01/01
  PixTyp *pix = img.begin();
  for (int j = 0; j < img.sizeImage(); ++j) {
    if (*pix == label_val) *pix = (PixTyp)0;
    else *pix = (PixTyp)1;
    pix++;
  }
}

/** fill the current image with unique labels. The background must be set to 0,
    the region borders must be set to 1. The regions will be labeled with values
    starting from 2. This version is 6 times faster by usage of pointers */
template<class PixTyp> PixTyp labelImage(ImageT<PixTyp> &img)
{
  // written by O. Stahlhut 01/01

  PixTyp *pix = img.begin();
  PixTyp label_set = 2;

  for ( int i = 0; i < (int)(img.sizeImage()); ++i, ++pix )
  {
    if (!(*pix))
    {
      label4((PixTyp*)img.begin(),img.sizeX(),img.sizeY(), i, (PixTyp)0, label_set);
      label_set += 1;
    }
  }

  return (label_set);
}

/** relabel a single label of an image. Consider only the 4 point neighbourhood of
    each pixel, i.e. diagonal positioned pixels are not traced and included.
    begin: pointer to image data, size_x / size_y: image width and height,
    index: offset to pixel of interest (seed), search for pixels of colour "label_val",
    set them to colour "label_set". returns the number of pixels in the new label.
*/

template<class PixTyp> unsigned int label4(PixTyp* begin, int size_x, int size_y, int index, PixTyp label_val, PixTyp label_set)
{
	unsigned int labelsize = 0;
	
  PixTyp* pix = begin+index;
  int x = index % size_x;
  int y = index / size_x;

  // store the point left to the start position for the second sweep
  PixTyp *left = pix; --left;
  int xl = x; --xl;

  // 1. sweep: fill one horizontal line to the right
  while ((*pix == label_val) && (x < size_x))
  {
    *pix = label_set;
    ++pix;
    ++x;
    ++labelsize;
  }

  // 2. sweep: fill one horizontal line to the left
  pix = left;
  x = xl;
  while ((x >= 0) && (*pix == label_val))
  {
    *pix = label_set;
    --pix;
    --x;
    ++labelsize;
  }

  // step one element to the right to get back into the labelarea
  ++x;
  ++pix;

  int check_next = (y + 1) < size_y;
  int check_prev = 0 <= (y - 1);
  PixTyp *next = pix + size_x;
  PixTyp *prev = pix - size_x;

  // 3. sweep: move over the horizontal line and check if there is more do to above or below (recursion)
  while (((*pix > 1)||(*pix==label_set)) && (x < size_x)) //XXX ??? warum >1 (jb)
  {
    if (check_next && (*next == label_val))
      labelsize += label4(begin, size_x, size_y, (y + 1) * size_x + x, label_val, label_set);

    if (check_prev && (*prev == label_val))
      labelsize += label4(begin, size_x, size_y, (y - 1) * size_x + x, label_val, label_set);

    ++x;
    ++pix;
    ++next;
    ++prev;
  }

  return labelsize;
}

/** used by labelImage, this is the cool (6 times faster) version with pointers ;) */
/*   TMP TMP old version ...
template<class PixTyp> void label4(PixTyp* begin, int size_x, int size_y, int index, PixTyp label_val)
{
  // written by O. Stahlhut 01/01

  PixTyp* pix = begin+index;
  int x = index % size_x;
  int y = index / size_x;

  // store the point left to the start position for the second sweep
  PixTyp *left = pix; --left;
  int xl = x; --xl;

  // 1. sweep: fill one horizontal line to the right
  while (!(*pix) && (x < size_x))
  {
    *pix = label_val;
    ++pix;
    ++x;
  }

  // 2. sweep: fill one horizontal line to the left
  pix = left;
  x = xl;
  while ((x >= 0) && !(*pix))
  {
    *pix = label_val;
    --pix;
    --x;
  }

  // step one element to the right to get back into the labelarea
  ++x;
  ++pix;

  int check_next = (y + 1) < size_y;
  int check_prev = 0 <= (y - 1);
  PixTyp *next = pix + size_x;
  PixTyp *prev = pix - size_x;

  // 3. sweep: move over the horizontal line and check if there is more do to above or below (recursion)
  while ((*pix > 1) && (x < size_x))
  {
    if (check_next && !(*next))
      label4(begin, size_x, size_y, (y + 1) * size_x + x, label_val);

    if (check_prev && !(*prev))
      label4(begin, size_x, size_y, (y - 1) * size_x + x, label_val);

    ++x;
    ++pix;
    ++next;
    ++prev;
  }
}
*/


/** de-speckle the current image. The background should be set to 0.
    A connected region with label "label_val" (1) is set to label "label_set" (0),
    if the region is smaller than "size". Be aware that the label -1 is used for
    an intermediate step and shouldn't be used in the labelimage! */
template<class PixTyp> void despeckle(ImageT<PixTyp> &in, int size, PixTyp label_val = 1, PixTyp label_set = 0)
{
  // written by O. Stahlhut 10/01

  PixTyp *pix = in.begin();
  unsigned int labelsize;

  for (int i = 0; i < (int)(in.sizeImage()); ++i, ++pix)
  {
    if (*pix == label_val)
    {
      labelsize = label8(in.begin(), in.sizeX(), in.sizeY(), i, label_val, (PixTyp)-1);
      if (labelsize < (unsigned int)size)
				label8(in.begin(), in.sizeX(), in.sizeY(), i, (PixTyp)-1, label_set);
    }
  }

  // restore original label values
  pix = in.begin();
  for (int i = (int)(in.sizeImage()); i; --i, ++pix)
  {
    if (*pix == (PixTyp)-1)
      *pix = label_val;
  }
}


/** relabel a single label of an image. Consider the 8 point neighbourhood of
    each pixel, i.e. diagonal positioned pixels are traced and included.
    begin: pointer to image data, size_x / size_y: image width and height,
    index: offset to pixel of interest (seed), search for pixels of colour "label_val",
    set them to colour "label_set". returns the number of pixels in the new label.
*/

template<class PixTyp>unsigned int label8(PixTyp* begin, int size_x, int size_y, int index, PixTyp label_val, PixTyp label_set)
{
  unsigned int labelsize = 0;

  PixTyp *pix = begin + index;
  int x = index % size_x;
  int y = index / size_x;

  // store the point left to the start position for the second sweep
  PixTyp *left = pix; --left;
  int xr, xl = x; --xl;

  // 1. sweep: fill one horizontal line to the right
  while (*pix == label_val && (x < size_x))
  {
    *pix = label_set;
    ++pix;
    ++x;
    ++labelsize;
  }

  // xr stores the rightmost pixel position plus one ...
  xr = (x < size_x ? x : x - 1);

  // 2. sweep: fill one horizontal line to the left
  pix = left;
  x = xl;
  while ((x >= 0) && *pix == label_val)
  {
    *pix = label_set;
    --pix;
    --x;
    ++labelsize;
  }

  // step one element to the right if we have reached the left border
  if (x < 0)
    {
      ++x;
      ++pix;
    }
  // xl stores the leftmost pixelposition minus one ...
  xl = x;

  int check_next = (y + 1) < size_y;
  int check_prev = 0 <= (y - 1);
  PixTyp *next = pix + size_x;
  PixTyp *prev = pix - size_x;

  // 3. sweep: move over the horizontal line and check if there is more do to above or below (recursion)
  while (xl <= x && x <= xr)
  {
    if (check_next && *next == label_val)
      labelsize += label8(begin, size_x, size_y, (y + 1) * size_x + x, label_val, label_set);

    if (check_prev && *prev == label_val)
      labelsize += label8(begin, size_x, size_y, (y - 1) * size_x + x, label_val, label_set);

    ++x;
    ++next;
    ++prev;
  }

  return labelsize;
}

/** returns a list containing the bounding boxes of all labels of
    the current image. This member function should be used immediately
    after calling labelImage() */
template<class PixTyp> void calcBoundingBoxes(ImageT<int>& result, ImageT<PixTyp> &img, int nlabels)
{
  if (!nlabels) return;
  result.resize(nlabels,4,1);

  int* llx=result.begin(0);
  int* lly=result.begin(1);
  int* urx=result.begin(2);
  int* ury=result.begin(3);
  for (int i = 0; i < nlabels; i ++) {
    llx[i] = INT_MAX;
    ury[i] = INT_MAX;
    urx[i] = 0;
    lly[i] = 0;
  }

  int size_x = img.sizeX(), size_y = img.sizeY();
  for (int y = 0; y < size_y; y ++)
  {
    typename ImageT<PixTyp>::Iterator row=img.begin(y);
    for (int x = 0; x < size_x; x ++)
    {
      int elem = (int)row[x];
      assert(elem < nlabels);
      if (x < llx[elem])
        llx[elem] = x;
      if (y < ury[elem])
        ury[elem] = y;
      if (x > urx[elem])
        urx[elem] = x;
      if (y > lly[elem])
        lly[elem] = y;
    }
  }
}

/**
blow or shrink binary image 'in'. The number of blow steps are given
by 'blow'. The sign of 'blow' decides between blow or shrink.
blow > 0 => blow labels != 0
blow < 0 => blow label 0
*/
template<class PixTyp> void blowshrink(ImageT<PixTyp> &in, int blow)
{
  PixTyp mode = blow > 0 ? 1 : 0;
  blow = abs(blow);

  ImageT<PixTyp> tmp = in;

  while (blow --)
    {
      for (int y = 0; y < in.sizeY(); y ++)
				for (int x = 0; x < in.sizeX(); x ++)
            if (((in.get(x, y) == 0) && (mode == 0))
                || ((in.get(x, y) != 0) && (mode == 1)))
                {
                    PixTyp v = in.get(x, y);
                    tmp.set(x - 1, y, v, 0, true);
                    tmp.set(x - 1, y - 1, v, 0, true);
                    tmp.set(x - 1, y + 1, v, 0, true);

                    tmp.set(x, y - 1, v, 0, true);
                    tmp.set(x, y + 1, v, 0, true);

                    tmp.set(x + 1, y, v, 0, true);
                    tmp.set(x + 1, y - 1, v, 0, true);
                    tmp.set(x + 1, y + 1, v, 0, true);


                }
      in = tmp;
    }
}

/**
blow the label 'label' in image 'in'. The number of blow steps are given
by 'iterations'.
*/
template<class PixTyp> void blow(ImageT<PixTyp> &in, PixTyp label, int iterations){

  ImageT<PixTyp> tmp = in;

  while (iterations --) {
    for (int y = 0; y < in.sizeY(); y ++)
      for (int x = 0; x < in.sizeX(); x ++)
        if (in.get(x, y) == label) {
          tmp.set(x - 1, y, label, 0, true);
          tmp.set(x + 1, y, label, 0, true);
          tmp.set(x, y + 1, label, 0, true);
          tmp.set(x, y - 1, label, 0, true);
        }
    in = tmp;
  }
}

inline static int distance(int r1, int g1, int b1, int r2, int g2, int b2)
{
  int dr=r2-r1;
  int dg=g2-g1;
  int db=b2-b1;
  return dr*dr+dg*dg+db*db;
}

inline static int distance(int p1, int p2)
{
  int d=p2-p1;
  return (d>=0?d:-d);
}

template<class PixTyp> void denoise(ImageT<PixTyp> &im)
{
  ImageT<PixTyp> dest=im;
  int ysize=im.sizeY();
  int xsize=im.sizeX();
  int offset_x[]={+1,+1,0,-1,-1,-1,0,+1};
  int offset_y[]={0,-1,-1,-1,0,+1,+1,+1};
  if (im.noChannels()==3) {
    for (int y=1; y<ysize-1; y++)
      for (int x=1; x<xsize-1; x++) {
        int pr[8],pg[8],pb[8];
        for (int r=0; r<8; r++) {
          pr[r]=im.getInt(x+offset_x[r],y+offset_y[r],0);
          pg[r]=im.getInt(x+offset_x[r],y+offset_y[r],1);
          pb[r]=im.getInt(x+offset_x[r],y+offset_y[r],2);
        }
        int cr=im.getInt(x,y,0);
        int cg=im.getInt(x,y,1);
        int cb=im.getInt(x,y,2);
        int v[4];
        v[0]=(distance(pr[0],pg[0],pb[0],cr,cg,cb)<distance(pr[4],pg[4],pb[4],cr,cg,cb) ? 0 : 4);
        v[1]=(distance(pr[1],pg[1],pb[1],cr,cg,cb)<distance(pr[5],pg[5],pb[5],cr,cg,cb) ? 1 : 5);
        v[2]=(distance(pr[2],pg[2],pb[2],cr,cg,cb)<distance(pr[6],pg[6],pb[6],cr,cg,cb) ? 2 : 6);
        v[3]=(distance(pr[3],pg[3],pb[3],cr,cg,cb)<distance(pr[7],pg[7],pb[7],cr,cg,cb) ? 3 : 7);
        cr=(pr[v[0]]+pr[v[1]]+pr[v[2]]+pr[v[3]])/4;
        cg=(pg[v[0]]+pg[v[1]]+pg[v[2]]+pg[v[3]])/4;
        cb=(pb[v[0]]+pb[v[1]]+pb[v[2]]+pb[v[3]])/4;
        dest.set(x,y,cr,0);
        dest.set(x,y,cg,1);
        dest.set(x,y,cb,2);
      }
  }
  else {
    for (int ch=0; ch<im.noChannels(); ch++)
      for (int y=1; y<ysize-1; y++)
        for (int x=1; x<xsize-1; x++) {
          int p[8];
          for (int r=0; r<8; r++) {
            p[r]=im.getInt(x+offset_x[r],y+offset_y[r],ch);
          }
          int c=im.getInt(x,y,ch);
          int v[4];
        v[0]=(distance(p[0],c)<distance(p[4],c) ? 0 : 4);
        v[1]=(distance(p[1],c)<distance(p[5],c) ? 1 : 5);
        v[2]=(distance(p[2],c)<distance(p[6],c) ? 2 : 6);
        v[3]=(distance(p[3],c)<distance(p[7],c) ? 3 : 7);
        c=(p[v[0]]+p[v[1]]+p[v[2]]+p[v[3]])/4;
        dest.set(x,y,c,ch);
      }
  }
  im=dest;
}

} // namespace Ga
#endif
