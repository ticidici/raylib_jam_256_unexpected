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
#include "raymath.h"
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

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

// TODO: Define global variables here, recommended to make them static


//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);      // Update and Draw one frame

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
	UnloadRenderTexture(target);

	// TODO: Unload all loaded resources at this point

	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}

//--------------------------------------------------------------------------------------------
// Module functions definition
//--------------------------------------------------------------------------------------------
// Update and draw frame
void UpdateDrawFrame(void)
{

	static bool init = false;
	static Camera camera = { 0 };

	static Model tower;
	static Texture2D texture;
	static Vector3 towerPos = { 0.0f, 0.0f, 0.0f };                        // Set model position
	static Shader shader;
	static Shader bloomShader;
	static Light lights[MAX_LIGHTS] = { 0 };

	static Model cube;


	if (!init) {
		init = true;
		camera.position = (Vector3){ 2.0f, 4.0f, 6.0f };    // Camera position
		camera.target = (Vector3){ 0.0f, 0.5f, 0.0f };      // Camera looking at point
		camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
		camera.fovy = 45.0f;                                // Camera field-of-view Y
		camera.projection = CAMERA_PERSPECTIVE;
		SetCameraMode(camera, CAMERA_FREE); // Set a free camera mode
		bloomShader = LoadShader(0, "resources/shaders/bloom.fs");

		shader = LoadShader("resources/shaders/lighting.vs", "resources/shaders/lighting.fs");
		// Get some required shader locations
		shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
		// NOTE: "matModel" location name is automatically assigned on shader loading, 
		// no need to get the location again if using that uniform name
		//shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(shader, "matModel");

		// Ambient light level (some basic lighting)
		int ambientLoc = GetShaderLocation(shader, "ambient");
		SetShaderValue(shader, ambientLoc, (float[4]) { 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);

		// Create lights
		lights[0] = CreateLight(LIGHT_DIRECTIONAL, (Vector3) { -0,10, -0 }, Vector3Zero(), WHITE, shader);
		lights[1] = CreateLight(LIGHT_POINT, (Vector3) { 2, 1, 2 }, Vector3Zero(), RED, shader);
		lights[2] = CreateLight(LIGHT_POINT, (Vector3) { -2, 1, 2 }, Vector3Zero(), GREEN, shader);
		lights[3] = CreateLight(LIGHT_POINT, (Vector3) { 2, 1, -2 }, Vector3Zero(), BLUE, shader);


		tower = LoadModel("resources/turret.obj");
		texture = LoadTexture("resources/turret_diffuse.png"); // Load model texture
		tower.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;            // Set model diffuse texture
		tower.materials[0].shader = shader;

		cube = LoadModelFromMesh(GenMeshCube(2.0f, 4.0f, 2.0f));
		cube.materials[0].shader = shader;
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

	UpdateCamera(&camera);


	// Update the shader with the camera view vector (points towards { 0.0f, 0.0f, 0.0f })
	float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
	SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

	// Update light values (actually, only enable/disable them)
	for (int i = 0; i < MAX_LIGHTS; i++) UpdateLightValues(shader, lights[i]);

	// TODO: Update variables / Implement example logic at this point
	//----------------------------------------------------------------------------------

	// Draw
	//----------------------------------------------------------------------------------
	// Render all screen to texture (for scaling)
	BeginTextureMode(target);
	{
		ClearBackground(RAYWHITE);

		BeginMode3D(camera);

		DrawModel(tower, towerPos, 0.2f, WHITE);
		DrawGrid(10, 10.0f);

		for (int i = 0; i < MAX_LIGHTS; i++)
		{
			if (lights[i].enabled) DrawSphereEx(lights[i].position, 0.2f, 8, 8, lights[i].color);
			else DrawSphereWires(lights[i].position, 0.2f, 8, 8, ColorAlpha(lights[i].color, 0.3f));
		}

		EndMode3D();

		DrawCircleLines(GetMouseX(), GetMouseY(), 10, MAROON);

	}
	EndTextureMode();

	BeginDrawing();
	{
		ClearBackground(RAYWHITE);
		BeginShaderMode(bloomShader);
		// Draw render texture to screen scaled as required
		DrawTexturePro(target.texture, (Rectangle) { 0, 0, (float)target.texture.width, -(float)target.texture.height }, (Rectangle) { 0, 0, (float)target.texture.width* screenScale, (float)target.texture.height* screenScale }, (Vector2) { 0, 0 }, 0.0f, WHITE);
		EndShaderMode();
		DrawFPS(10, 10);
	}
	EndDrawing();
	//----------------------------------------------------------------------------------  
}