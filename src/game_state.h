#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "raylib.h"
#include "terrain.h"


void GameStateInit();
void GameStateUpdate();
bool IsPaused();
int GetMoney();
int GetResource(TileType tileType);
float GetRunTime();
void ModifyMoney(int value);
void ModifyResource(TileType tileType, int value);
bool IsThunderboltReady();
Vector2 GetThunderboltDamage();
bool IsThunderboltUnlocked();
bool IsLavaUnlocked();

bool IsFastEnemyUnlocked();
bool IsRangedEnemyUnlocked();
bool ShouldSpawnBoss();
void SetBossSpawned();

#endif