#include "uiManager.h"
#include "terrain.h"
#include "raymath.h"

static int screenWidth;     //passed from game.c
static int screenHeight;    //passed from game.c

static Texture cursorTexture;
static Texture cursorStraightTexture;

static Rectangle tileButtonsContainer;
static RectangleSettings tileButtonsContainerSettings;
static int containerMargin = 2;

void UiInit(int aScreenWidth, int aScreenHeight)
{
    screenWidth = aScreenWidth;
    screenHeight = aScreenHeight;

    //textures
    cursorTexture = LoadTexture("resources/cursor.png");
    cursorStraightTexture = LoadTexture("resources/cursor_straight.png");

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
}

void UiUpdate(bool isPaused)
{
	//things like size and positioning or movement of ui
}

void UiRender(bool isPaused)
{
    //DrawText(TextFormat("(%04f,%04f)", mousePosition.x, mousePosition.y), 0, 0, 20, RED);
    //DrawCircleLines(GetMouseX(), GetMouseY(), 5.0f, MAROON);
    //DrawTexture(cursorTexture, GetMouseX() - 6, GetMouseY() - 2, WHITE);

    // Tile Info
    if (ShouldShowTileInfo())
    {
        char* tileTypeString = "grass";
        if (TerrainGetTileSelected()->tileType == LavaType) tileTypeString = "lava";
        else if (TerrainGetTileSelected()->tileType == DirtType) tileTypeString = "dirt";

        DrawText(tileTypeString, 0, 0, 20, RED);
    }

    //tile buttons container
    if (tileButtonsContainerSettings.isEnabled)
    {
        DrawRectangle(tileButtonsContainer.x - containerMargin, 
            tileButtonsContainer.y - containerMargin, 
            tileButtonsContainer.width + 2 * containerMargin + 2,
            tileButtonsContainer.height + 2 * containerMargin + 2,
            BLACK);
        DrawRectangleRounded(tileButtonsContainer, tileButtonsContainerSettings.roundness, 3, tileButtonsContainerSettings.color);
    }

    // Pause overlay
    if (isPaused)
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.5f));
        DrawText("GAME PAUSED", 66, 98, 18, MAROON);
    }

    // FPS
    DrawFPS(232, 0);

    // Cursor
    DrawTexture(cursorStraightTexture, GetMouseX() - 10, GetMouseY(), WHITE);
    if (IsCursorOnScreen()) HideCursor();
    else ShowCursor();
}

void ShowTileButtons()
{
    tileButtonsContainerSettings.isEnabled = true;
    //TODO enable all the others
}

void HideTileButtons()
{
    tileButtonsContainerSettings.isEnabled = false;
    //TODO disable all the others
}

void UiRelease()
{
    UnloadTexture(cursorTexture);
    UnloadTexture(cursorStraightTexture);
}

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
