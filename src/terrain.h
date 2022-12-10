#ifndef TERRAIN_H
#define TERRAIN_H

#include "raylib.h"
#include "building.h"

#define FORTRESS_SIZE 7     // have to be odd numbers
#define BATTLEFIELD_SIZE 13 // have to be odd numbers
#define BATTLEFIELD_RING_WIDTH (BATTLEFIELD_SIZE - FORTRESS_SIZE) / 2
#define MIDDLE_TILE_INDEX (BATTLEFIELD_SIZE - 1) / 2
#define FORTRESS_FIRST_TILE_INDEX BATTLEFIELD_RING_WIDTH
#define FORTRESS_LAST_TILE_INDEX BATTLEFIELD_SIZE - BATTLEFIELD_RING_WIDTH - 1
#define BATTLEFIELD_LAST_TILE_INDEX BATTLEFIELD_SIZE - 1
#define TILE_HALF_WIDTH 2.0f

struct Enemy typedef Enemy;

enum TileType
{
    GrassType,
    DirtType,
    WheatType,
    WoodType,
    ClayType,
    LavaType
} typedef TileType;

struct Tile
{
    Vector3 position;
    Model tileModel;
    TileType tileType;
    Building building;
    int coordX;
    int coordY;
    Enemy *enemy;

    //lava specifics
    float lavaNextBurn;
    float lavaNextExtendStart;
    float lavaNextExtendFinish;
    bool lavaAboutToExtend;
} typedef Tile;

extern int grassPrice;
extern int wheatPrice;
extern int woodPrice;
extern int clayPrice;
extern int lavaPrice;

// Public
void TerrainInit();

void TerrainRelease();

void TerrainUpdate();

void TerrainRender();

Tile *TerrainGetTileSelected();
Tile *TerrainGetTile(int x, int y);
void TerrainBuyTile(TileType tileType, Tile *tile);
bool ShouldShowTileInfo();

void EnemySteppedOnLava(Enemy* enemy, Tile* tile);
void BuildingLavaDamage(Tile* tile);
bool IsThunderboltReady();

// Private
void PerformThunderbolt();
void DrawGridCentered(float tileSpacing, int tileCount);
void UpdateTileSelector();
void CalculateTileEffects();

#endif