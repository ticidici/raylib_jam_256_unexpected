#include "game.h"

static const int screenWidth = 256;
static const int screenHeight = 256;

static unsigned int screenScale = 3;
static unsigned int prevScreenScale = 1;

static RenderTexture2D target = {0}; // Initialized at init

#define FORTRESS_SIZE 7     // have to be odd numbers
#define BATTLEFIELD_SIZE 13 // have to be odd numbers
#define BATTLEFIELD_RING_WIDTH (BATTLEFIELD_SIZE - FORTRESS_SIZE) / 2
#define MIDDLE_TILE_INDEX (BATTLEFIELD_SIZE - 1) / 2
#define FORTRESS_FIRST_TILE_INDEX BATTLEFIELD_RING_WIDTH
#define FORTRESS_LAST_TILE_INDEX BATTLEFIELD_SIZE - BATTLEFIELD_RING_WIDTH - 1
#define BATTLEFIELD_LAST_TILE_INDEX BATTLEFIELD_SIZE - 1

static Model tileSelector;

static Model red1;
static Model red2;
static Model red3;
static Model red4;

static Model pig;
static Model wolf;

static Model grassTile;
static Model grass2Tile;
static Model lavaTile;
static Model dirtBrownTile;
static Model dirtGrayTile;

static Texture2D backgroundTexture;

static Camera camera = {0};

static int cameraPositionIndex = 0;
static Vector3 camIsometricPositions[4] = {
    {250.0f, 250.0, 250.0f},
    {250.0f, 250.0, -250.0f},
    {-250.0f, 250.0, -250.0f},
    {-250.0f, 250.0, 250.0f}};
static Vector3 camZenitalPos = {0.0f, 250.0, 0.0f};
static float camIsometricFortressFov = 280.0f;
static float camIsometricBattlefieldFov = 520.0f;
static float camZenitalFortressFov = 280.0f;
static float camZenitalBattlefieldFov = 520.0f;

static bool isCamIsometric = true;
static bool isBattlefieldView = false;

static bool isGridShown = false;

static Vector3 YAW = {0, 1.0f, 0};

static struct Tile
{
    Vector3 positions[4];
    Model tileType;

    // entity occupying?
};

// still unused
static struct Wolf
{
    Vector3 position;
    float rotation; // degrees of yaw
    Vector3 scale;
    float speed;

    struct Tile *tileOccupied;
    Model model;
};

static struct Tile battlefieldTiles[BATTLEFIELD_SIZE][BATTLEFIELD_SIZE];

static Vector3 towerPos = {0.0f, 0.0f, 0.0f};

static Vector3 camPerspectivePos = {100.0f, 250.0, 100.0f};
static float camPerspectiveFov = 60.0f;

static float pigScale = 0.25f;
static float wolfScale = 0.25f;
static Vector3 wolfScaleVec = {0.25f, 0.25f, 0.25f};
static float normalScale = 10.0f;

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

    red1 = LoadModel("resources/red1.gltf");
    red2 = LoadModel("resources/red2.gltf");
    red3 = LoadModel("resources/red3.gltf");
    red4 = LoadModel("resources/red4.gltf");
    pig = LoadModel("resources/pig.glb");
    wolf = LoadModel("resources/wolf.glb");

    tileSelector = LoadModel("resources/tile_selector.gltf");

    grassTile = LoadModel("resources/grass_tile.gltf");
    grass2Tile = LoadModel("resources/grass2_tile.gltf");
    lavaTile = LoadModel("resources/lava_tile.gltf");
    dirtGrayTile = LoadModel("resources/dirtgray_tile.gltf");
    dirtBrownTile = LoadModel("resources/dirtbrown_tile.gltf");

    backgroundTexture = LoadTexture("resources/background.png");

    for (int i = 0; i < BATTLEFIELD_SIZE; i++)
    {
        for (int j = 0; j < BATTLEFIELD_SIZE; j++)
        {
            for (int k = 0; k < 4; k++) // max height, 3 floors + something standing on top
            {
                battlefieldTiles[i][j].positions[k].x = (i - MIDDLE_TILE_INDEX) * 40.0f;
                battlefieldTiles[i][j].positions[k].z = (j - MIDDLE_TILE_INDEX) * 40.0f;
                battlefieldTiles[i][j].positions[k].y = k * 20.0f;

                int randomValue = GetRandomValue(0, 100);
                if ((i >= FORTRESS_FIRST_TILE_INDEX && i <= FORTRESS_LAST_TILE_INDEX) && (j >= FORTRESS_FIRST_TILE_INDEX && j <= FORTRESS_LAST_TILE_INDEX))
                {
                    if (randomValue < 35)
                    {
                        battlefieldTiles[i][j].tileType = lavaTile;
                    }
                    // else if (randomValue < 55)
                    //{
                    //	battlefieldTiles[i][j].tileType = grassTile;
                    // }
                    else
                    {
                        battlefieldTiles[i][j].tileType = grass2Tile;
                    }
                }
                else
                {
                    if (randomValue < 35)
                    {
                        battlefieldTiles[i][j].tileType = dirtGrayTile;
                    }
                    else
                    {
                        battlefieldTiles[i][j].tileType = dirtBrownTile;
                    }
                }
            }
        }
    }
}

