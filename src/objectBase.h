#pragma once
//#include "renderer.h"
#include <utility>

class Point2D {
public:
	Point2D();
	Point2D(const int& x, const int& y);
	int x;
	int y;
};

class Coords2D {
public:
	Coords2D() = default;
	Coords2D(const int& x, const int& y, const int& width, const int& height);
	Coords2D(const Point2D& origine, const int& width, const int& height);
	//Coords2D& operator=(const Coords2D& coords) const;
	//int getX();
	//int getY();
	//int getWidth();
	//int getHeight();
//private:
	Point2D origine;
	int width;
	int height;
};
//class Point3D {
//public:
//	Point3D();
//	Point3D(const int& x, const int& y, const int& z);
//	int x;
//	int y;
//	int z;
//};

//class Coords3D {
//public:
//	//Coords3D() = default;
//	Coords3D(const int& x, const int& y, const int& width, const int& height, const int& length);
//	Coords3D(const Point3D& origine, const int& width, const int& height, const int& length);
//	Coords3D& operator=(const Coords3D& coords) const;
//	//int getX();
//	//int getY();
//	//int getWidth();
//	//int getHeight();
////private:
//	Point2D origine;
//	int width;
//	int height;
//	int length; // pour 3D (valeur z)
//};

template<typename T>
class ObjectBase2D {
public:
	ObjectBase2D() = default;
	ObjectBase2D(const int& x, const int& y, const int& width, const int& height, const T& object);
	ObjectBase2D(const Coords2D& coords, const T& object);
	T getObject() const;
	Coords2D getCoords()const;
	void createObject(const int& x, const int& y, const int& width, const int& height, const T& object);
	void createObject(const Coords2D& coords, const T& object);
private:
	std::pair<Coords2D, T> objet2D;
	//ObjectBase2D() = default;
};

//méthodes doivent être inline sinon ça ne compile pas :(
template<typename T>
inline ObjectBase2D<T>::ObjectBase2D(const int& x, const int& y, const int& width, const int& height, const T& object) {
	this->createObject(x, y, width, height, object);
}

template<typename T>
inline ObjectBase2D<T>::ObjectBase2D(const Coords2D& coords, const T& object) {
	this->createObject(coords, object);
}

template<typename T>
inline T ObjectBase2D<T>::getObject() const {
	return this->objet2D.second;
}

template<typename T>
inline Coords2D ObjectBase2D<T>::getCoords() const {
	return this->objet2D.first;
}

template<typename T>
inline void ObjectBase2D<T>::createObject(const int& x, const int& y, const int& width, const int& height, const T& object) {
	Coords2D coords = Coords2D(x, y, width, height);
	this->createObject(coords, object);
}

template<typename T>
inline void ObjectBase2D<T>::createObject(const Coords2D& coords, const T& object) {
	this->objet2D = std::make_pair(coords, object);
}

//template<class T>
//class ObjectBase3D {
//public:
//	T getObject() const;
//	Coords3D getCoords() const;
//	void createObject(const int& x, const int& y, const int& width, const int& height, const int& length, const T& object);
//	void createObject(const Coords3D& coords, const T& object);
//private:
//	std::pair<Coords3D, T> objet3D;
//	ObjectBase3D() = default;
//};