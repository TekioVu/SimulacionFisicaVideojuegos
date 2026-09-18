#include "P0S_Scene.h"

void P0S_Scene::init()
{
    physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(10.0f));
    m_transform = physx::PxTransform(physx::PxVec3(0.0f, 10.0f, 0.0f));
    m_renderItem = new RenderItem(shape, &m_transform, Vector4(0.25f, 0.88f, 0.82f, 1.0f));
}

void P0S_Scene::cleanup()
{
    if (m_renderItem) {
        m_renderItem->release();
        m_renderItem = nullptr;
    }
}

void P0S_Scene::update(double dt)
{

}

void P0S_Scene::keyPress(unsigned char key, const physx::PxTransform& camera){
    if (key == 'r' || key == 'R') {
        m_transform.p = physx::PxVec3(0.0f, 10.0f, 0.0f); // Reset
    }
}