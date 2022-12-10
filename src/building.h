#ifndef BUILDING_H
#define BUILDING_H

#include "raylib.h"

struct Tile typedef Tile;

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
    BuildingMaterial buildingMaterial;
    WeaponType weaponType;
    double lastAttackTime;
    float destroyOffset;
    float rotationTarget;
    float rotation;
} typedef Block;

struct Building
{
    Block blocks[3];
    int blockCount;
    bool isPorquet;
} typedef Building;

extern int weaponWeakPrice;
extern int weaponStrongPrice;
extern int weaponStrongIronNeeded;

extern int cubeStrawWheatNeeded;
extern int cubeStickWoodNeeded;
extern int cubeBrickClayNeeded;

void BuildingInit();

void BuildingUpdate(Building *building, Vector3 position);

void BuildingRender(Building *building, Vector3 position);

void BuildingDamageBlock(Building *building, int blockIndex, int damage);
void BuildingDestroyBlock(Building *building, int blockIndex);

void BuildingRelease();

void BuildingBuyCube(BuildingMaterial materialType, Tile *tile);
void BuildingBuyWeapon(WeaponType weaponType, Tile *tile, int cubeIndex);
void BuildingSellBlock(Tile* tile, int blockIndex);

int GetPorquetBlocksHp();

bool IsWholeSet(Tile* tile);

#endif