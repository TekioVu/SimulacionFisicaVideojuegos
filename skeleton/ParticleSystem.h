#pragma once
#include "Particle.h"
#include "ParticleGenerator.h"

static float DELETE_DISTANCE = 100.0f;

class ParticleSystem
{

public:
	ParticleSystem(ParticleModel m, ParticleGeneratorInfo info, int distanceEffect = INT_MAX);
	~ParticleSystem();

	void update(double dt);
private:
	float maxDistance;
	Vector3D iniPos;

	std::vector<Particle*> particles;
	ParticleGenerator* generator;
};

