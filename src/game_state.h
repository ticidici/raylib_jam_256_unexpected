#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "raylib.h"

enum TileType typedef TileType;

struct EnemyWave
{
    int enemiesCount;
    float startTime;
    float duration; // Time to next wave in seconds

} typedef EnemyWave;

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