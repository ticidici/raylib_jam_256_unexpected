#include "uiManager.h"
#include "terrain.h"
#include "camera.h"
#include "game_state.h"
#include "enemy.h"
#include "raymath.h"

static int screenWidth;     //passed from game.c
static int screenHeight;    //passed from game.c

static Texture cursorTexture;
static Texture cursorStraightTexture;

static Texture coinsIcon;

static Texture wheatIcon;
static Texture woodIcon;
static Texture brickIcon;
static Texture ironIcon;

static Texture wheatTileIcon;
static Texture woodTileIcon;
static Texture clayTileIcon;
static Texture grassTileIcon;
static Texture lavaTileIcon;

static Texture cubeStrawIcon;
static Texture cubeStickIcon;
static Texture cubeBrickIcon;

static Texture weaponWeakIcon;
static Texture weaponStrongIcon;


static Texture sellCubeIcon;//also changes the cursor 
static Texture thunderboltIcon;//also changes the cursor 

static Rectangle tileButtonsContainer;
static RectangleSettings tileButtonsContainerSettings;
static int containerMargin = 2;

//------- Buttons -------
//buy tile
static Rectangle grassButton;
static RectangleSettings grassButtonSettings;
static Rectangle wheatButton;
static RectangleSettings wheatButtonSettings;
static Rectangle woodButton;
static RectangleSettings woodButtonSettings;
static Rectangle clayButton;
static RectangleSettings clayButtonSettings;
static Rectangle lavaButton;
static RectangleSettings lavaButtonSettings;

//build cube
static Rectangle cubeStrawButton;
static RectangleSettings cubeStrawButtonSettings;
static Rectangle cubeStickButton;
static RectangleSettings cubeStickButtonSettings;
static Rectangle cubeBrickButton;
static RectangleSettings cubeBrickButtonSettings;

//buy weapon
static Rectangle weaponWeakButton;
static RectangleSettings weaponWeakButtonSettings;
static Rectangle weaponStrongButton;
static RectangleSettings weaponStrongButtonSettings;

//sell
static Rectangle sellButton;
static RectangleSettings sellButtonSettings;

static Rectangle sellBlock1Button;
static RectangleSettings sellBlock1ButtonSettings;
static Rectangle sellBlock2Button;
static RectangleSettings sellBlock2ButtonSettings;
static Rectangle sellBlock3Button;
static RectangleSettings sellBlock3ButtonSettings;

//thunderbolt
static Rectangle thunderboltButton;
static RectangleSettings thunderboltButtonSettings;

//exchange
static Rectangle exchangeWheatButton;
static RectangleSettings exchangeWheatButtonSettings;
static Rectangle exchangeWoodButton;
static RectangleSettings exchangeWoodButtonSettings;
static Rectangle exchangeClayButton;
static RectangleSettings exchangeClayButtonSettings;
static Rectangle exchangeIronButton;
static RectangleSettings exchangeIronButtonSettings;
//exchange rates
//  wheat
static Rectangle exchangeRate_Wheat_Wood_Button;
static RectangleSettings exchangeRate_Wheat_Wood_ButtonSettings;
static Rectangle exchangeRate_Wheat_Clay_Button;
static RectangleSettings exchangeRate_Wheat_Clay_ButtonSettings;
static Rectangle exchangeRate_Wheat_Iron_Button;
static RectangleSettings exchangeRate_Wheat_Iron_ButtonSettings;
//  wood
static Rectangle exchangeRate_Wood_Wheat_Button;
static RectangleSettings exchangeRate_Wood_Wheat_ButtonSettings;
static Rectangle exchangeRate_Wood_Clay_Button;
static RectangleSettings exchangeRate_Wood_Clay_ButtonSettings;
static Rectangle exchangeRate_Wood_Iron_Button;
static RectangleSettings exchangeRate_Wood_Iron_ButtonSettings;
//  clay
static Rectangle exchangeRate_Clay_Wheat_Button;
static RectangleSettings exchangeRate_Clay_Wheat_ButtonSettings;
static Rectangle exchangeRate_Clay_Wood_Button;
static RectangleSettings exchangeRate_Clay_Wood_ButtonSettings;
static Rectangle exchangeRate_Clay_Iron_Button;
static RectangleSettings exchangeRate_Clay_Iron_ButtonSettings;
//  iron
static Rectangle exchangeRate_Iron_Wheat_Button;
static RectangleSettings exchangeRate_Iron_Wheat_ButtonSettings;
static Rectangle exchangeRate_Iron_Wood_Button;
static RectangleSettings exchangeRate_Iron_Wood_ButtonSettings;
static Rectangle exchangeRate_Iron_Clay_Button;
static RectangleSettings exchangeRate_Iron_Clay_ButtonSettings;
//---------------------------

