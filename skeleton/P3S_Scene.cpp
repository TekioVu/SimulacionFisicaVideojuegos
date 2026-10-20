#include "P3S_Scene.h"

void P3S_Scene::init()
{
    physx::PxShape* suelo = CreateShape(physx::PxBoxGeometry(50.0f, 0.1f, 150.0f));
    addRenderItem(suelo, Vector3(50.0f, 42.0f, 50.0f), GRAY);
    
    addHose();
    addFog();
    addFire();
}

void P3S_Scene::cleanup()
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

void P3S_Scene::update(double dt)
{
    for (auto ps : particleSystems) {
        ps->update(dt);
    }
}

void P3S_Scene::keyPress(unsigned char key, const physx::PxTransform& camera) {
}

void P3S_Scene::addRenderItem(physx::PxShape* shape, const Vector3& pos, const Colors& color)
{
    m_transforms.push_back(physx::PxTransform(physx::PxVec3(pos.x, pos.y, pos.z)));
    m_renderItems.push_back(new RenderItem(shape, &m_transforms.back(), Color(color)));
}

void P3S_Scene::addHose()
{
    //MANGUERA
    ParticleModel pm;
    pm.pos = Vector3D{ 40.0f, 50.0f, 40.0f };   // boquilla de la manguera
    pm.vel = Vector3D{ 0.0f, 6.0f, 15.0f };     // chorro hacia delante (+Z) y algo hacia arriba (+Y)
    pm.acc = Vector3D{ 0.0f, -9.8f, 0.0f };     // gravedad
    pm.damping = 0.99f;                             // casi sin pérdida de velocidad
    pm.radius = 0.1f;                              // tamaño de cada gota
    pm.lifeTime = 5.0f;
    pm.color = Vector3D{ 0.3f, 0.6f, 1.0f };    // azul claro de agua
    pm.oppacity = 0.9f;                            // semitransparente

    ParticleGeneratorInfo pGI;

    pGI.pos = VectorSpec(UNIFORM);
    pGI.pos.variability = { 0.05, 0.05, 0.05 };    // boquilla de ~10 cm

    pGI.vel = VectorSpec(NORMAL);
    pGI.vel.variability = { 0.5, 0.5, 0.5 };       // abanico estrecho de chorro

    pGI.acc = VectorSpec(NORMAL);
    pGI.acc.variability = { 0.0, 0.0, 0.0 };

    pGI.color = VectorSpec(UNIFORM);
    pGI.color.variability = { 0.1, 0.1, 0.0 };   // R y G varían un poco, B siempre al máximo

    pGI.damping = ScalarSpec(UNIFORM);
    pGI.damping.variability = { 0.005 };           // 0.99 ± 0.005

    pGI.radius = ScalarSpec(UNIFORM);
    pGI.radius.variability = { 0.03 };             // gotas de tamaños parecidos

    pGI.lifeTime = ScalarSpec(UNIFORM);
    pGI.lifeTime.variability = { 0.5 };            // ±0.5 s

    pGI.oppacity = ScalarSpec(UNIFORM);
    pGI.oppacity.variability = { 0.15 };              // 0.6 ± 0.15

    ParticleSystem* hose = new ParticleSystem(pm, pGI);
    particleSystems.push_back(hose);
}

void P3S_Scene::addFog()
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
    fogModel.oppacity = 0.15f;                             // muy transparente

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

    particleSystems.push_back(new ParticleSystem(fogModel, fogInfo));
}

void P3S_Scene::addFire()
{
    // FUEGO
    ParticleModel fireModel;
    fireModel.pos = Vector3D{ 70.0f, 42.5f, 60.0f };  // base de la hoguera
    fireModel.vel = Vector3D{ 0.0f, 4.0f, 0.0f };     // sube
    fireModel.acc = Vector3D{ 0.0f, 3.0f, 0.0f };     // empuje por el calor (acc positiva en Y)
    fireModel.damping = 0.97f;                            // se frena algo, como las llamas
    fireModel.radius = 0.2f;
    fireModel.lifeTime = 1.5f;                             // vida corta
    fireModel.color = Vector3D{ 1.0f, 0.45f, 0.1f };    // naranja
    fireModel.oppacity = 0.8f;

    ParticleGeneratorInfo fireInfo;

    fireInfo.pos = VectorSpec(UNIFORM);
    fireInfo.pos.variability = { 0.5, 0.0, 0.5 };      // base de ~1 m

    fireInfo.vel = VectorSpec(NORMAL);
    fireInfo.vel.variability = { 0.6, 1.0, 0.6 };      // llama que se abre un poco

    fireInfo.acc = VectorSpec(NORMAL);
    fireInfo.acc.variability = { 0.5, 0.5, 0.5 };      // vaivén

    fireInfo.color = VectorSpec(UNIFORM);
    fireInfo.color.variability = { 0.0, 0.25, 0.05 };    // de rojo (G=0.2) a amarillo (G=0.7)

    fireInfo.damping = ScalarSpec(UNIFORM);
    fireInfo.damping.variability = { 0.01 };

    fireInfo.radius = ScalarSpec(UNIFORM);
    fireInfo.radius.variability = { 0.08 };

    fireInfo.lifeTime = ScalarSpec(UNIFORM);
    fireInfo.lifeTime.variability = { 0.5 };               // 1 a 2 s

    fireInfo.oppacity = ScalarSpec(UNIFORM);
    fireInfo.oppacity.variability = { 0.15 };              // 0.65 a 0.95

    particleSystems.push_back(new ParticleSystem(fireModel, fireInfo));
}


