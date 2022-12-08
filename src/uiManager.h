#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "raylib.h"

struct RectangleSettings
{
    Vector2 topLeftCorner;
    float width;
    float height;
    Color color;
    float roundness;
    bool isEnabled;
} typedef RectangleSettings;

void UiInit(int aScreenWidth, int aScreenHeight);
void UiUpdate(bool isPaused);
void UiRender(bool isPaused);
void UiRelease();
void UiShowTileButtons();
void UiHideTileButtons();

bool UiIsTileGrassButtonPressed();
bool UiIsTileLavaButtonPressed();

//private
void SetRectangleSettings(Rectangle *rectangle, RectangleSettings settings);
Vector2 GetWidthAndHeightFromCorners(Vector2 topLeft, Vector2 bottomRight);

#endif