#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "raylib.h"

enum TileType typedef TileType;

struct EnemyWave
{
    int number;
    int enemiesCount;
    float startTime;
    float duration; // Time to next wave in seconds

} typedef EnemyWave;

enum GameState {
    Start,
    Running,
    Paused,
    GameOver,
} typedef GameState;

extern GameState state;

void GameStateInit();
void GameStateUpdate();

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
bool IsStrongEnemyUnlocked();
bool ShouldSpawnBoss();
void SetBossSpawned();

#endif