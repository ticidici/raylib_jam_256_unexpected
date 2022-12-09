#include "game.h"
#include "enemy.h"
#include "terrain.h"
#include "camera.h"
#include "building.h"
#include "uiManager.h"
#include "game_state.h"
#include "bullet.h"

#include "raymath.h"

static const int screenWidth = 256;
static const int screenHeight = 256;

static RenderTexture2D target = {0}; // Initialized at init

static Texture2D backgroundTexture;

static Vector3 YAW = {0, 1.0f, 0};

void Init()
{
    InitWindow(screenWidth, screenHeight, "Unexpected");
    target = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    GameStateInit();

    CameraInit();

    UiInit(screenWidth, screenHeight);

    TerrainInit();
    BuildingInit();
    EnemyInit();
    BulletInit();
}

void Release()
{
    UnloadRenderTexture(target);

    UnloadTexture(backgroundTexture);

    EnemyRelease();
    TerrainRelease();
    BuildingRelease();
    BulletRelease();

    CloseWindow();
}

void Update()
{
    // Screen scale logic (x2)
    if (IsKeyPressed(KEY_ONE))
        SetCurrentScreenScale(1);
    else if (IsKeyPressed(KEY_TWO))
        SetCurrentScreenScale(2);
    else if (IsKeyPressed(KEY_THREE))
        SetCurrentScreenScale(3);

    unsigned int screenScale = GetCurrentScreenScale();

    if (screenScale != GetPreviousScreenScale())
    {
        // Scale window to fit the scaled render texture
        SetWindowSize(screenWidth * screenScale, screenHeight * screenScale);

        // Scale mouse proportionally to keep input logic inside the 256x256 screen limits
        SetMouseScale(1.0f / (float)screenScale, 1.0f / (float)screenScale);

        SetPreviousScreenScale(screenScale);
    }

    // Update variables / Implement example logic at this point
    //----------------------------------------------------------------------------------
    GameStateUpdate();

    if (!IsPaused())
    {
        CameraUpdate();
        TerrainUpdate();
        EnemyUpdate();
        BulletUpdate();
    }

    UiUpdate();

    // Draw
    //----------------------------------------------------------------------------------

    // Render all screen to texture (for scaling)
    BeginTextureMode(target);
    {

        BeginMode3D(GetCamera());
        ClearBackground(BLACK);

        TerrainRender();
        EnemyRender();
        BulletRender();

        EndMode3D();

        UiRender();       
    }

    EndTextureMode();

    BeginDrawing();
    {
        ClearBackground(RAYWHITE);

        // Draw render texture to screen scaled as required
        DrawTexturePro(target.texture, (Rectangle){0, 0, (float)target.texture.width, -(float)target.texture.height}, (Rectangle){0, 0, (float)target.texture.width * GetCurrentScreenScale(), (float)target.texture.height * GetCurrentScreenScale()}, (Vector2){0, 0}, 0.0f, WHITE);
    }
    EndDrawing();
}
