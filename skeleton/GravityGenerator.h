#pragma once
#include "ForceGenerator.h"
#include "Vector3D.h"

class GravityGenerator : public ForceGenerator
{
public:
	using ForceGenerator::ForceGenerator;

	Vector3D Force(Vector3D pos, Vector3D vel, float mass) override
	{
		return (Vector3D{0.0f, -9.8, 0.0f} * mass);
	}

};

