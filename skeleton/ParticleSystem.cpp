#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(ParticleModel m, ParticleGeneratorInfo info)
{
    generator = new ParticleGenerator(m , info);
}

ParticleSystem::~ParticleSystem()
{
    for (auto p : particles) {
        delete p;
    }
    particles.clear();

    delete generator;
    generator = nullptr;
}

void ParticleSystem::update(double dt)
{
    particles.push_back(generator->generateParticle());

    for (size_t i = 0; i < particles.size();) {
        particles[i]->integrateEuler(dt);
        particles[i]->udpateLifeTime(dt);

        if (!particles[i]->IsAlive()) {
            delete particles[i];
            particles[i] = particles.back();
            particles.pop_back();
        }
        else {
            ++i;
        }
    }
}
