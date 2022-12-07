#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "raylib.h"

void UiInit();
void UiUpdate(bool isPaused);
void UiRender(bool isPaused);
void UiRelease();

#endif