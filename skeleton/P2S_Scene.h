#pragma once
#include "Scene.h"
#include "RenderUtils.hpp"
#include <deque>
#include <vector>
#include "Weapon.h"

class P2S_Scene : public Scene
{
public:
	explicit P2S_Scene(std::string name) : Scene(std::move(name)) {}
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

	void AddBullseye(float r, int nCircles);

	std::deque<physx::PxTransform> m_transforms;
	std::vector<RenderItem*> m_renderItems;

	Weapon* weapon;
};

