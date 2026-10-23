#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(ParticleModel m, ParticleGeneratorInfo info, int _maxDistance) : maxDistance(_maxDistance), iniPos(m.pos)
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

        bool particleFar = (particles[i]->GetPos().p - iniPos).magnitude() >= maxDistance;

        if (!particles[i]->IsAlive() || particleFar) {
            delete particles[i];
            particles[i] = particles.back();
            particles.pop_back();
        }
        else {
            ++i;
        }
    }
}
