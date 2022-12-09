#include "terrain.h"
#include "utils.h"
#include "uiManager.h"
#include "game_state.h"

static Model tileSelector;
static Model grassTile;
static Model grassAltTile;
static Model lavaTile;
static Model dirtBrownTile;
static Model dirtGrayTile;
static Model wheatTile;
static Model woodTile;
static Model clayTile;

static bool isGridShown = false;

static Tile* tileHovered;
static bool isTileSelected = false;

static Tile battlefieldTiles[BATTLEFIELD_SIZE][BATTLEFIELD_SIZE] = {0};

bool showHoveredTileInfo = false;

//tile price
static int grassPrice = 1;//only useful to stop lava
static int wheatPrice = 2;
static int woodPrice = 5;
static int clayPrice = 11;
static int lavaPrice = 11;

//time tile needs to give resources
static float wheatYieldTime = 5;//save next timer value and check in game state instead of really counting
static float woodYieldTime = 10;
static float clayYieldTime = 15;
static float ironYieldTime = 30;
static float wholeSetBonusYield = 1;//+1 every yield if have all the cubes from the same set

//lava special effect
static float lavaHurtTime = 1;
static float lavaDamage = 1;
static float lavaStartExtendTime = 30;
static float lavaFinishExtendTime = 45;

//resources exchange rate
static Vector2 wheatWoodRate = { 3, 1 };//give X, receive Y
static Vector2 wheatClayRate = { 6, 1 };
static Vector2 wheatIronRate = { 10, 1 };

static Vector2 woodWheatRate = { 1, 2 };
static Vector2 woodClayRate = { 4, 1 };
static Vector2 woodIronRate = { 8, 1 };

static Vector2 clayWheatRate = { 1, 3 };
static Vector2 clayWoodRate = { 1, 2 };
static Vector2 clayIronRate = { 5, 1 };

static Vector2 ironWheatRate = { 1, 5 };
static Vector2 ironWoodRate = { 1, 4 };
static Vector2 ironClayRate = { 1, 3 };


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
    grassAltTile = LoadModel("resources/grass3_tile.gltf");
    lavaTile = LoadModel("resources/lava_tile.gltf");
    wheatTile = LoadModel("resources/wheat_tile.gltf");
    woodTile = LoadModel("resources/wood_tile.gltf");
    clayTile = LoadModel("resources/clay_tile.gltf");
    dirtGrayTile = LoadModel("resources/dirtgray_tile.gltf");
    dirtBrownTile = LoadModel("resources/dirtbrown_tile.gltf");

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
                    if (randomValue < 10)
                    {
                        battlefieldTiles[i][j].tileModel = lavaTile;
                        battlefieldTiles[i][j].tileType = LavaType;
                    }
                    else if (randomValue < 20)
                    {
                        battlefieldTiles[i][j].tileModel = wheatTile;
                        battlefieldTiles[i][j].tileType = WheatType;
                    }
                    else if (randomValue < 30)
                    {
                        battlefieldTiles[i][j].tileModel = woodTile;
                        battlefieldTiles[i][j].tileType = WoodType;
                    }
                    else if (randomValue < 40)
                    {
                        battlefieldTiles[i][j].tileModel = clayTile;
                        battlefieldTiles[i][j].tileType = ClayType;
                    }
                    else
                    {
                        if (GetRandomValue(0, 100) < 50) battlefieldTiles[i][j].tileModel = grassTile;
                        else battlefieldTiles[i][j].tileModel = grassAltTile;
                        battlefieldTiles[i][j].tileType = GrassType;
                    }
                }
                else
                {
                    battlefieldTiles[i][j].tileType = DirtType;
                    if (randomValue < 35)
                    {
                        battlefieldTiles[i][j].tileModel = dirtGrayTile;
                    }
                    else
                    {
                        battlefieldTiles[i][j].tileModel = dirtBrownTile;
                    }
                }
                battlefieldTiles[i][j].coordX = i;
                battlefieldTiles[i][j].coordY = j;
            }
        }
    }

    Building *building = &battlefieldTiles[FORTRESS_FIRST_TILE_INDEX][FORTRESS_FIRST_TILE_INDEX].building;
    building->blocks[0].buildingMaterial = Straw;
    building->blockCount = 1;

    Building *building2 = &battlefieldTiles[FORTRESS_LAST_TILE_INDEX][FORTRESS_LAST_TILE_INDEX].building;
    building2->blocks[0].buildingMaterial = Stick;
    building2->blockCount = 1;
    Building *building3 = &battlefieldTiles[MIDDLE_TILE_INDEX][MIDDLE_TILE_INDEX].building;
    building3->isPorquet = true;
    building3->blocks[0].buildingMaterial = Straw;
    building3->blocks[1].buildingMaterial = Brick;
    building3->blocks[2].buildingMaterial = Stick;
    building3->blockCount = 3;

    Building* building4 = &battlefieldTiles[7][6].building;
    building4->blocks[2].buildingMaterial = Stick;
    building4->blocks[2].buildingMaterial = Brick;
    building4->blockCount = 2;

    Building* building5 = &battlefieldTiles[8][7].building;
    building5->blocks[0].buildingMaterial = Brick;
    building5->blocks[0].weaponType = WeaponWeak;
    building5->blocks[1].buildingMaterial = Brick;
    building5->blocks[1].weaponType = WeaponWeak;
    building5->blocks[2].buildingMaterial = Brick;
    building5->blocks[2].weaponType = WeaponStrong;
    building5->blockCount = 3;

}

