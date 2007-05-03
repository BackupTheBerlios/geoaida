////////////////////////////////////////////////////////////////////////////////
///
/// \file		gapainter.h
/// \brief		prototype for class "Painter"
///
/// \date		Jan. 2007
/// \author		Torsten Büschenfeld (bfeld@tnt.uni-hannover.de)
///
////////////////////////////////////////////////////////////////////////////////

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __GA_PAINTER_H
#define __GA_PAINTER_H

//--- standard header --------------------------------------------------------//
#include <list>
#include <vector>

//--- project header ---------------------------------------------------------//
#include "gadraw.h"
#include "gaimage.h"
#include "gapoint.h"

namespace Ga
{

typedef std::vector<Ga::Point> PointArray;
typedef std::vector<unsigned int> IndexArray;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief class for drawing in an image
///
////////////////////////////////////////////////////////////////////////////////
class Painter
{
	public:
		//--- constructor/destructor -----------------------------------------//
		Painter(Image& img, int channel=0);

		//--- methods --------------------------------------------------------//
		void drawLine(Image&, int, int, int, int, double);
		void drawGeoLine(Image&, double, double, double, double, double, double, bool);

		void drawPolygon(const PointArray&, double);
		void fillPolygon(const PointArray&, double);

		void setImage(Image&, int channel=0);
		void setChannel(int);

	protected:
		Image&	img_;		///< image that is used for all operations
		int		channel_;	///< channel used for all operations on the image

	private:

		class Edge
		{
			public:

				Edge(int _y_min, int _y_max, double _dx_dy):
					y_min(_y_min),
					y_max(_y_max),
					dx_dy(_dx_dy),
					x(0.0){};

				int		y_max;
				int		y_min;
				double	dx_dy;
				double	x;
				
		};

		void qSortPointsY(const PointArray&, IndexArray&, int, int) const;

		friend bool EdgeSortX(const Edge&, const Edge&);
};

//--- Implementation goes here for inline reasons ----------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Tell the class which image to use, using channel 0 as default
///
/// \param img Image to be used by Painter class
/// \param channel Channel of the image that is used by methods of painter class
///
////////////////////////////////////////////////////////////////////////////////
inline void Painter::setImage(Image& img, int channel)
{
	img_ = img;
	channel_ = channel;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Tell the class which channel of the image to use
///
/// \param channel Channel of the image to be used by methods of painter class
///
////////////////////////////////////////////////////////////////////////////////
inline void Painter::setChannel(int channel)
{
	channel_ = channel;
}


//--- Global functions -------------------------------------------------------//
bool EdgeSortX(const Painter::Edge&, const Painter::Edge&);


} // namespace Ga

#endif
