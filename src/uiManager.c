#include "uiManager.h"
#include "terrain.h"

static Texture cursorTexture;
static Texture cursorStraightTexture;

//static Rectangle

void UiInit()
{
    cursorTexture = LoadTexture("resources/cursor.png");
    cursorStraightTexture = LoadTexture("resources/cursor_straight.png");
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

    if (ShouldShowTileInfo())
    {
        char* tileTypeString = "grass";
        if (TerrainGetTileSelected()->tileType == LavaType) tileTypeString = "lava";
        else if (TerrainGetTileSelected()->tileType == DirtType) tileTypeString = "dirt";

        DrawText(tileTypeString, 0, 0, 20, RED);
    }


    if (isPaused)
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.5f));
        DrawText("GAME PAUSED", 66, 98, 18, MAROON);
    }

    DrawFPS(232, 0);

    DrawTexture(cursorStraightTexture, GetMouseX() - 10, GetMouseY(), WHITE);
    if (IsCursorOnScreen()) HideCursor();
    else ShowCursor();
}

void UiRelease()
{
    UnloadTexture(cursorTexture);
    UnloadTexture(cursorStraightTexture);
}
