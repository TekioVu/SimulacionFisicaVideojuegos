#include "../Particle/Particle.h"

// Particle.cpp
Particle::Particle(const ParticleModel& m)
    : pose(m.pos), vel(m.vel), acc(m.acc), damping(m.damping),
    lifeTime(m.lifeTime), mass(m.mass), alive(true)
{
    physx::PxShape* shape = MakeShape(m.shape, m.radius);
    Vector4 c(m.color.x, m.color.y, m.color.z, m.opacity);
    renderItem = new RenderItem(shape, &pose, c);
}

Particle::Particle(Vector3D _pos, Vector3D _vel, Vector3D _acc, float _damping, float _radius,
    Vector4 color, float _lifeTime, ShapeType _shape, float _mass)
    : Particle(ParticleModel{
          .pos = _pos, .vel = _vel, .acc = _acc,
          .color = Vector3D(color.x, color.y, color.z),
          .opacity = color.w,
          .damping = _damping, .radius = _radius,
          .lifeTime = _lifeTime, .shape = _shape,
          .mass = _mass })
{
}

Particle::Particle(Vector3D _pos, Vector3D _vel, Vector3D _acc, float _damping, float _radius,
    Scene::Colors color, float _lifeTime, ShapeType shape, float _mass)
    : Particle(_pos, _vel, _acc, _damping, _radius, Scene::Color(color), _lifeTime, shape, _mass)
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
void Particle::applyForce(Vector3D force)
{
    forceAccum = forceAccum + force;       // acumula en vez de sobrescribir
}

void Particle::integrateEuler(double t)
{
    Vector3D a = acc + forceAccum / mass;  // aceleración constante + fuerzas
    pose = physx::PxTransform(pose.p + vel * t);
    vel = vel * pow(damping, t) + a * t;
    forceAccum = Vector3D{ 0.0f, 0.0f, 0.0f };
}

void Particle::integrateSemiEuler(double t)
{
    Vector3D a = acc + forceAccum / mass;
    vel = vel * pow(damping, t) + a * t;
    pose = physx::PxTransform(pose.p + vel * t);
    forceAccum = Vector3D{ 0.0f, 0.0f, 0.0f };
}

void Particle::integrateVerlet(double t)
{
    Vector3D a = acc + forceAccum / mass;
    Vector3D currentPos = pose.p;

    if (firstStep)
    {
        prevPos = currentPos - vel * t;
        firstStep = false;
    }

    Vector3D nextPos = currentPos * 2.0 - prevPos + a * (t * t);

    vel = (nextPos - prevPos) / (2.0 * t);

    prevPos = currentPos;
    pose = physx::PxTransform(nextPos, pose.q);
    forceAccum = Vector3D{ 0.0f, 0.0f, 0.0f };
}