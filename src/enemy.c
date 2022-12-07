#include "enemy.h"
#include "raymath.h"
#include "terrain.h"

static Vector3 YAW = {0, 1.0f, 0};

#define ENEMY_ATTACK_COLDOWN 1
#define ENEMY_MOVEMENT_COLDOWN 1

void EnemyUpdate(Enemy *enemy)
{

    double now = GetTime();

    // Find near target
    Tile *tileN = TerrainGetTile(enemy->x, enemy->y - 1);
    Tile *tileE = TerrainGetTile(enemy->x + 1, enemy->y);
    Tile *tileS = TerrainGetTile(enemy->x, enemy->y + 1);
    Tile *tileW = TerrainGetTile(enemy->x - 1, enemy->y);

    Tile *target = (tileN->building.blockCount > 0)   ? tileN
                   : (tileE->building.blockCount > 0) ? tileE
                   : (tileS->building.blockCount > 0) ? tileS
                   : (tileW->building.blockCount > 0) ? tileW
                                                      : 0;

    if (target != 0)
    {
        // Attack
        if (now - enemy->lastAttackTime >= ENEMY_ATTACK_COLDOWN)
        {
            enemy->lastAttackTime = now;
            enemy->lastMoveTime = now;
            target->building.blockCount--;
            // Atack
        }
    }
    else
    {
        // Move
        int distX = MIDDLE_TILE_INDEX - enemy->x;
        int distY = MIDDLE_TILE_INDEX - enemy->y;

        int dirX = (distX <= -1)  ? -1
                   : (distX >= 1) ? 1
                                  : 0;
        int dirY = (distY <= -1)  ? -1
                   : (distY >= 1) ? 1
                                  : 0;
        if (now - enemy->lastMoveTime > ENEMY_MOVEMENT_COLDOWN)
        {
            enemy->lastAttackTime = now;
            enemy->lastMoveTime = now;
            if (dirX != 0 && dirY != 0)
            {
                enemy->lastMoveDirX = !enemy->lastMoveDirX;
                dirX = enemy->lastMoveDirX ? dirX : 0;
                dirY = enemy->lastMoveDirX ? 0 : dirY;
            }
            enemy->x += dirX;
            enemy->y += dirY;
            Tile *nextTile = TerrainGetTile(enemy->x, enemy->y);
            enemy->position.x = nextTile->position.x;
            enemy->position.z = nextTile->position.z;
        }
    }

    // Move
}

void EnemyRender(Enemy *enemy)
{
    DrawModelEx(enemy->model, enemy->position, YAW, enemy->rotation, (Vector3){0.025, 0.025, 0.025}, WHITE);
}
