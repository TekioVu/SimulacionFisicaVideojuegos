#include "Ghost.h"
#include "ProbabilityDistributions.h"

Ghost::Ghost(const Vector3D& pos, Vector4 color)
    : Character(ParticleModel{
          .pos = pos,
          .color = Vector3D(color.x, color.y, color.z),
          .opacity = color.w,
          .damping = 1.0f,
          .radius = BODY_RADIUS })
{
    const float legRadius = BODY_RADIUS * 0.3f;
    const float dx = BODY_RADIUS * 0.6f;
    const float dy = -BODY_RADIUS * 0.9f;

    legOffsets = {
        physx::PxVec3(-dx, dy, -dx), physx::PxVec3(dx, dy, -dx),
        physx::PxVec3(-dx, dy,  dx), physx::PxVec3(dx, dy,  dx)
    };

    physx::PxShape* legShape = CreateShape(physx::PxSphereGeometry(legRadius));
    for (int i = 0; i < NUM_LEGS; i++) {
        legTransforms[i] = pose * physx::PxTransform(legOffsets[i]);
        legItems[i] = new RenderItem(legShape, &legTransforms[i], color);
    }
}

void Ghost::calcNewDir(int r, int c, bool freeUp, bool freeDown, bool freeLeft, bool freeRight)
{
    if (actualDir != nextDir) return;   // ya hay un giro pendiente, que Map lo aplique

    bool free[5] = { false, freeRight, freeLeft, freeUp, freeDown };   // índice = tDir

    // Delante bloqueado o parado
    bool blocked = (actualDir == NONE) || !free[actualDir];
    bool newCell = (r != lastR || c != lastC);
    if (!blocked && !newCell) return;   // una decisión por celda, salvo que choque

    tDir back = NONE;
    switch (actualDir) {
    case UP: back = DOWN; break;
    case DOWN: back = UP; break;
    case LEFT: back = RIGHT; break;
    case RIGHT: back = LEFT; break;
    default: break;
    }

    std::vector<tDir> options;
    for (int d = RIGHT; d <= DOWN; d++)
        if (free[d] && d != back) options.push_back((tDir)d);

    if (options.empty() && back != NONE && free[back]) options.push_back(back);   // callejón sin salida
    if (options.empty()) return;

    lastR = r; lastC = c;
    setNextDir(options[UniformDistribution(0, (int)options.size() - 1)]);
}

Ghost::~Ghost()
{
    for (auto& item : legItems) {
        if (item) { item->release(); item = nullptr; }
    }
}

void Ghost::update(double dt)
{
    updateLegs();
}

void Ghost::updateLegs()
{
    for (int i = 0; i < NUM_LEGS; i++)
        legTransforms[i] = pose * physx::PxTransform(legOffsets[i]);
}
