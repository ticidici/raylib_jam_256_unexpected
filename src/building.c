#include "building.h"
#include "enemy.h"

#define BLOCK_HEIGHT 2.0f

static Model weaponWeak;
static Model weaponStrong;

static Color weaponWeakColor = { 240, 223, 194, 255 };
static Color weaponStrongColor = { 169, 232, 227, 255 };

static int weakWeaponPrice = 5;
static int strongWeaponPrice = 15;
static int strongWeaponIronNeeded = 3;

static int strawCubeWheatNeeded = 10;
static int stickCubeWoodNeeded = 10;
static int brickCubeClayNeeded = 10;

static int sellPriceWeakWeapon = 2;
static int sellPriceStrongWeapon = 7;
static int sellIronReturnStrongWeapon = 1;

static int sellStrawCubePrice = 3;
static int sellStickCubePrice = 7;
static int sellBrickCubePrice = 13;


void BuildingInit()
{
    weaponWeak = LoadModel("resources/weapon_weak.gltf");
    weaponStrong = LoadModel("resources/weapon_strong.gltf");
}

void BuildingUpdate(Building *building)
{
    building->destroyOffset -= 6 * GetFrameTime();
    if (building->destroyOffset < 0)
    {
        building->destroyOffset = 0;
    }
}

void BuildingRender(Building *building, Vector3 position)
{
    for (int i = 0; i < building->blockCount; i++)
    {
        Block *block = &building->blocks[i];
        // TODO: Add rotation for 360 no scope
        float y = position.y + i * BLOCK_HEIGHT + building->destroyOffset;
        Vector3 blockPosition = {position.x, y, position.z};
        DrawModel(block->model, blockPosition, 1.f, WHITE);
        if (block->weaponType == WeaponWeak) DrawModel(weaponWeak, blockPosition, 1.f, weaponWeakColor);
        else if (block->weaponType == WeaponStrong) DrawModel(weaponStrong, blockPosition, 1.f, weaponStrongColor);
    }
}

void BuildingDestroyBlock(Building *building)
{
    if (building->blockCount == 0)
        return;

    building->destroyOffset = BLOCK_HEIGHT;
    for (int i = 1; i < building->blockCount; i++)
    {
        building->blocks[i - 1] = building->blocks[i];
    }

    building->blockCount -= 1;
}

void BuildingRelease()
{
    UnloadModel(weaponWeak);
    UnloadModel(weaponStrong);
}