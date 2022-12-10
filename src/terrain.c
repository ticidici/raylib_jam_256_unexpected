#include "terrain.h"
#include "utils.h"
#include "uiManager.h"
#include "enemy.h"
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
int grassPrice = 1;//only useful to stop lava
int wheatPrice = 2;
int woodPrice = 5;
int clayPrice = 11;
int lavaPrice = 11;

//time tile needs to give resources
static int moneyYieldQuantity = 1;
static float moneyYieldTime = 7;//each house yields 

static float wheatYieldTime = 5;//each tile yields
static float woodYieldTime = 10;
static float clayYieldTime = 15;
static float ironYieldTime = 30;

static int wholeSetBonusMoneyYield = 5;//+X every yield if we have all the cubes from the same set
static int wholeSetBonusResourceYield = 1;

static float nextMoneyYield = 0; //save next timer value and check in game state instead of really counting
static float nextWheatYield = 0;
static float nextWoodYield = 0;
static float nextClayYield = 0;
static float nextIronYield = 0;

//lava special effect
static float lavaBurnPeriod = 3;//every X seconds
static float lavaDamage = 1;
static float lavaDamageBuilding = 4;
static float lavaStartExtendTime = 30;
static float lavaFinishExtendTime = 45;
static Color lavaAboutToExtendColor = { 255, 160, 160, 255};

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

//thunderbolt
static float nextThunderboltActiveTime = 0;
static float thunderboltCooldown = 20.0f;
static float thunderboltDuration = 0.04f;
static float nextThunderboltStop = 0;
static Vector2 thunderboltDamage = { 999, 20 };//tile, adjacent

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
                    if (randomValue < 1 && IsLavaUnlocked() && i != MIDDLE_TILE_INDEX && j != MIDDLE_TILE_INDEX)
                    {
                        battlefieldTiles[i][j].tileModel = lavaTile;
                        battlefieldTiles[i][j].tileType = LavaType;
                        if (battlefieldTiles[i][j].building.blockCount > 0) BuildingLavaDamage(&battlefieldTiles[i][j]);
                        battlefieldTiles[i][j].lavaNextExtendStart = 0 + lavaStartExtendTime;
                        battlefieldTiles[i][j].lavaNextExtendFinish = 0 + lavaFinishExtendTime;
                        battlefieldTiles[i][j].lavaAboutToExtend = false;
                    }
                    else if (randomValue < 4)
                    {
                        battlefieldTiles[i][j].tileModel = wheatTile;
                        battlefieldTiles[i][j].tileType = WheatType;
                    }
                    else if (randomValue < 6)
                    {
                        battlefieldTiles[i][j].tileModel = woodTile;
                        battlefieldTiles[i][j].tileType = WoodType;
                    }
                    else if (randomValue < 5)
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

    nextMoneyYield = moneyYieldTime;
    nextWheatYield = wheatYieldTime;
    nextWoodYield = woodYieldTime;
    nextClayYield = clayYieldTime;
    nextIronYield = ironYieldTime;

    nextThunderboltActiveTime = thunderboltCooldown;
    nextThunderboltStop = 0;

    //only blocks in the begginning, don't erase
    Building *buildingPorquet = &battlefieldTiles[MIDDLE_TILE_INDEX][MIDDLE_TILE_INDEX].building;
    buildingPorquet->isPorquet = true;
    buildingPorquet->blocks[0].buildingMaterial = Brick;
    buildingPorquet->blocks[0].hp = GetPorquetBlocksHp();
    buildingPorquet->blocks[1].buildingMaterial = Brick;
    buildingPorquet->blocks[1].hp = GetPorquetBlocksHp();
    buildingPorquet->blocks[2].buildingMaterial = Brick;
    buildingPorquet->blocks[2].hp = GetPorquetBlocksHp();
    buildingPorquet->blockCount = 3;
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
    if (GetRunTime() == 0) return;

    bool tileJustDeselected = false;
    bool clickedInButton = false;
    if (isTileSelected)
    {
        //if clicked on option confirm and deselect (even if it is not executed)
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            clickedInButton = true;
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
                //don't deselect
            }
            else if (UiIsWeaponStrongButtonPressed())
            {
                //don't deselect
            }
            else if (UiIsWeaponFloor1ButtonPressed())
            {
                WeaponType weaponType = WhichWeaponIsSelected();
                BuildingBuyWeapon(weaponType, tileHovered, 0);
            }
            else if (UiIsWeaponFloor2ButtonPressed())
            {
                WeaponType weaponType = WhichWeaponIsSelected();
                BuildingBuyWeapon(weaponType, tileHovered, 1);
            }
            else if (UiIsWeaponFloor3ButtonPressed())
            {
                WeaponType weaponType = WhichWeaponIsSelected();
                BuildingBuyWeapon(weaponType, tileHovered, 2);
            }
            //sell
            else if (UiIsSellButtonPressed())
            {
                //don't deselect
            }
            else if (UiIsSellFloor1Pressed())
            {
                BuildingSellBlock(tileHovered, 0);
            }
            else if (UiIsSellFloor2Pressed())
            {
                BuildingSellBlock(tileHovered, 1);
            }
            else if (UiIsSellFloor3Pressed())
            {
                BuildingSellBlock(tileHovered, 2);
            }

            //outside tile buttons
            else
            {
                clickedInButton = false;

                if (UiIsThunderboltPressed())
                {
                    if (UiIsThunderboltCursor())
                    {
                        UiStopThunderbolt();
                    }
                }
                if (!IsPointInsideTileInScreenSpace(GetMousePosition(), tileHovered, TILE_HALF_WIDTH))
                {
                    isTileSelected = false;
                    tileJustDeselected = true;
                    UiHideRightSideButtons();
                }
                else
                {
                    showHoveredTileInfo = !showHoveredTileInfo;
                }
            }
        }

        //if right clicked or cancel button, deselect tile
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || IsKeyPressed(KEY_ESCAPE))
        {
            isTileSelected = false;
            tileJustDeselected = true;
            UiHideRightSideButtons();
        }
    }

    //we update selector before doing anything with it
    UpdateTileSelector();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (!clickedInButton && UiIsThunderboltCursor())
        {
            PerformThunderbolt();
        }
        else if (UiIsThunderboltPressed())
        {
            if (IsThunderboltReady())
            {
                tileJustDeselected = true;
            }
        }
    }

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
                UiShowRightSideButtons();
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

    CalculateTileEffects();
}

