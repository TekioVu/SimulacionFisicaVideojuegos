#pragma once
#include "SpringForceGenerator.h"

class RubberBandForceGenerator : public SpringForceGenerator
{
public:
	using SpringForceGenerator::SpringForceGenerator; 

	Vector3D Force(Vector3D pos, Vector3D vel, float mass) override
	{
		Vector3D d = other->GetPos().p - pos;
		float length = d.magnitude();

		if (length <= restLength || length < 1e-6f)
			return Vector3D{ 0.0f, 0.0f, 0.0f };

		Vector3D dir = d * (1.0f / length);
		return dir * ((length - restLength) * k);   
	}
};