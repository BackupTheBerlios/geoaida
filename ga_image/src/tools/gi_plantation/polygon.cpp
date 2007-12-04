#include <assert.h>
#include <cmath>

const double PI2 = 6.283185307179586477025617918812372408865;

struct Line
{
	Line(int x1, int y1, int x2, int y2)
	{
		x = x1;
		y = y1;
		ex = x2;
		ey = y2;

		x_inc = (x2 - x1 < 0) ? -1 : +1;
		y_inc = (y2 - y1 < 0) ? -1 : +1;

		error = 0;
		error_comp = abs(y2 - y1);
		error_dec = abs(y2 - y1) * 2;
		error_inc = abs(x2 - x1) * 2;
	}

	void step()
	{
		y += y_inc;
		error += error_inc;

		while (error >= error_comp)
		{
			x += x_inc;
			error -= error_dec;
		}
	}

	bool isActive()
	{
		return y < ey;
	}

	int x, y, ex, ey;
	int x_inc, y_inc;

	int error;
	int error_comp, error_inc, error_dec;
};

template<class T>
		struct SortPointsYthenX
{
	bool operator ()(const std::pair<int, Vertex<T> > &p1, const std::pair<int, Vertex<T> > &p2)
	{
		if (p1.second[1] == p2.second[1])
			return p1.second[0] < p2.second[0];
		else
			return p1.second[1] < p2.second[1];
	}
};

template<class T>
		struct GrahamSort
{
	public:
		GrahamSort(const Vertex<T> &refPoint1, const Vertex<T> &refPoint2)
		{
			_refPoint = refPoint1;

			Vertex<T> refV = refPoint2 - refPoint1;
			_refValue = std::atan2(refV[0], refV[1]);
		}
		
		bool operator()(const Vertex<T> &v1, const Vertex<T> &v2)
		{
			Vertex<T> refV1 = v1 - _refPoint;
			Vertex<T> refV2 = v2 - _refPoint;
			
			double P1 = std::atan2(refV1[0], refV1[1]) - _refValue;
			if (P1 < 0)
				P1 += PI2;
			
			double P2 = std::atan2(refV2[0], refV2[1]) - _refValue;
			if (P2 < 0)
				P2 += PI2;
			
			return (P1 < P2);
		}

	private:
		Vertex<T> _refPoint;
		T _refValue;
};

template<class T>
		void Polygon<T>::render(Ga::Image &image, int labelIndex) const
{
	T left, right, top, bottom;
	calculateBoundingBox(left, right, top, bottom);
	
	std::vector<std::pair<int, Vertex<T> > > points;
	for (int i=0; i<_vertices.size(); i++)
		points.push_back(std::pair<int, Vertex<T> >(i, _vertices.at(i)));
	std::sort(points.begin(), points.end(), SortPointsYthenX<T>());

	std::vector<Line> linelist;

	int pindex = 0;
	for (int y=top; y<bottom; y++)
	{
		// Schreite in allen Linien in der Linienliste einen Pixel nach unten
		for (size_t i=0; i<linelist.size(); i++)
			linelist.at(i).step();

		// Entferne alle Linien, die nun inaktiv geworden sind
		std::vector<Line>::iterator iter = linelist.begin();
		while (iter != linelist.end())
		{
			if (!iter->isActive())
				iter = linelist.erase(iter);
			else
				iter++;
		}

		// Erzeuge alle von diesem Punkt ausgehenden Linien, die nach unten weisen
		while ((pindex < points.size()) && (points.at(pindex).second[1] == y))
		{
			int index = points.at(pindex).first;
			int previndex = (index+points.size()-1) % points.size();
			int nextindex = (index+1) % points.size();

			if (_vertices.at(previndex)[1] > _vertices.at(index)[1])
			{
				Line line(_vertices.at(index)[0], _vertices.at(index)[1], _vertices.at(previndex)[0], _vertices.at(previndex)[1]);
				linelist.push_back(line);
			}

			if (_vertices.at(nextindex)[1] > _vertices.at(index)[1])
			{
				Line line(_vertices.at(index)[0], _vertices.at(index)[1], _vertices.at(nextindex)[0], _vertices.at(nextindex)[1]);
				linelist.push_back(line);
			}

			pindex++;
		}

		// Füge die aktuellen Punkte einer scanline Liste hinzu und sortiere die Punkte nach x
		std::vector<int> scanlist;
		for (size_t i=0; i<linelist.size(); i++)
			scanlist.push_back(linelist.at(i).x);
		std::sort(scanlist.begin(), scanlist.end());

		// Zeichne Segmente auf der aktuellen scanline
		assert(!(scanlist.size() & 1));

		for (int i=0; i<(int)scanlist.size()/2; i++)
			for (int x=scanlist.at(i*2); x<scanlist.at(i*2+1); x++)
				image.set(x, y, labelIndex);
	}
}

