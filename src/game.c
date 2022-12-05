#include "game.h"

static unsigned int screenWidth = 256;
static unsigned int screenHeight = 256;
static unsigned int screenScale = 3;
static unsigned int prevScreenScale = 1;

static RenderTexture2D target = {0};
static Camera camera = {0};
static Model tower = {0};
static Texture2D texture = {0};
static Vector3 towerPos = {0.0f, 0.0f, 0.0f};

void Init()
{
    InitWindow(screenWidth, screenHeight, "Unexpected");
    target = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);
    camera.position = (Vector3){200.0f, 200.0f, 200.0f}; // Camera position
    camera.target = (Vector3){0.0f, 8.0f, 0.0f};         // Camera looking at point
    camera.up = (Vector3){0.0f, 1.6f, 0.0f};             // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                 // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;              // Camera mode type

    tower = LoadModel("resources/turret.obj");
    texture = LoadTexture("resources/turret_diffuse.png");           // Load model texture
    tower.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture; // Set model diffuse texture

    SetCameraMode(camera, CAMERA_FREE); // Set a free camera mode
}

void Release()
{
    UnloadRenderTexture(target);
    UnloadTexture(texture);
    UnloadModel(tower);
    CloseWindow();
}

void Update()
{
    // Screen scale logic (x2)
    if (IsKeyPressed(KEY_ONE))
        screenScale = 1;
    else if (IsKeyPressed(KEY_TWO))
        screenScale = 2;
    else if (IsKeyPressed(KEY_THREE))
        screenScale = 3;

    if (screenScale != prevScreenScale)
    {
        // Scale window to fit the scaled render texture
        SetWindowSize(screenWidth * screenScale, screenHeight * screenScale);

        // Scale mouse proportionally to keep input logic inside the 256x256 screen limits
        SetMouseScale(1.0f / (float)screenScale, 1.0f / (float)screenScale);

        prevScreenScale = screenScale;
    }

    UpdateCamera(&camera);

    // Render all screen to texture (for scaling)
    BeginTextureMode(target);
    {

        BeginMode3D(camera);
        ClearBackground(RAYWHITE);

        DrawModel(tower, towerPos, 10.0f, WHITE);
        DrawGrid(10, 10.0f);

        EndMode3D();

        DrawCircleLines(GetMouseX(), GetMouseY(), 10, MAROON);
    }
    EndTextureMode();

    BeginDrawing();
    {
        ClearBackground(RAYWHITE);

        // Draw render texture to screen scaled as required
        DrawTexturePro(target.texture, (Rectangle){0, 0, (float)target.texture.width, -(float)target.texture.height}, (Rectangle){0, 0, (float)target.texture.width * screenScale, (float)target.texture.height * screenScale}, (Vector2){0, 0}, 0.0f, WHITE);
    }
    EndDrawing();
}
