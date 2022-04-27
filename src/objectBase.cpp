#include "objectBase.h"

Point2D::Point2D() {
	this->x = 0;
	this->y = 0;
}

Point2D::Point2D(const int& x, const int& y) {
	this->x = x;
	this->y = y;
}

Coords2D::Coords2D(const int& x, const int& y, const int& x1, const int& y1) {
	this->origine = ofVec2f(x, y);
	this->end = ofVec2f(x1, y1);
}

Coords2D::Coords2D(const ofVec2f& origine, const ofVec2f& end) {
	this->origine = origine;
	this->end = end;
}

int Coords2D::getWidth() const {
	return std::abs(this->end.x - this->origine.x);
}

int Coords2D::getHeight() const {
	return std::abs(this->end.y - this->origine.y);
}
