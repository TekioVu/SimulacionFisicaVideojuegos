#include "Ghost.h"

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