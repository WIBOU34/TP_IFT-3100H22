#include "objectBase3D.h"

Point3D::Point3D() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Point3D::Point3D(const int& x, const int& y, const int& z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Coords3D::Coords3D(const int& x, const int& y, const int& z, const int& width, const int& height, const int& length) {
	this->origine = Point3D(x, y, z);
	this->width = width;
	this->height = height;
	this->length = length;
}

Coords3D::Coords3D(const Point3D& origine, const int& width, const int& height, const int& length) {
	this->origine = origine;
	this->width = width;
	this->height = height;
	this->length = length;
}