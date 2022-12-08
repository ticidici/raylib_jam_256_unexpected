#include "enemy.h"
#include "raymath.h"
#include "terrain.h"

static Vector3 YAW = {0, 1.0f, 0};

#define ENEMY_ATTACK_COLDOWN 1
#define ENEMY_MOVEMENT_COLDOWN 1
#define ENEMY_MOVEMENT_SPEED 10.0f
#define ENEMY_ROTATION_SPEED (ENEMY_MOVEMENT_SPEED * 90.0f)

#define ENEMY_COUNT 100
static Enemy enemies[ENEMY_COUNT];
static Model wolf;

void EnemyInit()
{
    wolf = LoadModel("resources/wolf.glb");

    int totalEnemies = 1;

    Enemy *enemy = &enemies[0];
    *enemy = (Enemy){0};
    enemy->position = TerrainGetTile(0, 0)->position;
    enemy->x = 0;
    enemy->y = 0;
    enemy->model = wolf;
    enemy->alive = true;
    /*for (int i = 0; i < totalEnemies; i++)
    {
        Enemy *enemy = &enemies[i];
        *enemy = (Enemy){0};
        enemy->position = TerrainGetTile(i, i)->position;
        enemy->x = i;
        enemy->y = i;
        enemy->model = wolf;
        enemy->alive = true;
    }*/
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

    Tile *target = (tileN->building.blockCount > 0)   ? tileN
                   : (tileE->building.blockCount > 0) ? tileE
                   : (tileS->building.blockCount > 0) ? tileS
                   : (tileW->building.blockCount > 0) ? tileW
                                                      : 0;

    Tile *tile = TerrainGetTile(enemy->x, enemy->y);

    // Rotate towards target
    float rotationDistR = enemy->rotation - enemy->rotationTarget;
    float rotationDistL = enemy->rotationTarget - enemy->rotation;
    float rotateSign = fabs(rotationDistL) > fabs(rotationDistR) ? -1 : 1;

    float rotationIncrement = ENEMY_ROTATION_SPEED * delta;
    if (enemy->rotation < enemy->rotationTarget - rotationIncrement * 1.1f)
    {
        enemy->rotation += rotationIncrement;
    }
    else if (enemy->rotation > enemy->rotationTarget + rotationIncrement * 1.1f)
    {
        enemy->rotation -= rotationIncrement;
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
    else if (target != 0)
    {
        // Attack
        int dirX, dirY;
        EnemyGetTargetDir(enemy, target->coordX, target->coordY, &dirX, &dirY);
        enemy->rotationTarget = EnemyGetTargetRotation(dirX, dirY);

        if (now - enemy->lastAttackTime >= ENEMY_ATTACK_COLDOWN)
        {
            enemy->lastAttackTime = now;
            enemy->lastMoveTime = now;
            BuildingDestroyBlock(&target->building);
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

            enemy->x += dirX;
            enemy->y += dirY;
        }
    }
}

Enemy *FindClosestEnemy(int tileX, int tileY)
{
    Enemy *closestEnemy = 0;
    float closestDistance = 99999;
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        Enemy *enemy = &enemies[i];
        if (enemy->alive)
        {
            int distance = abs(enemy->x - tileX) + abs(enemy->y - tileY);
            if (distance < closestDistance)
            {
                closestDistance = distance;
                closestEnemy = enemy;
            }
        }
    }
    return closestEnemy;
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
           : x == -1 ? -90
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
            DrawModelEx(enemy->model, enemy->position, YAW, enemy->rotation, (Vector3){0.025, 0.025, 0.025}, WHITE);
        }
    }
}
