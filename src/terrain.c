#include "terrain.h"

static Model tileSelector;
static Model grassTile;
static Model grass2Tile;
static Model lavaTile;
static Model dirtBrownTile;
static Model dirtGrayTile;

static bool isGridShown = false;

void TerrainInit()
{

    tileSelector = LoadModel("resources/tile_selector.gltf");

    grassTile = LoadModel("resources/grass_tile.gltf");
    grass2Tile = LoadModel("resources/grass2_tile.gltf");
    lavaTile = LoadModel("resources/lava_tile.gltf");
    dirtGrayTile = LoadModel("resources/dirtgray_tile.gltf");
    dirtBrownTile = LoadModel("resources/dirtbrown_tile.gltf");

    for (int i = 0; i < BATTLEFIELD_SIZE; i++)
    {
        for (int j = 0; j < BATTLEFIELD_SIZE; j++)
        {
            for (int k = 0; k < 4; k++) // max height, 3 floors + something standing on top
            {
                battlefieldTiles[i][j].positions[k].x = (i - MIDDLE_TILE_INDEX) * 4.0f;
                battlefieldTiles[i][j].positions[k].z = (j - MIDDLE_TILE_INDEX) * 4.0f;
                battlefieldTiles[i][j].positions[k].y = k * 2.0f;

                int randomValue = GetRandomValue(0, 100);
                if ((i >= FORTRESS_FIRST_TILE_INDEX && i <= FORTRESS_LAST_TILE_INDEX) && (j >= FORTRESS_FIRST_TILE_INDEX && j <= FORTRESS_LAST_TILE_INDEX))
                {
                    if (randomValue < 35)
                    {
                        battlefieldTiles[i][j].tileType = lavaTile;
                    }
                    // else if (randomValue < 55)
                    //{
                    //	battlefieldTiles[i][j].tileType = grassTile;
                    // }
                    else
                    {
                        battlefieldTiles[i][j].tileType = grass2Tile;
                    }
                }
                else
                {
                    if (randomValue < 35)
                    {
                        battlefieldTiles[i][j].tileType = dirtGrayTile;
                    }
                    else
                    {
                        battlefieldTiles[i][j].tileType = dirtBrownTile;
                    }
                }
            }
        }
    }
}

void TerrainRelease()
{
    UnloadModel(tileSelector);
    UnloadModel(grassTile);
    UnloadModel(grass2Tile);
    UnloadModel(lavaTile);
    UnloadModel(dirtGrayTile);
    UnloadModel(dirtBrownTile);
}

void TerrainUpdate()
{
    if (IsKeyPressed(KEY_G))
    {
        isGridShown = !isGridShown;
    }
}

void TerrainRender()
{

    DrawModel(grassTile, (Vector3){12, 0, 4}, 1.f, WHITE);

    DrawModel(tileSelector, battlefieldTiles[9][9].positions[0], 1.f, WHITE);

    for (int i = 0; i < BATTLEFIELD_SIZE; i++)
    {
        for (int j = 0; j < BATTLEFIELD_SIZE; j++)
        {
            DrawModel(battlefieldTiles[i][j].tileType, battlefieldTiles[i][j].positions[0], 1.f, WHITE);
        }
    }

    if (isGridShown)
    {
        DrawGridCentered(4.0f, BATTLEFIELD_SIZE);
    }
}

void DrawGridCentered(float tileSpacing, int tileCount)
{
    int middle = (tileCount - 1) / 2; // odd number
    if (tileCount % 2 == 0)
        middle = tileCount / 2;

    for (int i = 0; i < tileCount + 1; i++)
    {
        Vector3 startPosVertical = {(i - middle) * tileSpacing - tileSpacing / 2, 0.0f, (0 - middle) * tileSpacing - tileSpacing / 2};
        Vector3 endPosVertical = {(i - middle) * tileSpacing - tileSpacing / 2, 0.0f, (tileCount - middle) * tileSpacing - tileSpacing / 2};

        Vector3 startPosHorizontal = {(0 - middle) * tileSpacing - tileSpacing / 2, 0.0f, (i - middle) * tileSpacing - tileSpacing / 2};
        Vector3 endPosHorizontal = {(tileCount - middle) * tileSpacing - tileSpacing / 2, 0.0f, (i - middle) * tileSpacing - tileSpacing / 2};

        DrawLine3D(startPosVertical, endPosVertical, GREEN);
        DrawLine3D(startPosHorizontal, endPosHorizontal, GREEN);
    }
}
