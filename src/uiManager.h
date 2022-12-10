#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "raylib.h"

enum WeaponType typedef WeaponType;

struct RectangleSettings
{
    Vector2 topLeftCorner;
    float width;
    float height;
    Color color;
    bool isEnabled;
} typedef RectangleSettings;

void UiInit(int aScreenWidth, int aScreenHeight);
void UiUpdate();
void UiRender();
void UiRelease();
void UiShowRightSideButtons();
void UiHideRightSideButtons();

WeaponType WhichWeaponIsSelected();

bool UiIsWeaponWeakButtonPressed();
bool UiIsWeaponStrongButtonPressed();
bool UiIsWeaponFloor1ButtonPressed();
bool UiIsWeaponFloor2ButtonPressed();
bool UiIsWeaponFloor3ButtonPressed();

bool UiIsCubeStrawButtonPressed();
bool UiIsCubeStickButtonPressed();
bool UiIsCubeBrickButtonPressed();

bool UiIsTileGrassButtonPressed();
bool UiIsTileWheatButtonPressed();
bool UiIsTileWoodButtonPressed();
bool UiIsTileClayButtonPressed();
bool UiIsTileLavaButtonPressed();

bool UiIsSellButtonPressed();
bool UiIsSellFloor1Pressed();
bool UiIsSellFloor2Pressed();
bool UiIsSellFloor3Pressed();

bool UiIsThunderboltPressed();

//private
void SetRectangleSettings(Rectangle *rectangle, RectangleSettings settings);
Vector2 GetWidthAndHeightFromCorners(Vector2 topLeft, Vector2 bottomRight);

#endif