static Rectangle* hoveredButton;

bool isSellMode = false;

bool hideUI = false;

void UiInit(int aScreenWidth, int aScreenHeight)
{
    screenWidth = aScreenWidth;
    screenHeight = aScreenHeight;

    //textures
    cursorTexture = LoadTexture("resources/cursor.png");
    cursorStraightTexture = LoadTexture("resources/cursor_straight.png");

    coinsIcon = LoadTexture("resources/coins.png");

    wheatIcon = LoadTexture("resources/wheat.png");
    woodIcon = LoadTexture("resources/wood.png");
    brickIcon = LoadTexture("resources/brick.png");
    ironIcon = LoadTexture("resources/iron.png");

    wheatTileIcon = LoadTexture("resources/wheat_tile_16.png");
    woodTileIcon = LoadTexture("resources/wood_tile_16.png");
    clayTileIcon = LoadTexture("resources/clay_tile_16.png");
    grassTileIcon = LoadTexture("resources/grass_tile_16.png");
    lavaTileIcon = LoadTexture("resources/lava_tile_16.png");

    cubeStrawIcon = LoadTexture("resources/cube_straw_16.png");
    cubeStickIcon = LoadTexture("resources/cube_stick_16.png");
    cubeBrickIcon = LoadTexture("resources/cube_brick_16.png");

    weaponWeakIcon = LoadTexture("resources/weapon_weak_16.png");
    weaponStrongIcon = LoadTexture("resources/weapon_strong_16.png");

    sellCubeIcon = LoadTexture("resources/sell.png");
    thunderboltIcon = LoadTexture("resources/thunderbolt.png");

    //BUTTON SETTINGS
    //right side container
    Vector2 containerTopLeft = { 232, 256 - 214 };
    Vector2 containerBottomRight = { (float)(screenWidth), (float)(screenHeight) };
    Vector2 containerDimensions = GetWidthAndHeightFromCorners(containerTopLeft, containerBottomRight);
    tileButtonsContainerSettings.topLeftCorner = containerTopLeft;
    tileButtonsContainerSettings.width = containerDimensions.x;
    tileButtonsContainerSettings.height = containerDimensions.y;
    tileButtonsContainerSettings.color = BEIGE;
    tileButtonsContainerSettings.isEnabled = false;
    SetRectangleSettings(&tileButtonsContainer, tileButtonsContainerSettings);

    //----------- buttons right side -----------
    //weapon icons
    weaponWeakButtonSettings.topLeftCorner = (Vector2){ containerTopLeft.x + 2, 256 - 210 };
    weaponWeakButtonSettings.width = 20.0f;
    weaponWeakButtonSettings.height = 20.0f;
    weaponWeakButtonSettings.color = DARKGRAY;
    weaponWeakButtonSettings.isEnabled = false;
    SetRectangleSettings(&weaponWeakButton, weaponWeakButtonSettings);

    weaponStrongButtonSettings.topLeftCorner = (Vector2){ containerTopLeft.x + 2, 256 - 190 };
    weaponStrongButtonSettings.width = 20.0f;
    weaponStrongButtonSettings.height = 20.0f;
    weaponStrongButtonSettings.color = DARKBLUE;
    weaponStrongButtonSettings.isEnabled = false;
    SetRectangleSettings(&weaponStrongButton, weaponStrongButtonSettings);

    //cube icons
    cubeStrawButtonSettings.topLeftCorner = (Vector2){ containerTopLeft.x + 2, 256 - 165 };
    cubeStrawButtonSettings.width = 20.0f;
    cubeStrawButtonSettings.height = 20.0f;
    cubeStrawButtonSettings.color = GOLD;
    cubeStrawButtonSettings.isEnabled = false;
    SetRectangleSettings(&cubeStrawButton, cubeStrawButtonSettings);

    cubeStickButtonSettings.topLeftCorner = (Vector2){ containerTopLeft.x + 2, 256 - 145 };
    cubeStickButtonSettings.width = 20.0f;
    cubeStickButtonSettings.height = 20.0f;
    cubeStickButtonSettings.color = BROWN;
    cubeStickButtonSettings.isEnabled = false;
    SetRectangleSettings(&cubeStickButton, cubeStickButtonSettings);

    cubeBrickButtonSettings.topLeftCorner = (Vector2){ containerTopLeft.x + 2, 256 - 125 };
    cubeBrickButtonSettings.width = 20.0f;
    cubeBrickButtonSettings.height = 20.0f;
    cubeBrickButtonSettings.color = MAROON;
    cubeBrickButtonSettings.isEnabled = false;
    SetRectangleSettings(&cubeBrickButton, cubeBrickButtonSettings);

    //tile icons
    grassButtonSettings.topLeftCorner = (Vector2){ containerTopLeft.x + 2, 256 - 100 };
    grassButtonSettings.width = 20.0f;
    grassButtonSettings.height = 20.0f;
    grassButtonSettings.color = LIME;
    grassButtonSettings.isEnabled = false;
    SetRectangleSettings(&grassButton, grassButtonSettings);
    
    wheatButtonSettings.topLeftCorner = (Vector2){ containerTopLeft.x + 2, 256 - 80 };
    wheatButtonSettings.width = 20.0f;
    wheatButtonSettings.height = 20.0f;
    wheatButtonSettings.color = GOLD;
    wheatButtonSettings.isEnabled = false;
    SetRectangleSettings(&wheatButton, wheatButtonSettings);
    
    woodButtonSettings.topLeftCorner = (Vector2){ containerTopLeft.x + 2, 256 - 60 };
    woodButtonSettings.width = 20.0f;
    woodButtonSettings.height = 20.0f;
    woodButtonSettings.color = BROWN;
    woodButtonSettings.isEnabled = false;
    SetRectangleSettings(&woodButton, woodButtonSettings);
    
    clayButtonSettings.topLeftCorner = (Vector2){ containerTopLeft.x + 2, 256 - 40 };
    clayButtonSettings.width = 20.0f;
    clayButtonSettings.height = 20.0f;
    clayButtonSettings.color = MAROON;
    clayButtonSettings.isEnabled = false;
    SetRectangleSettings(&clayButton, clayButtonSettings);

    lavaButtonSettings.topLeftCorner = (Vector2){ containerTopLeft.x + 2, 256 - 20};
    lavaButtonSettings.width = 20.0f;
    lavaButtonSettings.height = 20.0f;
    lavaButtonSettings.color = ORANGE;
    lavaButtonSettings.isEnabled = false;
    SetRectangleSettings(&lavaButton, lavaButtonSettings);
    //----------- /buttons right side -----------

}


