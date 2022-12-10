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


static Texture sellCubeIcon;
static Texture thunderboltIcon;//also changes the cursor 

static Rectangle tileButtonsContainer;
static RectangleSettings tileButtonsContainerSettings;

static Rectangle weaponsFloorContainer;
static RectangleSettings weaponsFloorContainerSettings;

static Rectangle thunderboltContainer;
static RectangleSettings thunderboltContainerSettings;

static Rectangle sellCubeButtonContainer;
static RectangleSettings sellCubeButtonContainerSettings;
static Rectangle sellCubeFloorContainer;
static RectangleSettings sellCubeFloorContainerSettings;

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

static Rectangle weaponBlock1Button;
static RectangleSettings weaponBlock1ButtonSettings;
static Rectangle weaponBlock2Button;
static RectangleSettings weaponBlock2ButtonSettings;
static Rectangle weaponBlock3Button;
static RectangleSettings weaponBlock3ButtonSettings;

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
static Rectangle* selectedTwoStepButton;

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
    Vector2 rightSideContainerTopLeft = { 232, 256 - 212 };
    Vector2 containerBottomRight = { 256, 256 };
    Vector2 containerDimensions = GetWidthAndHeightFromCorners(rightSideContainerTopLeft, containerBottomRight);
    tileButtonsContainerSettings.topLeftCorner = rightSideContainerTopLeft;
    tileButtonsContainerSettings.width = containerDimensions.x;
    tileButtonsContainerSettings.height = containerDimensions.y;
    tileButtonsContainerSettings.color = BEIGE;
    tileButtonsContainerSettings.isEnabled = false;
    SetRectangleSettings(&tileButtonsContainer, tileButtonsContainerSettings);

    //floors for weapons
    weaponsFloorContainerSettings.topLeftCorner = (Vector2) { rightSideContainerTopLeft.x - 26 , 256 - 190 - 32 };
    weaponsFloorContainerSettings.width = 24;
    weaponsFloorContainerSettings.height = 64;
    weaponsFloorContainerSettings.color = BEIGE;
    weaponsFloorContainerSettings.isEnabled = false;
    SetRectangleSettings(&weaponsFloorContainer, weaponsFloorContainerSettings);
    
    //sell cube
    sellCubeButtonContainerSettings.topLeftCorner = (Vector2){ rightSideContainerTopLeft.x - 32 - 10 - 2, 256 - 24 };
    sellCubeButtonContainerSettings.width = 24;
    sellCubeButtonContainerSettings.height = 24;
    sellCubeButtonContainerSettings.color = BEIGE;
    sellCubeButtonContainerSettings.isEnabled = false;
    SetRectangleSettings(&sellCubeButtonContainer, sellCubeButtonContainerSettings);
    //sell cube floors
    sellCubeFloorContainerSettings.topLeftCorner = (Vector2){ rightSideContainerTopLeft.x - 64 - 2, 256 - 50 };
    sellCubeFloorContainerSettings.width = 64;
    sellCubeFloorContainerSettings.height = 24;
    sellCubeFloorContainerSettings.color = BEIGE;
    sellCubeFloorContainerSettings.isEnabled = false;
    SetRectangleSettings(&sellCubeFloorContainer, sellCubeFloorContainerSettings);
    
    //thunderbolt
    thunderboltContainerSettings.topLeftCorner = (Vector2){ 0, 26 };
    thunderboltContainerSettings.width = 24;
    thunderboltContainerSettings.height = 24;
    thunderboltContainerSettings.color = BEIGE;
    thunderboltContainerSettings.isEnabled = true;
    SetRectangleSettings(&thunderboltContainer, thunderboltContainerSettings);


    //----------- buttons right side -----------
    //weapon icons
    weaponWeakButtonSettings.topLeftCorner = (Vector2){ rightSideContainerTopLeft.x + 2, 256 - 210 };
    weaponWeakButtonSettings.width = 20.0f;
    weaponWeakButtonSettings.height = 20.0f;
    weaponWeakButtonSettings.color = DARKGRAY;
    weaponWeakButtonSettings.isEnabled = false;
    SetRectangleSettings(&weaponWeakButton, weaponWeakButtonSettings);

    weaponStrongButtonSettings.topLeftCorner = (Vector2){ rightSideContainerTopLeft.x + 2, 256 - 190 };
    weaponStrongButtonSettings.width = 20.0f;
    weaponStrongButtonSettings.height = 20.0f;
    weaponStrongButtonSettings.color = DARKBLUE;
    weaponStrongButtonSettings.isEnabled = false;
    SetRectangleSettings(&weaponStrongButton, weaponStrongButtonSettings);

    //cube icons
    cubeStrawButtonSettings.topLeftCorner = (Vector2){ rightSideContainerTopLeft.x + 2, 256 - 165 };
    cubeStrawButtonSettings.width = 20.0f;
    cubeStrawButtonSettings.height = 20.0f;
    cubeStrawButtonSettings.color = GOLD;
    cubeStrawButtonSettings.isEnabled = false;
    SetRectangleSettings(&cubeStrawButton, cubeStrawButtonSettings);

    cubeStickButtonSettings.topLeftCorner = (Vector2){ rightSideContainerTopLeft.x + 2, 256 - 145 };
    cubeStickButtonSettings.width = 20.0f;
    cubeStickButtonSettings.height = 20.0f;
    cubeStickButtonSettings.color = BROWN;
    cubeStickButtonSettings.isEnabled = false;
    SetRectangleSettings(&cubeStickButton, cubeStickButtonSettings);

    cubeBrickButtonSettings.topLeftCorner = (Vector2){ rightSideContainerTopLeft.x + 2, 256 - 125 };
    cubeBrickButtonSettings.width = 20.0f;
    cubeBrickButtonSettings.height = 20.0f;
    cubeBrickButtonSettings.color = MAROON;
    cubeBrickButtonSettings.isEnabled = false;
    SetRectangleSettings(&cubeBrickButton, cubeBrickButtonSettings);

    //tile icons
    grassButtonSettings.topLeftCorner = (Vector2){ rightSideContainerTopLeft.x + 2, 256 - 100 };
    grassButtonSettings.width = 20.0f;
    grassButtonSettings.height = 20.0f;
    grassButtonSettings.color = LIME;
    grassButtonSettings.isEnabled = false;
    SetRectangleSettings(&grassButton, grassButtonSettings);
    
    wheatButtonSettings.topLeftCorner = (Vector2){ rightSideContainerTopLeft.x + 2, 256 - 80 };
    wheatButtonSettings.width = 20.0f;
    wheatButtonSettings.height = 20.0f;
    wheatButtonSettings.color = GOLD;
    wheatButtonSettings.isEnabled = false;
    SetRectangleSettings(&wheatButton, wheatButtonSettings);
    
    woodButtonSettings.topLeftCorner = (Vector2){ rightSideContainerTopLeft.x + 2, 256 - 60 };
    woodButtonSettings.width = 20.0f;
    woodButtonSettings.height = 20.0f;
    woodButtonSettings.color = BROWN;
    woodButtonSettings.isEnabled = false;
    SetRectangleSettings(&woodButton, woodButtonSettings);
    
    clayButtonSettings.topLeftCorner = (Vector2){ rightSideContainerTopLeft.x + 2, 256 - 40 };
    clayButtonSettings.width = 20.0f;
    clayButtonSettings.height = 20.0f;
    clayButtonSettings.color = MAROON;
    clayButtonSettings.isEnabled = false;
    SetRectangleSettings(&clayButton, clayButtonSettings);

    lavaButtonSettings.topLeftCorner = (Vector2){ rightSideContainerTopLeft.x + 2, 256 - 20};
    lavaButtonSettings.width = 20.0f;
    lavaButtonSettings.height = 20.0f;
    lavaButtonSettings.color = RED;
    lavaButtonSettings.isEnabled = false;
    SetRectangleSettings(&lavaButton, lavaButtonSettings);

    //sell icon
    sellButtonSettings.topLeftCorner = (Vector2){ rightSideContainerTopLeft.x - 32 - 10, 256 - 22 };
    sellButtonSettings.width = 20;
    sellButtonSettings.height = 20;
    sellButtonSettings.color = BROWN;
    sellButtonSettings.isEnabled = false;
    SetRectangleSettings(&sellButton, sellButtonSettings);
    
    //floors for selling
    sellBlock1ButtonSettings.topLeftCorner = (Vector2){ rightSideContainerTopLeft.x - 62, 256 - 48 };
    sellBlock1ButtonSettings.width = 20;
    sellBlock1ButtonSettings.height = 20;
    sellBlock1ButtonSettings.color = BROWN;
    sellBlock1ButtonSettings.isEnabled = false;
    SetRectangleSettings(&sellBlock1Button, sellBlock1ButtonSettings);

    sellBlock2ButtonSettings.topLeftCorner = (Vector2){ rightSideContainerTopLeft.x - 42, 256 - 48 };
    sellBlock2ButtonSettings.width = 20;
    sellBlock2ButtonSettings.height = 20;
    sellBlock2ButtonSettings.color = BROWN;
    sellBlock2ButtonSettings.isEnabled = false;
    SetRectangleSettings(&sellBlock2Button, sellBlock2ButtonSettings);

    sellBlock3ButtonSettings.topLeftCorner = (Vector2){ rightSideContainerTopLeft.x - 22, 256 - 48 };
    sellBlock3ButtonSettings.width = 20;
    sellBlock3ButtonSettings.height = 20;
    sellBlock3ButtonSettings.color = BROWN;
    sellBlock3ButtonSettings.isEnabled = false;
    SetRectangleSettings(&sellBlock3Button, sellBlock3ButtonSettings);
        
    //floors for weapons
    //buttons
    weaponBlock1ButtonSettings.topLeftCorner = (Vector2){ rightSideContainerTopLeft.x - 24 , 256 - 190 + 10 };
    weaponBlock1ButtonSettings.width = 20;
    weaponBlock1ButtonSettings.height = 20;
    weaponBlock1ButtonSettings.color = BROWN;
    weaponBlock1ButtonSettings.isEnabled = false;
    SetRectangleSettings(&weaponBlock1Button, weaponBlock1ButtonSettings);

    weaponBlock2ButtonSettings.topLeftCorner = (Vector2){ rightSideContainerTopLeft.x - 24 , 256 - 190 - 10 };
    weaponBlock2ButtonSettings.width = 20;
    weaponBlock2ButtonSettings.height = 20;
    weaponBlock2ButtonSettings.color = BROWN;
    weaponBlock2ButtonSettings.isEnabled = false;
    SetRectangleSettings(&weaponBlock2Button, weaponBlock2ButtonSettings);

    weaponBlock3ButtonSettings.topLeftCorner = (Vector2){ rightSideContainerTopLeft.x - 24 , 256 - 190 - 30 };
    weaponBlock3ButtonSettings.width = 20;
    weaponBlock3ButtonSettings.height = 20;
    weaponBlock3ButtonSettings.color = BROWN;
    weaponBlock3ButtonSettings.isEnabled = false;
    SetRectangleSettings(&weaponBlock3Button, weaponBlock3ButtonSettings);

    //thunderbolt
    thunderboltButtonSettings.topLeftCorner = (Vector2){ 0 + 2, 26 + 2 };
    thunderboltButtonSettings.width = 20;
    thunderboltButtonSettings.height = 20;
    thunderboltButtonSettings.color = GOLD;
    thunderboltButtonSettings.isEnabled = false;
    SetRectangleSettings(&thunderboltButton, thunderboltButtonSettings);
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

    //if (IsThunderboltUnlocked())
    {
        thunderboltButtonSettings.isEnabled = true;
    }

    Vector2 mousePosition = GetMousePosition();
    bool isMouseLeftPressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    //check hovered
    //tiles
    if (grassButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, grassButton))
    {
        hoveredButton = &grassButton;
        if (isMouseLeftPressed) selectedTwoStepButton = 0;
    }
    else if (wheatButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, wheatButton))
    {
        hoveredButton = &wheatButton;
        if (isMouseLeftPressed) selectedTwoStepButton = 0;
    }
    else if (woodButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, woodButton))
    {
        hoveredButton = &woodButton;
        if (isMouseLeftPressed) selectedTwoStepButton = 0;
    }
    else if (clayButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, clayButton))
    {
        hoveredButton = &clayButton;
        if (isMouseLeftPressed) selectedTwoStepButton = 0;
    }
    else if (lavaButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, lavaButton))
    {
        hoveredButton = &lavaButton;
        if (isMouseLeftPressed) selectedTwoStepButton = 0;
    }
    else if (lavaButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, lavaButton))
    {
        hoveredButton = &lavaButton;
        if (isMouseLeftPressed) selectedTwoStepButton = 0;
    }
    //cubes
    else if (cubeStrawButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, cubeStrawButton))
    {
        hoveredButton = &cubeStrawButton;
        if (isMouseLeftPressed) selectedTwoStepButton = 0;
    }
    else if (cubeStickButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, cubeStickButton))
    {
        hoveredButton = &cubeStickButton;
        if (isMouseLeftPressed) selectedTwoStepButton = 0;
    }
    else if (cubeBrickButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, cubeBrickButton))
    {
        hoveredButton = &cubeBrickButton;
        if (isMouseLeftPressed) selectedTwoStepButton = 0;
    }
    //weapons
    else if (weaponWeakButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, weaponWeakButton))
    {
        hoveredButton = &weaponWeakButton;
        if (isMouseLeftPressed) selectedTwoStepButton = &weaponWeakButton;
    }
    else if (weaponStrongButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, weaponStrongButton))
    {
        hoveredButton = &weaponStrongButton;
        if (isMouseLeftPressed) selectedTwoStepButton = &weaponStrongButton;
    }
    //floor for weapons
    else if (weaponBlock1ButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, weaponBlock1Button))
    {
        hoveredButton = &weaponBlock1Button;
    }
    else if (weaponBlock2ButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, weaponBlock2Button))
    {
        hoveredButton = &weaponBlock2Button;
    }
    else if (weaponBlock3ButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, weaponBlock3Button))
    {
        hoveredButton = &weaponBlock3Button;
    }
    //sell
    else if (sellButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, sellButton))
    {
        hoveredButton = &sellButton;
        if (isMouseLeftPressed) selectedTwoStepButton = &sellButton;
    }
    //floor for selling
    else if (sellBlock1ButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, sellBlock1Button))
    {
        hoveredButton = &sellBlock1Button;
    }
    else if (sellBlock2ButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, sellBlock2Button))
    {
        hoveredButton = &sellBlock2Button;
    }
    else if (sellBlock3ButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, sellBlock3Button))
    {
        hoveredButton = &sellBlock3Button;
    }
    //thunderbolt
    else if (thunderboltButtonSettings.isEnabled && CheckCollisionPointRec(mousePosition, thunderboltButton))
    {
        hoveredButton = &thunderboltButton;
        if (isMouseLeftPressed) selectedTwoStepButton = &thunderboltButton;
    }

    //unhover if no hover
    else
    {
        if (isMouseLeftPressed) selectedTwoStepButton = 0;

        hoveredButton = 0;
    }

    if (selectedTwoStepButton == &sellButton)
    {
        sellCubeFloorContainerSettings.isEnabled = true;
        sellBlock1ButtonSettings.isEnabled = true;
        sellBlock2ButtonSettings.isEnabled = true;
        sellBlock3ButtonSettings.isEnabled = true;
    }
    else
    {
        sellCubeFloorContainerSettings.isEnabled = false;
        sellBlock1ButtonSettings.isEnabled = false;
        sellBlock2ButtonSettings.isEnabled = false;
        sellBlock3ButtonSettings.isEnabled = false;
    }

    if (selectedTwoStepButton == &weaponWeakButton || selectedTwoStepButton == &weaponStrongButton)
    {
        weaponsFloorContainerSettings.isEnabled = true;
        weaponBlock1ButtonSettings.isEnabled = true;
        weaponBlock2ButtonSettings.isEnabled = true;
        weaponBlock3ButtonSettings.isEnabled = true;
    }
    else
    {
        weaponsFloorContainerSettings.isEnabled = false;
        weaponBlock1ButtonSettings.isEnabled = false;
        weaponBlock2ButtonSettings.isEnabled = false;
        weaponBlock3ButtonSettings.isEnabled = false;
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

        //if (IsLavaUnlocked())
        {
            DrawTextureEx(ironIcon, (Vector2) { 206.0f, 4.0f }, 0, 1, WHITE);
            DrawText(TextFormat("%03i", GetResource(LavaType)), 225, 6, 14, RAYWHITE);
        }

        // --------------- /Resources on Top --------------       

        // -------------- Tile Info -----------------------
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
                tileColor = lavaButtonSettings.color;
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
        //tunderbolt
        //if (IsThunderboltUnlocked())
        {
            DrawRectangle(thunderboltContainerSettings.topLeftCorner.x,
            thunderboltContainerSettings.topLeftCorner.y,
            thunderboltContainerSettings.width,
            thunderboltContainerSettings.height,
            Fade(thunderboltContainerSettings.color, 0.8f));

            Color thunderboltIconColor = WHITE;
            if (!IsThunderboltReady()) thunderboltIconColor = GRAY;
            if (hoveredButton == &thunderboltButton || selectedTwoStepButton == &thunderboltButton)
            {
                DrawRectangle(thunderboltButtonSettings.topLeftCorner.x, thunderboltButtonSettings.topLeftCorner.y, thunderboltButtonSettings.width, thunderboltButtonSettings.height, thunderboltButtonSettings.color);
            }
            DrawTexture(thunderboltIcon, thunderboltButtonSettings.topLeftCorner.x + 2, thunderboltButtonSettings.topLeftCorner.y + 2, thunderboltIconColor);
        }
    }

    //----------- buttons right side -----------
    //containers
    if (tileButtonsContainerSettings.isEnabled)
    {
        DrawRectangle(tileButtonsContainerSettings.topLeftCorner.x,
            tileButtonsContainerSettings.topLeftCorner.y,
            tileButtonsContainerSettings.width,
            tileButtonsContainerSettings.height,
            Fade(tileButtonsContainerSettings.color, 0.8f));
    }

    if (weaponsFloorContainerSettings.isEnabled)
    {
        DrawRectangle(weaponsFloorContainerSettings.topLeftCorner.x,
            weaponsFloorContainerSettings.topLeftCorner.y,
            weaponsFloorContainerSettings.width,
            weaponsFloorContainerSettings.height,
            Fade(weaponsFloorContainerSettings.color, 0.8f));
    }
    
    if (sellCubeButtonContainerSettings.isEnabled)
    {
        DrawRectangle(sellCubeButtonContainerSettings.topLeftCorner.x,
            sellCubeButtonContainerSettings.topLeftCorner.y,
            sellCubeButtonContainerSettings.width,
            sellCubeButtonContainerSettings.height,
            Fade(sellCubeButtonContainerSettings.color, 0.8f));
    }
    
    if (sellCubeFloorContainerSettings.isEnabled)
    {
        DrawRectangle(sellCubeFloorContainerSettings.topLeftCorner.x,
            sellCubeFloorContainerSettings.topLeftCorner.y,
            sellCubeFloorContainerSettings.width,
            sellCubeFloorContainerSettings.height,
            Fade(sellCubeFloorContainerSettings.color, 0.8f));
    }

    //buy weapons
    if(weaponWeakButtonSettings.isEnabled)
    {
        if (hoveredButton == &weaponWeakButton || selectedTwoStepButton == &weaponWeakButton)
        {
            DrawRectangle(weaponWeakButtonSettings.topLeftCorner.x, weaponWeakButtonSettings.topLeftCorner.y, weaponWeakButtonSettings.width, weaponWeakButtonSettings.height, weaponWeakButtonSettings.color);
        }
        DrawTexture(weaponWeakIcon, weaponWeakButtonSettings.topLeftCorner.x + 2, weaponWeakButtonSettings.topLeftCorner.y + 2, WHITE);
    }

    if (weaponStrongButtonSettings.isEnabled)
    {
        if (hoveredButton == &weaponStrongButton || selectedTwoStepButton == &weaponStrongButton)
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

    //sell cubes
    if (sellButtonSettings.isEnabled)
    {
        if (hoveredButton == &sellButton || selectedTwoStepButton == &sellButton)
        {
            DrawRectangle(sellButtonSettings.topLeftCorner.x, sellButtonSettings.topLeftCorner.y, sellButtonSettings.width, sellButtonSettings.height, sellButtonSettings.color);
        }

        DrawTexture(sellCubeIcon, sellButtonSettings.topLeftCorner.x + 2, sellButtonSettings.topLeftCorner.y + 2, WHITE);
    }

    //floors for selling
    if (sellBlock1ButtonSettings.isEnabled)
    {
        if (hoveredButton == &sellBlock1Button)
        {
            DrawRectangle(sellBlock1ButtonSettings.topLeftCorner.x, sellBlock1ButtonSettings.topLeftCorner.y, sellBlock1ButtonSettings.width, sellBlock1ButtonSettings.height, sellBlock1ButtonSettings.color);
        }

        DrawText("1", sellBlock1ButtonSettings.topLeftCorner.x + 8, sellBlock1ButtonSettings.topLeftCorner.y + 3, 16, RAYWHITE);
    }

    if (sellBlock2ButtonSettings.isEnabled)
    {
        if (hoveredButton == &sellBlock2Button)
        {
            DrawRectangle(sellBlock2ButtonSettings.topLeftCorner.x, sellBlock2ButtonSettings.topLeftCorner.y, sellBlock2ButtonSettings.width, sellBlock2ButtonSettings.height, sellBlock2ButtonSettings.color);
        }

        DrawText("2", sellBlock2ButtonSettings.topLeftCorner.x + 6, sellBlock2ButtonSettings.topLeftCorner.y + 3, 16, RAYWHITE);
    }

    if (sellBlock3ButtonSettings.isEnabled)
    {
        if (hoveredButton == &sellBlock3Button)
        {
            DrawRectangle(sellBlock3ButtonSettings.topLeftCorner.x, sellBlock3ButtonSettings.topLeftCorner.y, sellBlock3ButtonSettings.width, sellBlock3ButtonSettings.height, sellBlock3ButtonSettings.color);
        }

        DrawText("3", sellBlock3ButtonSettings.topLeftCorner.x + 6, sellBlock3ButtonSettings.topLeftCorner.y + 3, 16, RAYWHITE);
    }

    //floors for weapons
    if (weaponBlock1ButtonSettings.isEnabled)
    {
        if (hoveredButton == &weaponBlock1Button)
        {
            DrawRectangle(weaponBlock1ButtonSettings.topLeftCorner.x, weaponBlock1ButtonSettings.topLeftCorner.y, weaponBlock1ButtonSettings.width, weaponBlock1ButtonSettings.height, weaponBlock1ButtonSettings.color);
        }

        DrawText("1", weaponBlock1ButtonSettings.topLeftCorner.x + 8, weaponBlock1ButtonSettings.topLeftCorner.y + 3, 16, RAYWHITE);
    }
    
    if (weaponBlock2ButtonSettings.isEnabled)
    {
        if (hoveredButton == &weaponBlock2Button)
        {
            DrawRectangle(weaponBlock2ButtonSettings.topLeftCorner.x, weaponBlock2ButtonSettings.topLeftCorner.y, weaponBlock2ButtonSettings.width, weaponBlock2ButtonSettings.height, weaponBlock2ButtonSettings.color);
        }

        DrawText("2", weaponBlock2ButtonSettings.topLeftCorner.x + 6, weaponBlock2ButtonSettings.topLeftCorner.y + 3, 16, RAYWHITE);
    }
    
    if (weaponBlock3ButtonSettings.isEnabled)
    {
        if (hoveredButton == &weaponBlock3Button)
        {
            DrawRectangle(weaponBlock3ButtonSettings.topLeftCorner.x, weaponBlock3ButtonSettings.topLeftCorner.y, weaponBlock3ButtonSettings.width, weaponBlock3ButtonSettings.height, weaponBlock3ButtonSettings.color);
        }

        DrawText("3", weaponBlock3ButtonSettings.topLeftCorner.x + 6, weaponBlock3ButtonSettings.topLeftCorner.y + 3, 16, RAYWHITE);
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

void UiShowRightSideButtons()
{
    tileButtonsContainerSettings.isEnabled = true;
    sellCubeButtonContainerSettings.isEnabled = true;
    sellCubeFloorContainerSettings.isEnabled = false;
    weaponsFloorContainerSettings.isEnabled = false;

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

    sellButtonSettings.isEnabled = true;

    sellBlock1ButtonSettings.isEnabled = false;
    sellBlock2ButtonSettings.isEnabled = false;
    sellBlock3ButtonSettings.isEnabled = false;

    weaponBlock1ButtonSettings.isEnabled = false;
    weaponBlock2ButtonSettings.isEnabled = false;
    weaponBlock3ButtonSettings.isEnabled = false;
}

void UiHideRightSideButtons()
{
    hoveredButton = 0;

    tileButtonsContainerSettings.isEnabled = false;
    sellCubeButtonContainerSettings.isEnabled = false;
    sellCubeFloorContainerSettings.isEnabled = false;
    weaponsFloorContainerSettings.isEnabled = false;

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

    sellButtonSettings.isEnabled = false;

    sellBlock1ButtonSettings.isEnabled = false;
    sellBlock2ButtonSettings.isEnabled = false;
    sellBlock3ButtonSettings.isEnabled = false;

    weaponBlock1ButtonSettings.isEnabled = false;
    weaponBlock2ButtonSettings.isEnabled = false;
    weaponBlock3ButtonSettings.isEnabled = false;
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

WeaponType WhichWeaponIsSelected()
{
    if (selectedTwoStepButton == &weaponWeakButton)
    {
        return WeaponWeak;
    }
    else if (selectedTwoStepButton == &weaponStrongButton)
    {
        return WeaponStrong;
    }

    return WEAPON_NONE;
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

//  weapon floor
bool UiIsWeaponFloor1ButtonPressed()
{
    if (weaponBlock1ButtonSettings.isEnabled == false) return false;
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return false;
    return CheckCollisionPointRec(GetMousePosition(), weaponBlock1Button);
}

bool UiIsWeaponFloor2ButtonPressed()
{
    if (weaponBlock2ButtonSettings.isEnabled == false) return false;
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return false;
    return CheckCollisionPointRec(GetMousePosition(), weaponBlock2Button);
}

bool UiIsWeaponFloor3ButtonPressed()
{
    if (weaponBlock3ButtonSettings.isEnabled == false) return false;
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return false;
    return CheckCollisionPointRec(GetMousePosition(), weaponBlock3Button);
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

//selling
bool UiIsSellButtonPressed()
{
    if (sellButtonSettings.isEnabled == false) return false;
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return false;
    return CheckCollisionPointRec(GetMousePosition(), sellButton);
}

//  floor for selling
bool UiIsSellFloor1Pressed()
{
    if (sellBlock1ButtonSettings.isEnabled == false) return false;
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return false;
    return CheckCollisionPointRec(GetMousePosition(), sellBlock1Button);
}

bool UiIsSellFloor2Pressed()
{
    if (sellBlock2ButtonSettings.isEnabled == false) return false;
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return false;
    return CheckCollisionPointRec(GetMousePosition(), sellBlock2Button);
}

bool UiIsSellFloor3Pressed()
{
    if (sellBlock3ButtonSettings.isEnabled == false) return false;
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return false;
    return CheckCollisionPointRec(GetMousePosition(), sellBlock3Button);
}

//thunderbolt
bool UiIsThunderboltPressed()
{
    if (thunderboltButtonSettings.isEnabled == false) return false;
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return false;
    return CheckCollisionPointRec(GetMousePosition(), thunderboltButton);
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
