#pragma once
#include "ForceGenerator.h"
#include "Vector3D.h"

class BuoyancyForceGenerator : public ForceGenerator
{
public:
	BuoyancyForceGenerator(float h, float v, float d)
		:_height(h), _volume(v), _liquid_density(d){
	}

	Vector3D Force(Vector3D pos, Vector3D vel, float mass) override
	{
		float h = pos.y;
		float h0 = _liquid_particle->GetPos().p.y;

		Vector3 f(0, 0, 0);
		float immersed = 0.0f;

		if (h - h0 > _height * 0.5f) {
			immersed = 0.0f;
		}
		else if (h0 - h > _height * 0.5f) {
			immersed = 1.0f;
		}
		else {
			immersed = (h0 - h) / _height + 0.5f;
		}

		f.y = _liquid_density * _volume * immersed * 9.8f;
	}

private:
	float _height;
	float _volume;
	float _liquid_density;
	float _gravity = 9.8;

	Particle* _liquid_particle;
	
};