void UiUpdate()
{
    bool isPaused = IsPaused();

    if (isPaused) return;

	//things like size and positioning or movement of ui
    //Toggle UI
    if (IsKeyPressed(KEY_U) || (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE) && !IsCamIsometric()))
    {
        hideUI = !hideUI;
    }

    Vector2 mousePosition = GetMousePosition();
    
    //check hovered
    //tiles
    if (grassButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, grassButton))
    {
        hoveredButton = &grassButton;
    }
    else if (wheatButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, wheatButton))
    {
        hoveredButton = &wheatButton;
    }
    else if (woodButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, woodButton))
    {
        hoveredButton = &woodButton;
    }
    else if (clayButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, clayButton))
    {
        hoveredButton = &clayButton;
    }
    else if (lavaButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, lavaButton))
    {
        hoveredButton = &lavaButton;
    }
    else if (lavaButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, lavaButton))
    {
        hoveredButton = &lavaButton;
    }
    //cubes
    else if (cubeStrawButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, cubeStrawButton))
    {
        hoveredButton = &cubeStrawButton;
    }
    else if (cubeStickButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, cubeStickButton))
    {
        hoveredButton = &cubeStickButton;
    }
    else if (cubeBrickButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, cubeBrickButton))
    {
        hoveredButton = &cubeBrickButton;
    }
    //weapons
    else if (weaponWeakButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, weaponWeakButton))
    {
        hoveredButton = &weaponWeakButton;
    }
    else if (weaponStrongButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, weaponStrongButton))
    {
        hoveredButton = &weaponStrongButton;
    }
}

