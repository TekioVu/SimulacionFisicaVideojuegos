#include "FinalScene.h"

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

    mapa.clearCharacters();
    for (auto g : ghosts) delete g;
    ghosts.clear();
    delete pacman;
    pacman = nullptr;

    m_renderItems.clear();
    m_transforms.clear();
}

void FinalScene::update(double dt)
{
    for (auto ps : particleSystems) ps->update(dt);
    mapa.moveCharacters(dt);
}

void FinalScene::keyPress(unsigned char key, const physx::PxTransform& camera) {
    if (pacman != nullptr) {
        pacman->keyPress(key, camera);
    }
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
    std::vector<std::string> lineas(fils, std::string(cols, ' '));

    physx::PxShape* block = CreateShape(physx::PxBoxGeometry(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE));
    for (int i = 0; i < fils; i++)
        for (int j = 0; j < cols; j++)
        {
            char c = ' ';
            mapFile >> c;
            lineas[i][j] = c;
            Vector3D pos(j * BLOCK_SIZE * 2, 0.0f, i * BLOCK_SIZE * 2);
            Vector3D pillsPos(j * BLOCK_SIZE * 2, -2.0f, i * BLOCK_SIZE * 2);

            switch (c)
            {
            case 'x':
                addRenderItem(block, pos, BLUE);
                break;
            case 'o':
                pills.push_back(new Particle(ParticleModel{
                    .pos = pillsPos, .color = Vector3D(1, 1, 0), .radius = PILL_RADIUS }));
                break;
            case 'O':
                pills.push_back(new Particle(ParticleModel{
                    .pos = pillsPos, .color = Vector3D(1, 1, 0), .radius = POWER_RADIUS }));
                break;
            case 'P':
                pacman = new PacMan(pos);
                mapa.addCharacter(pacman);
                break;
            case 'g': {
                Ghost* g = new Ghost(pos, Vector4(1, 0, 0, 1));
                ghosts.push_back(g);
                mapa.addCharacter(g);
                break;
            }
            }
        }

    mapa.load(lineas);

    physx::PxShape* suelo = CreateShape(physx::PxBoxGeometry(cols * BLOCK_SIZE, 0.1f, fils * BLOCK_SIZE));
    addRenderItem(suelo, Vector3( (cols - 1) * BLOCK_SIZE, - BLOCK_SIZE, (fils - 1) * BLOCK_SIZE), BLACK);
}
