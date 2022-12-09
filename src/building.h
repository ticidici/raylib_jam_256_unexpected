#ifndef BUILDING_H
#define BUILDING_H

#include "raylib.h"

enum BuildingMaterial
{
    Straw,
    Sticks,
    Brick,
} typedef BuildingMaterial;

enum WeaponType
{
    WEAPON_NONE,
    WeaponWeak,
    WeaponStrong
} typedef WeaponType;

struct Block
{
    int hp;
    bool isPorquet;
    Model model;
    BuildingMaterial buildingMaterial;
    WeaponType weaponType;
} typedef Block;

struct Building
{
    Block blocks[3];
    int blockCount;
    float destroyOffset;

} typedef Building;

void BuildingInit();

void BuildingUpdate(Building *building);

void BuildingRender(Building *building, Vector3 position);

void BuildingDestroyBlock(Building *building);

void BuildingRelease();

#endif