void UiRender()
{
    bool isPaused = IsPaused();
    //DrawText(TextFormat("(%04f,%04f)", mousePosition.x, mousePosition.y), 0, 0, 20, RED);
    //DrawCircleLines(GetMouseX(), GetMouseY(), 5.0f, MAROON);
    //DrawTexture(cursorTexture, GetMouseX() - 6, GetMouseY() - 2, WHITE);


    if (!hideUI)
    {
        // --------- Resources on Top ----------
        DrawRectangle(0, 0, 256, 24, Fade(BEIGE, 0.8f));

        DrawTextureEx(coinsIcon, (Vector2) { 6.0f, 4.0f }, 0, 1, WHITE);
        DrawText(TextFormat("%03i", GetMoney()), 25, 6, 14, RAYWHITE);//icon +19

        DrawTextureEx(wheatIcon, (Vector2) { 56.0f, 4.0f }, 0, 1, WHITE);//icon +50
        DrawText(TextFormat("%03i", GetResource(WheatType)), 75, 6, 14, RAYWHITE);

        DrawTextureEx(woodIcon, (Vector2) { 106.0f, 4.0f }, 0, 1, WHITE);
        DrawText(TextFormat("%03i", GetResource(WoodType)), 125, 6, 14, RAYWHITE);

        DrawTextureEx(brickIcon, (Vector2) { 156.0f, 4.0f }, 0, 1, WHITE);
        DrawText(TextFormat("%03i", GetResource(ClayType)), 175, 6, 14, RAYWHITE);

        if (IsLavaUnlocked())
        {
            DrawTextureEx(ironIcon, (Vector2) { 206.0f, 4.0f }, 0, 1, WHITE);
            DrawText(TextFormat("%03i", GetResource(LavaType)), 225, 6, 14, RAYWHITE);
        }

        // --------------- /Resources on Top--------------

        // -------------- Tile Info ----------------------
        if (ShouldShowTileInfo())
        {
            DrawRectangle(0, 116, 105, 150, Fade(BEIGE, 0.8f));

            Tile* tileSelected = TerrainGetTileSelected();

            char* tileTypeString = "";

            Color tileColor = RAYWHITE;

            //tile type
            if (tileSelected->tileType == GrassType)
            {
                tileTypeString = "grass";
                tileColor = grassButtonSettings.color;
            }
            else if (tileSelected->tileType == WheatType)
            {
                tileTypeString = "wheat";
                tileColor = wheatButtonSettings.color;
            }
            else if (tileSelected->tileType == WoodType)
            {
                tileTypeString = "wood";
                tileColor = woodButtonSettings.color;
            }
            else if (tileSelected->tileType == ClayType)
            {
                tileTypeString = "clay";
                tileColor = clayButtonSettings.color;
            }
            else if (tileSelected->tileType == LavaType)
            {
                tileTypeString = "lava";
                tileColor = RED;
            }
            else if (tileSelected->tileType == DirtType)
            {
                tileTypeString = "dirt";
                tileColor = GRAY;
            }

            DrawText(tileTypeString, 4, 120, 12, tileColor);

            Building building = tileSelected->building;
            int blockCount = building.blockCount;
            //stack
            DrawText(TextFormat("stack: %i", blockCount), 4, 135, 12, RAYWHITE);
            {
                char* block1Text = "not built";
                char* weapon1Text = "no weapon";
                Color block1Color = GRAY;
                Color weapon1Color = GRAY;
                char* block2Text = "not built";
                char* weapon2Text = "no weapon";
                Color block2Color = GRAY;
                Color weapon2Color = GRAY;
                char* block3Text = "not built";
                char* weapon3Text = "no weapon";
                Color block3Color = GRAY;
                Color weapon3Color = GRAY;

                //block 1
                if (blockCount > 0)
                {
                    //block
                    BuildingMaterial material = building.blocks[0].buildingMaterial;

                    if (material == Straw)
                    {
                        block1Text = "straw";
                        block1Color = wheatButtonSettings.color;
                    }
                    else if (material == Stick)
                    {
                        block1Text = "stick";
                        block1Color = woodButtonSettings.color;
                    }
                    else if (material == Brick)
                    {
                        block1Text = "brick";
                        block1Color = clayButtonSettings.color;
                    }

                    //weapon
                    WeaponType weapon = building.blocks[0].weaponType;

                    if (weapon == WeaponWeak)
                    {
                        weapon1Text = "basic gun";
                        weapon1Color = weaponWeakButtonSettings.color;
                    }
                    else if (weapon == WeaponStrong)
                    {
                        weapon1Text = "heavy gun";
                        weapon1Color = weaponStrongButtonSettings.color;
                    }
                }
                
                //block 2
                if (blockCount > 1)
                {
                    //block
                    BuildingMaterial material = building.blocks[1].buildingMaterial;

                    if (material == Straw)
                    {
                        block2Text = "straw";
                        block2Color = wheatButtonSettings.color;
                    }
                    else if (material == Stick)
                    {
                        block2Text = "stick";
                        block2Color = woodButtonSettings.color;
                    }
                    else if (material == Brick)
                    {
                        block2Text = "brick";
                        block2Color = clayButtonSettings.color;
                    }

                    //weapon
                    WeaponType weapon = building.blocks[1].weaponType;

                    if (weapon == WeaponWeak)
                    {
                        weapon2Text = "basic gun";
                        weapon2Color = weaponWeakButtonSettings.color;
                    }
                    else if (weapon == WeaponStrong)
                    {
                        weapon2Text = "heavy gun";
                        weapon2Color = weaponStrongButtonSettings.color;
                    }
                }

                //block 3
                if (blockCount > 2)
                {
                    //block
                    BuildingMaterial material = building.blocks[2].buildingMaterial;

                    if (material == Straw)
                    {
                        block3Text = "straw";
                        block3Color = wheatButtonSettings.color;
                    }
                    else if (material == Stick)
                    {
                        block3Text = "stick";
                        block3Color = woodButtonSettings.color;
                    }
                    else if (material == Brick)
                    {
                        block3Text = "brick";
                        block3Color = clayButtonSettings.color;
                    }

                    //weapon
                    WeaponType weapon = building.blocks[2].weaponType;

                    if (weapon == WeaponWeak)
                    {
                        weapon3Text = "basic gun";
                        weapon3Color = weaponWeakButtonSettings.color;
                    }
                    else if (weapon == WeaponStrong)
                    {
                        weapon3Text = "heavy gun";
                        weapon3Color = weaponStrongButtonSettings.color;
                    }
                }

                //inverted order
                DrawText(TextFormat("\t3F - %s", block3Text), 4, 150, 12, block3Color);//material or "not built"
                    DrawText(TextFormat("\t\t%s", weapon3Text), 4, 165, 12, weapon3Color);
                DrawText(TextFormat("\t2F - %s", block2Text), 4, 180, 12, block2Color);
                    DrawText(TextFormat("\t\t%s", weapon2Text), 4, 195, 12, weapon2Color);
                DrawText(TextFormat("\t1F - %s", block1Text), 4, 210, 12, block1Color);
                    DrawText(TextFormat("\t\t%s", weapon1Text), 4, 225, 12, weapon1Color);
            }

            //enemy
            char* enemyText = "no enemy";
            Enemy* enemy = tileSelected->enemy;
            if (enemy != 0)
            {
                enemyText = TextFormat("wolf %iHP", enemy->hp);
            }
            DrawText(enemyText, 4, 240, 12, RAYWHITE);//enemy name or "no enemy"
        }
        // -------------- /Tile Info ----------------------
    }

    //----------- buttons right side -----------
    //container
    if (tileButtonsContainerSettings.isEnabled)
    {
        DrawRectangle(tileButtonsContainerSettings.topLeftCorner.x,
            tileButtonsContainerSettings.topLeftCorner.y,
            tileButtonsContainerSettings.width,
            tileButtonsContainerSettings.height,
            Fade(tileButtonsContainerSettings.color, 0.8f));
    }
    
    //buy weapons
    if(weaponWeakButtonSettings.isEnabled)
    {
        if (hoveredButton == &weaponWeakButton)
        {
            DrawRectangle(weaponWeakButtonSettings.topLeftCorner.x, weaponWeakButtonSettings.topLeftCorner.y, weaponWeakButtonSettings.width, weaponWeakButtonSettings.height, weaponWeakButtonSettings.color);
        }
        DrawTexture(weaponWeakIcon, weaponWeakButtonSettings.topLeftCorner.x + 2, weaponWeakButtonSettings.topLeftCorner.y + 2, WHITE);
    }

    if (weaponStrongButtonSettings.isEnabled)
    {
        if (hoveredButton == &weaponStrongButton)
        {
            DrawRectangle(weaponStrongButtonSettings.topLeftCorner.x, weaponStrongButtonSettings.topLeftCorner.y, weaponStrongButtonSettings.width, weaponStrongButtonSettings.height, weaponStrongButtonSettings.color);
        }
        DrawTexture(weaponStrongIcon, weaponStrongButtonSettings.topLeftCorner.x + 2, weaponStrongButtonSettings.topLeftCorner.y + 2, WHITE);
    }

    //build cubes
    if (cubeStrawButtonSettings.isEnabled)
    {
        if (hoveredButton == &cubeStrawButton)
        {
            DrawRectangle(cubeStrawButtonSettings.topLeftCorner.x, cubeStrawButtonSettings.topLeftCorner.y, cubeStrawButtonSettings.width, cubeStrawButtonSettings.height, cubeStrawButtonSettings.color);
        }
        DrawTexture(cubeStrawIcon, cubeStrawButtonSettings.topLeftCorner.x + 2, cubeStrawButtonSettings.topLeftCorner.y + 2, WHITE);
    }
    
    if (cubeStickButtonSettings.isEnabled)
    {
        if (hoveredButton == &cubeStickButton)
        {
            DrawRectangle(cubeStickButtonSettings.topLeftCorner.x, cubeStickButtonSettings.topLeftCorner.y, cubeStickButtonSettings.width, cubeStickButtonSettings.height, cubeStickButtonSettings.color);
        }
        DrawTexture(cubeStickIcon, cubeStickButtonSettings.topLeftCorner.x + 2, cubeStickButtonSettings.topLeftCorner.y + 2, WHITE);
    }
    
    if (cubeBrickButtonSettings.isEnabled)
    {
        if (hoveredButton == &cubeBrickButton)
        {
            DrawRectangle(cubeBrickButtonSettings.topLeftCorner.x, cubeBrickButtonSettings.topLeftCorner.y, cubeBrickButtonSettings.width, cubeBrickButtonSettings.height, cubeBrickButtonSettings.color);
        }
        DrawTexture(cubeBrickIcon, cubeBrickButtonSettings.topLeftCorner.x + 2, cubeBrickButtonSettings.topLeftCorner.y + 2, WHITE);
    }

    //buy tiles
    if (grassButtonSettings.isEnabled)
    {
        if (hoveredButton == &grassButton)
        {
            DrawRectangle(grassButtonSettings.topLeftCorner.x, grassButtonSettings.topLeftCorner.y, grassButtonSettings.width, grassButtonSettings.height, grassButtonSettings.color);
        }
        DrawTexture(grassTileIcon, grassButtonSettings.topLeftCorner.x + 2, grassButtonSettings.topLeftCorner.y + 2, WHITE);
    }
    
    if (wheatButtonSettings.isEnabled)
    {
        if (hoveredButton == &wheatButton)
        {
            DrawRectangle(wheatButtonSettings.topLeftCorner.x, wheatButtonSettings.topLeftCorner.y, wheatButtonSettings.width, wheatButtonSettings.height, wheatButtonSettings.color);
        }

        DrawTexture(wheatTileIcon, wheatButtonSettings.topLeftCorner.x + 2, wheatButtonSettings.topLeftCorner.y + 2, WHITE);
    }
    
    if (woodButtonSettings.isEnabled)
    {
        if (hoveredButton == &woodButton)
        {
            DrawRectangle(woodButtonSettings.topLeftCorner.x, woodButtonSettings.topLeftCorner.y, woodButtonSettings.width, woodButtonSettings.height, woodButtonSettings.color);
        }

        DrawTexture(woodTileIcon, woodButtonSettings.topLeftCorner.x + 2, woodButtonSettings.topLeftCorner.y + 2, WHITE);
    }
    
    if (clayButtonSettings.isEnabled)
    {
        if (hoveredButton == &clayButton)
        {
            DrawRectangle(clayButtonSettings.topLeftCorner.x, clayButtonSettings.topLeftCorner.y, clayButtonSettings.width, clayButtonSettings.height, clayButtonSettings.color);
        }
        DrawTexture(clayTileIcon, clayButtonSettings.topLeftCorner.x + 2, clayButtonSettings.topLeftCorner.y + 2, WHITE);
    }

    if (lavaButtonSettings.isEnabled)
    {
        if (hoveredButton == &lavaButton)
        {
            DrawRectangle(lavaButtonSettings.topLeftCorner.x, lavaButtonSettings.topLeftCorner.y, lavaButtonSettings.width, lavaButtonSettings.height, lavaButtonSettings.color);
        }

        DrawTexture(lavaTileIcon, lavaButtonSettings.topLeftCorner.x + 2, lavaButtonSettings.topLeftCorner.y + 2, WHITE);
    }

    //----------- /buttons right side -----------

    // Timer
    int timeInSeconds = floorf(GetRunTime());
    int seconds = timeInSeconds % 60;
    int minutes = floorf(timeInSeconds / 60);
    DrawText(TextFormat("%02i:%02i", minutes, seconds), 107, 238, 18, RAYWHITE);


    // Pause overlay
    if (isPaused)
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.5f));
        DrawText("GAME PAUSED", 66, 98, 18, MAROON);
    }

    // FPS
    //DrawFPS(232, 0);

    // Cursor
    if (isSellMode)
    {
        DrawTexture(sellCubeIcon, GetMouseX() - 10, GetMouseY(), WHITE);
    }
    else
    {
        DrawTexture(cursorStraightTexture, GetMouseX() - 10, GetMouseY(), WHITE);
    }

    if (IsCursorOnScreen()) HideCursor();
    else ShowCursor();
}

