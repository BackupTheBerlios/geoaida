#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <vector>
#include <algorithm>
#include <cmath>

enum VertexComponents
{
	X	= 0,
	Y,
	Z,
	W,

	R	= 0,
	G,
	B,
	A
};

// d-dimensional vector
template<class T>
class Vertex
{
	public:
		// Constructor / Destructor
		Vertex();
		Vertex(const Vertex<T> &arg);

		~Vertex();
		
		// Misc
		int dimension() const;
		void setDimension(int dimension);
		
		// Length calculations
		double euclideanLength() const;
		double manhattanLength() const;
		double chessboardLength() const;
		
		// Operators
		const T &operator [](int index) const;
		T &operator [](int index);
		T &at(int index);

		Vertex<T> operator +(const Vertex<T> &arg) const;
		Vertex<T> operator -(const Vertex<T> &arg) const;
		Vertex<T> operator *(const T &arg) const;
		Vertex<T> operator /(const T &arg) const;
		
		double dot(const Vertex<T> &arg) const;
		Vertex<T> cross(const Vertex<T> &arg) const;
		
	private:
		std::vector<T> _components;
		T _zeroElement;
};

#include "vertex.cpp"

#endif
