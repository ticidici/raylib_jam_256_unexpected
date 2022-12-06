#include "camera.h"

static const int screenWidth = 256;
static const int screenHeight = 256;

static unsigned int screenScale = 3;
static unsigned int prevScreenScale = 1;

static Camera camera = { 0 };

static int cameraPositionIndex = 0;
static Vector3 camIsometricPositions[4] = {
    {25.0f, 25.0, 25.0f},
    {25.0f, 25.0, -25.0f},
    {-25.0f, 25.0, -25.0f},
    {-25.0f, 25.0, 25.0f} };
static Vector3 camZenitalPos = { 0.0f, 25.0, 0.0f };

static float camIsometricFortressFov = 28.0f;
static float camIsometricBattlefieldFov = 52.0f;
static float camZenitalFortressFov = 28.0f;
static float camZenitalBattlefieldFov = 52.0f;

static bool isCamIsometric = true;
static bool isBattlefieldView = false;

static Vector3 camPerspectivePos = { 10.0f, 25.0, 10.0f };
static float camPerspectiveFov = 60.0f;

unsigned int GetPreviousScreenScale() { return prevScreenScale; }

unsigned int GetCurrentScreenScale() { return screenScale; }

void SetPreviousScreenScale(unsigned int scale) { screenScale = scale; }

void SetCurrentScreenScale(unsigned int scale) { screenScale = scale; }

Camera GetCamera() { return camera; }

bool IsCamIsometric() { return isCamIsometric; }
bool IsBattlefieldView() { return isBattlefieldView; }

void CameraInit()
{
    //-------- CAM SETTINGS ---------
    // camera.position = camPerspectivePos; // Camera position
    camera.position = camIsometricPositions[0]; // Camera position
    // camera.position = camZenitalPos; // Camera position

    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f }; // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };     // Camera up vector (rotation towards target)

    // camera.fovy = camPerspectiveFov;                                // Camera field-of-view Y
    // camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    camera.fovy = camIsometricFortressFov;   // Camera field-of-view Y
    camera.projection = CAMERA_ORTHOGRAPHIC; // Camera mode type
    SetCameraMode(camera, CAMERA_FREE);      // Set a free camera mode

    //-------- /CAM SETTINGS ---------
}

void CameraUpdate()
{
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

        camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };

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
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
}
