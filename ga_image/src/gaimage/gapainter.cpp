////////////////////////////////////////////////////////////////////////////////
///
/// \file		gapainter.cpp
/// \brief		Implementation for class "Painter"
///
/// \date		Jan. 2007
/// \author		Torsten Büschenfeld (bfeld@tnt.uni-hannover.de)
///				Karsten Vogt (vogt@tnt.uni-hannover.de)
///
/// \note		Tabulator size 4 used
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

#include "gapainter.h"

namespace Ga {

////////////////////////////////////////////////////////////////////////////////
///
/// \brief constructor
///
/// \param img image that will be used for all drawing operations
///
////////////////////////////////////////////////////////////////////////////////
Painter::Painter(Image& img, int channel) : img_(img), channel_(channel)
{
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draws a filled circle
///
/// \param center center point of the circle
/// \param radius radius of the circle
/// \param val value of the circle
///
////////////////////////////////////////////////////////////////////////////////
int Painter::fillCircle(Ga::Point center, int radius, double val)
{
  #define fill_line(a, b, c)\
    line_y = a;\
    line_xl = b;\
    line_xr = c;\
    if (line_y < 0 || line_y >= img_.sizeY()) { is_outside = true; }\
    else if (line_xr < 0 || line_xl >= img_.sizeX()) { is_outside = true; }\
    else {\
      is_inside = true;\
      if (line_xl < 0) { line_xl = 0; is_outside = true; }\
      if (line_xr >= img_.sizeX()) { line_xr = img_.sizeX()-1; is_outside = true; }\
      img_.fillRow(line_y, line_xl, line_xr, val);\
    }
  
  int x0 = center.x();
  int y0 = center.y();
  
  int f = 1 - radius;
  int ddF_x = 1;
  int ddF_y = -2 * radius;
  int x = 0;
  int y = radius;
 
  int line_y, line_xl, line_xr;
  bool is_inside = false;
  bool is_outside = false;
  
  fill_line(y0, x0 - radius, x0 + radius);
  fill_line(y0 - radius, x0, x0);
  fill_line(y0 + radius, x0, x0);
  
  while(x < y)
  {
    if(f >= 0) 
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    
    x++;
    ddF_x += 2;
    f += ddF_x;
    
    fill_line(y0 + y, x0 - x, x0 + x);
    fill_line(y0 - y, x0 - x, x0 + x);
    fill_line(y0 + x, x0 - y, x0 + y);
    fill_line(y0 - x, x0 - y, x0 + y);
  }
  
  if (!is_inside)
    return -1;
  else if (is_inside && is_outside)
    return -2;
  else
    return 0;
  
  #undef fill_line
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draws a line with the Bresenham-algorithm using pointers (fast!), with range-check
///
/// \param x1 x coordinate of the starting point
/// \param y1 y coordinate of the starting point
/// \param x2 x coordinate of the end point
/// \param y2 y cooridnate of the end point
/// \param c value of the line points
///
////////////////////////////////////////////////////////////////////////////////
void Painter::drawLine(Image& img, int x1, int y1, int x2, int y2, double c)
{
	Ga::drawLine(img, x1, y1, x2, y2, c);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draws a line with antialiasing and variable line-width (with range-check)
/// \param gx1 x geo-coordinate of point 1
/// \param gy2 y geo-coordinate of point 1
/// \param gx2 x geo-coordinate of point 2
/// \param gy2 y geo-coordinate of point 2
/// \param c value of the line points
/// \param antialias toggle antialiasing
///
////////////////////////////////////////////////////////////////////////////////
void Painter::drawGeoLine(Image& img, double gx1, double gy1, double gx2, double gy2, double width, double c, bool antialias)
{
	Ga::drawGeoLine(img, gx1, gy1, gx2, gy2, width, c, antialias);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief method to draw a polygon, i.e. its outline
///
/// \param points array of points defining the polygon
/// \param val value for pixels of polygon edges
///
////////////////////////////////////////////////////////////////////////////////
void Painter::drawPolygon(const PointArray& points, double val)
{
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief method to draw a filled polygon
///
/// \todo Eventually, concave polygons that are outside the image won't return
///       -1, nevertheless they are not drawn.
///
/// \param points array of points defining the polygon
/// \param val value for pixels of polygon
///
/// \return Returns -1 if polygon is outside, -2 if polygon is partly outside
///
////////////////////////////////////////////////////////////////////////////////
int Painter::fillPolygon(const PointArray& points, double val)
{
	IndexArray			indices;
	IndexArray			indices_x;
	std::vector<Edge>	edges;
	std::list<Edge>		active_edges;
	int					ret_val = 0;

	// create index array
	for (int i=0; i<points.size(); ++i) indices.push_back(i);
	indices_x=indices; // indices_x is just for outlier detection

	// sort points by their y coordinate
	qSortPointsY(points, indices, 0, points.size()-1);

	// Outlier detection
	if ((points[indices[0]].y() >= img_.sizeY()) || (points[indices.back()].y() < 0))
		return -1;
	qSortPointsX(points, indices_x, 0, points.size()-1);
	if ((points[indices_x[0]].x() >= img_.sizeX()) || (points[indices_x.back()].x() < 0))
		return -1;

	// Polygon partly outside?
	if ((points[indices_x[0]].x() >= 0) && (points[indices_x.back()].x() < img_.sizeX()) &&
		(points[indices[0]].y() >= 0) && (points[indices.back()].y() < img_.sizeY()))
		ret_val = 0;
	else
		ret_val = -2;

	// create and sort edges by their topmost point
	for (int i=0; i<indices.size()-1; ++i)
	{
		Point p=points[indices[i]];
		Point prev(0,0);
		Point next(0,0);

		// catch overflow
		if (0 == indices[i])
			prev=points.back();
		else
			prev=points[indices[i]-1];
		if (points.size()-1 == indices[i])
			next=points[0];
		else
			next=points[indices[i]+1];
		
		// insert edges in vector
		if (next.y() >= p.y())
		{
			edges.push_back(Edge(p.y(), next.y(), static_cast<double>(next.x()-p.x())/(next.y()-p.y())));
			edges.back().x = p.x();
		}
		if (prev.y() >= p.y())
		{
			edges.push_back(Edge(p.y(), prev.y(), static_cast<double>(prev.x()-p.x())/(prev.y()-p.y())));
			edges.back().x = p.x();
		}
	}

	// begin filling the polygon
	for (int i=points[indices[0]].y(); i<=points[indices.back()].y(); ++i)
	{
		// select active edges
		active_edges.clear();
		for (int j=0; j<edges.size(); ++j)
		{
			if ((edges[j].y_min <= i) && (i < edges[j].y_max))
			{
				active_edges.push_back(edges[j]);
				active_edges.back().x = static_cast<double>(i-edges[j].y_min)*active_edges.back().dx_dy + edges[j].x;
			}
		}

		// sort active edges
		active_edges.sort(EdgeSortX);

		// draw horizontal lines
		std::list<Edge>::const_iterator ci=active_edges.begin();
		while (ci != active_edges.end())
		{
			int x1 = static_cast<int>((*ci).x); ++ci;
			int x2 = static_cast<int>((*ci).x);

			// x2-1 to make sure to neighbouring plys do not overlap
			// This is important for drawing polys with holes
			img_.fillRow(i, x1, x2-1, val, 0, true);

			if (ci != active_edges.end()) ++ci;
		}

	}
	return ret_val;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief method to draw a filled polygon with holes
///
/// \todo Eventually, concave polygons that are outside the image won't return
///       -1, nevertheless they are not drawn.
///
/// \param points array of array of points defining the polygon, first index is the outer ring
/// \param val value for pixels of polygon
///
/// \return Returns -1 if polygon is outside, -2 if polygon is partly outside
///
////////////////////////////////////////////////////////////////////////////////
int Painter::fillPolygonWithHoles(const std::vector<PointArray>& points, double val)
{
	int ret_val = 0;
	int ring_count = points.size();
	
	std::vector<IndexArray>				indices(ring_count);
	std::vector<std::vector<Edge> >		edges(ring_count);

	for (int j = 0; j < ring_count; j++)
	{
		// create index array
		for (int i = 0; i < points[j].size(); i++)
			indices[j].push_back(i);
		
		// sort points by their y coordinate
		qSortPointsY(points[j], indices[j], 0, points[j].size()-1);
	}

	// Detect if the polygon is completely or partly outside the canvas
	int minX = points[0][0].x();
	int maxX = points[0][0].x();
	int minY = points[0][indices[0].front()].y();
	int maxY = points[0][indices[0].back()].y();
	
	for (int i = 0; i < points[0].size(); i++)
	{
		minX = std::min(minX, points[0][i].x());
		maxX = std::max(maxX, points[0][i].x());
	}
	
	if ((minX >= img_.sizeX()) || (maxX < 0) || (minY >= img_.sizeY()) || (maxY < 0))
		return -1;
	else if ((minX >= 0) && (maxX < img_.sizeX()) && (minY >= 0) && (maxY < img_.sizeY()))
		ret_val = 0;
	else
		ret_val = -2;
	
	// create and sort edges by their topmost point
	for (int j = 0; j < ring_count; j++)
	{
		for (int i = 0; i < indices[j].size() - 1; i++)
		{
			Point p = points[j][indices[j][i]];
			Point prev(0,0);
			Point next(0,0);

			// catch overflow
			if (indices[j][i] == 0)
				prev = points[j].back();
			else
				prev = points[j][indices[j][i] - 1];
			
			if (indices[j][i] == points[j].size() - 1)
				next = points[j][0];
			else
				next = points[j][indices[j][i] + 1];
			
			// insert edges in vector
			if (next.y() >= p.y())
			{
				edges[j].push_back(Edge(p.y(), next.y(), static_cast<double>(next.x() - p.x()) / (next.y() - p.y())));
				edges[j].back().x = p.x();
			}
			if (prev.y() >= p.y())
			{
				edges[j].push_back(Edge(p.y(), prev.y(), static_cast<double>(prev.x() - p.x()) / (prev.y() - p.y())));
				edges[j].back().x = p.x();
			}
		}
	}

	// begin filling the polygon
	for (int i = std::max(0, minY); i <= std::min(maxY, img_.sizeY() - 1); i++)
	{
		std::vector<std::deque<std::pair<int, int> > > spans(ring_count);
		
		for (int j = 0; j < ring_count; j++)
		{
			// select active edges
			std::list<Edge> active_edges;
			
			for (int k = 0; k < edges[j].size(); k++)
			{
				if ((edges[j][k].y_min <= i) && (i < edges[j][k].y_max))
				{
					active_edges.push_back(edges[j][k]);
					active_edges.back().x = static_cast<double>(i - edges[j][k].y_min) * active_edges.back().dx_dy + edges[j][k].x;
				}
			}

			// sort active edges
			active_edges.sort(EdgeSortX);

			// create horizontal spans of filled pixels
			std::list<Edge>::const_iterator ci = active_edges.begin();
			while (ci != active_edges.end())
			{
				int x1 = static_cast<int>((*ci).x); ++ci;
				int x2 = static_cast<int>((*ci).x);

				spans[j].push_back(std::pair<int, int>(std::max(0, x1), std::min(x2, img_.sizeX()) - 1));

				if (ci != active_edges.end()) ++ci;
			}
		}
		
		// Draw outer polygon spans and substract inner polygon spans
		while (!spans[0].empty())
		{
			// Get next outer span
			std::pair<int, int> outer_span = spans[0].front();
			spans[0].pop_front();
			
			// Substract inner spans, split outer span if necessary
			for (int k = 1; k < spans.size(); k++)
			{
				while (!spans[k].empty())
				{
					std::pair<int, int> inner_span = spans[k].front();
					
					if (inner_span.first > outer_span.second)
					{
						break;
					}
					else if (inner_span.second < outer_span.first)
					{
						spans[k].pop_front();
						continue;
					}
					else if ((inner_span.first <= outer_span.first) && (inner_span.second >= outer_span.second))
					{
						outer_span.first = outer_span.second = -1;
						break;
					}
					else if (inner_span.first <= outer_span.first)
					{
						outer_span.first = inner_span.second + 1;
						continue;
					}
					else if (inner_span.second >= outer_span.second)
					{
						outer_span.second = inner_span.first - 1;
						continue;
					}
					else
					{
						spans[0].push_front(std::pair<int, int>(inner_span.second + 1, outer_span.second));
						outer_span.second = inner_span.first - 1;
						continue;
					}
				}
			}
			
			// Draw remaining outer span
			img_.fillRow(i, outer_span.first, outer_span.second, val, 0, true);
		}
	}

	return ret_val;
}

//////////////////////////////////////////////////////////////////////////////
///
/// \brief compare function for std::sort algorithm
///
/// \param e1 edge 1
/// \param e2 edge 2
///
/// \return edge 1 smaller than edge 2?
///
////////////////////////////////////////////////////////////////////////////////
bool EdgeSortX(const Painter::Edge& e1, const Painter::Edge& e2)
{
  return e1.x < e2.x;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief method to sort points by their x position using quicksort
///
/// \param points array of points to be sorted
/// \param indices array of indices for sorted point array
///
////////////////////////////////////////////////////////////////////////////////
void Painter::qSortPointsX(const PointArray& points, IndexArray& indices, int min, int max) const
{
	if (min < max)
	{
		int px = points[indices[max]].x();

		int i = min-1;
		int j = max;
		while (true)
		{
			do ++i; while (points[indices[i]].x() < px);
			do --j; while ((points[indices[j]].x() > px) && (j>0));
			if (i<j)
			{
				unsigned int k = indices[i];
				indices[i] = indices[j];
				indices[j] = k;
			}
			else break;
		}
		unsigned int k = indices[i];
		indices[i] = indices[max];
		indices[max] = k;

		qSortPointsX(points, indices, min, i-1);
		qSortPointsX(points, indices, i+1, max);
	}
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief method to sort points by their y position using quicksort
///
/// \param points array of points to be sorted
/// \param indices array of indices for sorted point array
///
////////////////////////////////////////////////////////////////////////////////
void Painter::qSortPointsY(const PointArray& points, IndexArray& indices, int min, int max) const
{
	if (min < max)
	{
		int py = points[indices[max]].y();

		int i = min-1;
		int j = max;
		while (true)
		{
			do ++i; while (points[indices[i]].y() < py);
			do --j; while ((points[indices[j]].y() > py) && (j>0));
			if (i<j)
			{
				unsigned int k = indices[i];
				indices[i] = indices[j];
				indices[j] = k;
			}
			else break;
		}
		unsigned int k = indices[i];
		indices[i] = indices[max];
		indices[max] = k;

		qSortPointsY(points, indices, min, i-1);
		qSortPointsY(points, indices, i+1, max);
	}
}

} // namespace Ga
