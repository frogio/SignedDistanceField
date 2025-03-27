#include "Particle.h"
#include "glut.h"

Particle::Particle(float _radius, Vector2D _pos) 
	: radius(_radius), position(_pos), old_position(_pos), mass(1), acc(Vector2D()){}

void Particle::AddForce(Vector2D _f) {

	acc = acc +  _f * (1.f / mass);

}
void Particle::Draw() {

	glBegin(GL_LINE_LOOP);

	for (int i = 0; i < 360; i++) {
		float radian = 2 * PI * (float)i / 360;
		float x = position.x + radius * cos(radian), y = position.y + radius * sin(radian);

		glVertex2d(x, y);

	}

	glEnd();

}
void Particle::UpdatePosition() {
	// Verlet Intergration을 활용한 위치 업데이트
	// 마찰을 고려 안함.
	Vector2D tmp = position;
	position = position + (position - old_position) + acc;
	acc = Vector2D();
	old_position = position;

}

Vector2D Particle::GetPosition() {
	return position;
}

void Particle::SetPosition(Vector2D _pos) {
	position = _pos;
	old_position = _pos;
}

Vector2D Particle::GetCurVector() {
	return acc;
}

void Particle::SetCurVector(Vector2D& v) {
	acc = v;
}