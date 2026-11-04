#include "Map.h"
#include <cmath>

using physx::PxVec3;

Map::Cell Map::cellAt(const PxVec3& p) const
{
    return { (int)std::lround(p.z / paso), (int)std::lround(p.x / paso) };
}

PxVec3 Map::cellCenter(Cell cell) const
{
    return PxVec3(cell.c * paso, 0.0f, cell.r * paso);
}

bool Map::canPass(int r, int c) const
{
    if (r < 0 || r >= (int)grid.size()) return false;
    if (c < 0 || c >= (int)grid[r].size()) return false;
    char ch = grid[r][c];
    return ch != 'x' && ch != 'n';   // 'n' = casa de fantasmas, ver nota
}

bool Map::centerReached(const PxVec3& difference, const PxVec3& dir)
{
    if (dir.isZero()) return false;
    return difference.dot(dir) >= 0;
}

void Map::moveCharacters(double dt)
{
    for (auto c : characters) {
        moveCharacter(dt, c);
        c->update(dt);
    }
}

void Map::moveCharacter(double dt, Character* c)
{
    if (!c) return;

    PxVec3 pos = c->getPosition();
    PxVec3 dir = c->getGridOrientation();
    Cell cell = cellAt(pos);
    PxVec3 center = cellCenter(cell);

    Cell front = { cell.r + (int)dir.z, cell.c + (int)dir.x };

    c->calcNewDir(cell.r, cell.c,
        canPass(cell.r - 1, cell.c), canPass(cell.r + 1, cell.c),
        canPass(cell.r, cell.c - 1), canPass(cell.r, cell.c + 1));
    // Sin cambio de dirección: avanzar
    if (!c->isDirectionModified()) {
        stepForward(c, cell, front, dt);
        return;
    }

    // Nueva dirección pedida
    PxVec3 nd = c->getNextDirVector();
    bool newDirFree = canPass(cell.r + (int)nd.z, cell.c + (int)nd.x);

    // Parado o media vuelta: se puede girar al instante
    if (newDirFree && (c->getActualDir() == Character::NONE || c->is180Turn())) {
        c->rotateToNewDirection();
        return;
    }
    if (c->is180Turn()) {            // media vuelta siempre posible: se vuelve por donde se vino
        c->rotateToNewDirection();
        return;
    }

    // Giro de 90º: solo al llegar al centro de la celda y si el hueco está libre
    if (newDirFree) {
        PxVec3 newPos = pos + dir * c->getSpeed() * (float)dt;
        if (centerReached(newPos - center, dir)) {
            c->setPosition(PxVec3(center.x, pos.y, center.z));   // centrar
            c->rotateToNewDirection();
            return;
        }
    }

    // No se puede girar todavía: seguir recto si se puede
    stepForward(c, cell, front, dt);
}

bool Map::stepForward(Character* c, Cell cell, Cell front, double dt)
{
    PxVec3 dir = c->getGridOrientation();
    PxVec3 pos = c->getPosition();
    PxVec3 center = cellCenter(cell);
    PxVec3 delta = dir * c->getSpeed() * (float)dt;

    if (centerReached((pos + delta) - center, dir) && !canPass(front.r, front.c)) {
        c->setPosition(PxVec3(center.x, pos.y, center.z));   // tope contra el muro
        return false;
    }
    c->move(delta);
    return true;
}