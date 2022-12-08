#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"

struct Tile typedef Tile;

struct Enemy
{
   int hp;
   int damage;
   double lastAttackTime;
   int x;
   int y;
   double lastMoveTime;
   bool lastMoveDirX;
   Vector3 position;
   float rotation;
   float rotationTarget;
   Model model;
} typedef Enemy;

// Public
void EnemyUpdate(Enemy *enemy);

void EnemyRender(Enemy *enemy);

// Private
void EnemyGetTargetDir(Enemy *enemy, int tileX, int tileY, int *dirX, int *dirY);

float EnemyGetTargetRotation(int x, int y);

#endif
