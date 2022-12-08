#ifndef BUILDING_H
#define BUILDING_H

#include "raylib.h"

enum BuildingMaterial
{
    Straw,
    Sticks,
    Brick,
} typedef BuildingMaterial;

struct Block
{
    int hp;
    bool isPorquet;
    Model model;
    BuildingMaterial buildingMaterial;
    double lastAttackTime;
} typedef Block;

struct Building
{
    Block blocks[3];
    int blockCount;
    float destroyOffset;
} typedef Building;

struct Bullet
{
    Vector3 position;
    Vector3 target;
    Model model;
    bool alive;
} typedef Bullet;

void BuildingUpdate(Building *building, Vector3 position);

void BuildingRender(Building *building, Vector3 position);

void BuildingDestroyBlock(Building *building);

// Private
void BuildingSpawnBullet(Vector3 position, Vector3 target);
#endif