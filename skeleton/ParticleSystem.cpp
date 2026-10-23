#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(ParticleModel m, ParticleGeneratorInfo info, int _maxDistance) 
    : maxDistance(_maxDistance), iniPos(m.pos), applyForces(false)
{
    generator = new ParticleGenerator(m , info);
}

ParticleSystem::ParticleSystem(ParticleModel m, ParticleGeneratorInfo info, const std::vector<ForceGenerator*>& _forces,
    int _maxDistance) : maxDistance(_maxDistance), iniPos(m.pos), applyForces(true), forces(_forces)
{
    generator = new ParticleGenerator(m, info);
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

        //Apply forces
        if (applyForces) {
            Vector3D force;

            Vector3D pos = particles[i]->GetPos().p;
            Vector3D vel = particles[i]->GetVel();
            float mass = particles[i]->GetMass();

            for (auto f : forces) {
                force += f->Force(pos, vel, mass);
            }

            particles[i]->applyForce(force);
        }

        //Update particles
        particles[i]->integrateEuler(dt);
        particles[i]->udpateLifeTime(dt);

        //Delete particles
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
