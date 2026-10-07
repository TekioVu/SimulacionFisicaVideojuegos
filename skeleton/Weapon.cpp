#include "Weapon.h"

Weapon::Weapon()
{
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

    for (auto b : bullets) {
        b->keyPress(key, camera);
    }
}

void Weapon::Shoot(const physx::PxTransform& camera)
{
    const Vector3D pos = camera.p;
    const Vector3D direccion = camera.q.rotate(Vector3D(0.0f, 0.0f, -1.0f));
    const Vector3D realV(direccion * 25.0f);
    const Vector3D simV(direccion * 10.0f);

    const float realMass = 0.1f;
    const float realGravity = -9.8f;
    const Vector3D a = Vector3D(0.0f, 0.0f, 0.0f);
    const float damping = 0.8f;
    float radius = 0.4f;

    Projectile* p = new Projectile(pos, realV, simV, realMass, realGravity, a, damping, radius);
    bullets.push_back(p);
}

void Weapon::updateBullets(double dt)
{
    for (auto b : bullets) {
        b->integrateEuler(dt);
    }
}