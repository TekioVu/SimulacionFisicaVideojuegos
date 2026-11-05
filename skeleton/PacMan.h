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
    void updateCamera(double dt);
    void cycleCamera();

    enum class Input { Forward, Back, Left, Right, None };
    static Input keyToInput(unsigned char key);
    tDir resolveDir(Input in) const;
    static tDir vecToDir(const physx::PxVec3& v);

    physx::PxVec3 lastDir{ 0, 0, -1 };
    CamMode camMode = CamMode::ThirdPerson;
    physx::PxVec3 camEye{ 0, 0, 0 };   
    physx::PxVec3 camDir{ 0, 0, -1 };
    bool camInit = false;
    float camSmooth = 6.0f;             
};