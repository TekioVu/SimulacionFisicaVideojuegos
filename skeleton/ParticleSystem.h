#pragma once
#include "Particle.h"
#include "ParticleGenerator.h"

static float DELETE_DISTANCE = 100.0f;

class ParticleSystem
{

public:
	ParticleSystem(ParticleModel m, ParticleGeneratorInfo info);
	~ParticleSystem();

	void update(double dt);
private:
	std::vector<Particle*> particles;
	ParticleGenerator* generator;
};

