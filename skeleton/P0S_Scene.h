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
	enum OBJECTS{
		X_AXIS,
		Y_AXIS,
		Z_AXIS,
		ENEMY,
		P_1,
		P_2,
		P_3,
		P_4,
		OBJECTS_COUNT
	};

	enum Colors
	{
		RED,
		GREEN,
		BLUE,
		YELLOW,
		BLACK
	};

private:
	void addRenderItem(physx::PxShape* shape, const Vector3& pos, const Colors& color);
	Vector4 Color(Colors color);
	Colors ColorVision(float dot);

	std::deque<physx::PxTransform> m_transforms;
	std::vector<RenderItem*> m_renderItems;
};

