
#include "bullet.h"
#include "raymath.h"
#include "terrain.h"
#include "enemy.h"

#define BULLET_COUNT 256

static Bullet bullets[BULLET_COUNT];

static Model smallBullet;
static Model bigBullet;

void BulletInit()
{
    smallBullet = LoadModelFromMesh(GenMeshSphere(0.2f, 6, 6));
    bigBullet = LoadModelFromMesh(GenMeshSphere(0.3f, 6, 6));
}

void BulletRelease()
{
    UnloadModel(smallBullet);
    UnloadModel(bigBullet);
}

void BulletUpdate()
{
    float delta = GetFrameTime();
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        Bullet *bullet = &bullets[i];
        if (bullet->alive)
        {
            Vector3 increment = Vector3Scale(bullet->direction, bullet->speed * delta);
            bullet->position = Vector3Add(bullet->position, increment);
            if (bullet->position.y <= 0)
            {
                // Collision
                Tile *tile = TerrainGetTile(bullet->targetX, bullet->targetY);

                if (tile && tile->enemy && tile->enemy->alive)
                {
                    Enemy *enemy = tile->enemy;
                    enemy->hp -= bullet->damage;
                    if (enemy->hp <= 0)
                    {
                        enemy->alive = false;
                    }
                }

                bullet->alive = false;
            }
        }
    }
}

void BulletRender()
{
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        Bullet *bullet = &bullets[i];
        if (bullet->alive)
        {
            DrawModel(bullet->model, bullet->position, 1.f, WHITE);
        }
    }
}

void BulletSpawn(Vector3 position, Enemy *target, WeaponType type)
{
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        Bullet *bullet = &bullets[i];
        if (!bullet->alive)
        {
            Tile *tile = TerrainGetTile(target->x, target->y);
            *bullet = (Bullet){0};
            bullet->alive = true;
            bullet->position = position;
            bullet->direction = Vector3Normalize(Vector3Subtract(tile->position, position));
            bullet->speed = type == WeaponWeak ? 60.0f : 40.0f;
            bullet->damage = type == WeaponWeak ? 5 : 2;
            bullet->targetX = tile->coordX;
            bullet->targetY = tile->coordY;
            bullet->model = type == WeaponWeak ? smallBullet : bigBullet;
            return;
        }
    }
}