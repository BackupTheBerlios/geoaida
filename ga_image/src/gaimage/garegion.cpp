/***************************************************************************
                          ga_region.h  -  region oriented functions for
                                          geoAIDA template image class
                             -------------------
    begin                : Wed Sep 05 2001
    copyright            : (C) 2001 by Martin Pahl
    email                : pahl@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "gaimage.h"
#include "gaimaget.h"
#include "garegiont.h"

namespace Ga {

/** relabel the image: all pixel with label 'label_val' become the value '0'
    and the rest (background) become the value '1'.
    preparation for function 'labelImage' */
template <class PixTyp> void labelBin(ImageBase& img, int label_val)
{
  ImageT<PixTyp>& image=(ImageT<PixTyp>&) img;
  labelBin(image,(PixTyp)label_val);
}

void labelBin(Image& img, int label_val)
{
  ForEachTypeDo(img.typeId(),labelBin,(*(img.pImage()),label_val));
}

/** fill the current image with unique labels. The background must be set to 0,
    the region borders must be set to 1. The regions will be labeled with values
    starting from 2.*/
template<class PixTyp> int labelImage(ImageBase &img)
{
  ImageT<PixTyp>& image=(ImageT<PixTyp>&) img;
  return (int)labelImage(image);
}

int labelImage(Image &img)
{
  ForEachTypeDo(img.typeId(),return labelImage,(*(img.pImage())));
  return 0;
}

/** relabel the image: all pixel with label 'label_val' become the value '0'
    and the rest (background) become the value '1'.
    preparation for function 'labelImage' */
template <class PixTyp> void label4(ImageBase& img, int index,
                                    double label_val, double label_set, unsigned int& ret_val)
{
  ImageT<PixTyp>& image=(ImageT<PixTyp>&) img;
  ret_val=label4(image.begin(),image.sizeX(),image.sizeY(),index,PixTyp(label_val),PixTyp(label_set));
}

unsigned int label4(Image& img, int index, double label_val, double label_set)
{
  unsigned int ret_val;
  ForEachTypeDo(img.typeId(),label4,(*(img.pImage()),index,label_val,label_set,ret_val));
  return ret_val;
}

/** returns a list containing the bounding boxes of all labels of
    the current image. This member function should be used immediately
    after calling labelImage() */
template<class PixTyp> void calcBoundingBoxes(ImageT<int>& result, ImageBase &img, int nlabels)
{
  ImageT<PixTyp>& image=(ImageT<PixTyp>&) img;
  calcBoundingBoxes(result,image,nlabels);
}

Image calcBoundingBoxes(Image &img, int nlabels)
{
  Image im;
  ImageT<int> *result=new ImageT<int>;
  ForEachTypeDo(img.typeId(),calcBoundingBoxes,(*result,*(img.pImage()),nlabels));
  im.pImage(result);
  return im;
}

template <class PixTyp> void despeckle(ImageBase& in, int size, int label_val = 1, int label_set = 0 )
{
  ImageT<PixTyp>& image=(ImageT<PixTyp>&) in;
  despeckle(image, size, (PixTyp)label_val, (PixTyp)label_set);
}

void despeckle(Image &in, int size, int label_val = 1, int label_set = 0)
{
  ForEachTypeDo(in.typeId(),despeckle,(*(in.pImage()), size, label_val, label_set));
}

template <class PixTyp> void denoise(ImageBase& in)
{
  ImageT<PixTyp>& image=(ImageT<PixTyp>&) in;
  denoise(image);
}

void denoise(Image &in)
{
  ForEachTypeDo(in.typeId(),denoise,(*(in.pImage())));
}


template <class PixTyp> void blowshrink(ImageBase &in, int blow){
    ImageT<PixTyp>& image=(ImageT<PixTyp>&) in;
    blowshrink(image, blow);
}

void blowshrink(Image &in, int blow){
    ForEachTypeDo(in.typeId(),blowshrink,(*(in.pImage()), blow));
}


} // namespace Ga

