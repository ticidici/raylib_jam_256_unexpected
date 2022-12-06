#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"

struct Enemy
{
   int hp;
   int damage;
   float coldown;
   double lastAttackTime;
   float speed;
   float range;
   Vector3 position;
   float rotation; // degrees of yaw
   Vector3 target;
   Model model;
} typedef Enemy;

// Public
void EnemyUpdate(Enemy *enemy);

void EnemyRender(Enemy *enemy);

#endif
