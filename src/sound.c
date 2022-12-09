#include "sound.h"


Sound waveSound;

void SoundInit() {
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
