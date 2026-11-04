#pragma once
#include "Character.h"

class PacMan : public Character
{
public:
    explicit PacMan(const Vector3D& pos);
    void keyPress(unsigned char key, const physx::PxTransform& camera);
    void update(double dt) override;

private:
    enum class CamMode { FirstPerson, ThirdPerson, TopDown };

    void updateCamera();

    CamMode camMode = CamMode::ThirdPerson;
    physx::PxVec3 lastDir{ 0, 0, -1 };
};