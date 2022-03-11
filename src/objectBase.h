#pragma once
//#include "renderer.h"
#include <utility>
#include <string>
#include "ofColor.h"
#include "ofVec2f.h"
#include <vector>

// énumération des différents types de primitives vectorielles
enum class VectorPrimitiveType { none, pixel, point, line, square, rectangle, circle, ellipse };
enum class VectorFormeType { primitiveGroup, primitive, triangle };

// structure de primitive vectorielle générique
struct VectorPrimitive {
	VectorPrimitiveType type;				// 1 * 4 = 4  octets
	ofVec2f				posStart;			// 2 * 4 = 8  octets
	ofVec2f				posEnd;				// 2 * 4 = 8  octets
	float				strokeWidth;		// 1 * 4 = 4  octets
	ofColor				strokeColor;		// 4 * 1 = 4  octets
	ofColor				fillColor;			// 4 * 1 = 4  octets

	bool operator==(const VectorPrimitive& primitive) const {
		return primitive.type == this->type
			&& primitive.posStart == this->posStart && primitive.posEnd == this->posEnd;
	}
};											//       = 32 octets

// structure de groupe de primitive vectorielle générique
struct VectorForme {
	VectorFormeType type;						//			= 40 octets
	std::vector<VectorPrimitive*> primitives;	//	32 * n	>= 32 octets

	bool operator==(const VectorForme& forme) const {
		const int size = forme.primitives.size();
		if (size != this->primitives.size()) {
			return false;
		}
		if (forme.type != this->type) {
			return false;
		}
		for (int i = 0; i < size; i++) {
			const VectorPrimitive& primitiveCompare = *forme.primitives.at(i);
			const VectorPrimitive& primitiveThis = *this->primitives.at(i);
			if (!(primitiveCompare == primitiveThis)) {
				return false;
			}
		}
		return true;
	}
};

//struct VectorPrimitive {
//	VectorPrimitiveType type;				// 1 * 4 = 4  octets
//	std::list<ofVec2f>	positions;			// 2 * 4 = 8  octets
//	//ofVec2f				posEnd;				// 2 * 4 = 8  octets
//	float				strokeWidth;		// 1 * 4 = 4  octets
//	ofColor				strokeColor;		// 4 * 1 = 4  octets
//	ofColor				fillColor;			// 4 * 1 = 4  octets
//};											//       = 32 octets

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
	Coords2D(const int& x, const int& y, const int& x1, const int& y1);
	Coords2D(const ofVec2f& origine, const ofVec2f& end);
	int getWidth() const;
	int getHeight() const;
	ofVec2f origine;
	ofVec2f end;
	bool operator==(const Coords2D& coords) const {
		return coords.origine == this->origine && coords.end == this->end;
	}
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
	ObjectBase2D(const int& x, const int& y, const int& x1, const int& y1, const T& object, const std::string& name);
	ObjectBase2D(const Coords2D& coords, const T& object, const std::string& name);
	T getObject() const;
	Coords2D getCoords()const;
	std::string getName() const;
	void setName(const std::string& name);
	void createObject(const int& x, const int& y, const int& x1, const int& y1, const T& object, const std::string& name);
	void createObject(const Coords2D& coords, const T& object, const std::string& name);
	bool isPointInObject(const int& x, const int& y) const;
	bool operator==(const ObjectBase2D& objetBase2D) const {

		if (objetBase2D.name != this->name) {
			return false;
		}
		return objetBase2D.objet2D == this->objet2D;
	}
private:
	std::pair<Coords2D, T> objet2D;
	std::string name;
	//ObjectBase2D() = default;
};

//méthodes doivent être inline sinon ça ne compile pas :(
template<typename T>
inline ObjectBase2D<T>::ObjectBase2D(const int& x, const int& y, const int& x1, const int& y1, const T& object, const std::string& name) {
	this->createObject(x, y, x1, y1, object, name);
}

template<typename T>
inline ObjectBase2D<T>::ObjectBase2D(const Coords2D& coords, const T& object, const std::string& name) {
	this->createObject(coords, object, name);
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
inline std::string ObjectBase2D<T>::getName() const {
	return this->name;
}

template<typename T>
inline void ObjectBase2D<T>::setName(const std::string& name) {
	this->name = name;
}

template<typename T>
inline void ObjectBase2D<T>::createObject(const int& x, const int& y, const int& x1, const int& y1, const T& object, const std::string& name) {
	Coords2D coords = Coords2D(x, y, x1, y1);
	this->createObject(coords, object, name);
}

template<typename T>
inline void ObjectBase2D<T>::createObject(const Coords2D& coords, const T& object, const std::string& name) {
	this->objet2D = std::make_pair(coords, object);
	this->name = name;
}

template<typename T>
// Vérifie seulement la zone rectangulaire formée par les points dans Coords2D
inline bool ObjectBase2D<T>::isPointInObject(const int& x, const int& y) const {
	const Coords2D coords = this->getCoords();
	return coords.origine.x <= x && coords.origine.y <= y
		&& coords.end.x >= x && coords.end.y >= y;
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

