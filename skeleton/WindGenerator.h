#pragma once
#include "ForceGenerator.h"

class WindGenerator : public ForceGenerator
{
public:
	WindGenerator(Vector3D _windSpeed, float _k1, float _k2 = 0) 
		: windSpeed(_windSpeed), k1(_k1), k2(_k2)
	{
	}

	Vector3D Force(Vector3D pos, Vector3D vel, float mass) override
	{
		return ((windSpeed - vel) * k1 + (windSpeed - vel)* (windSpeed - vel).magnitude() * k2);
	}
private:
	Vector3D windSpeed;
	float k1, k2;
};

