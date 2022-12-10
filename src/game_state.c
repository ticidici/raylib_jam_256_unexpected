#include "game_state.h"
#include "enemy.h"
#include "terrain.h"
#include "sound.h"

//session stats
static float runsFinished = 0;

//run stats
static float runTimePassed = 0;

static float money = 0;

static float wheat = 10;
static float wood = 10;
static float clay = 10;
static float iron = 0;

static float lavaUnlockTime = 300;
static float thunderboltUnlockTime = 420;
static float fastEnemyUnlockTime = 180;
static float strongEnemyUnlockTime = 400;

static bool bossSpawned = false;
static float bossSpawnTime = 900;

static EnemyWave wave;

GameState state;


void GameStateInit()
{
	state = Start;

	runTimePassed = 0;
	money = 11;
	wheat = 10;
	wood = 10;
	clay = 10;
	iron = 0;

	bossSpawned = false;

	wave.startTime = 0;
	wave.duration = 3;
	wave.enemiesCount = 0;

}

void GameStateUpdate()
{
	if(state == Running) runTimePassed += GetFrameTime();

	if (state == Start && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		state = Running;
	}

	if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_P))
	{
		state = state == Running ? Paused : Running;
	}

	if (runTimePassed - wave.startTime >= wave.duration)
	{
		if (wave.number > 0) {
			// PlaySoundMulti(waveSound);
		}

		// Next wave
		wave.duration = 20;
		wave.number += 1;
		wave.enemiesCount += 2;
		wave.startTime = runTimePassed;


		for (int i = 0; i < wave.enemiesCount; i++)
		{
			Tile* freeSpawnTiles[BATTLEFIELD_SIZE * BATTLEFIELD_SIZE] = { 0 };
			int freeSpawnTilesCount = 0;
			for (int x = 0; x < BATTLEFIELD_SIZE; x++)
			{
				for (int y = 0; y < BATTLEFIELD_SIZE; y++)
				{
					if (x == 0 || x == BATTLEFIELD_SIZE - 1 || y == 0 || y == BATTLEFIELD_SIZE - 1)
					{
						Tile* tile = TerrainGetTile(x, y);
						if (!tile->enemy)
						{
							freeSpawnTiles[freeSpawnTilesCount] = TerrainGetTile(x, y);
							freeSpawnTilesCount++;
						}
					}
				}
			}
			if (freeSpawnTilesCount > 0)
			{
				int tileIndex = GetRandomValue(0, freeSpawnTilesCount - 1);
				Tile* spawnTile = freeSpawnTiles[tileIndex];
				EnemySpawn(spawnTile->coordX, spawnTile->coordY);
			}
		}
	}
}

int GetMoney()
{
	return money;
}

int GetResource(TileType tileType)
{
	if (tileType == WheatType)
		return wheat;
	else if (tileType == WoodType)
		return wood;
	else if (tileType == ClayType)
		return clay;
	else if (tileType == LavaType)
		return iron;
	return 0;
}

float GetRunTime()
{
	return runTimePassed;
}

void ModifyMoney(int value)
{
	money += value;
	if (money > 999)
		money = 999;
	else if (money < 0)
		money = 0;
}

void ModifyResource(TileType tileType, int value)
{
	if (tileType == WheatType)
	{
		wheat += value;
		if (wheat > 999)
			wheat = 999;
		else if (wheat < 0)
			wheat = 0;
	}
	else if (tileType == WoodType)
	{
		wood += value;
		if (wood > 999)
			wood = 999;
		else if (wood < 0)
			wood = 0;
	}
	else if (tileType == ClayType)
	{
		clay += value;
		if (clay > 999)
			clay = 999;
		else if (clay < 0)
			clay = 0;
	}
	else if (tileType == LavaType)
	{
		iron += value;
		if (iron > 999)
			iron = 999;
		else if (iron < 0)
			iron = 0;
	}
}

bool IsThunderboltUnlocked()
{
	if (runsFinished > 0 || runTimePassed >= thunderboltUnlockTime)
		return true;
	return false;
}

bool IsLavaUnlocked()
{
	if (runsFinished > 0 || runTimePassed >= lavaUnlockTime)
		return true;
	return false;
}

bool IsFastEnemyUnlocked()
{
	if (runTimePassed >= fastEnemyUnlockTime)
		return true;
	return false;
}

bool IsStrongEnemyUnlocked()
{
	if (runTimePassed >= strongEnemyUnlockTime)
		return true;
	return false;
}

bool ShouldSpawnBoss()
{
	if (bossSpawned)
		return false;

	if (runTimePassed >= bossSpawnTime)
		return true;

	return false;
}

void SetBossSpawned()
{
	bossSpawned = true;
}