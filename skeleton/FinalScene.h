#pragma once
#include "Scene.h"
#include "ParticleSystem.h"
#include "Particle.h"
#include "PacMan.h"
#include "RenderUtils.hpp"
#include <deque>
#include <vector>

const int BLOCK_SIZE = 5;
const float PILL_RADIUS = BLOCK_SIZE / 5.0f;
const float POWER_RADIUS = PILL_RADIUS * 2.5f;

class FinalScene : public Scene
{
public:
	explicit FinalScene(std::string name) : Scene(std::move(name)) {}
	void init() override;
	void cleanup() override;
	void update(double dt) override;
	void keyPress(unsigned char key, const physx::PxTransform& camera) override;

public:
	// Enumeración del orden de los objetos a renderizar, para poder acceder a ellos por índice
	enum OBJECTS {

	};

private:
	// Función para agregar un RenderItem a la escena, almacenando su transformada y color
	void addRenderItem(physx::PxShape* shape, const Vector3& pos, const Colors& color);
	void ReadMap();

	std::deque<physx::PxTransform> m_transforms;
	std::vector<RenderItem*> m_renderItems;

	PacMan* pacman;
	std::vector<Particle*> pills;
	std::vector<ParticleSystem*> particleSystems;
};

