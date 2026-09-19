#pragma once
#include "Scene.h"
#include "RenderUtils.hpp"
#include <deque>
#include <vector>

class P0S_Scene : public Scene
{
public:
	explicit P0S_Scene(std::string name) : Scene(std::move(name)) {}
	void init() override;
	void cleanup() override;
    void update(double dt) override;
	void keyPress(unsigned char key, const physx::PxTransform& camera) override;

public:
	// Enumeración del orden de los objetos a renderizar, para poder acceder a ellos por índice
	enum OBJECTS{
		X_AXIS,
		Y_AXIS,
		Z_AXIS,
		ENEMY,
		P_1,
		P_2,
		P_3,
		P_4,
		A,
		B,
		I_0,
		I_1,
		I_2,
		I_3,
		I_4,
		I_5,
		I_6,
		I_7,
		I_8,
		I_9,
		OBJECTS_COUNT
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
	// Función para determinar el color de un objetivo basado en la visión del enemigo
	Colors ColorVision(float dot);

	std::deque<physx::PxTransform> m_transforms;
	std::vector<RenderItem*> m_renderItems;
};

