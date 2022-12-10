#include "enemy.h"
#include "raymath.h"
#include "terrain.h"
#include <stdlib.h>
#include "game_state.h"

static Vector3 YAW = {0, 1.0f, 0};

#define ENEMY_ATTACK_COLDOWN 3
#define ENEMY_MOVEMENT_COLDOWN 3
#define ENEMY_MOVEMENT_SPEED 7.0f
#define ENEMY_ROTATION_SPEED (ENEMY_MOVEMENT_SPEED * 45.0f)

#define ENEMY_COUNT 100
static Enemy enemies[ENEMY_COUNT];
static Model wolf;

void EnemyInit()
{
    wolf = LoadModel("resources/wolf.glb");
}

void EnemyRelease()
{
    UnloadModel(wolf);
}

void EnemyUpdate()
{
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        Enemy *enemy = &enemies[i];
        if (enemy->alive)
        {
            EnemyUpdateOne(enemy);
        }
    }
}

void EnemyUpdateOne(Enemy *enemy)
{

    double now = GetTime();
    float delta = GetFrameTime();

    // Find near building
    Tile *tileN = TerrainGetTile(enemy->x, enemy->y - 1);
    Tile *tileE = TerrainGetTile(enemy->x + 1, enemy->y);
    Tile *tileS = TerrainGetTile(enemy->x, enemy->y + 1);
    Tile *tileW = TerrainGetTile(enemy->x - 1, enemy->y);

    Tile *target = (tileN && tileN->building.blockCount > 0)   ? tileN
                   : (tileE && tileE->building.blockCount > 0) ? tileE
                   : (tileS && tileS->building.blockCount > 0) ? tileS
                   : (tileW && tileW->building.blockCount > 0) ? tileW
                                                               : 0;

    Tile *tile = TerrainGetTile(enemy->x, enemy->y);

    // Rotate towards target
    float rotationDiff = enemy->rotationTarget - enemy->rotation;
    rotationDiff += (rotationDiff > 180)    ? -360
                    : (rotationDiff < -180) ? 360
                                            : 0;
    float rotationIncrement = ENEMY_ROTATION_SPEED * delta;
    if (fabs(rotationDiff) > rotationIncrement)
    {
        int sign = rotationDiff > 0 ? 1 : -1;
        enemy->rotation += rotationIncrement * sign;
    }
    else
    {
        enemy->rotation = enemy->rotationTarget;
    }

    float positionIncrement = ENEMY_MOVEMENT_SPEED * delta;
    Vector2 enemyPos = {enemy->position.x, enemy->position.z};
    Vector2 tilePos = {tile->position.x, tile->position.z};
    float tileDistance = Vector2Distance(enemyPos, tilePos);
    if (tileDistance > positionIncrement)
    {
        // Move
        Vector2 dir = Vector2Normalize(Vector2Subtract(tilePos, enemyPos));
        Vector2 increment = Vector2Scale(dir, positionIncrement);
        enemy->position.x += increment.x;
        enemy->position.z += increment.y;

        float x = ((TILE_HALF_WIDTH * 2) - tileDistance) / (TILE_HALF_WIDTH * 2);
        enemy->position.y = (1 - (x * x - x + 1)) * 6.0f;
    }
    else
    {
        if (!enemy->arrivedToTile)
        {
            enemy->arrivedToTile = true;
            if (tile->tileType == LavaType)
            {
                EnemySteppedOnLava(enemy, tile);
            }
            if (enemy->x == MIDDLE_TILE_INDEX && enemy->y == MIDDLE_TILE_INDEX) {
                // Enemy placed on center, enemy won
                state = GameOver;
            }
        }

        if (target != 0)
        {
            // Attack
            int dirX, dirY;
            EnemyGetTargetDir(enemy, target->coordX, target->coordY, &dirX, &dirY);
            enemy->rotationTarget = EnemyGetTargetRotation(dirX, dirY);

            if (now - enemy->lastAttackTime >= ENEMY_ATTACK_COLDOWN)
            {
                enemy->lastAttackTime = now;
                enemy->lastMoveTime = now;
                BuildingDestroyBlock(&target->building, 0);
            }
        }
        else
        {
            // Find center
            if (now - enemy->lastMoveTime > ENEMY_MOVEMENT_COLDOWN)
            {
                enemy->lastAttackTime = now;
                enemy->lastMoveTime = now;

                int dirX, dirY;
                EnemyGetTargetDir(enemy, MIDDLE_TILE_INDEX, MIDDLE_TILE_INDEX, &dirX, &dirY);
                enemy->rotationTarget = EnemyGetTargetRotation(dirX, dirY);


                int nextX = enemy->x + dirX;
                int nextY = enemy->y + dirY;

                Tile *nextTile = TerrainGetTile(nextX, nextY);
                if (nextTile->enemy)
                {
                    // Tile already has an enemy, wait for his moves
                }
                else
                {
                    tile->enemy = 0;
                    enemy->x = nextX;
                    enemy->y = nextY;
                    nextTile->enemy = enemy;
                    enemy->arrivedToTile = false;
                }

            }
        }
    }
}

Enemy *FindClosestEnemy(Vector3 position, float maxDistance)
{
    Enemy *closestEnemy = 0;
    float closestDistance = 99999;
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        Enemy *enemy = &enemies[i];
        if (enemy->alive)
        {
            Tile *enemyTile = TerrainGetTile(enemy->x, enemy->y);
            Vector3 enemyPosition = enemyTile->position;
            float distance = Vector3Distance(position, enemyPosition);
            if (distance < closestDistance && distance < maxDistance)
            {
                closestDistance = distance;
                closestEnemy = enemy;
            }
        }
    }
    return closestEnemy;
}

void EnemySpawn(int tileX, int tileY)
{
    Tile *tile = TerrainGetTile(tileX, tileY);
    if (!tile || tile->enemy != 0 || tile->building.blockCount > 0) // Can not spawn on an occupied tile
    {
        return;
    }
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        if (!(&enemies[i])->alive)
        {
            Enemy enemy = {0};
            enemy.x = tileX;
            enemy.y = tileY;
            enemy.position = tile->position;
            enemy.hp = 4;
            enemy.alive = true;
            enemy.model = wolf;
            enemies[i] = enemy;
            tile->enemy = &enemies[i];
            return;
        }
    }
}

void EnemyGetTargetDir(Enemy *enemy, int tileX, int tileY, int *dirX, int *dirY)
{
    int distX = tileX - enemy->x;
    int distY = tileY - enemy->y;
    *dirX = (int)Clamp(distX, -1, 1);
    *dirY = (int)Clamp(distY, -1, 1);
    if (*dirX != 0 && *dirY != 0)
    {
        enemy->lastMoveDirX = !enemy->lastMoveDirX;
        *dirX = enemy->lastMoveDirX ? *dirX : 0;
        *dirY = enemy->lastMoveDirX ? 0 : *dirY;
    }
}

float EnemyGetTargetRotation(int x, int y)
{
    return x == 1    ? 90
           : x == -1 ? 270
           : y == 1  ? 0
                     : 180;
}

void EnemyRender()
{
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        Enemy *enemy = &enemies[i];
        if (enemy->alive)
        {
            DrawModelEx(enemy->model, enemy->position, YAW, enemy->rotation, (Vector3){0.025f, 0.025f, 0.025f}, WHITE);
        }
    }
}
