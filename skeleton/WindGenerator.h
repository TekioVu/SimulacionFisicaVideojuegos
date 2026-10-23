#pragma once
#include "ForceGenerator.h"

class WindGenerator : public ForceGenerator
{
public:
	using ForceGenerator::ForceGenerator;

	Vector3D Force(Vector3D pos, Vector3D vel, float mass) override
	{

	}

};

