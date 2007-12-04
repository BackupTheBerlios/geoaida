// Constructor / Destructor
template<class T>
		Vertex<T>::Vertex() : _zeroElement(0)
{
	_components.clear();
}

template<class T>
		Vertex<T>::Vertex(const Vertex<T> &arg) : _zeroElement(0), _components(arg._components)
{
}

template<class T>
		Vertex<T>::~Vertex()
{
}

// Misc
template<class T>
		int Vertex<T>::dimension() const
{
	return _components.size();
}

template<class T>
		void Vertex<T>::setDimension(int dimension)
{
	_components.resize(dimension);
}

// Length calculations
template<class T>
		double Vertex<T>::euclideanLength() const
{
	double sum = 0.0;
	for (int i=0; i<dimension(); i++)
		sum += static_cast<double>((*this)[i] * (*this)[i]);
	
	return std::sqrt(sum);
}

template<class T>
		double Vertex<T>::manhattanLength() const
{
	double sum = 0.0;
	for (int i=0; i<dimension(); i++)
		sum += static_cast<double>(std::abs((*this)[i]));
	
	return sum;
}

template<class T>
		double Vertex<T>::chessboardLength() const
{
	double sum = 0.0;
	for (int i=0; i<dimension(); i++)
		sum = std::max(sum, static_cast<double>(std::abs((*this)[i])));
	
	return sum;
}

// Operators
template<class T>
		const T &Vertex<T>::operator [](int index) const
{
	if (index < dimension())
		return _components.at(index);
	else
		return _zeroElement;
}

template<class T>
		T &Vertex<T>::operator [](int index)
{
	if (index >= dimension())
		setDimension(index+1);

	return _components.at(index);
}

template<class T>
		T &Vertex<T>::at(int index)
{
	if (index >= dimension())
		setDimension(index+1);

	return _components.at(index);
}

template<class T>
		Vertex<T> Vertex<T>::operator +(const Vertex<T> &arg) const
{
	Vertex<T> retValue(*this);
	for (int i=0; i<arg.dimension(); i++)
		retValue.at(i) += arg[i];
	
	return retValue;
}

template<class T>
		Vertex<T> Vertex<T>::operator -(const Vertex<T> &arg) const
{
	Vertex<T> retValue(*this);
	for (int i=0; i<arg.dimension(); i++)
		retValue.at(i) -= arg[i];
	
	return retValue;
}

template<class T>
		Vertex<T> Vertex<T>::operator *(const T &arg) const
{
	Vertex<T> retValue(*this);
	for (int i=0; i<retValue.dimension(); i++)
		retValue.at(i) *= arg;
	
	return retValue;
}

template<class T>
		Vertex<T> Vertex<T>::operator /(const T &arg) const
{
	Vertex<T> retValue(*this);
	for (int i=0; i<retValue.dimension(); i++)
		retValue.at(i) /= arg;
	
	return retValue;
}

template<class T>
		double Vertex<T>::dot(const Vertex<T> &arg) const
{
	if (this->dimension() != arg.dimension())
		return 0.0;
	
	double sum = 0.0;
	for (int i=0; i<this->dimension(); i++)
		sum += (*this)[i] * arg[i];
	
	return sum;
}

template<class T>
		Vertex<T> Vertex<T>::cross(const Vertex<T> &arg) const
{
	Vertex<T> returnValue();
	return returnValue;
}
