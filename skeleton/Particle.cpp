#include "Particle.h"

Particle::Particle(Vector3D _pos, Vector3D _vel, Vector3D _acc, float _damping, float _radius, Scene::Colors color, float _lifeTime)
	: pose(_pos), vel(_vel), acc(_acc), damping(_damping), lifeTime(_lifeTime), alive(true)
{
	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(_radius));
	renderItem = new RenderItem(shape, &pose, Scene::Color(color));
}

Particle::~Particle()
{
	if (renderItem) {
		renderItem->release();
		renderItem = nullptr;
	}
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