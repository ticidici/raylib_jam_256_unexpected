#include "building.h"
#include "enemy.h"
#include "raymath.h"
#include "bullet.h"
#include "game_state.h"
#include "sound.h"
#include "terrain.h"

#define BLOCK_HEIGHT 2.0f
#define BLOCK_FALL_SPEED 6.0f
#define BLOCK_ATTACK_COLDOWN 1.1f
#define BLOCK_ROTATION_SPEED 720.0f

static Model pig;
static float pigScale = 0.020f;

static Model strawCube;
static Model stickCube;
static Model brickCube;

static Model weaponWeak;
static Model weaponStrong;

static Color weaponWeakColor = {240, 223, 194, 255};
static Color weaponStrongColor = {169, 232, 227, 255};

int weaponWeakPrice = 5;
int weaponStrongPrice = 15;
int weaponStrongIronNeeded = 3;

int cubeStrawWheatNeeded = 10;
int cubeStickWoodNeeded = 10;
int cubeBrickClayNeeded = 10;

int sellPriceWeakWeapon = 2;
int sellPriceStrongWeapon = 7;
int sellIronReturnStrongWeapon = 1;

int sellStrawCubePrice = 3;
int sellStickCubePrice = 7;
int sellBrickCubePrice = 13;

int extraPorquetSellBonusMoney = 50;
int extraPorquetSellBonusWheat = 5;
int extraPorquetSellBonusWood = 5;
int extraPorquetSellBonusClay = 10;

static int porquetBlocksHp = 50;
static int strawBlockHp = 10;
static int stickBlockHp = 22;
static int brickBlockHp = 35;

void BuildingInit()
{
    pig = LoadModel("resources/pig.glb");

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
            Enemy *targetEnemy = FindClosestEnemy(position, BATTLEFIELD_SIZE * 4);
            if (targetEnemy)
            {
                block->lastAttackTime = now;
                BulletSpawn(blockPosition, targetEnemy, block->weaponType);
                Vector2 pos = { position.x, position.z };
                Vector2 targetPos = { targetEnemy->position.x,  targetEnemy->position.z};
                Vector2 dir = Vector2Normalize(Vector2Subtract(targetPos, pos));
                float angle = atan2f(-dir.y, dir.x);
                block->rotationTarget = angle * RAD2DEG + 90;
            }
            else {
                block->rotationTarget = 0;
            }
        }

        // Rotate towards target
        float rotationDiff = block->rotationTarget - block->rotation;
        rotationDiff += (rotationDiff > 180) ? -360
            : (rotationDiff < -180) ? 360
            : 0;
        float rotationIncrement = BLOCK_ROTATION_SPEED * delta;
        if (fabs(rotationDiff) > rotationIncrement)
        {
            int sign = rotationDiff > 0 ? 1 : -1;
            block->rotation += rotationIncrement * sign;
        }
        else
        {
            block->rotation = block->rotationTarget;
        }

    }
}

void BuildingRender(Building *building, Vector3 position)
{
    int blockCount = building->blockCount;
    for (int i = 0; i < blockCount; i++)
    {
        Block *block = &building->blocks[i];
        // TODO: Add rotation for 360 no scope
        float y = position.y + i * BLOCK_HEIGHT + block->destroyOffset;
        Vector3 blockPosition = {position.x, y, position.z};

        float angle = block->rotation;
        if (block->buildingMaterial == Straw)
        {
            DrawModelEx(strawCube, blockPosition, (Vector3) { 0, 1, 0 }, angle, (Vector3) { 1, 1, 1 }, WHITE);
        }
        else if (block->buildingMaterial == Stick)
        {
            DrawModelEx(stickCube, blockPosition, (Vector3) { 0, 1, 0 }, angle, (Vector3) { 1, 1, 1 }, WHITE);
        }
        else if (block->buildingMaterial == Brick)
        {
            DrawModelEx(brickCube, blockPosition, (Vector3) { 0, 1, 0 }, angle, (Vector3) { 1, 1, 1 }, WHITE);
        }

        if (block->weaponType == WeaponWeak)
        {
            DrawModelEx(weaponWeak, blockPosition, (Vector3) { 0, 1, 0 }, angle, (Vector3) { 1, 1, 1 }, weaponWeakColor);
        }
        else if (block->weaponType == WeaponStrong)
        {
            DrawModelEx(weaponStrong, blockPosition, (Vector3) { 0, 1, 0 }, angle, (Vector3) { 1, 1, 1 }, weaponStrongColor);
        }


    }

    if (building->isPorquet)
    {
        Vector3 pigPosition = position;
        if (blockCount > 0)
        {
            Block* block = &building->blocks[blockCount - 1];
            pigPosition.y += blockCount * BLOCK_HEIGHT + block->destroyOffset;
        }
        DrawModel(pig, pigPosition, pigScale, WHITE);
    }
}

void BuildingDamageBlock(Building* building, int blockIndex, int damage)
{
    if (blockIndex - 1 > building->blockCount) return;

    building->blocks[blockIndex].hp -= damage;
    if (building->blocks[blockIndex].hp <= 0)
    {
        BuildingDestroyBlock(building, blockIndex);
    }
}

void BuildingDestroyBlock(Building *building, int blockIndex)
{
    if (blockIndex - 1 > building->blockCount) return;

    building->blocks[blockIndex].weaponType = WEAPON_NONE;

    for (int i = blockIndex + 1; i < building->blockCount; i++)
    {
        building->blocks[i - 1] = building->blocks[i];
        building->blocks[i - 1].destroyOffset = BLOCK_HEIGHT;
    }

    building->blockCount -= 1;
    PlaySound(soundBreak);
}