void UiShowTileButtons()
{
    tileButtonsContainerSettings.isEnabled = true;

    weaponWeakButtonSettings.isEnabled = true;
    /*if (IsLavaUnlocked()) */weaponStrongButtonSettings.isEnabled = true;

    cubeStrawButtonSettings.isEnabled = true;
    cubeStickButtonSettings.isEnabled = true;
    cubeBrickButtonSettings.isEnabled = true;

    grassButtonSettings.isEnabled = true;
    wheatButtonSettings.isEnabled = true;
    woodButtonSettings.isEnabled = true;
    clayButtonSettings.isEnabled = true;
    /*if (IsLavaUnlocked()) */lavaButtonSettings.isEnabled = true;
}

void UiHideTileButtons()
{
    tileButtonsContainerSettings.isEnabled = false;

    weaponWeakButtonSettings.isEnabled = false;
    weaponStrongButtonSettings.isEnabled = false;

    cubeStrawButtonSettings.isEnabled = false;
    cubeStickButtonSettings.isEnabled = false;
    cubeBrickButtonSettings.isEnabled = false;

    grassButtonSettings.isEnabled = false;
    wheatButtonSettings.isEnabled = false;
    woodButtonSettings.isEnabled = false;
    clayButtonSettings.isEnabled = false;
    lavaButtonSettings.isEnabled = false;
}

