#pragma once
#include "ForceGenerator.h"
#include <cmath>

class ExplosionGenerator : public ForceGenerator
{
public:
	ExplosionGenerator(Vector3D _center, float _ve, float _intensity, float _tau)
		: center(_center), ve(_ve), intensity(_intensity), tau(_tau)
	{
	}

	Vector3D Force(Vector3D pos, Vector3D vel, float mass) override
	{
		if (!active) return Vector3D(0, 0, 0);

		Vector3D d = pos - center;
		float r = d.magnitude();
		float radius = ve * time;    

		if (r >= radius || r < 1e-6f)
			return Vector3D(0, 0, 0);

		return d * (intensity / (r * r)) * expf(-time / tau);
	}

	void update(double dt) override
	{
		time += (float)dt;
		if (time > 4 * tau)
			active = false;
	}

	bool isActive() const { return active; }

private:
	Vector3D center;
	float ve;          // velocidad de expansión de la onda
	float intensity;   // K
	float tau;         // constante de tiempo
	float time = 0.f;  // t, tiempo desde la explosión
	bool active = true;
};