#pragma once
#include "Vector3D.h"
#include "RenderUtils.hpp"

class Particle
{
public:
	Particle(Vector3D _pos, Vector3D _vel, Vector3D _acc, float _damping);
	~Particle();

	void integrateEuler(double t);
	void integrateSemiEuler(double t);
	void integrateVerlet(double t);

	void setAcceleration(Vector3D a);

private:
	Vector3D vel;
	Vector3D acc;
	double damping;

	Vector3D prevPos;
	bool firstStep;

	physx::PxTransform pose;
	RenderItem* renderItem;
};

