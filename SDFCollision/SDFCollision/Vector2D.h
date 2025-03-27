#pragma once

class Vector2D {

public:
	float x, y;

	Vector2D() : x(0), y(0) {}

	Vector2D(float _x, float _y)
		: x(_x), y(_y) {}

	Vector2D operator+(const Vector2D& _v) {
		Vector2D v(this->x + _v.x, this->y + _v.y);
		return v;
	}

	Vector2D operator-(const Vector2D& _v) {
		Vector2D v(this->x - _v.x, this->y - _v.y);
		return v;
	}

	Vector2D& operator=(const Vector2D& _v) {
		this->x = _v.x, this->y = _v.y;
		return *this;
	}

	Vector2D operator*(const float scalar) {
		Vector2D v(this->x * scalar, this->y * scalar);
		return v;
	}

	Vector2D operator/(const float scalar) {
		Vector2D v(this->x / scalar, this->y / scalar);
		return v;
	}

	float GetSize() {
		return sqrt(x * x + y * y);
	}

	Vector2D GetUnitVector() {
		return Vector2D(x / GetSize(), y / GetSize());
	}

	float GetX() {
		return x;
	}

	float GetY() {
		return y;
	}

	float Dot(Vector2D& v) {
		return x * v.x + y * v.y;
	}
};