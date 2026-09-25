#pragma once
#include "Scene.h"
#include "RenderUtils.hpp"
#include <deque>
#include <vector>
#include "Particle.h"

class P1S_Scene : public Scene
{
public:
	explicit P1S_Scene(std::string name) : Scene(std::move(name)) {}
	void init() override;
	void cleanup() override;
	void update(double dt) override;
	void keyPress(unsigned char key, const physx::PxTransform& camera) override;

public:
	// Enumeración del orden de los objetos a renderizar, para poder acceder a ellos por índice
	enum OBJECTS {

	};

	enum Colors
	{
		RED,
		DARK_RED,
		GREEN,
		DARK_GREEN,
		BLUE,
		YELLOW,
		BLACK,
		GRAY
	};

private:
	// Función para agregar un RenderItem a la escena, almacenando su transformada y color
	void addRenderItem(physx::PxShape* shape, const Vector3& pos, const Colors& color);
	// Funcion para convertir un color enumerado a un Vector4 RGBA con el fin de mejorar la claridad
	Vector4 Color(Colors color);

	std::deque<physx::PxTransform> m_transforms;
	std::vector<RenderItem*> m_renderItems;

	Particle* particle;
};

