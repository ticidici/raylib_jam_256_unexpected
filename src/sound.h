#ifndef SOUND_H
#define SOUND_H

#include "raylib.h"

enum SoundType
{
    SoundWrong
} typedef SoundType;


extern Sound waveSound;
extern Sound soundImpact;
extern Sound soundEnemyDeath;
extern Sound soundShot;

void SoundInit();

void SoundRelease();

void EmitSound(SoundType soundType);

#endif