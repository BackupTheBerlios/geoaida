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

#ifndef _GAREGION_H
#define _GAREGION_H

#include "gaimage.h"

namespace Ga {

/** relabel the image: all pixel with label 'label_val' become the value '0'
    and the rest (background) become the value '1'.
    preparation for function 'labelImage' */
/** this is the cool and fasted version than possible ;) */
void labelBin(Image& img, int label_val);

/** fill the current image with unique labels. The background must be set to 0,
    the region borders must be set to 1. The regions will be labeled with values
    starting from 2. This version is 6 times faster by usage of pointers */
int labelImage(Image &img);

/** relabel a single label of an image. Consider only the 4 point neighbourhood of
    each pixel, i.e. diagonal positioned pixels are not traced and included.
    image: image, 
    index: offset to pixel of interest (seed), search for pixels of colour "label_val",
    set them to colour "label_set". returns the number of pixels in the new label.
*/
unsigned int label4(Image& img, int index, double label_val, double label_set);

/** returns a list containing the bounding boxes of all labels of
    the current image. This member function should be used immediately
    after calling labelImage() */
Image calcBoundingBoxes(Image &img, int nlabels);

/** de-speckle the current image. The background should be set to 0.
    A connected region with label "label_val" (1) is set to label "label_set" (0),
    if the region is smaller than "size". Be aware that the label -1 is used for
    an intermediate step and shouldn't be used in the labelimage! */
void despeckle(Image &in, int size, int label_val = 1, int label_set = 0);

/** de-noise the current image. Remove noise from an image without removing the edges */
void denoise(Image &in);

} // namespace Ga


#endif
