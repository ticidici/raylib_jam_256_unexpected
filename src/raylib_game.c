/*******************************************************************************************
*
*   raylib 9years gamejam template
*
*   Template originally created with raylib 4.5-dev, last time updated with raylib 4.5-dev
*
*   Template licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2022 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#if defined(PLATFORM_WEB)
#define CUSTOM_MODAL_DIALOGS            // Force custom modal dialogs usage
#include <emscripten/emscripten.h>      // Emscripten library - LLVM to JavaScript compiler
#endif

#include <stdio.h>                          // Required for: printf()
#include <stdlib.h>                         // Required for: 
#include <string.h>                         // Required for: 

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum {
	SCREEN_LOGO = 0,
	SCREEN_TITLE,
	SCREEN_GAMEPLAY,
	SCREEN_ENDING
} GameScreen;

// TODO: Define your custom data types here

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static const int screenWidth = 256;
static const int screenHeight = 256;

static unsigned int screenScale = 3;
static unsigned int prevScreenScale = 1;

static RenderTexture2D target = { 0 };  // Initialized at init

#define FORTRESS_SIZE 7			//have to be odd numbers
#define BATTLEFIELD_SIZE 13		//have to be odd numbers
#define BATTLEFIELD_RING_WIDTH (BATTLEFIELD_SIZE-FORTRESS_SIZE)/2
#define MIDDLE_TILE_INDEX (BATTLEFIELD_SIZE-1)/2
#define FORTRESS_FIRST_TILE_INDEX BATTLEFIELD_RING_WIDTH
#define FORTRESS_LAST_TILE_INDEX BATTLEFIELD_SIZE-BATTLEFIELD_RING_WIDTH-1
#define BATTLEFIELD_LAST_TILE_INDEX BATTLEFIELD_SIZE-1

// TODO: Define global variables here, recommended to make them static

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);      // Update and Draw one frame
void Deinitialize(void);
void TurnCamera(bool right);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
#if !defined(_DEBUG)
	SetTraceLogLevel(LOG_NONE);         // Disable raylib trace log messsages
#endif

	// Initialization
	//--------------------------------------------------------------------------------------
	InitWindow(screenWidth, screenHeight, "raylib 9yr gamejam");

	// TODO: Load resources / Initialize variables at this point

	// Render texture to draw full screen, enables screen scaling
	// NOTE: If screen is scaled, mouse input should be scaled proportionally
	target = LoadRenderTexture(screenWidth, screenHeight);
	SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

#if defined(PLATFORM_WEB)
	emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
	SetTargetFPS(60);     // Set our game frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button
	{
		UpdateDrawFrame();
	}
#endif

	// De-Initialization
	//--------------------------------------------------------------------------------------
	Deinitialize();
	// TODO: Unload all loaded resources at this point

	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}


static Model tower;

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
static Texture2D towerTexture;

static Camera camera = { 0 };

static int cameraPositionIndex = 0;
static Vector3 camIsometricPositions[4] = {
	{ 150.0f, 150.0, 150.0f },
	{ 150.0f, 150.0, -150.0f },
	{ -150.0f, 150.0, -150.0f },
	{ -150.0f, 150.0, 150.0f }
};



//--------------------------------------------------------------------------------------------
// Module functions definition
//--------------------------------------------------------------------------------------------
// Update and draw frame
void UpdateDrawFrame(void)
{

	static bool init = false;

	static Vector3 YAW = { 0, 1.0f, 0 };

	static struct Tile
	{
		Vector3 positions[4];
		Model tileType;

		//entity occupying?
	};

	//still unused
	static struct Wolf
	{
		Vector3 position;
		float rotation;//degrees of yaw
		Vector3 scale;
		float speed;

		struct Tile* tileOccupied;
		Model model;
	};

	static struct Tile battlefieldTiles[BATTLEFIELD_SIZE][BATTLEFIELD_SIZE];

	static Vector3 towerPos = { 0.0f, 0.0f, 0.0f };                          

	static Vector3 camZenitalPos = { 0.0f, 250.0, 0.0f };
	static Vector3 camIsometricPos = { 150.0f, 150.0, 150.0f };
	static Vector3 camPerspectivePos = { 100.0f, 250.0, 100.0f };
	static float camPerspectiveFov = 60.0f;
	static float camOrthographicFov = 275.0f;

	static float pigScale = 0.25f;
	static float wolfScale = 0.25f;
	static Vector3 wolfScaleVec = { 0.25f, 0.25f , 0.25f };
	static float normalScale = 10.0f;

	if (!init) {
		init = true;

		//camera.position = camPerspectivePos; // Camera position
		camera.position = camIsometricPositions[0]; // Camera position
		camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
		camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
		//camera.fovy = camPerspectiveFov;                                // Camera field-of-view Y
		//camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type
		camera.fovy = camOrthographicFov;                                // Camera field-of-view Y
		camera.projection = CAMERA_ORTHOGRAPHIC;             // Camera mode type
		
		red1 = LoadModel("resources/red1.gltf");
		red2 = LoadModel("resources/red2.gltf");
		red3 = LoadModel("resources/red3.gltf");
		red4 = LoadModel("resources/red4.gltf");
		pig = LoadModel("resources/pig.glb");
		wolf = LoadModel("resources/wolf.glb");

		grassTile = LoadModel("resources/grass_tile.gltf");
		grass2Tile = LoadModel("resources/grass2_tile.gltf");
		lavaTile = LoadModel("resources/lava_tile.gltf");
		dirtGrayTile = LoadModel("resources/dirtgray_tile.gltf");
		dirtBrownTile = LoadModel("resources/dirtbrown_tile.gltf");

		backgroundTexture = LoadTexture("resources/background.png");

		tower = LoadModel("resources/turret.obj");
		towerTexture = LoadTexture("resources/turret_diffuse.png"); // Load model texture
		tower.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = towerTexture;            // Set model diffuse texture

		BoundingBox towerBBox = GetMeshBoundingBox(tower.meshes[0]);    // Get m
		SetCameraMode(camera, CAMERA_FREE); // Set a free camera mode


		for (int i = 0; i < BATTLEFIELD_SIZE; i++)
		{
			for (int j = 0; j < BATTLEFIELD_SIZE; j++)
			{
				for (int k = 0; k < 4; k++)//max height, 3 floors + something standing on top
				{
					battlefieldTiles[i][j].positions[k].x = (i - MIDDLE_TILE_INDEX) * 40;
					battlefieldTiles[i][j].positions[k].z = (j - MIDDLE_TILE_INDEX) * 40;
					battlefieldTiles[i][j].positions[k].y = k * 20;

					if ((i >= FORTRESS_FIRST_TILE_INDEX && i <= FORTRESS_LAST_TILE_INDEX)
						&& (j >= FORTRESS_FIRST_TILE_INDEX && j <= FORTRESS_LAST_TILE_INDEX))
					{
						int randomValue = GetRandomValue(0, 100);
						if (randomValue < 35)
						{
							battlefieldTiles[i][j].tileType = lavaTile;
						}
						//else if (randomValue < 55)
						//{
						//	battlefieldTiles[i][j].tileType = grassTile;
						//}
						else
						{
							battlefieldTiles[i][j].tileType = grass2Tile;
						}

					}
					else
					{
						if (GetRandomValue(0, 100) < 35)
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


	// Update
	//----------------------------------------------------------------------------------
	// Screen scale logic (x2)
	if (IsKeyPressed(KEY_ONE)) screenScale = 1;
	else if (IsKeyPressed(KEY_TWO)) screenScale = 2;
	else if (IsKeyPressed(KEY_THREE)) screenScale = 3;

	if (screenScale != prevScreenScale)
	{
		// Scale window to fit the scaled render texture
		SetWindowSize(screenWidth * screenScale, screenHeight * screenScale);

		// Scale mouse proportionally to keep input logic inside the 256x256 screen limits
		SetMouseScale(1.0f / (float)screenScale, 1.0f / (float)screenScale);

		prevScreenScale = screenScale;
	}

	if (IsKeyPressed(KEY_Q))
	{
		TurnCamera(false);
	}
	else if (IsKeyPressed(KEY_E))
	{
		TurnCamera(true);
	}

	//unless we use CUSTOM_CAMERA UpdateCamera overrides our rotation in favor of mouse input
	UpdateCamera(&camera);


	// TODO: Update variables / Implement example logic at this point
	//----------------------------------------------------------------------------------

	// Draw
	//----------------------------------------------------------------------------------
	// Render all screen to texture (for scaling)

	BeginTextureMode(target);
	{

		BeginMode3D(camera);
		ClearBackground(BLACK);
		//DrawGrid(40.0f, 40.0f);
		//DrawTexture(towerTexture, 0, 0, WHITE);

		//corners
		DrawModel(red1, battlefieldTiles[FORTRESS_FIRST_TILE_INDEX][FORTRESS_FIRST_TILE_INDEX].positions[0], normalScale, WHITE);
		DrawModel(red2, battlefieldTiles[FORTRESS_LAST_TILE_INDEX][FORTRESS_LAST_TILE_INDEX].positions[0], normalScale, WHITE);
		DrawModel(red3, battlefieldTiles[FORTRESS_LAST_TILE_INDEX][FORTRESS_FIRST_TILE_INDEX].positions[0], normalScale, WHITE);
		DrawModel(red4, battlefieldTiles[FORTRESS_FIRST_TILE_INDEX][FORTRESS_LAST_TILE_INDEX].positions[0], normalScale, WHITE);

		//center
		DrawModel(red1, battlefieldTiles[MIDDLE_TILE_INDEX][MIDDLE_TILE_INDEX].positions[0], normalScale, WHITE);
		DrawModel(red1, battlefieldTiles[MIDDLE_TILE_INDEX][MIDDLE_TILE_INDEX].positions[1], normalScale, WHITE);
		DrawModel(red4, battlefieldTiles[MIDDLE_TILE_INDEX][MIDDLE_TILE_INDEX].positions[2], normalScale, WHITE);

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

		//DrawModel(tower, towerPos, 10.0f, WHITE);

		EndMode3D();

		DrawCircleLines(GetMouseX(), GetMouseY(), 5.0f, MAROON);

	}
	EndTextureMode();

	BeginDrawing();
	{
		ClearBackground(RAYWHITE);

		// Draw render texture to screen scaled as required
		DrawTexturePro(target.texture, (Rectangle) { 0, 0, (float)target.texture.width, -(float)target.texture.height }, (Rectangle) { 0, 0, (float)target.texture.width* screenScale, (float)target.texture.height* screenScale }, (Vector2) { 0, 0 }, 0.0f, WHITE);
	}
	EndDrawing();
	//----------------------------------------------------------------------------------  
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

	if (cameraPositionIndex < 0) cameraPositionIndex = 3;
	else if (cameraPositionIndex > 3) cameraPositionIndex = 0;

	camera.position = camIsometricPositions[cameraPositionIndex];
	camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
}

void Deinitialize(void)
{
	UnloadRenderTexture(target);

	UnloadTexture(towerTexture);
	UnloadTexture(backgroundTexture);

	UnloadModel(tower);

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
}