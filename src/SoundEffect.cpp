#include "SoundEffect.h"
#include "EngineGlobal.h"

using namespace Global;

SoundEffectLoader loader;

float se_volume = 1;

SoundInstance se_pldead;
SoundInstance se_eat;
SoundInstance se_timeout;
SoundInstance se_plst;
SoundInstance se_damage00;
SoundInstance se_damage01;
SoundInstance se_nep00;
SoundInstance se_bombtan;
SoundInstance se_slash;
SoundInstance se_powerup;
SoundInstance se_ch00;
SoundInstance se_ch01;
SoundInstance se_ch02;
SoundInstance se_ch03;
SoundInstance se_enep00;
SoundInstance se_enep01;
SoundInstance se_enep02;
SoundInstance se_tan00;
SoundInstance se_tan01;
SoundInstance se_graze;
SoundInstance se_cat00;
SoundInstance se_fault;
SoundInstance se_cardget;
SoundInstance se_extend;
SoundInstance se_bonus;
SoundInstance se_bonus2;
SoundInstance se_boon00;
SoundInstance se_select00;
SoundInstance se_ok00;
SoundInstance se_cancel00;
SoundInstance se_lazer02;
SoundInstance se_msl;
SoundInstance se_pause;
SoundInstance se_lazer00;
SoundInstance se_lazer01;
SoundInstance se_kira00;

void SoundEffectLoader::AddSoundEffect(SoundInstance& pAudio, const string& path, float baseVolume, double interval)
{
	units.push_back({ &pAudio.pAudio,path });
	pAudio.SetBaseVolumn(baseVolume);
	pAudio.SetVolume(se_volume);
	pAudio.interval = interval;
	pInstances.push_back(&pAudio);
}

void SoundEffectLoader::Load(void(*progressReport)(float percent))
{
	for (int i = 0; i < units.size(); i++)
	{
		audio->CreateAudio(units[i].path.c_str(), units[i].addressOf);
		if (progressReport)
			progressReport((i + 1) / (float)units.size());
	}
}

void LoadSoundEffect()
{
	loader.AddSoundEffect(se_pldead, "sound\\se_pldead00.wav");
	loader.AddSoundEffect(se_eat, "sound\\se_item00.wav", 0.4, 0.04);
	loader.AddSoundEffect(se_timeout, "sound\\se_timeout.wav");
	loader.AddSoundEffect(se_plst, "sound\\se_plst00.wav", 0.2);
	loader.AddSoundEffect(se_damage00, "sound\\se_damage00.wav", 0.4, 0.06);
	loader.AddSoundEffect(se_damage01, "sound\\se_damage01.wav", 0.7, 0.06);
	loader.AddSoundEffect(se_nep00, "sound\\se_nep00.wav");
	loader.AddSoundEffect(se_bombtan, "sound\\se_tan00.wav", 0.4);
	loader.AddSoundEffect(se_slash, "sound\\se_slash.wav");
	loader.AddSoundEffect(se_powerup, "sound\\se_powerup.wav", 0.5);
	loader.AddSoundEffect(se_ch00, "sound\\se_ch00.wav", 0.8);
	loader.AddSoundEffect(se_ch01, "sound\\se_ch01.wav", 0.8);
	loader.AddSoundEffect(se_ch02, "sound\\se_ch02.wav", 0.8);
	loader.AddSoundEffect(se_ch03, "sound\\se_ch03.wav", 0.8);
	loader.AddSoundEffect(se_enep00, "sound\\se_enep01.wav", 0.36);
	loader.AddSoundEffect(se_enep01, "sound\\se_enep01.wav", 0.36);
	loader.AddSoundEffect(se_enep02, "sound\\se_enep02.wav", 0.5);
	loader.AddSoundEffect(se_tan00, "sound\\se_tan00.wav", 0.1);
	loader.AddSoundEffect(se_tan01, "sound\\se_tan00.wav", 0.1);
	loader.AddSoundEffect(se_graze, "sound\\se_graze.wav", 0.5);
	loader.AddSoundEffect(se_cat00, "sound\\se_cat00.wav", 0.8);
	loader.AddSoundEffect(se_fault, "sound\\se_fault.wav", 0.6);
	loader.AddSoundEffect(se_cardget, "sound\\se_cardget.wav", 0.6);
	loader.AddSoundEffect(se_extend, "sound\\se_extend.wav", 0.5);
	loader.AddSoundEffect(se_bonus, "sound\\se_bonus.wav");
	loader.AddSoundEffect(se_bonus2, "sound\\se_bonus2.wav");
	loader.AddSoundEffect(se_boon00, "sound\\se_boon00.wav", 0.5);
	loader.AddSoundEffect(se_select00, "sound\\se_select00.wav", 0.85);
	loader.AddSoundEffect(se_ok00, "sound\\se_ok00.wav", 0.85);
	loader.AddSoundEffect(se_cancel00, "sound\\se_cancel00.wav", 0.85);
	loader.AddSoundEffect(se_lazer02, "sound\\se_lazer02.wav", 0.02);
	loader.AddSoundEffect(se_msl, "sound\\se_msl.wav", 0.4);
	loader.AddSoundEffect(se_pause, "sound\\se_pause.wav", 0.6);
	loader.AddSoundEffect(se_lazer00, "sound\\se_lazer00.wav", 0.4);
	loader.AddSoundEffect(se_lazer01, "sound\\se_lazer01.wav", 0.4);
	loader.AddSoundEffect(se_kira00, "sound\\se_kira00.wav", 0.4);
	loader.Load();
}

void SetSoundEffectVolume(float _volume)
{
	se_volume = _volume;
	for (auto e : loader.pInstances)
		e->SetVolume(se_volume);
}

float GetSoundEffectVolume()
{
	return se_volume;
}