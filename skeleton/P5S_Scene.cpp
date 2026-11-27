#include "P5S_Scene.h"
#include <algorithm>
#include "RubberBandForceGenerator.h"

void P5S_Scene::init()
{
    physx::PxShape* suelo = CreateShape(physx::PxBoxGeometry(50.0f, 0.1f, 150.0f));
    addRenderItem(suelo, Vector3(50.0f, 30.0f, 50.0f), GRAY);
    
    ParticleModel springModel;
    springModel.pos = Vector3D{ 60.0f, 58.0f, 60.0f };
    springModel.vel = Vector3D{ 0.0f, 0.0f, 0.0f };
    springModel.acc = Vector3D{ 0.0f, 0.0f, 0.0f };
    springModel.damping = 0.99f;
    springModel.radius = 1.0f;
    springModel.color = Vector3D{ 1.0f, 0.2f, 0.2f };
    springModel.opacity = 1.0f;

    //APARTADO 1
    AddStaticSpring(springModel);
    //APARTADO 2
    AddTwoParticleSpring(springModel);
    //GOMA
    AddRubberBand(springModel);
    //SLINKY
    AddSlinky(springModel);
    //FLOTACION
    AddBuoyancy();
}

void P5S_Scene::AddStaticSpring(ParticleModel springModel)
{
    Vector3D ANCHOR_POS = Vector3(60.0f, 60.0f, 60.0f);
    physx::PxShape* cuboReferencia = CreateShape(physx::PxBoxGeometry(1.0f, 1.0f, 1.0f));
    addRenderItem(cuboReferencia, ANCHOR_POS, RED);


    staticParticle = new Particle(springModel);
    particles.push_back(staticParticle);

    muelle = new AnchoredSpringForceGenerator(K_INICIAL, REST_LENGTH, ANCHOR_POS);
    muelle->AddParticle(staticParticle);
    forces.push_back(muelle);

    GravityGenerator* g = new GravityGenerator();
    g->AddParticle(staticParticle);
    forces.push_back(g);
}

void P5S_Scene::AddTwoParticleSpring(ParticleModel springModel)
{
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

void P5S_Scene::AddRubberBand(ParticleModel springModel)
{
    springModel.color = Vector3D{ 0.2f, 1.0f, 0.4f };
    springModel.damping = 0.76f;

    springModel.pos = Vector3D{ 70.0f, 80.0f, 60.0f };
    Particle* p3 = new Particle(springModel);
    springModel.pos = Vector3D{ 82.0f, 80.0f, 60.0f };
    Particle* p4 = new Particle(springModel);
    particles.push_back(p3);
    particles.push_back(p4);

    RubberBandForceGenerator* r1 = new RubberBandForceGenerator(20.0f, 10.0f, p4);
    r1->AddParticle(p3);
    forces.push_back(r1);
    RubberBandForceGenerator* r2 = new RubberBandForceGenerator(20.0f, 10.0f, p3);
    r2->AddParticle(p4);
    forces.push_back(r2);
}

void P5S_Scene::AddBuoyancy()
{
    const float WATER_Y = 50.0f;

    physx::PxShape* agua = CreateShape(physx::PxBoxGeometry(20.0f, 0.05f, 20.0f));
    addRenderItem(agua, Vector3(100.0f, WATER_Y, 60.0f), BLUE);  

    ParticleModel m;
    m.shape = ShapeType::Box;
    m.radius = 0.5f;                              
    m.mass = boxMass;
    m.pos = Vector3D{ 100.0f, 58.0f, 60.0f };  
    m.damping = 1.0f;                            
    m.color = Vector3D{ 0.8f, 0.5f, 0.2f };
    boxP = new Particle(m);
    particles.push_back(boxP);

    buoy = new BuoyancyForceGenerator(1.0f, 1.0f, 1000.0f, WATER_Y, 2000.0f);
    buoy->AddParticle(boxP);
    forces.push_back(buoy);

    GravityGenerator* gBox = new GravityGenerator();
    gBox->AddParticle(boxP);
    forces.push_back(gBox);
}

void P5S_Scene::AddSlinky(ParticleModel m)
{
    m.color = Vector3D{ 0.9f, 0.8f, 0.1f };
    m.radius = 0.5f;

    const float spacing = SLINKY_REST * SLINKY_ELONG;
    const float x = 20.0f, z = 60.0f, top = 100.0f;

    GravityGenerator* gSlinky = new GravityGenerator();
    Particle* prev = nullptr;

    for (int i = 0; i < SLINKY_N; ++i) {
        m.pos = Vector3D{ x, top - i * spacing, z };
        Particle* p = new Particle(m);
        particles.push_back(p);
        gSlinky->AddParticle(p);

        if (prev) {
            auto a = new SpringForceGenerator(slinkyK, SLINKY_REST, prev);  a->AddParticle(p);
            auto b = new SpringForceGenerator(slinkyK, SLINKY_REST, p);     b->AddParticle(prev);
            forces.push_back(a);  forces.push_back(b);
            slinkySprings.push_back(a);  slinkySprings.push_back(b);
        }
        else {
            slinkyAnchor = new AnchoredSpringForceGenerator(
                slinkyK, SLINKY_REST, Vector3D{ x, top + spacing, z });
            slinkyAnchor->AddParticle(p);
            forces.push_back(slinkyAnchor);
        }
        prev = p;
    }
    forces.push_back(gSlinky);
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
    else if (key == 'q' || key == 'Q') { boxMass = (std::max)(boxMass - 50.0f, 50.0f);  boxP->setMass(boxMass); }
    else if (key == 'r' || key == 'R') { boxMass += 50.0f;                              boxP->setMass(boxMass); }
    else if (key == 'e' || key == 'E') buoy->setVolume((std::max)(buoy->getVolume() - 0.1f, 0.1f));
    else if (key == 't' || key == 'T') buoy->setVolume(buoy->getVolume() + 0.1f);
}

void P5S_Scene::addRenderItem(physx::PxShape* shape, const Vector3& pos, const Colors& color)
{
    m_transforms.push_back(physx::PxTransform(physx::PxVec3(pos.x, pos.y, pos.z)));
    m_renderItems.push_back(new RenderItem(shape, &m_transforms.back(), Color(color)));
}

