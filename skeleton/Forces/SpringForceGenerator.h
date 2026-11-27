#pragma once
#include "../Forces/ForceGenerator.h"
#include "../Utils/Vector3D.h"
#include "../Particle/Particle.h"
#include <iostream>

class SpringForceGenerator : public ForceGenerator
{
public:
	SpringForceGenerator(float _k, float _restLength, Particle* p)
		: k(_k), restLength(_restLength), other(p) {
	}

	Vector3D Force(Vector3D pos, Vector3D vel, float mass) override
	{
		Vector3D d = other->GetPos().p - pos;
		float length = d.magnitude();
		if (length < 1e-6f) return Vector3D{ 0.0f, 0.0f, 0.0f };

		Vector3D dir = d * (1.0f / length);
		return dir * ((length - restLength) * k); 
	}
	float getK() const { return k; }
	void setK(float _k) { k = _k; }

protected:
	float k;
	float restLength;
	Particle* other;
};