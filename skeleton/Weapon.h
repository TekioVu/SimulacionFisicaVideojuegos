#pragma once
#include "Projectile.h"

class Weapon
{
public:
	Weapon();
	~Weapon();
	void keyPress(unsigned char key, const physx::PxTransform& camera);
	void updateBullets(double dt);

private:
	void Shoot(const physx::PxTransform& camera);
	std::vector<Projectile*> bullets;
};

