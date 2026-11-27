#include "P1S_Scene.h"

void P1S_Scene::init()
{
    physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(0.5f));

    //ENEMY
    addRenderItem(shape, Vector3(0.0f, 0.0f, 0.0f), BLACK);
    Vector3 v = Vector3(5.0f, 0.0f, 0.0f);
    Vector3 a = Vector3(3.0f, 0.0f, 0.0f);
	float damping = 0.8f;
    float radius = 1.0f;

    float mass1 = 0.1f;
	particle = new Particle(Vector3(0.0f, 0.0f, 0.0f), v, a, damping, radius, GREEN,100.0f,ShapeType::Sphere, mass1);

    float mass2 = 20.0f;
    particle2 = new Particle(Vector3(0.0f, 0.0f, 5.0f), v, a, damping, radius, RED, 100.0f, ShapeType::Sphere, mass2);
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

    delete particle2;
    particle2 = nullptr;
}

void P1S_Scene::update(double dt)
{
    particle->integrateEuler(dt);
    particle2->integrateEuler(dt);
}

void P1S_Scene::keyPress(unsigned char key, const physx::PxTransform& camera) {
}

void P1S_Scene::addRenderItem(physx::PxShape* shape, const Vector3& pos, const Colors& color)
{
    m_transforms.push_back(physx::PxTransform(physx::PxVec3(pos.x, pos.y, pos.z)));
    m_renderItems.push_back(new RenderItem(shape, &m_transforms.back(), Color(color)));
}

