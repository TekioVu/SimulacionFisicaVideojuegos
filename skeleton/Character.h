#pragma once
#include "Particle.h"

class Character : public Particle
{
public:
    enum tDir { NONE, RIGHT, LEFT, UP, DOWN };

    explicit Character(const ParticleModel& pm);
    virtual ~Character() = default;

    virtual void update(double dt) = 0;

    void rotateToNewDirection();
    bool isDirectionModified() const { return actualDir != nextDir; }
    bool is180Turn() const;

    void setNextDir(tDir d) { nextDir = d; }
    tDir getActualDir() const { return actualDir; }
    tDir getNextDir() const { return nextDir; }

    static physx::PxVec3 dirToVector(tDir d);
    physx::PxVec3 getNextDirVector() const { return dirToVector(nextDir); }

    physx::PxVec3 getPosition() const { return pose.p; }
    void setPosition(const physx::PxVec3& p) { pose.p = p; }
    void move(const physx::PxVec3& d) { pose.p += d; }
    float getSpeed() const { return speed; }
    physx::PxVec3 getGridOrientation() const { return dirToVector(actualDir); }

protected:
    void rotate(const physx::PxQuat& q) { pose.q = q; }
    physx::PxQuat getQuaternionForNewDirection();

    float speed = 20.0f;
    tDir actualDir = NONE;               
    tDir nextDir = NONE;
};