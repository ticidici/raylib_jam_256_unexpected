#include "uiManager.h"
#include "terrain.h"
#include "camera.h"
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

static Texture wheatCubeIcon;
static Texture woodCubeIcon;
static Texture brickCubeIcon;

static Texture weaponWeakIcon;
static Texture weaponStrongIcon;


static Texture sellCubeIcon;//also changes the cursor 
static Texture thunderboltIcon;//also changes the cursor 

static Rectangle tileButtonsContainer;
static RectangleSettings tileButtonsContainerSettings;
static int containerMargin = 2;

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

    sellCubeIcon = LoadTexture("resources/sell.png");
    thunderboltIcon = LoadTexture("resources/thunderbolt.png");

    //settings
    Vector2 containerTopLeft = { 232, 130 };
    Vector2 containerBottomRight = { (float)(screenWidth), (float)(screenHeight) };
    Vector2 containerDimensions = GetWidthAndHeightFromCorners(containerTopLeft, containerBottomRight);
    tileButtonsContainerSettings.topLeftCorner = containerTopLeft;
    tileButtonsContainerSettings.width = containerDimensions.x;
    tileButtonsContainerSettings.height = containerDimensions.y;
    tileButtonsContainerSettings.color = BEIGE;
    tileButtonsContainerSettings.isEnabled = false;
    SetRectangleSettings(&tileButtonsContainer, tileButtonsContainerSettings);

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

}


void UiUpdate(bool isPaused)
{
	//things like size and positioning or movement of ui
    //Toggle UI
    if (IsKeyPressed(KEY_U) || (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE) && !IsCamIsometric()))
    {
        hideUI = !hideUI;
    }

    Vector2 mousePosition = GetMousePosition();
    
    //check hovered
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
}

void UiRender(bool isPaused)
{
    //DrawText(TextFormat("(%04f,%04f)", mousePosition.x, mousePosition.y), 0, 0, 20, RED);
    //DrawCircleLines(GetMouseX(), GetMouseY(), 5.0f, MAROON);
    //DrawTexture(cursorTexture, GetMouseX() - 6, GetMouseY() - 2, WHITE);


    if (!hideUI)
    {
        // --------- Resources on Top ----------
        DrawRectangle(0, 0, 256, 24, Fade(BEIGE, 0.8f));

        DrawTextureEx(coinsIcon, (Vector2) { 6.0f, 4.0f }, 0, 1, WHITE);
        DrawText("000", 25, 6, 14, RAYWHITE);//icon +19

        DrawTextureEx(wheatIcon, (Vector2) { 56.0f, 4.0f }, 0, 1, WHITE);//icon +50
        DrawText("000", 75, 6, 14, RAYWHITE);

        DrawTextureEx(woodIcon, (Vector2) { 106.0f, 4.0f }, 0, 1, WHITE);
        DrawText("000", 125, 6, 14, RAYWHITE);

        DrawTextureEx(brickIcon, (Vector2) { 156.0f, 4.0f }, 0, 1, WHITE);
        DrawText("000", 175, 6, 14, RAYWHITE);

        DrawTextureEx(ironIcon, (Vector2) { 206.0f, 4.0f }, 0, 1, WHITE);
        DrawText("000", 225, 6, 14, RAYWHITE);

        // ------------------------------------

        // -------------- Tile Info -----------
        if (ShouldShowTileInfo())
        {
            DrawRectangle(0, 116, 90, 150, Fade(BEIGE, 0.8f));

            char* tileTypeString = "";

            //tile type
            if (TerrainGetTileSelected()->tileType == GrassType) tileTypeString = "grass";
            else if (TerrainGetTileSelected()->tileType == WheatType) tileTypeString = "wheat";
            else if (TerrainGetTileSelected()->tileType == WoodType) tileTypeString = "wood";
            else if (TerrainGetTileSelected()->tileType == ClayType) tileTypeString = "clay";
            else if (TerrainGetTileSelected()->tileType == LavaType) tileTypeString = "lava";
            else if (TerrainGetTileSelected()->tileType == DirtType) tileTypeString = "dirt";

            DrawText(tileTypeString, 4, 120, 12, RED);

            //stack
            DrawText("stack: 3", 4, 135, 12, RED);
            {
                DrawText("\tstraw", 4, 150, 12, RED);//material or "not built"
                    DrawText("\t\tweak gun", 4, 165, 12, RED);
                DrawText("\tstick", 4, 180, 12, RED);
                    DrawText("\t\tstrong gun", 4, 195, 12, RED);
                DrawText("\tbrick", 4, 210, 12, RED);
                    DrawText("\t\tunarmed", 4, 225, 12, RED);
            }

            //enemy
            DrawText("wolf 3HP", 4, 240, 12, RED);//enemy name or "no enemy"
        }
        // ------------------------------------
    }

    //tile buttons container
    if (tileButtonsContainerSettings.isEnabled)
    {
        DrawRectangle(tileButtonsContainerSettings.topLeftCorner.x,
            tileButtonsContainerSettings.topLeftCorner.y,
            tileButtonsContainerSettings.width,
            tileButtonsContainerSettings.height,
            Fade(tileButtonsContainerSettings.color, 0.8f));
    }

    
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
    grassButtonSettings.isEnabled = true;
    wheatButtonSettings.isEnabled = true;
    woodButtonSettings.isEnabled = true;
    clayButtonSettings.isEnabled = true;
    lavaButtonSettings.isEnabled = true;
}

void UiHideTileButtons()
{
    tileButtonsContainerSettings.isEnabled = false;
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

   UnloadTexture(wheatCubeIcon);
   UnloadTexture(woodCubeIcon);
   UnloadTexture(brickCubeIcon);

   UnloadTexture(weaponWeakIcon);
   UnloadTexture(weaponStrongIcon);

   UnloadTexture(sellCubeIcon); 
   UnloadTexture(thunderboltIcon);

}

//BUTTON PRESSED FUNCTIONS
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
