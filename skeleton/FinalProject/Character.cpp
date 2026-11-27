#include "Character.h"
#include <cmath>

Character::Character(const ParticleModel& pm) : Particle(pm) {}

void Character::rotateToNewDirection()
{
    rotate(getQuaternionForNewDirection());
}

physx::PxVec3 Character::dirToVector(tDir d)
{
    switch (d) {
    case RIGHT: return physx::PxVec3(1, 0, 0);
    case LEFT:  return physx::PxVec3(-1, 0, 0);
    case DOWN:  return physx::PxVec3(0, 0, 1);
    case UP:    return physx::PxVec3(0, 0, -1);
    default:    return physx::PxVec3(0, 0, 0);
    }
}

bool Character::is180Turn() const
{
    return dirToVector(nextDir) == -dirToVector(actualDir);
}

physx::PxQuat Character::getQuaternionForNewDirection()
{
    physx::PxVec3 d = getNextDirVector();
    actualDir = nextDir;

    if (d.isZero()) return pose.q;

    float yaw = std::atan2(-d.x, -d.z);
    return physx::PxQuat(yaw, physx::PxVec3(0, 1, 0));
}