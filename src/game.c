#include "game.h"
#include "enemy.h"
#include "terrain.h"
#include "camera.h"
#include "raymath.h"

static const int screenWidth = 256;
static const int screenHeight = 256;

static RenderTexture2D target = {0}; // Initialized at init

static Texture cursorTexture;
static Texture cursorStraightTexture;

static Model pig;
static Model wolf;

static Texture2D backgroundTexture;

static Vector3 YAW = {0, 1.0f, 0};

static float pigScale = 0.025f;

static Enemy enemies[30];
static int enemiesCount = 0;

bool showTileInfo = false;

void Init()
{
    InitWindow(screenWidth, screenHeight, "Unexpected");
    target = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);    

    CameraInit();

    pig = LoadModel("resources/pig.glb");
    wolf = LoadModel("resources/wolf.glb");

    backgroundTexture = LoadTexture("resources/background.png");
    cursorTexture = LoadTexture("resources/cursor.png");
    cursorStraightTexture = LoadTexture("resources/cursor_straight.png");

    TerrainInit();

    int totalEnemies = 5;
    for (int i = 0; i < totalEnemies; i++)
    {
        Enemy *enemy = &enemies[i];
        *enemy = (Enemy){0};
        enemy->speed = 10;
        enemy->position = TerrainGetTile(i, 6)->position;
        enemy->target = TerrainGetTile(FORTRESS_FIRST_TILE_INDEX, i)->position;
        enemy->model = wolf;
        enemy->coldown = 3.0;
        enemiesCount++;
    }
}

void Release()
{
    UnloadRenderTexture(target);

    UnloadTexture(backgroundTexture);
    UnloadTexture(cursorTexture);
    UnloadTexture(cursorStraightTexture);

    UnloadModel(pig);
    UnloadModel(wolf);

    TerrainRelease();

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

    CameraUpdate();

    TerrainUpdate();

    for (int i = 0; i < enemiesCount; i++)
    {
        EnemyUpdate(&enemies[i]);
    }

    if (IsKeyPressed(KEY_I))
    {
        showTileInfo = !showTileInfo;
    }


    // TODO: Update variables / Implement example logic at this point
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    // Render all screen to texture (for scaling)

    BeginTextureMode(target);
    {

        BeginMode3D(GetCamera());
        ClearBackground(BLACK);

        for (int i = 0; i < enemiesCount; i++)
        {
            EnemyRender(&enemies[i]);
        }

        TerrainRender();

        EndMode3D();

        //DrawText(TextFormat("(%04f,%04f)", mousePosition.x, mousePosition.y), 0, 0, 20, RED);
        //DrawCircleLines(GetMouseX(), GetMouseY(), 5.0f, MAROON);
        //DrawTexture(cursorTexture, GetMouseX() - 6, GetMouseY() - 2, WHITE);
        DrawTexture(cursorStraightTexture, GetMouseX() - 10, GetMouseY(), WHITE);
        if (IsCursorOnScreen()) HideCursor();
        else ShowCursor();

        if (showTileInfo)
        {
            char* tileTypeString = "grass";
            if(GetTileSelected().tileType == LavaType) tileTypeString = "lava";
            else if(GetTileSelected().tileType == DirtType) tileTypeString = "dirt";

            DrawText(tileTypeString, 0, 0, 20, RED);
        }

        DrawFPS(232, 0);
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


