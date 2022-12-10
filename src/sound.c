#include "sound.h"


Sound waveSound;

static bool init = false;

void SoundInit() {
	if (init) return;
	init = true;
	InitAudioDevice();
	waveSound = LoadSound("resources/wave.mp3");
}

void SoundRelease() {
	StopSoundMulti();
	UnloadSound(waveSound);
	CloseAudioDevice();
}

void EmitSound(SoundType soundType)
{
	
	//TODO
}
