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

Point3D::Point3D(const ofVec3f& coords)
{
	this->x = coords[0];
	this->y = coords[1];
	this->z = coords[2];
}

Coords3D::Coords3D(const int& x, const int& y, const int& z, const int& width, const int& height, const int& length) {
	this->start = Point3D(x, y, z);
	this->width = width;
	this->height = height;
	this->length = length;
}

Coords3D::Coords3D(const Point3D& start, const int& width, const int& height, const int& length) {
	this->start = start;
	this->width = width;
	this->height = height;
	this->length = length;
}