#include "FinalScene.h"
#include "GravityGenerator.h"
#include "WindGenerator.h"
#include "WhirlwindGenerator.h"
#include "ExplosionGenerator.h"

#include <string>
#include <fstream>
#include <iostream>

void FinalScene::init()
{
    ReadMap();
}

void FinalScene::cleanup()
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

    for (auto p : pills) delete p;
    pills.clear();
}

void FinalScene::update(double dt)
{
    for (auto ps : particleSystems) {
        ps->update(dt);
    }
}

void FinalScene::keyPress(unsigned char key, const physx::PxTransform& camera) {
    pacman->keyPress(key, camera);
}

void FinalScene::addRenderItem(physx::PxShape* shape, const Vector3& pos, const Colors& color)
{
    m_transforms.push_back(physx::PxTransform(physx::PxVec3(pos.x, pos.y, pos.z)));
    m_renderItems.push_back(new RenderItem(shape, &m_transforms.back(), Color(color)));
}

void FinalScene::ReadMap() {
    std::string mapFilePath = std::string("map.txt");
    std::ifstream mapFile(mapFilePath);

    if (!mapFile.is_open()) {
        std::cerr << "No se pudo abrir el archivo" << std::endl;
        return;
    }

    int cols, fils;
    mapFile >> cols >> fils;

    physx::PxShape* block = CreateShape(physx::PxBoxGeometry(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE));
    for (int i = 0; i < fils; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            char c = ' ';
            mapFile >> c;

            if (c == 'x') {
                addRenderItem(block, Vector3(j * BLOCK_SIZE * 2, 0.0f, i * BLOCK_SIZE * 2), BLUE);
            }
            else if(c == 'o') {
                pills.push_back(new Particle(ParticleModel{
                  .pos = Vector3D(j * BLOCK_SIZE * 2, 0.0f, i * BLOCK_SIZE * 2),
                  .color = Vector3D(1.0f, 1.0f, 0.0f),
                  .radius = PILL_RADIUS
                    }));
            }
            else if(c == 'O') {
                pills.push_back(new Particle(ParticleModel{
                  .pos = Vector3D(j * BLOCK_SIZE * 2, 0.0f, i * BLOCK_SIZE * 2),
                  .color = Vector3D(1.0f, 1.0f, 0.0f),
                  .radius = POWER_RADIUS
                    }));
            }
            else if (c == 'P') {
                pacman = new PacMan();
            }
        }

        std::cout << std::endl;
    }

    physx::PxShape* suelo = CreateShape(physx::PxBoxGeometry(cols * BLOCK_SIZE, 0.1f, fils * BLOCK_SIZE));
    addRenderItem(suelo, Vector3( (cols - 1) * BLOCK_SIZE, - BLOCK_SIZE, (fils - 1) * BLOCK_SIZE), BLACK);
}
