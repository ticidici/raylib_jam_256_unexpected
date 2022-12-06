#ifndef GAME_H
#define GAME_H

#include "raylib.h"

// Public

void Init();

void Release();

void Update();

// Private
void TurnCamera(bool right);

void SetView(bool aIsBattlefieldView);

void DrawGridCentered(float spacing, int count);

#endif