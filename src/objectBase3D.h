#pragma once

#include "ofColor.h"
#include "ofVec3f.h"
#include <string>

// énumération des différents types de primitives vectorielles
enum class VectorObject3DType { none, cube, sphere, cone, cylinder };
enum class MeshRenderMode { fill, wireframe, vertex };

// structure d'un objet 3D
struct VectorObjectSettings {
	VectorObject3DType   type;				// 1 * 4 = 4  octets
	ofVec3f				posStart;			// 3 * 4 = 12  octets
	float				width;				// 1 * 4 = 4  octets	x
	float				height;				// 1 * 4 = 4  octets	y
	float				length;				// 1 * 4 = 4  octets	z
	float				radius = 0.0f;		// 1 * 4 = 4  octets	r
	ofColor				fillColor = 200;	// 4 * 1 = 4  octets
};											//       = 36 octets

// structure d'un objet et son mode de rendu
struct VectorObjs3D {
	MeshRenderMode renderMode;					// 1 * 1 =  1 octet
	std::vector<VectorObjectSettings*> object3D;		//		 = 36 octets
};												//		 = 37 octets

class Point3D {
public:
	Point3D();
	Point3D(const int& x, const int& y, const int& z);
	int x;
	int y;
	int z;
};

class Coords3D {
public:
	Coords3D() = default;
	Coords3D(const int& x, const int& y, const int& z, const int& width, const int& height, const int& length);
	Coords3D(const Point3D& origine, const int& width, const int& height, const int& length);
	//Coords3D& operator=(const Coords3D& coords) const;
	//int getX();
	//int getY();
	//int getz();
	//int getWidth();
	//int getHeight();
	//int getLength();
	// 
//private:
	Point3D origine;
	int width;
	int height;
	int length; // pour 3D (valeur z)
};

template<typename T>
class ObjectBase3D {
public:
	ObjectBase3D() = default;
	ObjectBase3D(const int& x, const int& y, const int& z, const int& width, const int& height, const int& length, const T& object, const std::string& name);
	ObjectBase3D(const Coords3D& coords, const T& object, const std::string& name);
	T getObject() const;
	Coords3D getCoords()const;
	std::string getName() const;
	void setName(const std::string& name);
	void createObject(const int& x, const int& y, const int& z, const int& width, const int& height, const int& length, const T& object, const std::string& name);
	void createObject(const Coords3D& coords, const T& object, const std::string& name);
	bool isPointInObject(const int& x, const int& y, const int& z) const;
private:
	std::pair<Coords3D, T> objet3D;
	std::string name;
};

template<typename T>
inline ObjectBase3D<T>::ObjectBase3D(const int& x, const int& y, const int& z, const int& width, const int& height, const int& length, const T& object, const std::string& name) {
	this->createObject(x, y, z, width, height, length, object, name);
}

template<typename T>
inline ObjectBase3D<T>::ObjectBase3D(const Coords3D& coords, const T& object, const std::string& name) {
	this->createObject(coords, object, name)
}

template<typename T>
inline T ObjectBase3D<T>::getObject() const {
	return this->objet3D.second;
}

template<typename T>
inline Coords3D ObjectBase3D<T>::getCoords() const {
	return this->objet3D.first;
}

template<typename T>
inline std::string ObjectBase3D<T>::getName() const {
	return this->name;
}

template<typename T>
inline void ObjectBase3D<T>::setName(const std::string& name) {
	this->name = name;
}

template<typename T>
inline void ObjectBase3D<T>::createObject(const int& x, const int& y, const int& z, const int& width, const int& height, const int& length, const T& object, const std::string& name) {
	Coords3D coords = Coords3D(x, y, z, width, height, length);
	this->createObject(coords, object, name);
}

template<typename T>
inline void ObjectBase3D<T>::createObject(const Coords3D& coords, const T& object, const std::string& name) {
	this->objet3D = std::make_pair(coords, object);
	this->name = name;
}

template<typename T>
inline bool ObjectBase3D<T>::isPointInObject(const int& x, const int& y, const int&z) const {
	const Coords3D coords = this->getCoords();
	return coords.origine.x <= x && coords.origine.y <= y && coords.origine.z <= z
		&& coords.width + coords.origine.x >= x
		&& coords.height + coords.origine.y >= y
		&& coords.length + coords.origine.z >= z;
}