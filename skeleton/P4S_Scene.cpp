#include "P4S_Scene.h"
#include "GravityGenerator.h"
#include "WindGenerator.h"
#include "WhirlwindGenerator.h"
#include "ExplosionGenerator.h"

void P4S_Scene::init()
{
    physx::PxShape* suelo = CreateShape(physx::PxBoxGeometry(50.0f, 0.1f, 150.0f));
    addRenderItem(suelo, Vector3(50.0f, 42.0f, 50.0f), GRAY);

    addFog();
}

void P4S_Scene::cleanup()
{
    for (auto& renderItem : m_renderItems) {
        if (renderItem) {
            renderItem->release();
            renderItem = nullptr;
        }
    }

    for (auto ps : particleSystems) {
        delete ps;
    }
    particleSystems.clear();
}

void P4S_Scene::update(double dt)
{
    for (auto ps : particleSystems) {
        ps->update(dt);
    }
}

void P4S_Scene::keyPress(unsigned char key, const physx::PxTransform& camera) {
    if (key == 'k' || key == 'K') {
        for (auto ps : particleSystems) {
            ExplosionGenerator* eg = new ExplosionGenerator(Vector3D(50.0f, 43.0f, 50.0f), 50.0f, 25000.0f, 0.5f);
            ps->addForceGenerator(eg);
        }
    }
}

void P4S_Scene::addRenderItem(physx::PxShape* shape, const Vector3& pos, const Colors& color)
{
    m_transforms.push_back(physx::PxTransform(physx::PxVec3(pos.x, pos.y, pos.z)));
    m_renderItems.push_back(new RenderItem(shape, &m_transforms.back(), Color(color)));
}

void P4S_Scene::addFog()
{
    // NIEBLA
    ParticleModel fogModel;
    fogModel.pos = Vector3D{ 50.0f, 43.0f, 50.0f };   // centro de la zona de niebla, a ras de suelo
    fogModel.vel = Vector3D{ 0.3f, 0.0f, 0.0f };      // deriva lenta con el "viento" (+X)
    fogModel.acc = Vector3D{ 0.0f, 0.0f, 0.0f };      // sin fuerzas
    fogModel.damping = 0.99f;
    fogModel.radius = 1.5f;                              // partículas grandes que se solapan
    fogModel.lifeTime = 8.0f;                              // vida larga para que se acumulen
    fogModel.color = Vector3D{ 0.8f, 0.82f, 0.85f };    // gris blanquecino
    fogModel.opacity = 0.15f;                             // muy transparente

    ParticleGeneratorInfo fogInfo;

    fogInfo.pos = VectorSpec(UNIFORM);
    fogInfo.pos.variability = { 30.0, 1.0, 30.0 };    // zona ancha y muy baja

    fogInfo.vel = VectorSpec(NORMAL);
    fogInfo.vel.variability = { 0.2, 0.05, 0.2 };     // movimiento casi imperceptible

    fogInfo.acc = VectorSpec(UNIFORM);
    fogInfo.acc.variability = { 0.0, 0.0, 0.0 };

    fogInfo.color = VectorSpec(UNIFORM);
    fogInfo.color.variability = { 0.05, 0.05, 0.05 };   // grises ligeramente distintos

    fogInfo.damping = ScalarSpec(UNIFORM);
    fogInfo.damping.variability = { 0.005 };

    fogInfo.radius = ScalarSpec(UNIFORM);
    fogInfo.radius.variability = { 0.5 };                // 1.0 a 2.0

    fogInfo.lifeTime = ScalarSpec(UNIFORM);
    fogInfo.lifeTime.variability = { 2.0 };               // 6 a 10 s

    fogInfo.oppacity = ScalarSpec(UNIFORM);
    fogInfo.oppacity.variability = { 0.05 };              // 0.10 a 0.20

    std::vector<ForceGenerator*> forces;
    //forces.push_back(new WhirlwindGenerator(Vector3D(50.0f, 43.0f, 50.0f), 3.0f));
    //forces.push_back(new WindGenerator(Vector3D(50.0f, 43.0f, 50.0f), 0.5f, 0.05f));
    //forces.push_back(new GravityGenerator());
    ParticleSystem* hose = new ParticleSystem(fogModel, fogInfo, forces);
    particleSystems.push_back(hose);
}
