#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(ParticleModel m, ParticleGeneratorInfo info, int _maxDistance) 
    : maxDistance(_maxDistance), iniPos(m.pos), applyForces(false)
{
    generators.push_back(new ParticleGenerator(m , info));
}

ParticleSystem::ParticleSystem(ParticleModel m, ParticleGeneratorInfo info, const std::vector<ForceGenerator*>& _forces,
    int _maxDistance) : maxDistance(_maxDistance), iniPos(m.pos), applyForces(true), forces(_forces)
{
    generators.push_back(new ParticleGenerator(m, info));
}

ParticleSystem::~ParticleSystem()
{
    for (auto p : particles) {
        delete p;
    }
    particles.clear();

    for (auto g : generators) {
        delete g;
    }
    generators.clear();
}

void ParticleSystem::update(double dt)
{
    for (auto f : forces)
        f->update(dt);

    for (auto g : generators) {
        if(g->isActive())
            particles.push_back(g->generateParticle());
    }

    for (size_t i = 0; i < particles.size();) {

        //Apply forces
        if (applyForces) {
            Vector3D force;

            const Vector3D pos = particles[i]->GetPos().p;
            const Vector3D vel = particles[i]->GetVel();
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

void ParticleSystem::addForceGenerator(ForceGenerator* fg)
{
    forces.push_back(fg);
}

void ParticleSystem::addParticleGenerator(ParticleGenerator* pg)
{
    generators.push_back(pg);
}

