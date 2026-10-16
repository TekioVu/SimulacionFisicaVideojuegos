#pragma once
#include "Particle.h"
#include "ProbabilityDistributions.h"
#include <array>   

template <size_t N>
struct Spec
{
    Distribution dist;                   
    std::array<double, N> variability;  

    Spec(Distribution d = Distribution()) : dist(d)
    {
        variability.fill(0.0f);
    }
};

using ScalarSpec = Spec<1>;
using VectorSpec = Spec<3>;

struct ParticleGeneratorInfo
{
    VectorSpec pos, vel, acc;
    ScalarSpec damping, radius, lifeTime;
};

class ParticleGenerator
{
	
public:
	ParticleGenerator(ParticleModel _model, ParticleGeneratorInfo _info);
	~ParticleGenerator();

	void update(double dt);
	Particle* generateParticle();

private:
	ParticleModel model;
	ParticleGeneratorInfo info;

	float sample(float iniV, ScalarSpec _spec) const;
	Vector3D sample(Vector3D iniV, VectorSpec _spec) const;
	float sample(float iniV, double _var, Distribution _distribution) const;

};


