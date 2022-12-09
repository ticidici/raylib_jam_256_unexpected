
#ifndef BULLET_H
#define BULLET_H

#include "raylib.h"

struct Enemy typedef Enemy;

enum WeaponType typedef WeaponType;

struct Bullet
{
    Vector3 position;
    Vector3 direction;
    int targetX;
    int targetY;
    Model model;
    float speed;
    int damage;
    Color tint;
    bool alive;
} typedef Bullet;

void BulletInit();

void BulletRelease();

void BulletUpdate();

void BulletRender();

void BulletSpawn(Vector3 position, Enemy *target, WeaponType type);

#endif
