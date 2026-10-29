#pragma once
#include "ForceGenerator.h"

class WhirlwindGenerator : public ForceGenerator
{
public:
	WhirlwindGenerator(Vector3D _center, float _k) : center(_center), k(_k)
	{
	}

	Vector3D Force(Vector3D pos, Vector3D vel, float mass) override
	{
		return(Vector3D{ -(pos.z - center.z), 50 - (pos.y - center.y), (pos.x - center.x) } * k);
	}
private:
	Vector3D center;
	float k;
};

