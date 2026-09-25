#include "P1S_Scene.h"

void P1S_Scene::init()
{
    physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(0.5f));

    //ENEMY
    addRenderItem(shape, Vector3(0.0f, 0.0f, 0.0f), BLACK);
    Vector3 v = Vector3(1.0f, 0.0f, 1.0f);
    Vector3 a = Vector3(3.0f, 0.0f, 0.0f);
	float damping = 0.8f;

	particle = new Particle(Vector3(0.0f, 0.0f, 0.0f), v, a, damping);
}

void P1S_Scene::cleanup()
{
    for (auto& renderItem : m_renderItems) {
        if (renderItem) {
            renderItem->release();
            renderItem = nullptr;
        }
    }

    delete particle;
    particle = nullptr;
}

void P1S_Scene::update(double dt)
{
	particle->integrateEuler(dt);
}

void P1S_Scene::keyPress(unsigned char key, const physx::PxTransform& camera) {
    if (key == 'r' || key == 'R') {
    }
}

void P1S_Scene::addRenderItem(physx::PxShape* shape, const Vector3& pos, const Colors& color)
{
    m_transforms.push_back(physx::PxTransform(physx::PxVec3(pos.x, pos.y, pos.z)));
    m_renderItems.push_back(new RenderItem(shape, &m_transforms.back(), Color(color)));
}

Vector4 P1S_Scene::Color(Colors color)
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