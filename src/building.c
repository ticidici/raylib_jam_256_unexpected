#include "building.h"
#include "enemy.h"
#include "raymath.h"

#define BLOCK_HEIGHT 2.0f
#define BLOCK_FALL_SPEED 6.0f
#define BLOCK_ATTACK_COLDOWN 1.0f

#define BULLET_COUNT 256

static Bullet bullets[BULLET_COUNT];

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

        // Atack
        if (now - block->lastAttackTime > BLOCK_ATTACK_COLDOWN)
        {
            Enemy *targetEnemy = FindClosestEnemy(position, 10);
            if (targetEnemy)
            {
                block->lastAttackTime = now;
                BuildingSpawnBullet(position, targetEnemy->position);
            }
        }
    }

    // Update bullets
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        Bullet *bullet = &bullets[i];
        if (bullet->alive)
        {
            Vector3 dir = Vector3Normalize(Vector3Subtract(bullet->target, bullet->position));
            Vector3 increment = Vector3Scale(dir, 0.5f * delta);
            bullet->position = Vector3Add(bullet->position, increment);
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
    // Update bullets
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        Bullet *bullet = &bullets[i];
        if (bullet->alive)
        {
            DrawModel(bullet->model, bullet->position, 1.f, WHITE);
        }
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

void BuildingSpawnBullet(Vector3 position, Vector3 target)
{
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        Bullet *bullet = &bullets[i];
        if (!bullet->alive)
        {
            *bullet = (Bullet){0};
            bullet->alive = true;
            bullet->position = position;
            bullet->target = target;
            bullet->model = LoadModelFromMesh(GenMeshSphere(0.5f, 10, 10));
            return;
        }
    }
}