#include "enemy.h"
#include "raymath.h"

static Vector3 YAW = {0, 1.0f, 0};

void EnemyUpdate(Enemy *enemy)
{
    Vector3 direction = Vector3Subtract(enemy->position, enemy->target);
    float distanceSqr = Vector3LengthSqr(direction);
    direction = Vector3Normalize(direction);
    enemy->rotation = atan2f(direction.z, direction.x) * RAD2DEG;
    if (distanceSqr > (enemy->range * enemy->range) - 10.0f)
    {
        // Move behaviour
        Vector3 direction = Vector3Normalize(Vector3Subtract(enemy->target, enemy->position));
        Vector3 increment = Vector3Scale(direction, enemy->speed * GetFrameTime());
        enemy->position = Vector3Add(enemy->position, increment);
    }
    else
    {
        // Atack behaviour
        double now = GetTime();
        if (now - enemy->lastAttackTime >= enemy->coldown)
        {
            enemy->lastAttackTime = now;
            // Atack
        }
    }
}

void EnemyRender(Enemy *enemy)
{
    DrawModelEx(enemy->model, enemy->position, YAW, enemy->rotation, (Vector3){0.025, 0.025, 0.025}, WHITE);
}
