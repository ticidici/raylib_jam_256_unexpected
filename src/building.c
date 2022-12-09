#include "building.h"
#include "enemy.h"
#include "raymath.h"
#include "bullet.h"

#define BLOCK_HEIGHT 2.0f
#define BLOCK_FALL_SPEED 6.0f
#define BLOCK_ATTACK_COLDOWN 1.0f

void BuildingUpdate(Building *building, Vector3 position)
{
    float delta = GetFrameTime();
    building->destroyOffset -= BLOCK_FALL_SPEED * delta;
    if (building->destroyOffset < 0)
    {
        building->destroyOffset = 0;
    }

    double now = GetTime();
    for (int i = 0; i < building->blockCount; i++)
    {
        Block *block = &building->blocks[i];
        Vector3 blockPosition = position;
        blockPosition.y += (BLOCK_HEIGHT / 2) * (i + 1);

        // Atack
        if (now - block->lastAttackTime > BLOCK_ATTACK_COLDOWN)
        {
            Enemy *targetEnemy = FindClosestEnemy(position, 10);
            if (targetEnemy)
            {
                block->lastAttackTime = now;
                BulletSpawn(blockPosition, targetEnemy, BulletSmall);
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