void UiRelease()
{
    UnloadTexture(cursorTexture);
    UnloadTexture(cursorStraightTexture);

    UnloadTexture(coinsIcon);
    UnloadTexture(wheatIcon);
    UnloadTexture(woodIcon);
    UnloadTexture(brickIcon);
    UnloadTexture(ironIcon);

    UnloadTexture(wheatTileIcon);
    UnloadTexture(woodTileIcon);
    UnloadTexture(clayTileIcon);
    UnloadTexture(grassTileIcon);
    UnloadTexture(lavaTileIcon);

    UnloadTexture(cubeStrawIcon);
    UnloadTexture(cubeStickIcon);
    UnloadTexture(cubeBrickIcon);

    UnloadTexture(weaponWeakIcon);
    UnloadTexture(weaponStrongIcon);

    UnloadTexture(sellCubeIcon); 
    UnloadTexture(thunderboltIcon);
}

//BUTTON PRESSED FUNCTIONS
//weapons
bool UiIsWeaponWeakButtonPressed()
{
    if (weaponWeakButtonSettings.isEnabled == false) return false;
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return false;
    return CheckCollisionPointRec(GetMousePosition(), weaponWeakButton);
}

bool UiIsWeaponStrongButtonPressed()
{
    if (weaponStrongButtonSettings.isEnabled == false) return false;
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return false;
    return CheckCollisionPointRec(GetMousePosition(), weaponStrongButton);
}

