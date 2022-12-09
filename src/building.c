#include "building.h"
#include "enemy.h"
#include "raymath.h"
#include "bullet.h"
#include "game_state.h"
#include "sound.h"

#define BLOCK_HEIGHT 2.0f
#define BLOCK_FALL_SPEED 6.0f
#define BLOCK_ATTACK_COLDOWN 1.0f

static Model strawCube;
static Model stickCube;
static Model brickCube;

static Model weaponWeak;
static Model weaponStrong;

static Color weaponWeakColor = {240, 223, 194, 255};
static Color weaponStrongColor = {169, 232, 227, 255};

static int weaponWeakPrice = 5;
static int weaponStrongPrice = 15;
static int weaponStrongIronNeeded = 3;

static int cubeStrawWheatNeeded = 10;
static int cubeStickWoodNeeded = 10;
static int cubeBrickClayNeeded = 10;

static int sellPriceWeakWeapon = 2;
static int sellPriceStrongWeapon = 7;
static int sellIronReturnStrongWeapon = 1;

static int sellStrawCubePrice = 3;
static int sellStickCubePrice = 7;
static int sellBrickCubePrice = 13;

void BuildingInit()
{
    strawCube = LoadModel("resources/straw_cube.gltf");
    stickCube = LoadModel("resources/stick_cube.gltf");
    brickCube = LoadModel("resources/brick_cube.gltf");

    weaponWeak = LoadModel("resources/weapon_weak.gltf");
    weaponStrong = LoadModel("resources/weapon_strong.gltf");
}

void BuildingUpdate(Building *building, Vector3 position)
{
    float delta = GetFrameTime();

    double now = GetTime();
    for (int i = 0; i < building->blockCount; i++)
    {
        Block *block = &building->blocks[i];
        Vector3 blockPosition = position;

        block->destroyOffset -= BLOCK_FALL_SPEED * delta;
        if (block->destroyOffset < 0)
        {
            block->destroyOffset = 0;
        }

        blockPosition.y += (BLOCK_HEIGHT / 2) * (i + 1) + block->destroyOffset;

        // Atack
        if (block->weaponType != WEAPON_NONE && now - block->lastAttackTime > BLOCK_ATTACK_COLDOWN)
        {
            Enemy *targetEnemy = FindClosestEnemy(position, 10);
            if (targetEnemy)
            {
                block->lastAttackTime = now;
                BulletSpawn(blockPosition, targetEnemy, block->weaponType);
            }
        }
    }
}

void BuildingRender(Building *building, Vector3 position)
{
    for (int i = 0; i < building->blockCount; i++)
    {
        Block *block = &building->blocks[i];
        // TODO: Add rotation for 360 no scope
        float y = position.y + i * BLOCK_HEIGHT + block->destroyOffset;
        Vector3 blockPosition = {position.x, y, position.z};

        if (block->buildingMaterial == Straw)
        {
            DrawModel(strawCube, blockPosition, 1.f, WHITE);
        }
        else if (block->buildingMaterial == Stick)
        {
            DrawModel(stickCube, blockPosition, 1.f, WHITE);
        }
        else if (block->buildingMaterial == Brick)
        {
            DrawModel(brickCube, blockPosition, 1.f, WHITE);
        }

        if (block->weaponType == WeaponWeak)
            DrawModel(weaponWeak, blockPosition, 1.f, weaponWeakColor);
        else if (block->weaponType == WeaponStrong)
            DrawModel(weaponStrong, blockPosition, 1.f, weaponStrongColor);
    }
}

void BuildingDestroyBlock(Building *building, int blockPosition)
{
    if (blockPosition - 1 > building->blockCount)
        return;

    for (int i = blockPosition + 1; i < building->blockCount; i++)
    {
        building->blocks[i - 1] = building->blocks[i];
        building->blocks[i - 1].destroyOffset = BLOCK_HEIGHT;
    }

    building->blockCount -= 1;
}

void BuildingBuyCube(BuildingMaterial materialType, Tile* tile)
{
    if (tile->building.blockCount >= 3) return;//should not happen
    if (materialType == Straw)
    {
        if(GetResource(WheatType) < cubeStrawWheatNeeded)
        {
            PlaySound(SoundWrong);
            return;
        }

        ModifyResource(WheatType, -cubeStrawWheatNeeded);
        int index = tile->building.blockCount;
        tile->building.blocks[index].buildingMaterial = materialType;
        tile->building.blockCount++;
    }
    else if (materialType == Stick)
    {
        if (GetResource(WoodType) < cubeStickWoodNeeded)
        {
            PlaySound(SoundWrong);
            return;
        }

        ModifyResource(WoodType, -cubeStickWoodNeeded);
        int index = tile->building.blockCount;
        tile->building.blocks[index].buildingMaterial = materialType;
        tile->building.blockCount++;
    }
    else if (materialType == Brick)
    {
        if (GetResource(ClayType) < cubeBrickClayNeeded)
        {
            PlaySound(SoundWrong);
            return;
        }

        ModifyResource(ClayType, -cubeBrickClayNeeded);
        int index = tile->building.blockCount;
        tile->building.blocks[index].buildingMaterial = materialType;
        tile->building.blockCount++;
    }

}

void BuildingBuyWeapon(WeaponType weaponType, Tile* tile, int cubeIndex)
{
    if (cubeIndex > 2) return;
    if (tile->building.blockCount <= cubeIndex) return;//should not happen
    if (tile->building.blocks[cubeIndex].weaponType == weaponType) return;//should not happen

    if (weaponType == WeaponWeak)
    {
        if (GetMoney() < weaponWeakPrice)
        {
            PlaySound(SoundWrong);
            return;
        }

        ModifyMoney(-weaponWeakPrice);
        tile->building.blocks[cubeIndex].weaponType = weaponType;
    }
    else if (weaponType == WeaponStrong)
    {
        if (GetResource(LavaType) < weaponStrongIronNeeded || GetMoney() < weaponStrongPrice)
        {
            PlaySound(SoundWrong);
            return;
        }
        
        ModifyMoney(-weaponStrongPrice);
        ModifyResource(LavaType, -weaponStrongIronNeeded);
        tile->building.blocks[cubeIndex].weaponType = weaponType;
    }
}


void BuildingRelease()
{
    UnloadModel(strawCube);
    UnloadModel(stickCube);
    UnloadModel(brickCube);

    UnloadModel(weaponWeak);
    UnloadModel(weaponStrong);
}