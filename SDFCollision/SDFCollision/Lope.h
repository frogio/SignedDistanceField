#pragma once

#include <vector>
#include "Particle.h"

using namespace std;

class Lope {
public:
	int length;
	float thickness;
	float restLength;
	bool activateGravity;
	Vector2D gravity;
	vector<Particle *> particles;

public:
	Lope(int _length, float _thickness, bool _activateGravity);
	Lope(float headPos, int _length, float _thickness, bool _activateGravity);
	Lope(float headPos, int _length, float _thickness, bool _activateGravity,Vector2D _gravity);

	void Draw();
	void Update();
	void SetHeadPosition(Vector2D _pos);
	void SetGravity(bool _gravity);
	Particle* GetParticle(float x, float y);
};