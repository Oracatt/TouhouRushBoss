#pragma once
#include "VirtualLib.h"

class SoundInstance
{
public:

	double interval;

	SoundInstance();

	ComPtr<IAudio> pAudio;

	ComPtr<IAudioInstance> pInstance;

	void Play(bool loop = false);

	void Stop(bool immediate = true);

	void Continue();

	float GetVolume() const;

	void SetVolume(float volume);

	void SetBaseVolumn(float baseVolumn);

private:

	float volume;

	float baseVolumn;

	double lastPlay;
};