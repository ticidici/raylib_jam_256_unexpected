#include "building.h"

void BuildingUpdate(Building *building)
{
}

void BuildingRender(Building *building, Vector3 position)
{
    for (int i = 0; i < building->blockCount; i++)
    {
        Block *block = &building->blocks[i];
        // TODO: Add rotation for 360 no scope
        Vector3 blockPosition = {position.x, position.y + i * 2.0f, position.z};
        DrawModel(block->model, blockPosition, 1.f, WHITE);
    }
}