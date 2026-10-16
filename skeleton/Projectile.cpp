#include "Projectile.h"
#include <sstream>
#include <iomanip>

extern std::string display_text;

Projectile::Projectile(Vector3D _pos, Vector3D _realV, Vector3D _simV, float _realMass, float _realGravity, Vector3D _acc, float _damping, float _radius)
	: Particle(_pos, _simV, _acc, _damping, _radius), realVel(_realV), realMass(_realMass), realGravity(_realGravity) {

	updateText(setSimulatedGravity(), realVel.magnitude(), setSimulatedMass(), realMass);
}

float Projectile::setSimulatedMass() {
	mass = realMass * pow(realVel.magnitude(), 2) / pow(vel.magnitude(), 2);

	return mass;
}

float Projectile::setSimulatedGravity() {
	simGravity = pow(vel.magnitude(), 2) * realGravity / pow(realVel.magnitude(), 2);
	setAcceleration(Vector3D(0, simGravity, 0));

	return simGravity;
}

void Projectile::keyPress(unsigned char key, const physx::PxTransform& camera) {
	if (key == 'v' || key == 'V') {
		selectedParameter = REAL_VELOCITY;
	}
	else if (key == 'm' || key == 'M') {
		selectedParameter = REAL_MASS;
	}
	else if (key == 'o' || key == 'O') {
		modifyParameter(selectedParameter, -1);
	}
	else if (key == 'p' || key == 'P') {
		modifyParameter(selectedParameter, 1);
	}
}

void Projectile::modifyParameter(adjustableParameters parameter, int sign) {
	switch (parameter) {
	case REAL_VELOCITY:
		realVel += Vector3D(0, sign * VEL_INCREMENT, 0);
		if (realVel.magnitude() < 1e-3f)          // no permitir 0
			realVel += Vector3D(0, sign * VEL_INCREMENT, 0);
		break;

	case REAL_MASS:
		realMass += sign * MASS_INCREMENT;
		if (fabs(realMass) < 1e-3f)               // no permitir 0
			realMass += sign * MASS_INCREMENT;
		break;
	}

	updateText(setSimulatedGravity(), realVel.y, setSimulatedMass(), realMass);
}

void Projectile::updateText(float simG, float realV, float simMass, float rMass) 
{
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(2)
		<< "SimG: " << setSimulatedGravity()
		<< " | RealV:" << realV
		<< " | SimM: " << setSimulatedMass()
		<< " | RealM: " << rMass;

	display_text = ss.str();
}

