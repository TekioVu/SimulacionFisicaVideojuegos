#pragma once
#include "Particle.h"
#include "ParticleGenerator.h"
#include "ForceGenerator.h"

static float DELETE_DISTANCE = 100.0f;

class ParticleSystem
{

public:
	ParticleSystem(ParticleModel m, ParticleGeneratorInfo info, int distanceEffect = INT_MAX);
	ParticleSystem(ParticleModel m, ParticleGeneratorInfo info, const std::vector<ForceGenerator*>& _forces,
		int distanceEffect = INT_MAX);
	~ParticleSystem();

	void update(double dt);
private:
	float maxDistance;
	Vector3D iniPos;

	std::vector<Particle*> particles;
	ParticleGenerator* generator;

	bool applyForces;
	std::vector<ForceGenerator*> forces;

};

