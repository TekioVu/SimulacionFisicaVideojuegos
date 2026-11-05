#include "PacMan.h"

PacMan::PacMan(const Vector3D& pos)
    : Character(ParticleModel{
          .pos = pos,
          .color = Vector3D(1.0f, 1.0f, 0.0f),
          .damping = 1.0f,
          .radius = 4.0f })
{
}

PacMan::Input PacMan::keyToInput(unsigned char key)
{
    switch (std::tolower(key)) {
    case 'i': return Input::Forward;
    case 'k': return Input::Back;
    case 'j': return Input::Left;
    case 'l': return Input::Right;
    default:  return Input::None;
    }
}

Character::tDir PacMan::vecToDir(const physx::PxVec3& v)
{
    if (std::abs(v.x) > std::abs(v.z))
        return v.x > 0 ? Character::RIGHT : Character::LEFT;
    return v.z > 0 ? Character::DOWN : Character::UP;
}

void PacMan::cycleCamera()
{
    camMode = static_cast<CamMode>((static_cast<int>(camMode) + 1) % 3);
}

Character::tDir PacMan::resolveDir(Input in) const
{
    if (camMode == CamMode::TopDown) {
        switch (in) {
        case Input::Forward: return UP;
        case Input::Back:    return DOWN;
        case Input::Left:    return LEFT;
        default:             return RIGHT;
        }
    }

    // 1ª y 3ª persona: relativo a hacia dónde mira Pac-Man
    physx::PxVec3 fwd = dirToVector(actualDir);
    if (fwd.isZero()) fwd = lastDir;
    physx::PxVec3 right = fwd.cross(physx::PxVec3(0, 1, 0));

    switch (in) {
    case Input::Forward: return vecToDir(fwd);
    case Input::Back:    return vecToDir(-fwd);
    case Input::Left:    return vecToDir(-right);
    default:             return vecToDir(right);
    }
}

void PacMan::keyPress(unsigned char key, const physx::PxTransform&)
{
    if (std::tolower(key) == 'c') {
        cycleCamera();
        return;
    }

    Input in = keyToInput(key);
    if (in != Input::None)
        setNextDir(resolveDir(in));
}


void PacMan::update(double dt)
{
    updateCamera(dt);
}

void PacMan::updateCamera(double dt)
{
    auto* cam = GetCamera();
    if (!cam) return;

    physx::PxVec3 d = dirToVector(actualDir);
    if (!d.isZero()) lastDir = d;
    d = lastDir;

    physx::PxVec3 p = pose.p;
    physx::PxVec3 targetEye, targetDir;

    switch (camMode) {
    case CamMode::FirstPerson:
        targetEye = p + d * 8.0f;
        targetDir = d;
        break;
    case CamMode::ThirdPerson:
        targetEye = p - d * 30.0f + physx::PxVec3(0, 20.0f, 0);
        targetDir = (p + d * 20.0f - targetEye).getNormalized();
        break;
    case CamMode::TopDown:
    default:
        targetEye = p + physx::PxVec3(0, 120.0f, 40.0f);
        targetDir = physx::PxVec3(0.0f, -1.0f, -0.3f).getNormalized();
        break;
    }

    if (!camInit) {            
        camEye = targetEye;
        camDir = targetDir;
        camInit = true;
    }
    else {
        float t = 1.0f - std::exp(-camSmooth * (float)dt);
        camEye = camEye + (targetEye - camEye) * t;
        physx::PxVec3 nd = camDir + (targetDir - camDir) * t;
        if (nd.magnitudeSquared() > 1e-6f) camDir = nd.getNormalized();
    }

    cam->setEye(camEye);
    cam->setDir(camDir);
}