#include "Weapon.h"

Weapon::Weapon()
{
    //Default projectile
    ProjectileInfo p = { 25.0f,10.0f, 0.1f,-9.8f, 0.4f };
    bulletTypes.push_back(p);
}

Weapon::~Weapon()
{
    for (auto b : bullets){
        delete b;
    }
    bullets.clear();
}

void Weapon::keyPress(unsigned char key, const physx::PxTransform& camera) {
	if (key == 't' || key == 'T') {
        Shoot(camera);
    }
    else if (key == 'y' || key == 'Y') {
        currentBulletType++;
        if (currentBulletType >= bulletTypes.size()) currentBulletType = 0;
    }

    for (auto b : bullets) {
        b->keyPress(key, camera);
    }
}

void Weapon::addProjectile(ProjectileInfo p_info)
{
    bulletTypes.push_back(p_info);
}


void Weapon::Shoot(const physx::PxTransform& camera)
{
    ProjectileInfo currentP = bulletTypes[currentBulletType];

    const Vector3D pos = camera.p;
    const Vector3D direccion = camera.q.rotate(Vector3D(0.0f, 0.0f, -1.0f));
    const Vector3D realV(direccion * currentP.realV);
    const Vector3D simV(direccion * currentP.simV);

    const Vector3D a = Vector3D(0.0f, 0.0f, 0.0f);
    const float damping = 0.99f;

    Projectile* p = new Projectile(pos, realV, simV, currentP.realMass, currentP.realGravity, a, damping, currentP.radius);
    bullets.push_back(p);
}

void Weapon::updateBullets(double dt)
{
    for (auto b : bullets) {
        b->integrateEuler(dt);
    }
}