template<class T>
		void Polygon<T>::computeConvexHull(const std::vector<Vertex<T> > &points)
{
	if (points.size() < 3)
		return;

	// Find reference point
	Vertex<T> referencePoint1;
	for (size_t i=0; i<points.size(); i++)
		referencePoint1 = referencePoint1 + points.at(i);

	referencePoint1 = referencePoint1 / static_cast<T>(points.size());

	// Find one point that lies on the convex hull
	Vertex<T> referencePoint2 = points.at(0);
	for (size_t i=1; i<points.size(); i++)
		if (points.at(0)[0] < referencePoint2[0])
			referencePoint2 = points.at(i);

	// Sort all points according to these reference points
	std::list<Vertex<T> > points_copy(points.begin(), points.end());
	points_copy.sort(GrahamSort<T>(referencePoint1, referencePoint2));

	// Compute 2D convex hull using Graham-Scan
	typename std::list<Vertex<T> >::iterator edgeListIter = points_copy.begin();

	std::vector<Vertex<T> > edgeStack;
	edgeStack.push_back(*(edgeListIter++));
	edgeStack.push_back(*(edgeListIter++));
	while (edgeListIter != points_copy.end())
	{
		while ((edgeStack.size() >= 2) && (pseudoCross(getStackElement(edgeStack, 1), getStackElement(edgeStack, 0), *edgeListIter) >= 0))
			edgeStack.pop_back();
		
		edgeStack.push_back(*(edgeListIter++));
	}

	if (pseudoCross(getStackElement(edgeStack, 1), getStackElement(edgeStack, 0), edgeStack.at(0)) >= 0)
		edgeStack.pop_back();

	// Copy edge vertices to polygon
	_vertices.clear();
	while (!edgeStack.empty())
	{
		_vertices.push_back(getStackElement(edgeStack, 0));
		edgeStack.pop_back();
	}

	// Print resulting vertices
	/*std::cout << "Print convex hull vertices" << std::endl;
	for (typename std::vector<Vertex<T> >::iterator iter=_vertices.begin(); iter!=_vertices.end(); iter++)
	{
		std::cout << "X: " << (*iter)[0] << "\tY: " << (*iter)[1] << std::endl;
	}
	std::cout << std::endl;*/
}

template<class T>
		void Polygon<T>::calculateBoundingBox(T &left, T &right, T &top, T &bottom) const
{
	if (_vertices.empty())
	{
		left = 0;
		right = 0;
		top = 0;
		bottom = 0;
		return;
	}
	
	left = _vertices.at(0)[0];
	right = _vertices.at(0)[0];
	top = _vertices.at(0)[1];
	bottom = _vertices.at(0)[1];

	for (size_t i=0; i<_vertices.size(); i++)
	{
		left = std::min(left, _vertices.at(i)[0]);
		right = std::max(right, _vertices.at(i)[0]);
		top = std::min(top, _vertices.at(i)[1]);
		bottom = std::max(bottom, _vertices.at(i)[1]);
	}
}

template<class T>
		double Polygon<T>::pseudoCross(const Vertex<T> &p1, const Vertex<T> &p2, const Vertex<T> &p3) const
{
	return (p2[0] - p1[0])*(p3[1] - p1[1]) - (p3[0] - p1[0])*(p2[1] - p1[1]);
}

template<class T>
		Vertex<T> Polygon<T>::getStackElement(std::vector<Vertex<T> > &vstack, int no)
{
	assert(vstack.size() > no);
	return vstack.at(vstack.size()-1-no);
}