void TerrainRelease()
{
    UnloadModel(tileSelector);
    UnloadModel(grassTile);
    UnloadModel(grassAltTile);
    UnloadModel(lavaTile);
    UnloadModel(dirtGrayTile);
    UnloadModel(dirtBrownTile);
    UnloadModel(wheatTile);
    UnloadModel(woodTile);
    UnloadModel(clayTile);
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
            //tile
            if (UiIsTileGrassButtonPressed())
            {
                TerrainBuyTile(GrassType, tileHovered);

            }
            else if (UiIsTileWheatButtonPressed())
            {
                TerrainBuyTile(WheatType, tileHovered);
            }
            else if (UiIsTileWoodButtonPressed())
            {
                TerrainBuyTile(WoodType, tileHovered);
            }
            else if (UiIsTileClayButtonPressed())
            {
                TerrainBuyTile(ClayType, tileHovered);
            }
            else if (UiIsTileLavaButtonPressed())
            {
                TerrainBuyTile(LavaType, tileHovered);
            }
            //cube
            else if (UiIsCubeStrawButtonPressed())
            {
                BuildingBuyCube(Straw, tileHovered);
            }
            else if (UiIsCubeStickButtonPressed())
            {
                BuildingBuyCube(Stick, tileHovered);
            }
            else if (UiIsCubeBrickButtonPressed())
            {
                BuildingBuyCube(Brick, tileHovered);
            }
            //weapon
            else if (UiIsWeaponWeakButtonPressed())
            {
                if (tileHovered->building.blockCount > 0)
                {
                    //BuildingBuyWeapon(WeaponWeak, tileHovered);
                }
            }
            else if (UiIsWeaponStrongButtonPressed())
            {
                if (tileHovered->building.blockCount > 0)
                {
                    //BuildingBuyWeapon(WeaponStrong, tileHovered);
                }
            }

            //outside tile buttons
            else if (!IsPointInsideTileInScreenSpace(GetMousePosition(), tileHovered, TILE_HALF_WIDTH))
            {
                isTileSelected = false;
                tileJustDeselected = true;
                UiHideTileButtons();
            }
            else
            {
                showHoveredTileInfo = !showHoveredTileInfo;
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
            BuildingUpdate(&tile->building, tile->position);
        }
    }

    if (IsKeyPressed(KEY_I) /*&& !isTileSelected*/)
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
    //if (isTileSelected) return true;
    return showHoveredTileInfo;
}

void TerrainBuyTile(TileType tileType, Tile *tile)
{
    if (tileType == tile->tileType) return;

    if (tile->coordX < FORTRESS_FIRST_TILE_INDEX || tile->coordX > FORTRESS_LAST_TILE_INDEX
        || tile->coordY < FORTRESS_FIRST_TILE_INDEX || tile->coordY > FORTRESS_LAST_TILE_INDEX)
    {
        //TODO throw sound or message
        return;
    }

    switch (tileType)
    {
        case GrassType:
            if (GetMoney() < grassPrice) return;
            ModifyMoney(-grassPrice);

            if(GetRandomValue(0, 100) < 50) tile->tileModel = grassTile;
            else tile->tileModel = grassAltTile;
            break;

        case LavaType:
            if (GetMoney() < lavaPrice) return;
            ModifyMoney(-lavaPrice);

            tile->tileModel = lavaTile;
            break;
    
        case WheatType:
            if (GetMoney() < wheatPrice) return;
            ModifyMoney(-wheatPrice);

            tile->tileModel = wheatTile;
            break;
    
        case WoodType:
            if (GetMoney() < woodPrice) return;
            ModifyMoney(-woodPrice);

            tile->tileModel = woodTile;
            break;
    
        case ClayType:
            if (GetMoney() < clayPrice) return;
            ModifyMoney(-clayPrice);

            tile->tileModel = clayTile;
            break;
    }
    tile->tileType = tileType;
}
