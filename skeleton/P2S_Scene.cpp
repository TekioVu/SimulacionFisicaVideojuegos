#include "P2S_Scene.h"

void P2S_Scene::init()
{
    physx::PxShape* diana = CreateShape(physx::PxSphereGeometry(5.0f));
    addRenderItem(diana, Vector3(50.0f, 50.0f, -50.0f), BLACK);

    physx::PxShape* dianaR = CreateShape(physx::PxSphereGeometry(2.0f));
    addRenderItem(dianaR, Vector3(50.0f, 50.0f, -45.0f), RED);

    physx::PxShape* suelo = CreateShape(
        physx::PxBoxGeometry(50.0f, 0.1f, 150.0f)
    );

    addRenderItem(suelo,Vector3(50.0f, 42.0f, 50.0f),GRAY);

    weapon = new Weapon();
}

void P2S_Scene::cleanup()
{
    for (auto& renderItem : m_renderItems) {
        if (renderItem) {
            renderItem->release();
            renderItem = nullptr;
        }
    }

    delete weapon;
    weapon = nullptr;
}

void P2S_Scene::update(double dt)
{
    weapon->updateBullets(dt);
}

void P2S_Scene::keyPress(unsigned char key, const physx::PxTransform& camera) {
    weapon->keyPress(key, camera);
}

void P2S_Scene::addRenderItem(physx::PxShape* shape, const Vector3& pos, const Colors& color)
{
    m_transforms.push_back(physx::PxTransform(physx::PxVec3(pos.x, pos.y, pos.z)));
    m_renderItems.push_back(new RenderItem(shape, &m_transforms.back(), Color(color)));
}

Vector4 P2S_Scene::Color(Colors color)
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