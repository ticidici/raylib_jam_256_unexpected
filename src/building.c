#include "building.h"
#include "enemy.h"

#define BLOCK_HEIGHT 2.0f

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