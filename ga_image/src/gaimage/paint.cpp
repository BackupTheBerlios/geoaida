////////////////////////////////////////////////////////////////////////////////
///
/// \file		paint.cpp
/// \brief		simple program for polyfill test purposes
///
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

//--- standard header --------------------------------------------------------//
#include <iostream>

//--- project header ---------------------------------------------------------//
#include "gaimaget.h"
#include "gapainter.h"

//--- misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief main function
///
/// this is the entrance point for program startup.
///
/// \param	argc number of given arguments
/// \param	argv array, storing the arguments
/// \return	exit code
///
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	Ga::Image img(typeid(unsigned char),500, 500);
	FILE* File;

	Ga::PointArray	poly1;
	Ga::PointArray	poly2;
	Ga::PointArray	poly3;
	Ga::Painter		painter(img);

	// white image
	img=255;

	// define the first polygon
	poly1.push_back(Ga::Point(50,200));
	poly1.push_back(Ga::Point(200,100));
	poly1.push_back(Ga::Point(220,130));
	poly1.push_back(Ga::Point(310,80));
	poly1.push_back(Ga::Point(400,300));
	poly1.push_back(Ga::Point(180,280));
	poly1.push_back(Ga::Point(100,260));

	// define a second polygon
	poly2.push_back(Ga::Point(50,350));
	poly2.push_back(Ga::Point(100,400));
	poly2.push_back(Ga::Point(150,350));
	poly2.push_back(Ga::Point(200,400));
	poly2.push_back(Ga::Point(250,360));
	poly2.push_back(Ga::Point(300,410));
	poly2.push_back(Ga::Point(150,420));

	// define a third polygon, this one should be clipped
	poly3.push_back(Ga::Point(450,450));
	poly3.push_back(Ga::Point(470,460));
	poly3.push_back(Ga::Point(530,470));
	poly3.push_back(Ga::Point(480,550));
	poly3.push_back(Ga::Point(460,480));

	// testing with 1000 fills of 3 polygons
	for (int i=0; i<1000; ++i)
	{
		// draw the filled polygons
		painter.fillPolygon(poly1, 150.0);
		painter.fillPolygon(poly2, 0.0);
		painter.fillPolygon(poly3, 78.4);
		painter.drawLine(img, 10,30,420,320, 200.0);
	}

	// write file to disc
	File=fopen("polytest.pnm","w"); 
	img.write(File);
	fclose(File);

	return 0;
}
