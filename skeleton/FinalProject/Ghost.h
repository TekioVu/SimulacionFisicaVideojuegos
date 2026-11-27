#pragma once
#include "Character.h"
#include <array>

class Ghost : public Character
{
public:
    Ghost(const Vector3D& pos, Vector4 color);
    ~Ghost();
    void update(double dt) override;
    void calcNewDir(int r, int c, bool freeUp, bool freeDown, bool freeLeft, bool freeRight) override;

private:
    static constexpr int NUM_LEGS = 4;
    static constexpr float BODY_RADIUS = 4.0f;

    int lastR = -1, lastC = -1;   // última celda en la que decidió

    void updateLegs();

    std::array<physx::PxTransform, NUM_LEGS> legTransforms;
    std::array<physx::PxVec3, NUM_LEGS> legOffsets;
    std::array<RenderItem*, NUM_LEGS> legItems{};
};