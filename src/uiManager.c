#include "uiManager.h"
#include "terrain.h"
#include "raymath.h"

static int screenWidth;     //passed from game.c
static int screenHeight;    //passed from game.c

static Texture cursorTexture;
static Texture cursorStraightTexture;

static Texture coinsIcon;

static Texture wheatIcon;//used both for resources and tile
static Texture woodIcon;
static Texture brickIcon;
static Texture ironIcon;

static Texture wheatCubeIcon;
static Texture woodCubeIcon;
static Texture brickCubeIcon;

static Texture weaponWeakIcon;
static Texture weaponStrongIcon;

static Texture sellCubeIcon;//also changes the cursor 

static Rectangle tileButtonsContainer;
static RectangleSettings tileButtonsContainerSettings;
static int containerMargin = 2;

static Rectangle grassButton;
static RectangleSettings grassButtonSettings;
static Rectangle lavaButton;
static RectangleSettings lavaButtonSettings;

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
    sellCubeIcon = LoadTexture("resources/sell.png");

    //settings
    Vector2 containerTopLeft = { screenWidth * 0.9f, screenHeight * 0.8f };
    Vector2 containerBottomRight = { (float)(screenWidth - containerMargin), (float)(screenHeight - containerMargin) };
    Vector2 containerDimensions = GetWidthAndHeightFromCorners(containerTopLeft, containerBottomRight);
    tileButtonsContainerSettings.topLeftCorner = containerTopLeft;
    tileButtonsContainerSettings.width = containerDimensions.x;
    tileButtonsContainerSettings.height = containerDimensions.y;
    tileButtonsContainerSettings.color = BEIGE;
    tileButtonsContainerSettings.roundness = 0.5f;
    tileButtonsContainerSettings.isEnabled = false;
    SetRectangleSettings(&tileButtonsContainer, tileButtonsContainerSettings);






    //testing the shit
    grassButtonSettings.topLeftCorner = containerTopLeft;
    grassButtonSettings.width = 20.0f;
    grassButtonSettings.height = 20.0f;
    grassButtonSettings.color = GREEN;
    grassButtonSettings.roundness = 0.0f;
    grassButtonSettings.isEnabled = false;
    SetRectangleSettings(&grassButton, grassButtonSettings);

    lavaButtonSettings.topLeftCorner = (Vector2){ containerTopLeft.x, containerTopLeft.y + 23.0f};
    lavaButtonSettings.width = 20.0f;
    lavaButtonSettings.height = 20.0f;
    lavaButtonSettings.color = RED;
    lavaButtonSettings.roundness = 0.0f;
    lavaButtonSettings.isEnabled = false;
    SetRectangleSettings(&lavaButton, lavaButtonSettings);
}


void UiUpdate(bool isPaused)
{
	//things like size and positioning or movement of ui
    //Toggle UI
    if (IsKeyPressed(KEY_U))
    {
        hideUI = !hideUI;
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
        DrawRectangle(tileButtonsContainer.x - containerMargin, 
            tileButtonsContainer.y - containerMargin, 
            tileButtonsContainer.width + 2 * containerMargin + 2,
            tileButtonsContainer.height + 2 * containerMargin + 2,
            Fade(BLACK, 0.5f));
        DrawRectangleRounded(tileButtonsContainer, tileButtonsContainerSettings.roundness, 3, Fade(tileButtonsContainerSettings.color, 0.5f));
    }

    if (lavaButtonSettings.isEnabled)
    {
        DrawRectangle(lavaButtonSettings.topLeftCorner.x, lavaButtonSettings.topLeftCorner.y, lavaButtonSettings.width, lavaButtonSettings.height, lavaButtonSettings.color);
    }
    
    if (grassButtonSettings.isEnabled)
    {
        DrawRectangle(grassButtonSettings.topLeftCorner.x, grassButtonSettings.topLeftCorner.y, grassButtonSettings.width, grassButtonSettings.height, grassButtonSettings.color);
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
    //TODO enable all the others
    lavaButtonSettings.isEnabled = true;
    grassButtonSettings.isEnabled = true;
}

void UiHideTileButtons()
{
    tileButtonsContainerSettings.isEnabled = false;
    //TODO disable all the others
    lavaButtonSettings.isEnabled = false;
    grassButtonSettings.isEnabled = false;
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
   UnloadTexture(wheatCubeIcon);
   UnloadTexture(woodCubeIcon);
   UnloadTexture(brickCubeIcon);
   UnloadTexture(weaponWeakIcon);
   UnloadTexture(weaponStrongIcon);
   UnloadTexture(sellCubeIcon); 
}

//BUTTON PRESSED FUNCTIONS
bool UiIsTileGrassButtonPressed()
{
    if (grassButtonSettings.isEnabled == false) return false;
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return false;
    return CheckCollisionPointRec(GetMousePosition(), grassButton);
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
