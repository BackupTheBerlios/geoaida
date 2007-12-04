#ifndef _POLYGON_H_
#define _POLYGON_H_

#include <vector>
#include <list>
#include <stack>

#include <gaimage.h>
#include "vertex.h"

template<class T>
class Polygon
{
	public:
		void render(Ga::Image &image, int labelIndex=0) const;
		void computeConvexHull(const std::vector<Vertex<T> > &points);

		void calculateBoundingBox(T &left, T &right, T &top, T &bottom) const;
		
	private:
		double pseudoCross(const Vertex<T> &p1, const Vertex<T> &p2, const Vertex<T> &p3) const;
		Vertex<T> getStackElement(std::vector<Vertex<T> > &vstack, int no);
		
		std::vector<Vertex<T> > _vertices;
};

#include "polygon.cpp"

#endif
