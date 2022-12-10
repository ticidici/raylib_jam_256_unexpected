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
   bool alive;
   bool arrivedToTile;
} typedef Enemy;

// Public
void EnemyUpdate();

void EnemyInit();

void EnemyRelease();

void EnemyRender();

Enemy *FindClosestEnemy(Vector3 position, float maxDistance);

void EnemySpawn(int tileX, int tileY);

// Private
void EnemyUpdateOne(Enemy *enemy);

void EnemyGetTargetDir(Enemy *enemy, int tileX, int tileY, int *dirX, int *dirY);

float EnemyGetTargetRotation(int x, int y);

#endif
