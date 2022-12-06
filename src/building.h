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
} typedef Block;

struct Building
{
    Block blocks[3];
    int blockCount;

} typedef Building;

void BuildingUpdate(Building *building);

void BuildingRender(Building *building, Vector3 position);

#endif