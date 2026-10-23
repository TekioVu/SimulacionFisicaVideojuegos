#include "P4S_Scene.h"
#include "GravityGenerator.h"

void P4S_Scene::init()
{
    physx::PxShape* suelo = CreateShape(physx::PxBoxGeometry(50.0f, 0.1f, 150.0f));
    addRenderItem(suelo, Vector3(50.0f, 42.0f, 50.0f), GRAY);

    addHose();
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
}

void P4S_Scene::addRenderItem(physx::PxShape* shape, const Vector3& pos, const Colors& color)
{
    m_transforms.push_back(physx::PxTransform(physx::PxVec3(pos.x, pos.y, pos.z)));
    m_renderItems.push_back(new RenderItem(shape, &m_transforms.back(), Color(color)));
}

void P4S_Scene::addHose()
{
    //MANGUERA
    ParticleModel pm;
    pm.pos = Vector3D{ 40.0f, 50.0f, 40.0f };   // boquilla de la manguera
    pm.vel = Vector3D{ 0.0f, 6.0f, 15.0f };     // chorro hacia delante (+Z) y algo hacia arriba (+Y)
    pm.acc = Vector3D{ 0.0f, 0.0f, 0.0f };     // gravedad
    pm.damping = 0.99f;                             // casi sin pérdida de velocidad
    pm.radius = 0.1f;                              // tamaño de cada gota
    pm.lifeTime = 5.0f;
    pm.color = Vector3D{ 0.3f, 0.6f, 1.0f };    // azul claro de agua
    pm.opacity = 1.0f;

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
    pGI.oppacity.variability = { 0.0 };

    std::vector<ForceGenerator*> forces;
    forces.push_back(new GravityGenerator());
    ParticleSystem* hose = new ParticleSystem(pm, pGI, forces);
    particleSystems.push_back(hose);
}
