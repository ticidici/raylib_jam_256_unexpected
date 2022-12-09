#include "game_state.h"

static bool isPaused = false;

//session stats
static float runsFinished = 0;

//run stats
static float runTimePassed = 0;

static float money = 0;

static float wheat = 10;
static float wood = 10;
static float clay = 10;
static float iron = 0;

static float thunderboltNextTime = 0;
static float thunderboltCooldown = 15.0f;
static Vector2 thunderboltDamage = { 999, 20 };//tile, adjacent

static float lavaUnlockTime = 300;
static float thunderboltUnlockTime = 420;
static float fastEnemyUnlockTime = 180;
static float rangedEnemyUnlockTime = 400;

static bool bossSpawned = false;
static float bossSpawnTime = 900;


void GameStateInit()
{
	isPaused = false;

	runTimePassed = 0;
	money = 11;
	wheat = 10;
	wood = 10;
	clay = 10;
	iron = 0;
	thunderboltNextTime = 0;

	bossSpawned = false;
}

void GameStateUpdate()
{
	if(!isPaused) runTimePassed += GetFrameTime();

	if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_P))
	{
		isPaused = !isPaused;
	}
}

bool IsPaused()
{
	return isPaused;
}

int GetMoney()
{
	return money;
}

int GetResource(TileType tileType)
{
	if (tileType == WheatType) return wheat;
	else if (tileType == WoodType) return wood;
	else if (tileType == ClayType) return clay;
	else if (tileType == LavaType) return iron;
	return 0;
}

float GetRunTime()
{
	return runTimePassed;
}

void ModifyMoney(int value)
{
	money += value;
	if (money > 999) money = 999;
	else if (money < 0) money = 0;
}

void ModifyResource(TileType tileType, int value)
{
	if (tileType == WheatType)
	{
		wheat += value;
		if (wheat > 999) wheat = 999;
		else if (wheat < 0) wheat = 0;
	}
	else if (tileType == WoodType)
	{
		wood += value;
		if (wood > 999) wood = 999;
		else if (wood < 0) wood = 0;
	}
	else if (tileType == ClayType)
	{
		clay += value;
		if (clay > 999) clay = 999;
		else if (clay < 0) clay = 0;
	}
	else if (tileType == LavaType)
	{
		iron += value;
		if (iron > 999) iron = 999;
		else if (iron < 0) iron = 0;
	}
}

bool IsThunderboltReady()
{
	return runTimePassed >= thunderboltNextTime;
}

Vector2 GetThunderboltDamage()
{
	return thunderboltDamage;
}

bool IsThunderboltUnlocked()
{
	if (runsFinished > 0 || runTimePassed >= thunderboltUnlockTime) return true;
	return false;
}

bool IsLavaUnlocked()
{
	if (runsFinished > 0 || runTimePassed >= lavaUnlockTime) return true;
	return false;
}

bool IsFastEnemyUnlocked()
{
	if (runTimePassed >= fastEnemyUnlockTime) return true;
	return false;
}

bool IsRangedEnemyUnlocked()
{
	if (runTimePassed >= rangedEnemyUnlockTime) return true;
	return false;
}

bool ShouldSpawnBoss()
{
	if (bossSpawned) return false;
	
	if (runTimePassed >= bossSpawnTime) return true;

	return false; 
}

void SetBossSpawned()
{
	bossSpawned = true;
}