#pragma once
#include "RenderUtils.hpp"

class PacMan
{
public:
    PacMan();

    void keyPress(unsigned char key, const physx::PxTransform& camera);
    void update();
private:
};

