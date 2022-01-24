#include "objectBase.h"

Point2D::Point2D() {
	this->x = 0;
	this->y = 0;
}

Point2D::Point2D(const int& x, const int& y) {
	this->x = x;
	this->y = y;
}

Coords2D::Coords2D(const int& x, const int& y, const int& width, const int& height) {
	this->origine = Point2D(x, y);
	this->width = width;
	this->height = height;
}

Coords2D::Coords2D(const Point2D& origine, const int& width, const int& height) {
	this->origine = origine;
	this->width = width;
	this->height = height;
}
