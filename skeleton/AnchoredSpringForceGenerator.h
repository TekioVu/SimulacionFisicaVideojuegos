#pragma once
#include "SpringForceGenerator.h"

class AnchoredSpringForceGenerator : public SpringForceGenerator
{
public:

	AnchoredSpringForceGenerator(double k, double resting, const Vector3& anchor)
		:SpringForceGenerator(k, resting, nullptr)
	{
        ParticleModel pModel;
        pModel.pos = anchor;
        pModel.vel = Vector3D{ 0.0f, 0.0f, 0.0f };
        pModel.acc = Vector3D{ 0.0f, 0.0f, 0.0f };

        other = new Particle(pModel);
	}


};