void TerrainRender()
{

    DrawModel(tileSelector, tileHovered->position, 1.f, WHITE);

    bool isThunderbolt = UiIsThunderboltCursor();
    Vector2 surroundingTilesIndexes[8] = {0};
    Vector2 tileHoveredIndexes = { tileHovered->coordX,  tileHovered->coordY };
    if (isThunderbolt)
    {
        Vector2 surroundingTilesOffsets[8] = {  (Vector2) { -1, 1 },    (Vector2) { 0, 1 },     (Vector2) { 1, 1 }, 
                                                (Vector2) { -1, 0 },                            (Vector2) { 1, 0 }, 
                                                (Vector2) { -1, -1 },   (Vector2) { 0, -1 },    (Vector2) { 1, -1 } };
        for (int i = 0; i < 8; i++)
        {
            surroundingTilesIndexes[i] = (Vector2){ tileHoveredIndexes.x + surroundingTilesOffsets[i].x,  tileHoveredIndexes.y + surroundingTilesOffsets[i].y };
        }
    }

    for (int i = 0; i < BATTLEFIELD_SIZE; i++)
    {
        for (int j = 0; j < BATTLEFIELD_SIZE; j++)
        {
            Tile *tile = &battlefieldTiles[i][j];
            BuildingRender(&tile->building, tile->position);
            Color color = WHITE;
            if (tile->tileType == LavaType && tile->lavaAboutToExtend) color = lavaAboutToExtendColor;

            if (isThunderbolt)
            {
                if (i == tileHoveredIndexes.x && j == tileHoveredIndexes.y) color = RED;
                for (int surroundingIndex = 0; surroundingIndex < 8; surroundingIndex++)
                {
                    if (i == surroundingTilesIndexes[surroundingIndex].x && j == surroundingTilesIndexes[surroundingIndex].y)
                    {
                        color = (Color) { 255, 160, 160, 255 };
                        break;
                    }
                }
            }
            DrawModel(battlefieldTiles[i][j].tileModel, battlefieldTiles[i][j].position, 1.f, color);
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

void EnemySteppedOnLava(Enemy* enemy, Tile* tile)
{

    if(!DamageEnemy(tile,enemy, lavaDamage))
    {
        tile->lavaNextBurn = GetRunTime() + lavaBurnPeriod;
    }
}

void BuildingLavaDamage(Tile* tile)
{
    BuildingDamageBlock(&tile->building, 0, lavaDamageBuilding);
    tile->lavaNextBurn = GetRunTime() + lavaBurnPeriod;
}

bool IsThunderboltReady()
{
    return GetRunTime() >= nextThunderboltActiveTime;
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
            tile->lavaNextExtendStart = GetRunTime() + lavaStartExtendTime;
            tile->lavaNextExtendFinish = GetRunTime() + lavaFinishExtendTime;
            tile->lavaAboutToExtend = false;
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

void CalculateTileEffects()
{
    float runTime = GetRunTime();

    bool yieldMoney = false;
    bool yieldWheat = false;
    bool yieldWood = false;
    bool yieldClay = false;
    bool yieldIron = false;

    if (runTime >= nextMoneyYield)
    {
        nextMoneyYield += moneyYieldTime;
        yieldMoney = true;
    }
    if (runTime >= nextWheatYield)
    {
        nextWheatYield += wheatYieldTime;
        yieldWheat = true;
    }
    if (runTime >= nextWoodYield)
    {
        nextWoodYield += woodYieldTime;
        yieldWood = true;
    }
    if (runTime >= nextClayYield)
    {
        nextClayYield += clayYieldTime;
        yieldClay = true;
    }
    if (runTime >= nextIronYield)
    {
        nextIronYield += ironYieldTime;
        yieldIron = true;
    }

    //if (!yieldMoney && !yieldWheat && !yieldWood && !yieldClay && !yieldIron) return;

    for (int i = 0; i < BATTLEFIELD_SIZE; i++)
    {
        //if outside the fortress, continue
        if (i < FORTRESS_FIRST_TILE_INDEX || i > FORTRESS_LAST_TILE_INDEX) continue;

        for (int j = 0; j < BATTLEFIELD_SIZE; j++)
        {
            //if outside the fortress, continue
            if (j < FORTRESS_FIRST_TILE_INDEX || j > FORTRESS_LAST_TILE_INDEX) continue;

            Tile* candidateTile = &battlefieldTiles[i][j];

            if (yieldMoney)
            {
                if (candidateTile->building.blockCount > 0)
                {
                    ModifyMoney(candidateTile->building.blockCount * moneyYieldQuantity);
                    if (IsWholeSet(candidateTile)) ModifyMoney(wholeSetBonusMoneyYield);
                }
            }

            if (yieldWheat && candidateTile->tileType == WheatType)
            {
                ModifyResource(WheatType, 1);
                if (IsWholeSet(candidateTile)) ModifyResource(WheatType, wholeSetBonusResourceYield);
            }

            if (yieldWood && candidateTile->tileType == WoodType)
            {
                ModifyResource(WoodType, 1);
                if (IsWholeSet(candidateTile)) ModifyResource(WoodType, wholeSetBonusResourceYield);
            }
        
            if (yieldClay && candidateTile->tileType == ClayType)
            {
                ModifyResource(ClayType, 1);
                if (IsWholeSet(candidateTile)) ModifyResource(ClayType, wholeSetBonusResourceYield);
            }
        
            if (candidateTile->tileType == LavaType)
            {
                if (yieldIron)
                {
                    ModifyResource(LavaType, 1);
                    if (IsWholeSet(candidateTile)) ModifyResource(LavaType, wholeSetBonusResourceYield);
                }

                if (runTime >= candidateTile->lavaNextExtendStart)
                {
                    candidateTile->lavaNextExtendStart = candidateTile->lavaNextExtendFinish + lavaStartExtendTime;
                    candidateTile->lavaAboutToExtend = true;
                }
                
                if (runTime >= candidateTile->lavaNextExtendFinish)
                {
                    candidateTile->lavaNextExtendFinish = runTime + lavaFinishExtendTime;
                    candidateTile->lavaAboutToExtend = false;

                    int lavaCoordX = candidateTile->coordX;
                    int lavaCoordY = candidateTile->coordY;

                    //Vector2 surroundingTiles[8] = { (Vector2) { -1, 1 },    (Vector2) { 0, 1 },     (Vector2) { 1, 1 }, 
                    //                                (Vector2) { -1, 0 },                            (Vector2) { 1, 0 }, 
                    //                                (Vector2) { -1, -1 },   (Vector2) { 0, -1 },    (Vector2) { 1, -1 } };
                    
                    Vector2 surroundingTiles[4] = {                         (Vector2) { 0, 1 },      
                                                    (Vector2) { -1, 0 },                            (Vector2) { 1, 0 }, 
                                                                            (Vector2) { 0, -1 },                      };

                    int indexOffset = GetRandomValue(0, 4);
                    for (int indexNoOffset = 0; indexNoOffset < 4; indexNoOffset++)
                    {
                        int surroundingTileIdx = (indexNoOffset + indexOffset) % 4;
                        Vector2 coordOffset = surroundingTiles[surroundingTileIdx];
                        Vector2 surroundingTileCoordinates = { lavaCoordX + coordOffset.x, lavaCoordY + coordOffset.y };
                        Tile* surroundingTile = TerrainGetTile(surroundingTileCoordinates.x, surroundingTileCoordinates.y);

                        if (surroundingTile->tileType == DirtType || surroundingTile->tileType == LavaType) continue;
                        if (surroundingTile->building.isPorquet) continue;//don't lava porquet's butt

                        surroundingTile->tileType = LavaType;
                        surroundingTile->tileModel = lavaTile;
                        surroundingTile->lavaAboutToExtend = false;
                        surroundingTile->lavaNextExtendStart = runTime + lavaStartExtendTime;
                        surroundingTile->lavaNextExtendFinish = runTime + lavaFinishExtendTime;
                        if (surroundingTile->enemy != 0) EnemySteppedOnLava(surroundingTile->enemy, surroundingTile);
                        break;
                    }
                
                }
                
                if (candidateTile->enemy != 0 && runTime >= candidateTile->lavaNextBurn)
                {
                    EnemySteppedOnLava(candidateTile->enemy, candidateTile);
                }
            
                if (candidateTile->building.blockCount > 0 && runTime >= candidateTile->lavaNextBurn)
                {
                    BuildingLavaDamage(candidateTile);
                }
            }
        }
    }
}

void PerformThunderbolt()
{
    nextThunderboltActiveTime = GetRunTime() + thunderboltCooldown;
    nextThunderboltStop = GetRunTime() + thunderboltDuration;

    Vector2 affectedTilesIndexes[9] = { 0 };
    Vector2 tileHoveredIndexes = { tileHovered->coordX,  tileHovered->coordY };
    Vector2 affectedTilesOffsets[9] = {  (Vector2) { -1, 1 },    (Vector2) { 0, 1 },     (Vector2) { 1, 1 }, 
                                            (Vector2) { -1, 0 },    (Vector2) { 0, 0 },     (Vector2) { 1, 0 },
                                            (Vector2) { -1, -1 },   (Vector2) { 0, -1 },    (Vector2) { 1, -1 } };
    for (int i = 0; i < 9; i++)
    {
        Vector2 tileIndexes = (Vector2){ tileHoveredIndexes.x + affectedTilesOffsets[i].x,  tileHoveredIndexes.y + affectedTilesOffsets[i].y };
        int damage = thunderboltDamage.y;
        Tile* affectedTile = TerrainGetTile(tileIndexes.x, tileIndexes.y);
        if (tileIndexes.x == tileHoveredIndexes.x && tileIndexes.y == tileHoveredIndexes.y)
        {
            damage = thunderboltDamage.x;
            affectedTile->tileType = LavaType;
            affectedTile->tileModel = lavaTile;
            if (affectedTile->building.blockCount > 0) BuildingLavaDamage(affectedTile);
        }
        if (affectedTile->enemy != 0)
        {
            DamageEnemy(affectedTile, affectedTile->enemy, damage);
        }

        if (affectedTile->building.blockCount > 0)
        {
            BuildingDamageBlock(&affectedTile->building,0,damage);
        }
    }

}