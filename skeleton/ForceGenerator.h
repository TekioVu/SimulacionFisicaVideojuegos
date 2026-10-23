#pragma once
#include "Vector3D.h"

class ForceGenerator
{
public:
	ForceGenerator() = default;
	virtual ~ForceGenerator() = default;

	virtual Vector3D Force(Vector3D pos, Vector3D vel, float mass) = 0;
private:

};

