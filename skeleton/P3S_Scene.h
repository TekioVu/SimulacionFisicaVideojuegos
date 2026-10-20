#pragma once
#include "Scene.h"
#include "ParticleSystem.h"
#include "RenderUtils.hpp"
#include <deque>
#include <vector>

class P3S_Scene : public Scene
{
public:
	explicit P3S_Scene(std::string name) : Scene(std::move(name)) {}
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
	void addHose();
	void addFog();
	void addFire();

	std::deque<physx::PxTransform> m_transforms;
	std::vector<RenderItem*> m_renderItems;

	std::vector<ParticleSystem*> particleSystems;
};

