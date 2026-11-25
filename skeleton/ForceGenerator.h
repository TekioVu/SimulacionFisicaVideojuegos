#pragma once
#include "Vector3D.h"
#include "Particle.h"

class ForceGenerator
{
public:
	ForceGenerator() = default;
	virtual ~ForceGenerator() = default;

	virtual void update(double dt) {}
	virtual Vector3D Force(Vector3D pos, Vector3D vel, float mass) = 0;

	void update() {
		for (auto p : particles) {
			p->applyForce(Force(p->GetPos().p, p->GetVel(), p->GetMass()));
		}
	}

	void AddParticle(Particle* p) {
		particles.push_back(p);
	}
protected:
	std::vector<Particle*> particles;
};

