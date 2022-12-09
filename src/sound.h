#ifndef SOUND_H
#define SOUND_H

#include "raylib.h"

enum SoundType
{
    SoundWrong
} typedef SoundType;


void PlaySound(SoundType soundType);

#endif