void Release()
{
    UnloadRenderTexture(target);

    UnloadTexture(backgroundTexture);

    UnloadModel(tileSelector);

    UnloadModel(pig);
    UnloadModel(wolf);

    UnloadModel(red1);
    UnloadModel(red2);
    UnloadModel(red3);
    UnloadModel(red4);

    UnloadModel(grassTile);
    UnloadModel(grass2Tile);
    UnloadModel(lavaTile);
    UnloadModel(dirtGrayTile);
    UnloadModel(dirtBrownTile);
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

    if (IsKeyPressed(KEY_G))
    {
        isGridShown = !isGridShown;
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

        // corners
        DrawModel(red1, battlefieldTiles[FORTRESS_FIRST_TILE_INDEX][FORTRESS_FIRST_TILE_INDEX].positions[0], normalScale, WHITE);
        DrawModel(red2, battlefieldTiles[FORTRESS_LAST_TILE_INDEX][FORTRESS_LAST_TILE_INDEX].positions[0], normalScale, WHITE);
        DrawModel(red3, battlefieldTiles[FORTRESS_LAST_TILE_INDEX][FORTRESS_FIRST_TILE_INDEX].positions[0], normalScale, WHITE);
        DrawModel(red4, battlefieldTiles[FORTRESS_FIRST_TILE_INDEX][FORTRESS_LAST_TILE_INDEX].positions[0], normalScale, WHITE);

        // center
        DrawModel(red1, battlefieldTiles[MIDDLE_TILE_INDEX][MIDDLE_TILE_INDEX].positions[0], normalScale, WHITE);
        DrawModel(red1, battlefieldTiles[MIDDLE_TILE_INDEX][MIDDLE_TILE_INDEX].positions[1], normalScale, WHITE);
        DrawModel(red4, battlefieldTiles[MIDDLE_TILE_INDEX][MIDDLE_TILE_INDEX].positions[2], normalScale, WHITE);

        DrawModel(tileSelector, battlefieldTiles[9][9].positions[0], normalScale, WHITE);

        DrawModel(pig, battlefieldTiles[MIDDLE_TILE_INDEX][MIDDLE_TILE_INDEX].positions[3], pigScale, WHITE);
        DrawModelEx(wolf, battlefieldTiles[0][6].positions[0], YAW, 90, wolfScaleVec, WHITE);
        DrawModelEx(wolf, battlefieldTiles[6][0].positions[0], YAW, 0, wolfScaleVec, WHITE);

        for (int i = 0; i < BATTLEFIELD_SIZE; i++)
        {
            for (int j = 0; j < BATTLEFIELD_SIZE; j++)
            {
                DrawModel(battlefieldTiles[i][j].tileType, battlefieldTiles[i][j].positions[0], normalScale, WHITE);
            }
        }

        if (isGridShown)
        {
            DrawGridCentered(40.0f, BATTLEFIELD_SIZE);
        }

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

void DrawGridCentered(float tileSpacing, int tileCount)
{
    int middle = (tileCount - 1) / 2;//odd number
    if (tileCount % 2 == 0) middle = tileCount / 2;
    
    for (int i = 0; i < tileCount + 1; i++)
    {
        Vector3 startPosVertical = { (i - middle) * tileSpacing - tileSpacing/2, 0.0f, (0 - middle) * tileSpacing - tileSpacing/2 };
        Vector3 endPosVertical = { (i - middle) * tileSpacing - tileSpacing / 2, 0.0f, (tileCount - middle) * tileSpacing - tileSpacing / 2 };

        Vector3 startPosHorizontal = { (0 - middle) * tileSpacing - tileSpacing/2, 0.0f, (i - middle) * tileSpacing - tileSpacing/2 };
        Vector3 endPosHorizontal = { (tileCount - middle) * tileSpacing - tileSpacing/2, 0.0f, (i - middle) * tileSpacing - tileSpacing / 2 };

        DrawLine3D(startPosVertical, endPosVertical, GREEN);
        DrawLine3D(startPosHorizontal, endPosHorizontal, GREEN);
    }
}
