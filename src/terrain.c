#include "terrain.h"

static Model tileSelector;
static Model grassTile;
static Model grass2Tile;
static Model lavaTile;
static Model dirtBrownTile;
static Model dirtGrayTile;

static Model red1;
static Model red2;
static Model red3;
static Model red4;

static bool isGridShown = false;

static Tile battlefieldTiles[BATTLEFIELD_SIZE][BATTLEFIELD_SIZE] = {0};

Tile *TerrainGetTile(int x, int y)
{
    return &battlefieldTiles[x][y];
}

void TerrainInit()
{

    tileSelector = LoadModel("resources/tile_selector.gltf");

    grassTile = LoadModel("resources/grass_tile.gltf");
    grass2Tile = LoadModel("resources/grass2_tile.gltf");
    lavaTile = LoadModel("resources/lava_tile.gltf");
    dirtGrayTile = LoadModel("resources/dirtgray_tile.gltf");
    dirtBrownTile = LoadModel("resources/dirtbrown_tile.gltf");

    red1 = LoadModel("resources/red1.gltf");
    red2 = LoadModel("resources/red2.gltf");
    red3 = LoadModel("resources/red3.gltf");
    red4 = LoadModel("resources/red4.gltf");

    for (int i = 0; i < BATTLEFIELD_SIZE; i++)
    {
        for (int j = 0; j < BATTLEFIELD_SIZE; j++)
        {
            for (int k = 0; k < 4; k++) // max height, 3 floors + something standing on top
            {
                battlefieldTiles[i][j].position.x = (i - MIDDLE_TILE_INDEX) * 4.0f;
                battlefieldTiles[i][j].position.z = (j - MIDDLE_TILE_INDEX) * 4.0f;
                battlefieldTiles[i][j].position.y = 0.f;

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

    Building *building = &battlefieldTiles[FORTRESS_FIRST_TILE_INDEX][FORTRESS_FIRST_TILE_INDEX].building;
    building->blocks[0].model = red1;
    building->blockCount = 1;

    Building *building2 = &battlefieldTiles[FORTRESS_LAST_TILE_INDEX][FORTRESS_LAST_TILE_INDEX].building;
    building2->blocks[0].model = red2;
    building2->blockCount = 1;

    Building *building3 = &battlefieldTiles[MIDDLE_TILE_INDEX][MIDDLE_TILE_INDEX].building;
    building3->blocks[0].model = red1;
    building3->blocks[1].model = red1;
    building3->blocks[2].model = red4;
    building3->blockCount = 3;
}

void TerrainRelease()
{
    UnloadModel(tileSelector);
    UnloadModel(grassTile);
    UnloadModel(grass2Tile);
    UnloadModel(lavaTile);
    UnloadModel(dirtGrayTile);
    UnloadModel(dirtBrownTile);
    UnloadModel(red1);
    UnloadModel(red2);
    UnloadModel(red3);
    UnloadModel(red4);
}

void TerrainUpdate()
{
    if (IsKeyPressed(KEY_G))
    {
        isGridShown = !isGridShown;
    }
    for (int i = 0; i < BATTLEFIELD_SIZE; i++)
    {
        for (int j = 0; j < BATTLEFIELD_SIZE; j++)
        {
            Tile *tile = &battlefieldTiles[i][j];
            BuildingUpdate(&tile->building);
        }
    }
}

void TerrainRender()
{

    DrawModel(grassTile, (Vector3){12, 0, 4}, 1.f, WHITE);

    DrawModel(tileSelector, battlefieldTiles[9][9].position, 1.f, WHITE);

    for (int i = 0; i < BATTLEFIELD_SIZE; i++)
    {
        for (int j = 0; j < BATTLEFIELD_SIZE; j++)
        {
            Tile *tile = &battlefieldTiles[i][j];
            BuildingRender(&tile->building, tile->position);
            DrawModel(battlefieldTiles[i][j].tileType, battlefieldTiles[i][j].position, 1.f, WHITE);
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
