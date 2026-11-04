#include "PacMan.h"

PacMan::PacMan(const Vector3D& pos)
    : Character(ParticleModel{
          .pos = pos,
          .color = Vector3D(1.0f, 1.0f, 0.0f),
          .damping = 1.0f,
          .radius = 4.0f })
{
}

void PacMan::keyPress(unsigned char key, const physx::PxTransform&)
{
    switch (key) {
    case 'i': case 'I': setNextDir(UP);    break;
    case 'k': case 'K': setNextDir(DOWN);  break;
    case 'j': case 'J': setNextDir(LEFT);  break;
    case 'l': case 'L': setNextDir(RIGHT); break;
    case 'c': case 'C':
        switch (camMode) {
        case CamMode::FirstPerson: camMode = CamMode::ThirdPerson; break;
        case CamMode::ThirdPerson: camMode = CamMode::TopDown;     break;
        case CamMode::TopDown:     camMode = CamMode::FirstPerson; break;
        }
        break;
    }
}

void PacMan::update(double dt)
{
    updateCamera();
}

void PacMan::updateCamera()
{
    auto* cam = GetCamera();
    if (!cam) return;

    physx::PxVec3 d = dirToVector(actualDir);
    if (!d.isZero()) lastDir = d;
    d = lastDir;

    physx::PxVec3 p = pose.p;

    switch (camMode) {
    case CamMode::FirstPerson:
        cam->setEye(p + d * 5.0f);
        cam->setDir(d);
        break;

    case CamMode::ThirdPerson: {
        physx::PxVec3 eye = p - d * 30.0f + physx::PxVec3(0, 20.0f, 0);
        cam->setEye(eye);
        cam->setDir((p + d * 20.0f - eye).getNormalized());
        break;
    }

    case CamMode::TopDown:
        cam->setEye(p + physx::PxVec3(0, 120.0f, 40.0f));
        cam->setDir(physx::PxVec3(0.0f, -1.0f, -0.3f).getNormalized());
        break;
    }
}