#ifndef SOUND_H
#define SOUND_H

#include "raylib.h"

enum SoundType
{
    SoundWrong
} typedef SoundType;


extern Sound waveSound;

void SoundInit();

void SoundRelease();

void EmitSound(SoundType soundType);

#endif