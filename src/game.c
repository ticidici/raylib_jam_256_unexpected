#include "game.h"
#include "enemy.h"
#include "terrain.h"

static const int screenWidth = 256;
static const int screenHeight = 256;

static unsigned int screenScale = 3;
static unsigned int prevScreenScale = 1;

static RenderTexture2D target = {0}; // Initialized at init

static Model pig;
static Model wolf;

static Texture2D backgroundTexture;

static Camera camera = {0};

static int cameraPositionIndex = 0;
static Vector3 camIsometricPositions[4] = {
    {25.0f, 25.0, 25.0f},
    {25.0f, 25.0, -25.0f},
    {-25.0f, 25.0, -25.0f},
    {-25.0f, 25.0, 25.0f}};
static Vector3 camZenitalPos = {0.0f, 25.0, 0.0f};
static float camIsometricFortressFov = 28.0f;
static float camIsometricBattlefieldFov = 52.0f;
static float camZenitalFortressFov = 28.0f;
static float camZenitalBattlefieldFov = 52.0f;

static bool isCamIsometric = true;
static bool isBattlefieldView = false;

static Vector3 YAW = {0, 1.0f, 0};

static Vector3 camPerspectivePos = {10.0f, 25.0, 10.0f};
static float camPerspectiveFov = 60.0f;

static float pigScale = 0.025f;

static Enemy enemies[30];
static int enemiesCount = 0;

void Init()
{
    InitWindow(screenWidth, screenHeight, "Unexpected");
    target = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    //-------- CAM SETTINGS ---------
    // camera.position = camPerspectivePos; // Camera position
    camera.position = camIsometricPositions[0]; // Camera position
    // camera.position = camZenitalPos; // Camera position

    camera.target = (Vector3){0.0f, 0.0f, 0.0f}; // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};     // Camera up vector (rotation towards target)

    // camera.fovy = camPerspectiveFov;                                // Camera field-of-view Y
    // camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    camera.fovy = camIsometricFortressFov;   // Camera field-of-view Y
    camera.projection = CAMERA_ORTHOGRAPHIC; // Camera mode type
    SetCameraMode(camera, CAMERA_FREE);      // Set a free camera mode

    //-------- /CAM SETTINGS ---------

    pig = LoadModel("resources/pig.glb");
    wolf = LoadModel("resources/wolf.glb");

    backgroundTexture = LoadTexture("resources/background.png");

    TerrainInit();

    int totalEnemies = 5;
    for (int i = 0; i < totalEnemies; i++)
    {
        Enemy *enemy = &enemies[i];
        *enemy = (Enemy){0};
        enemy->speed = 10;
        enemy->position = battlefieldTiles[i][6].position;
        enemy->target = battlefieldTiles[FORTRESS_FIRST_TILE_INDEX][i].position;
        enemy->model = wolf;
        enemy->coldown = 3.0;
        enemiesCount++;
    }
}

void Release()
{
    UnloadRenderTexture(target);

    UnloadTexture(backgroundTexture);

    UnloadModel(pig);
    UnloadModel(wolf);

    TerrainRelease();

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

    bool cameraAngleChanged = false;

    if (IsKeyPressed(KEY_F))
    {
        isBattlefieldView = !isBattlefieldView;
        SetView(isBattlefieldView);
    }
    else if (IsKeyPressed(KEY_Q) && isCamIsometric)
    {
        TurnCamera(false);
        SetCameraMode(camera, CAMERA_CUSTOM);
        cameraAngleChanged = true;
    }
    else if (IsKeyPressed(KEY_E) && isCamIsometric)
    {
        TurnCamera(true);
        SetCameraMode(camera, CAMERA_CUSTOM);
        cameraAngleChanged = true;
    }
    else if (IsKeyPressed(KEY_C))
    {
        isCamIsometric = !isCamIsometric;
        SetView(isBattlefieldView);

        if (isCamIsometric)
        {
            camera.position = camIsometricPositions[cameraPositionIndex];
        }
        else
        {
            camera.position = camZenitalPos;
        }

        camera.target = (Vector3){0.0f, 0.0f, 0.0f};

        SetCameraMode(camera, CAMERA_FREE);
        UpdateCamera(&camera);
        cameraAngleChanged = true;
    }

    if (!cameraAngleChanged)
    {
        // unless we use CUSTOM_CAMERA UpdateCamera overrides our rotation in favor of mouse input
        if (isCamIsometric)
        {
            UpdateCamera(&camera);
            SetCameraMode(camera, CAMERA_FREE);
        }
    }

    TerrainUpdate();

    for (int i = 0; i < enemiesCount; i++)
    {
        EnemyUpdate(&enemies[i]);
    }

    // TODO: Update variables / Implement example logic at this point
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    // Render all screen to texture (for scaling)

    BeginTextureMode(target);
    {

        BeginMode3D(camera);
        ClearBackground(BLACK);
        // DrawTexture(towerTexture, 0, 0, WHITE);

        for (int i = 0; i < enemiesCount; i++)
        {
            EnemyRender(&enemies[i]);
        }

        TerrainRender();

        EndMode3D();

        DrawCircleLines(GetMouseX(), GetMouseY(), 5.0f, MAROON);
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

void TurnCamera(bool right)
{
    if (right)
    {
        cameraPositionIndex++;
    }
    else
    {
        cameraPositionIndex--;
    }

    if (cameraPositionIndex < 0)
        cameraPositionIndex = 3;
    else if (cameraPositionIndex > 3)
        cameraPositionIndex = 0;

    camera.position = camIsometricPositions[cameraPositionIndex];
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    // camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
}

void SetView(bool aIsBattlefieldView)
{
    if (aIsBattlefieldView)
    {
        if (isCamIsometric)
            camera.fovy = camIsometricBattlefieldFov;
        else
            camera.fovy = camZenitalBattlefieldFov;
    }
    else
    {
        if (isCamIsometric)
            camera.fovy = camIsometricFortressFov;
        else
            camera.fovy = camZenitalFortressFov;
    }
}
