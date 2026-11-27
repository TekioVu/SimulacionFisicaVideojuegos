#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(ParticleModel _model, ParticleGeneratorInfo _info): info(_info), model(_model)
{
}

ParticleGenerator::~ParticleGenerator()
{
}

void ParticleGenerator::update(double dt)
{

}

Particle* ParticleGenerator::generateParticle()
{
	Vector3D varPos =  sample(model.pos, info.pos);
	Vector3D varVel =  sample(model.vel, info.vel);
	Vector3D varAcc = sample(model.acc, info.acc);
	Vector3D varColor =  sample(model.color, info.color);
	float varDamping = sample(model.damping, info.damping);
	float varRadius = sample(model.radius, info.radius);
	float varOppacity =  sample(model.opacity, info.oppacity);
	Vector4 color = {varColor, varOppacity};

	Particle* p = new Particle(varPos, varVel, varAcc, varDamping, varRadius, color, model.lifeTime, model.shape);

	return p;
}

Vector3D ParticleGenerator::sample(Vector3D _iniV, VectorSpec _spec) const
{
	Vector3D v;

	v.x = sample(_iniV.x, _spec.variability[0], _spec.dist);
	v.y = sample(_iniV.y, _spec.variability[1], _spec.dist);
	v.z = sample(_iniV.z, _spec.variability[2], _spec.dist);

	return v;
}

float ParticleGenerator::sample(float iniV, ScalarSpec _spec) const
{
	return sample(iniV, _spec.variability.front(), _spec.dist);
}

float ParticleGenerator::sample(float iniV, double _var, Distribution _distribution) const
{
	switch (_distribution)
	{
	case UNIFORM:
		return UniformDistribution(iniV - _var, iniV + _var);
		break;

	case NORMAL:
		return NormalDistribution(iniV, _var);
		break;
	}
}