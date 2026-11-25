#include "P5S_Scene.h"
#include <algorithm>

void P5S_Scene::init()
{
    physx::PxShape* suelo = CreateShape(physx::PxBoxGeometry(50.0f, 0.1f, 150.0f));
    addRenderItem(suelo, Vector3(50.0f, 30.0f, 50.0f), GRAY);
    
    //APARTADO 1
    Vector3D ANCHOR_POS = Vector3(60.0f, 60.0f, 60.0f);
    physx::PxShape* cuboReferencia = CreateShape(physx::PxBoxGeometry(1.0f, 1.0f, 1.0f));
    addRenderItem(cuboReferencia, ANCHOR_POS, RED);

    ParticleModel springModel;
    springModel.pos = Vector3D{ 60.0f, 58.0f, 60.0f };  
    springModel.vel = Vector3D{ 0.0f, 0.0f, 0.0f };
    springModel.acc = Vector3D{ 0.0f, 0.0f, 0.0f };
    springModel.damping = 0.99f;
    springModel.radius = 1.0f;
    springModel.color = Vector3D{ 1.0f, 0.2f, 0.2f };
    springModel.opacity = 1.0f;

    staticParticle = new Particle(springModel);
    particles.push_back(staticParticle);

    muelle = new AnchoredSpringForceGenerator(K_INICIAL, REST_LENGTH, ANCHOR_POS);
    muelle->AddParticle(staticParticle);
    forces.push_back(muelle);

    GravityGenerator* g = new GravityGenerator();
    g->AddParticle(staticParticle);
    forces.push_back(g);

    //APARTADO 2
    springModel.pos = Vector3D{ 70.0f, 68.0f, 60.0f };
    Particle* p1 = new Particle(springModel);
    springModel.pos = Vector3D{ 50.0f, 68.0f, 60.0f };
    Particle* p2 = new Particle(springModel);
    particles.push_back(p1);
    particles.push_back(p2);

    SpringForceGenerator* f1 = new SpringForceGenerator(20, 10, p2);
    f1->AddParticle(p1);
    forces.push_back(f1);
    SpringForceGenerator* f2 = new SpringForceGenerator(20, 10, p1);
    f2->AddParticle(p2);
    forces.push_back(f2);

}

void P5S_Scene::cleanup()
{
    for (auto& renderItem : m_renderItems) {
        if (renderItem) {
            renderItem->release();
            renderItem = nullptr;
        }
    }

    for (auto p : particles) {
        delete p;
    }

    for (auto fg : forces) {
        delete fg;
    }

    delete muelle;
}

void P5S_Scene::update(double dt)
{
    if (impulseTimer > 0.0f) {                      
        staticParticle->applyForce(Vector3D{ 300.0f, 0.0f, 0.0f });
        impulseTimer -= (float)dt;
    }

    for (auto f : forces) {
        f->update();
    }

    for (auto p : particles) {
        p->integrateSemiEuler(dt);                   
    }
}

void P5S_Scene::keyPress(unsigned char key, const physx::PxTransform& camera)
{
    if (key == 'k' || key == 'K')
        muelle->setK((std::max)(muelle->getK() - K_PASO, K_MIN));
    else if (key == 'l' || key == 'L')
        muelle->setK((std::min)(muelle->getK() + K_PASO, K_MAX));
    else if (key == 'f' || key == 'F')
        impulseTimer = 0.2f;                         
    else if (key == 'm' || key == 'M')
        muelle->setK(600.0f);      
}

void P5S_Scene::addRenderItem(physx::PxShape* shape, const Vector3& pos, const Colors& color)
{
    m_transforms.push_back(physx::PxTransform(physx::PxVec3(pos.x, pos.y, pos.z)));
    m_renderItems.push_back(new RenderItem(shape, &m_transforms.back(), Color(color)));
}

