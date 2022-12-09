#ifndef BUILDING_H
#define BUILDING_H

#include "raylib.h"

#define STRAW_CUBE_HP 100
#define STICK_CUBE_HP 250
#define BRICK_CUBE_HP 500

enum BuildingMaterial
{
    Straw,
    Stick,
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
    double lastAttackTime;
    float destroyOffset;
} typedef Block;

struct Building
{
    Block blocks[3];
    int blockCount;
} typedef Building;

void BuildingInit();

void BuildingUpdate(Building *building, Vector3 position);

void BuildingRender(Building *building, Vector3 position);

void BuildingDestroyBlock(Building *building, int blockPosition);

void BuildingRelease();

#endif