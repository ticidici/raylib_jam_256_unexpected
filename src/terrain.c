#include "terrain.h"
#include "utils.h"
#include "uiManager.h"

static Model tileSelector;
static Model grassTile;
static Model lavaTile;
static Model dirtBrownTile;
static Model dirtGrayTile;
static Model wheatTile;
static Model woodTile;
static Model clayTile;

static Model red1;
static Model red2;
static Model red3;
static Model red4;

static bool isGridShown = false;

static Tile* tileHovered;
static bool isTileSelected = false;

static Tile battlefieldTiles[BATTLEFIELD_SIZE][BATTLEFIELD_SIZE] = {0};

bool showHoveredTileInfo = false;


Tile *TerrainGetTileSelected()
{
    return tileHovered;
}

Tile *TerrainGetTile(int x, int y)
{
    if (x < 0 || x >= BATTLEFIELD_SIZE || y < 0 || y > BATTLEFIELD_SIZE)
    {
        return 0;
    }
    return &battlefieldTiles[x][y];
}

void TerrainInit()
{

    tileSelector = LoadModel("resources/tile_selector.gltf");

    grassTile = LoadModel("resources/grass2_tile.gltf");
    lavaTile = LoadModel("resources/lava_tile.gltf");
    dirtGrayTile = LoadModel("resources/dirtgray_tile.gltf");
    dirtBrownTile = LoadModel("resources/dirtbrown_tile.gltf");

    red1 = LoadModel("resources/red1.gltf");
    red2 = LoadModel("resources/red2.gltf");
    red3 = LoadModel("resources/red3.gltf");
    red4 = LoadModel("resources/red4.gltf");

    tileHovered = &battlefieldTiles[MIDDLE_TILE_INDEX][MIDDLE_TILE_INDEX];

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
                        battlefieldTiles[i][j].tileModel = lavaTile;
                        battlefieldTiles[i][j].tileType = LavaType;
                    }
                    // else if (randomValue < 55)
                    //{
                    //	battlefieldTiles[i][j].tileModel = grassTile;
                    // }
                    else
                    {
                        battlefieldTiles[i][j].tileModel = grassTile;
                        battlefieldTiles[i][j].tileType = GrassType;
                    }
                }
                else
                {
                    if (randomValue < 35)
                    {
                        battlefieldTiles[i][j].tileModel = dirtGrayTile;
                        battlefieldTiles[i][j].tileType = DirtType;
                    }
                    else
                    {
                        battlefieldTiles[i][j].tileModel = dirtBrownTile;
                        battlefieldTiles[i][j].tileType = DirtType;
                    }
                }
                battlefieldTiles[i][j].coordX = i;
                battlefieldTiles[i][j].coordY = j;
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
    UnloadModel(lavaTile);
    UnloadModel(dirtGrayTile);
    UnloadModel(dirtBrownTile);
    UnloadModel(wheatTile);
    UnloadModel(woodTile);
    UnloadModel(clayTile);
    UnloadModel(red1);
    UnloadModel(red2);
    UnloadModel(red3);
    UnloadModel(red4);
}

void TerrainUpdate()
{
    bool tileJustDeselected = false;

    if (isTileSelected)
    {
        //if clicked on option confirm and deselect (even if it is not executed)
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            //if button is clicked don't deselect
            if (UiIsTileGrassButtonPressed())
            {
                TerrainBuyTile(GrassType, tileHovered);

            }
            else if (UiIsTileLavaButtonPressed())
            {
                TerrainBuyTile(LavaType, tileHovered);
            }
            
            
            else if (!IsPointInsideTileInScreenSpace(GetMousePosition(), tileHovered, TILE_HALF_WIDTH))
            {
                isTileSelected = false;
                tileJustDeselected = true;
                UiHideTileButtons();
            }
        }

        //if right clicked or cancel button, deselect tile
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || IsKeyPressed(KEY_ESCAPE))
        {
            isTileSelected = false;
            tileJustDeselected = true;
            UiHideTileButtons();
        }
    }

    //we update selector before doing anything with it
    UpdateTileSelector();

    if (!tileJustDeselected && !isTileSelected)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            //if it's outside the fortress we cannot select, just switch the tile info toggle
            if (tileHovered->coordX < FORTRESS_FIRST_TILE_INDEX || tileHovered->coordX > FORTRESS_LAST_TILE_INDEX
                || tileHovered->coordY < FORTRESS_FIRST_TILE_INDEX || tileHovered->coordY > FORTRESS_LAST_TILE_INDEX)
            {
                showHoveredTileInfo = !showHoveredTileInfo;
            }
            else
            {
                isTileSelected = true;
                UiShowTileButtons();
            }
        }
    }

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

    if (IsKeyPressed(KEY_I) && !isTileSelected)
    {
        showHoveredTileInfo = !showHoveredTileInfo;
    }
}

void TerrainRender()
{

    DrawModel(tileSelector, tileHovered->position, 1.f, WHITE);

    for (int i = 0; i < BATTLEFIELD_SIZE; i++)
    {
        for (int j = 0; j < BATTLEFIELD_SIZE; j++)
        {
            Tile *tile = &battlefieldTiles[i][j];
            BuildingRender(&tile->building, tile->position);
            DrawModel(battlefieldTiles[i][j].tileModel, battlefieldTiles[i][j].position, 1.f, WHITE);
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

void UpdateTileSelector()
{
    if (isTileSelected) return;

    bool tileFound = false;
    Vector2 mousePosition = GetMousePosition();
    for (int i = 0; i < BATTLEFIELD_SIZE; i++)
    {
        for (int j = 0; j < BATTLEFIELD_SIZE; j++)
        {
            Tile* candidateTile = &battlefieldTiles[i][j];
            tileFound = IsPointInsideTileInScreenSpace(mousePosition, candidateTile, TILE_HALF_WIDTH);

            if (tileFound)
            {           
                tileHovered = candidateTile;
                return;
            }
        }
        //if (tileFound) return;
    }
}

bool ShouldShowTileInfo()
{
    if (isTileSelected) return true;
    return showHoveredTileInfo;
}

int TerrainGetTileCost(TileType tileType)
{
    //TODO
}

void TerrainBuyTile(TileType tileType, Tile* tile)
{
    if (tile->coordX < FORTRESS_FIRST_TILE_INDEX || tile->coordX > FORTRESS_LAST_TILE_INDEX
        || tile->coordY < FORTRESS_FIRST_TILE_INDEX || tile->coordY > FORTRESS_LAST_TILE_INDEX)
    {
        //TODO throw sound or message
        return;
    }

    tile->tileType = tileType;
    switch (tileType)
    {
        case GrassType:
            tile->tileModel = grassTile;
            break;
        case LavaType:
            tile->tileModel = lavaTile;
            break;
    
        case WheatType:
            tile->tileModel = wheatTile;
            break;
    
        case WoodType:
            tile->tileModel = woodTile;
            break;
    
        case ClayType:
            tile->tileModel = clayTile;
            break;
    }
}
