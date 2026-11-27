#pragma once
#include "Character.h"
#include <string>
#include <vector>

class Map
{
public:
    explicit Map(float cellSize) : paso(cellSize) {}

    void load(const std::vector<std::string>& lines) { grid = lines; }
    void addCharacter(Character* c) { characters.push_back(c); }
    void clearCharacters() { characters.clear(); }   // no los borra, son de la escena

    void moveCharacters(double dt);

    struct Cell { int r, c; };
    Cell cellAt(const physx::PxVec3& p) const;
    physx::PxVec3 cellCenter(Cell cell) const;
    bool canPass(int r, int c) const;

private:
    void moveCharacter(double dt, Character* c);
    bool stepForward(Character* c, Cell cell, Cell front, double dt);
    static bool centerReached(const physx::PxVec3& difference, const physx::PxVec3& dir);

    float paso;
    std::vector<std::string> grid;
    std::vector<Character*> characters;
};