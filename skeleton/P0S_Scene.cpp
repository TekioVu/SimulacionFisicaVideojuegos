#include "P0S_Scene.h"

void P0S_Scene::init()
{
    Vector3 u(3.0f, 1.0f, 0.0f);
    Vector3 v(0.0f, 4.0f, 0.0f);
	Vector3 w = u.cross(v);

	u.normalize();
	v.normalize();
	w.normalize();  

	u *= 5.0f;
	v *= 5.0f;
	w *= 5.0f;

    physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(0.5f));

    //AXIS
	addRenderItem(shape, u, RED);
    addRenderItem(shape, v, GREEN);
    addRenderItem(shape, w, BLUE);

    //ENEMY
    addRenderItem(shape, Vector3(0.0f, 0.0f, 0.0f), BLACK);
	Vector3 enemyVision = Vector3(0.0f, 0.0f, 1.0f);

    //OBJECTIVES
    Vector3 objectivePositions[] = {
       Vector3(2.0f, 0.0f, 3.0f),
       Vector3(-4.0f, 0.0f, 1.0f),
       Vector3(0.0f, 0.0f, -5.0f),
       Vector3(3.0f, 0.0f, 0.0f),
    };
    
    for (const auto& pos : objectivePositions) {
        addRenderItem(shape, pos, ColorVision(enemyVision.dot(pos)));
    }

    //LERP
    Vector3 A(-8.0f, 1.0f, -8.0f);
    Vector3 B(8.0f, 8.0f, 8.0f);

    addRenderItem(shape, A, DARK_RED);
    addRenderItem(shape, B, DARK_RED);

	for (float i = 0; i < 10; ++i) {
        Vector3 pos = A + (i/10) * (B - A);
		addRenderItem(shape, pos, GRAY);
	}
}

void P0S_Scene::cleanup()
{
    for (auto& renderItem : m_renderItems) {
        if (renderItem) {
            renderItem->release();
            renderItem = nullptr;
        }
    }
}

void P0S_Scene::update(double dt)
{

}

void P0S_Scene::keyPress(unsigned char key, const physx::PxTransform& camera){
    if (key == 'r' || key == 'R') {
    }
}

void P0S_Scene::addRenderItem(physx::PxShape* shape, const Vector3& pos, const Colors& color)
{
    m_transforms.push_back(physx::PxTransform(physx::PxVec3(pos.x, pos.y, pos.z)));
    m_renderItems.push_back(new RenderItem(shape, &m_transforms.back(), Color(color)));
}

Vector4 P0S_Scene::Color(Colors color)
{
    switch (color) {
        case RED:
            return Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		case DARK_RED:
			return Vector4(0.5f, 0.0f, 0.0f, 1.0f);
        case GREEN:
            return Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		case DARK_GREEN:
			return Vector4(0.0f, 0.5f, 0.0f, 1.0f);
        case BLUE:
            return Vector4(0.0f, 0.0f, 1.0f, 1.0f);
        case YELLOW:
            return Vector4(1.0f, 1.0f, 0.0f, 1.0f);
        case BLACK:
            return Vector4(0.0f, 0.0f, 0.0f, 1.0f);
        case GRAY:
            return Vector4(0.5f, 0.5f, 0.5f, 1.0f);
        default:
            return Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    }
} 

P0S_Scene::Colors P0S_Scene::ColorVision(float dot)
{
    if (dot > 0) {
		return DARK_GREEN;
	}
	else if (dot < 0) {
		return DARK_RED;
	}
	else {
		return YELLOW;
	}
}