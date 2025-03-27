#pragma once

#include <math.h>

class Vector2D {
public:
	double x, y;

	Vector2D() : x(0), y(0) {}
	Vector2D(double _x, double _y) : x(_x), y(_y) {}

	Vector2D operator+(const Vector2D & v) {
		return Vector2D(x + v.x, y + v.y);
	}

	Vector2D operator-(const Vector2D& v) {
		return Vector2D(x - v.x, y - v.y);
	}
	
	Vector2D operator*(const double scalar) {
		return Vector2D(x * scalar, y * scalar);
	}
	
	Vector2D operator/(const double scalar) {
		return Vector2D(x / scalar, y / scalar);
	}

	double GetLength() {
		return sqrt(x * x + y * y);
	}
};