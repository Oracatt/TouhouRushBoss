#pragma once
#include "VirtualLib.h"
#include "SoundInstance.h"

extern SoundInstance se_pldead;
extern SoundInstance se_eat;
extern SoundInstance se_timeout;
extern SoundInstance se_plst;
extern SoundInstance se_damage00;
extern SoundInstance se_damage01;
extern SoundInstance se_nep00;
extern SoundInstance se_bombtan;
extern SoundInstance se_slash;
extern SoundInstance se_powerup;
extern SoundInstance se_ch00;
extern SoundInstance se_ch01;
extern SoundInstance se_ch02;
extern SoundInstance se_ch03;
extern SoundInstance se_enep00;
extern SoundInstance se_enep01;
extern SoundInstance se_enep02;
extern SoundInstance se_tan00;
extern SoundInstance se_tan01;
extern SoundInstance se_graze;
extern SoundInstance se_cat00;
extern SoundInstance se_fault;
extern SoundInstance se_cardget;
extern SoundInstance se_extend;
extern SoundInstance se_bonus;
extern SoundInstance se_bonus2;
extern SoundInstance se_boon00;
extern SoundInstance se_select00;
extern SoundInstance se_ok00;
extern SoundInstance se_cancel00;
extern SoundInstance se_lazer02;
extern SoundInstance se_msl;
extern SoundInstance se_pause;
extern SoundInstance se_lazer00;
extern SoundInstance se_lazer01;
extern SoundInstance se_kira00;

struct SoundUnit
{
	IAudio** addressOf;

	string path;
};

class SoundEffectLoader
{
public:

	vector<SoundUnit> units;
	vector<SoundInstance*> pInstances;

	void AddSoundEffect(SoundInstance& pAudio, const string& path, float volume = 1, double interval = 0);
	void Load(void(*progressReport)(float percent) = 0);
};

void LoadSoundEffect();

void SetSoundEffectVolume(float volume);

float GetSoundEffectVolume();