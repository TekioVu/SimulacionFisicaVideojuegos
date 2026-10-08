#pragma once
#include "Projectile.h"
#include "Vector3D.h"

class Weapon
{
public:
	struct ProjectileInfo
	{
		float realV;
		float simV;

		float realMass;
		float realGravity;

		float radius;
	};

	Weapon();
	~Weapon();
	void keyPress(unsigned char key, const physx::PxTransform& camera);
	void updateBullets(double dt);
	void addProjectile(ProjectileInfo p_info);

private:
	void Shoot(const physx::PxTransform& camera);
	std::vector<Projectile*> bullets;
	std::vector<ProjectileInfo> bulletTypes;
	int currentBulletType = 0;
};

