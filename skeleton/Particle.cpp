#include "Particle.h"

// Particle.cpp
Particle::Particle(Vector3D _pos, Vector3D _vel, Vector3D _acc, float _damping, float _radius,
    Vector4 color, float _lifeTime, ShapeType _shape)
    : pose(_pos), vel(_vel), acc(_acc), damping(_damping), lifeTime(_lifeTime), alive(true)
{
    physx::PxShape* shape = MakeShape(_shape, _radius);
    renderItem = new RenderItem(shape, &pose, color);
}

// El del enum solo convierte y delega
Particle::Particle(Vector3D _pos, Vector3D _vel, Vector3D _acc, float _damping, float _radius,
    Scene::Colors color, float _lifeTime, ShapeType shape)
    : Particle(_pos, _vel, _acc, _damping, _radius, Scene::Color(color), _lifeTime, shape)
{
}


Particle::~Particle()
{
	if (renderItem) {
		renderItem->release();
		renderItem = nullptr;
	}
}
physx::PxShape* Particle::MakeShape(ShapeType type, float size)
{
    switch (type) {
    case ShapeType::Sphere:
        return CreateShape(physx::PxSphereGeometry(size));
    case ShapeType::Box:
        return CreateShape(physx::PxBoxGeometry(size, size, size));
    case ShapeType::Capsule:
        return CreateShape(physx::PxCapsuleGeometry(size, size));
    }
    return nullptr;
}


void Particle::setAcceleration(Vector3D a)
{
	acc = a;
}

void Particle::udpateLifeTime(double dt)
{
	lifeTime -= dt;
	if (lifeTime <= 0) {
		alive = false;
	}
}

void Particle::integrateEuler(double t)
{
	pose = physx::PxTransform(pose.p + vel * t);
	vel = vel * pow(damping, t) + acc * t;
}

void Particle::integrateSemiEuler(double t)
{
	vel = vel * pow(damping, t) + acc * t;
	pose = physx::PxTransform(pose.p + vel * t);
}

void Particle::integrateVerlet(double t)
{
    Vector3D currentPos = pose.p;

    if (firstStep)
    {
        prevPos = currentPos - vel * t;
        firstStep = false;
    }

    Vector3D nextPos = currentPos * 2.0 - prevPos + acc * (t * t);

    vel = (nextPos - prevPos) / (2.0 * t);

    prevPos = currentPos;
    pose = physx::PxTransform(nextPos, pose.q);
}