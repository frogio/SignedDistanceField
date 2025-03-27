#pragma once
#include <math.h>
#include "Vector2D.h"

#define PI 3.141592

class Particle {
public:
	Vector2D position;
	Vector2D old_position;
	float mass;
	Vector2D acc;
	float radius;

public:
	Particle(float _radius, Vector2D _pos);

	void AddForce(Vector2D _f);
	void Draw();
	void UpdatePosition();
	Vector2D GetPosition();

	void SetPosition(Vector2D _pos);
	Vector2D GetCurVector();
	void SetCurVector(Vector2D& v);

};