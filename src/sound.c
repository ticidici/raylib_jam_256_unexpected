#include "sound.h"


Sound waveSound;
Sound soundImpact;
Sound soundEnemyDeath;
Sound soundShot;
Sound soundLose;
Sound soundBreak;
Sound soundBuild;
Sound soundThunderbolt;
Sound soundForbidden;

static bool init = false;

void SoundInit() {
	if (init) return;
	init = true;
	InitAudioDevice();
	waveSound = LoadSound("resources/wave.mp3");
	soundImpact = LoadSound("resources/impact.mp3");
	soundEnemyDeath = LoadSound("resources/enemy_death.mp3");
	soundShot = LoadSound("resources/shot.mp3");
	soundLose = LoadSound("resources/lose.mp3");
	soundBreak = LoadSound("resources/break.mp3");
	soundBuild = LoadSound("resources/build.mp3");
	soundThunderbolt = LoadSound("resources/thunderbolt.mp3");
	soundForbidden = LoadSound("resources/forbidden.mp3");
}

void SoundRelease() {
	StopSoundMulti();
	UnloadSound(waveSound);
	UnloadSound(soundImpact);
	UnloadSound(soundEnemyDeath);
	UnloadSound(soundShot);
	UnloadSound(soundLose);
	UnloadSound(soundBreak);
	UnloadSound(soundBuild);
	UnloadSound(soundThunderbolt);
	UnloadSound(soundForbidden);
	CloseAudioDevice();
}

void EmitSound(SoundType soundType)
{
	
	//TODO
}
