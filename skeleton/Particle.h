#pragma once
#include "Vector3D.h"
#include "RenderUtils.hpp"
#include "Scene.h"
#include <cfloat>

enum class ShapeType { Sphere, Box, Capsule };

struct ParticleModel {
	Vector3D pos{ 0,0,0 };
	Vector3D vel{ 0,0,0 };
	Vector3D acc{ 0,0,0 };
	Vector3D color{ 0,1,0 };
	float opacity = 1.0f;
	float damping = 0.99f;
	float radius = 1.0f;
	float lifeTime = FLT_MAX;
	ShapeType shape = ShapeType::Sphere;
};

class Particle
{
public:
	Particle(const ParticleModel& m);
	Particle(Vector3D _pos, Vector3D _vel, Vector3D _acc, float _damping, float _radius, 
		Vector4 color, float _lifeTime = FLT_MAX, ShapeType shape = ShapeType::Sphere);
	Particle(Vector3D _pos, Vector3D _vel, Vector3D _acc, float _damping, float _radius, 
		Scene::Colors color = Scene::GREEN, float _lifeTime = FLT_MAX, ShapeType shape = ShapeType::Sphere);
	~Particle();

	void integrateEuler(double t);
	void integrateSemiEuler(double t);
	void integrateVerlet(double t);

	void setAcceleration(Vector3D a);

	void udpateLifeTime(double t);

	bool IsAlive() { return alive; }
	physx::PxTransform GetPos() { return pose; }

protected:
	Vector3D vel;
	Vector3D acc;
	double damping;
	float mass;

	float lifeTime;
	bool alive;

private:
	physx::PxShape* MakeShape(ShapeType type, float size);

	Vector3D prevPos;
	bool firstStep;

	physx::PxTransform pose;
	RenderItem* renderItem;
};

