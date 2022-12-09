#include "building.h"
#include "enemy.h"

#define BLOCK_HEIGHT 2.0f

static Model weaponWeak;
static Model weaponStrong;

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
        if (block->weaponType == WeaponWeak) DrawModel(weaponWeak, blockPosition, 1.f, WHITE);
        else if (block->weaponType == WeaponStrong) DrawModel(weaponStrong, blockPosition, 1.f, WHITE);
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