void BuildingBuyCube(BuildingMaterial materialType, Tile* tile)
{
    if (tile->building.blockCount >= 3)
    {
        PlaySound(soundForbidden);
        return;
    }

    if (tile->building.isPorquet)
    {
        PlaySound(soundForbidden);//cannot build in porquet
        return;
    }

    if (materialType == Straw)
    {
        if(GetResource(WheatType) < cubeStrawWheatNeeded)
        {
            PlaySound(soundForbidden);
            return;
        }

        ModifyResource(WheatType, -cubeStrawWheatNeeded);
        int index = tile->building.blockCount;
        tile->building.blocks[index].buildingMaterial = materialType;
        tile->building.blocks[index].weaponType = WEAPON_NONE;
        tile->building.blocks[index].hp = strawBlockHp;
        tile->building.blockCount++;
        PlaySound(soundBuild);
    }
    else if (materialType == Stick)
    {
        if (GetResource(WoodType) < cubeStickWoodNeeded)
        {
            PlaySound(soundForbidden);
            return;
        }

        ModifyResource(WoodType, -cubeStickWoodNeeded);
        int index = tile->building.blockCount;
        tile->building.blocks[index].buildingMaterial = materialType;
        tile->building.blocks[index].weaponType = WEAPON_NONE;
        tile->building.blocks[index].hp = stickBlockHp;
        tile->building.blockCount++;
        PlaySound(soundBuild);
    }
    else if (materialType == Brick)
    {
        if (GetResource(ClayType) < cubeBrickClayNeeded)
        {
            PlaySound(soundForbidden);
            return;
        }

        ModifyResource(ClayType, -cubeBrickClayNeeded);
        int index = tile->building.blockCount;
        tile->building.blocks[index].buildingMaterial = materialType;
        tile->building.blocks[index].weaponType = WEAPON_NONE;
        tile->building.blocks[index].hp = brickBlockHp;
        tile->building.blockCount++;
        PlaySound(soundBuild);
    }
    if(tile->building.blockCount == 1 && tile->tileType == LavaType) BuildingLavaDamage(tile);
}

void BuildingBuyWeapon(WeaponType weaponType, Tile* tile, int cubeIndex)
{
    if (weaponType == WEAPON_NONE) return; // should not happen
    if (cubeIndex > 2) return;

    if (tile->building.blockCount <= cubeIndex)
    {
        PlaySound(soundForbidden);
        return;
    }

    if (tile->building.blocks[cubeIndex].weaponType == weaponType)
    {
        PlaySound(soundForbidden);
        return;
    }

    if (weaponType == WeaponWeak)
    {
        if (GetMoney() < weaponWeakPrice)
        {
            PlaySound(soundForbidden);
            return;
        }

        ModifyMoney(-weaponWeakPrice);
        tile->building.blocks[cubeIndex].weaponType = weaponType;
    }
    else if (weaponType == WeaponStrong)
    {
        if (GetResource(LavaType) < weaponStrongIronNeeded || GetMoney() < weaponStrongPrice)
        {
            PlaySound(soundForbidden);
            return;
        }
        
        ModifyMoney(-weaponStrongPrice);
        ModifyResource(LavaType, -weaponStrongIronNeeded);
        tile->building.blocks[cubeIndex].weaponType = weaponType;
    }
}

void BuildingSellBlock(Tile* tile, int blockIndex)
{
    if (blockIndex > 2) return;

    if (tile->building.blockCount <= blockIndex)
    {
        PlaySound(soundForbidden);
        return;
    }

    Block block = tile->building.blocks[blockIndex];
    BuildingMaterial material = block.buildingMaterial;
    WeaponType weaponType = block.weaponType;
    bool isPorquet = tile->building.isPorquet;
    BuildingDestroyBlock(&tile->building, blockIndex);

    if (material == Straw)
    {
        ModifyMoney(sellStrawCubePrice);
    }
    else if (material == Stick)
    {
        ModifyMoney(sellStickCubePrice);
    }
    else if (material == Brick)
    {
        ModifyMoney(sellBrickCubePrice);
    }

    if (weaponType == WeaponWeak)
    {
        ModifyMoney(sellPriceWeakWeapon);
    }
    else if (weaponType == WeaponStrong)
    {
        ModifyMoney(sellPriceStrongWeapon);
        ModifyResource(LavaType, sellIronReturnStrongWeapon);
    }

    if (isPorquet)
    {
        ModifyMoney(extraPorquetSellBonusMoney);
        ModifyResource(WheatType, extraPorquetSellBonusWheat);
        ModifyResource(WoodType, extraPorquetSellBonusWood);
        ModifyResource(ClayType, extraPorquetSellBonusClay);
    }
}

bool IsWholeSet(Tile* tile)
{
    if (tile->building.blockCount < 3) return false;
    BuildingMaterial firstMaterialFound = tile->building.blocks[0].buildingMaterial;
    if (tile->building.blocks[1].buildingMaterial != firstMaterialFound) return false;
    if (tile->building.blocks[2].buildingMaterial != firstMaterialFound) return false;
    return true;
}

void BuildingRelease()
{
    UnloadModel(pig);

    UnloadModel(strawCube);
    UnloadModel(stickCube);
    UnloadModel(brickCube);

    UnloadModel(weaponWeak);
    UnloadModel(weaponStrong);
}

int GetPorquetBlocksHp()
{
    return porquetBlocksHp;
}
