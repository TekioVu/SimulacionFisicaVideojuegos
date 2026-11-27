#include "P2S_Scene.h"

void P2S_Scene::init()
{
    AddBullseye(10.0f, 6);

    physx::PxShape* suelo = CreateShape(physx::PxBoxGeometry(50.0f, 0.1f, 150.0f));
    addRenderItem(suelo,Vector3(50.0f, 42.0f, 50.0f), GRAY);

    weapon = new Weapon();

    //                            realV     simV   realMass  realGravity  radius
    Weapon::ProjectileInfo p1 = { 350.0f,  40.0f,   0.008f,    -9.8f,    0.15f }; // Pistola 9 mm
    Weapon::ProjectileInfo p2 = { 900.0f,  70.0f,   0.004f,    -9.8f,    0.10f }; // Fusil de asalto 5.56 mm
    Weapon::ProjectileInfo p3 = { 400.0f,  30.0f,   6.0f,      -9.8f,    0.50f }; // Cañón (bola de hierro)
    Weapon::ProjectileInfo p4 = { 800.0f,  50.0f,  15.0f,      -9.8f,    0.80f }; // Obús de artillería

    weapon->addProjectile(p1);
    weapon->addProjectile(p2);
    weapon->addProjectile(p3);
    weapon->addProjectile(p4);
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

void P2S_Scene::AddBullseye(float r, int nCircles)
{
    for (int i = 0; i < nCircles; i++)
    {
        float radius = r * (i + 1) / nCircles;
        physx::PxShape* diana = CreateShape(physx::PxBoxGeometry(radius, radius, 0.01f));
        Colors c = (i % 2 == 0) ? RED : BLACK;
        addRenderItem(diana, Vector3(50.0f, 55.0f, -50.0f - i*0.1), c);
    }
}