//cubes
bool UiIsCubeStrawButtonPressed()
{
    if (cubeStrawButtonSettings.isEnabled == false) return false;
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return false;
    return CheckCollisionPointRec(GetMousePosition(), cubeStrawButton);
}

bool UiIsCubeStickButtonPressed()
{
    if (cubeStickButtonSettings.isEnabled == false) return false;
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return false;
    return CheckCollisionPointRec(GetMousePosition(), cubeStickButton);
}

bool UiIsCubeBrickButtonPressed()
{
    if (cubeBrickButtonSettings.isEnabled == false) return false;
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return false;
    return CheckCollisionPointRec(GetMousePosition(), cubeBrickButton);
}

//tiles
bool UiIsTileGrassButtonPressed()
{
    if (grassButtonSettings.isEnabled == false) return false;
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return false;
    return CheckCollisionPointRec(GetMousePosition(), grassButton);
}

bool UiIsTileWheatButtonPressed()
{
    if (wheatButtonSettings.isEnabled == false) return false;
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return false;
    return CheckCollisionPointRec(GetMousePosition(), wheatButton);
}

bool UiIsTileWoodButtonPressed()
{
    if (woodButtonSettings.isEnabled == false) return false;
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return false;
    return CheckCollisionPointRec(GetMousePosition(), woodButton);
}

bool UiIsTileClayButtonPressed()
{
    if (clayButtonSettings.isEnabled == false) return false;
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return false;
    return CheckCollisionPointRec(GetMousePosition(), clayButton);
}

bool UiIsTileLavaButtonPressed()
{
    if (lavaButtonSettings.isEnabled == false) return false;
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return false;
    return CheckCollisionPointRec(GetMousePosition(), lavaButton);
}

//Private
void SetRectangleSettings(Rectangle *rectangle, RectangleSettings settings)
{
    rectangle->x = settings.topLeftCorner.x;
    rectangle->y = settings.topLeftCorner.y;
    rectangle->width = settings.width;
    rectangle->height = settings.height;
}

/// <summary>
/// </summary>
/// <param name="topLeft"></param>
/// <param name="bottomRight"></param>
/// <returns>x is width, y is height</returns>
Vector2 GetWidthAndHeightFromCorners(Vector2 topLeft, Vector2 bottomRight)
{
    return (Vector2) { fabsf(bottomRight.x - topLeft.x), fabsf(bottomRight.y - topLeft.y) };
}
