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

struct Tile
{
    Vector3 position;
    Model tileModel;

    Building building;

    // entity occupying?
} typedef Tile;

Tile battlefieldTiles[BATTLEFIELD_SIZE][BATTLEFIELD_SIZE];


// Public
void TerrainInit();

void TerrainRelease();

void TerrainUpdate();

void TerrainRender();

Tile GetTileSelected();

// Private
void DrawGridCentered(float tileSpacing, int tileCount);
void UpdateTileSelector();

#endif