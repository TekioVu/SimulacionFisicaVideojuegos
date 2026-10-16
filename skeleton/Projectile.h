#pragma once
#include "Particle.h"

static constexpr float MASS_INCREMENT = 0.1f;
static constexpr float VEL_INCREMENT = 3.0f;

class Projectile : public Particle
{
public:
	Projectile(Vector3D _pos, Vector3D _realV, Vector3D _simV, float _realMass, float _realGravity, 
				Vector3D _acc, float _damping, float _radius);

	void keyPress(unsigned char key, const physx::PxTransform& camera);

private:
	Vector3D realVel;

	float realMass;
	float realGravity;

	float simGravity;

	enum adjustableParameters {
		REAL_MASS,
		REAL_VELOCITY
	};

	adjustableParameters selectedParameter;
	void modifyParameter(adjustableParameters parameter, int sign);

	float setSimulatedMass();
	float setSimulatedGravity();

	void updateText(float simG, float rG, float simMass, float rMass);
};

