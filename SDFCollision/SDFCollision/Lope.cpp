#include "Lope.h"

Lope::Lope(float headPos, int _length, float _thickness, bool _activateGravity, Vector2D _gravity) :
	length(_length), thickness(_thickness / 2), restLength(_thickness),
	activateGravity(_activateGravity) ,gravity(_gravity)
{
	for (int i = 0; i < length; i++) {
		Particle* p = new Particle(thickness, Vector2D(i / 500.f + 0.1f, headPos));
		particles.push_back(p);
	}
}

Lope::Lope(float headPos, int _length, float _thickness, bool _activateGravity) : 
length(_length), thickness(_thickness / 2), restLength(_thickness),
activateGravity(_activateGravity)
{
	for (int i = 0; i < length; i++) {
		Particle* p = new Particle(thickness, Vector2D(i / 500.f + 0.1f, headPos));
		particles.push_back(p);
	}

}


Lope::Lope(int _length, float _thickness, bool _activateGravity)
	: length(_length), thickness(_thickness / 2), restLength(_thickness),
	activateGravity(_activateGravity)
{

	for (int i = 0; i < length; i++) {
		Particle* p = new Particle(thickness, Vector2D(i / 500.f + 0.1f, 0.f));
		particles.push_back(p);
	}

}
void Lope::Draw() {

	for (int i = 0; i < length; i++)
		particles[i]->Draw();

}
void Lope::Update() {

	for (int i = 0; i < length - 1; i++) {
		Particle * p1 = particles[i];
		Particle * p2 = particles[i + 1];
		Vector2D d = p1->GetPosition() - p2->GetPosition();
		float force = -3.f * (d.GetSize() - restLength);
		Vector2D forceVector = d.GetUnitVector() * force;

		p1->AddForce(forceVector * 0.1);
		p2->AddForce(forceVector * 0.1 * -1);
		
		//p1->UpdatePosition();
		//p2->UpdatePosition();
	}
	
	for (int i = 0; i < length - 2; i++) {
		Particle* p1 = particles[i];
		Particle* p2 = particles[i + 2];
		Vector2D d = p1->GetPosition() - p2->GetPosition();
		float force = -3.f * (d.GetSize() - restLength * 2);
		Vector2D forceVector = d.GetUnitVector() * force;

		p1->AddForce(forceVector * 0.1);
		p2->AddForce(forceVector * 0.1 * -1);

		//p1->UpdatePosition();
		//p2->UpdatePosition();
	}
	
	if(activateGravity){

		for (int i = 0; i < length; i++){
			particles[i]->AddForce(gravity);
			//particles[i]->UpdatePosition();
		}
	}

	for (int i = 0; i < length; i++)
		particles[i]->UpdatePosition();


}

void Lope::SetGravity(bool _gravity) {
	activateGravity = _gravity;
}

void Lope::SetHeadPosition(Vector2D _pos) {

	particles[length - 1]->SetPosition(_pos);

}

Particle * Lope::GetParticle(float x, float y) {

	float radius = thickness;

	for (int i = 0; i < length; i++) {

		Particle* p = particles[i];
		Vector2D v(x, y);
		Vector2D dist = v - p->GetPosition();

		if (dist.GetSize() < radius)
			return p;

	}

	return NULL;
}