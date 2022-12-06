#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"

unsigned int GetPreviousScreenScale();
unsigned int GetCurrentScreenScale();
void SetPreviousScreenScale(unsigned int scale);
void SetCurrentScreenScale(unsigned int scale);
bool IsCamIsometric();
bool IsBattlefieldView();
void SetView(bool aIsBattlefieldView);
void TurnCamera(bool right);
Camera GetCamera();

void CameraInit();

void CameraUpdate();

#endif