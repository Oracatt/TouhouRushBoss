#include "SoundInstance.h"
#include "EngineGlobal.h"

SoundInstance::SoundInstance() :volume(1), baseVolumn(1), lastPlay(0), interval(0.0) {}

void SoundInstance::Play(bool loop)
{
	if (Global::timer->GetGameTime() - lastPlay >= interval)
	{
		lastPlay = Global::timer->GetGameTime();
		pAudio->CreateInstance(&pInstance);
		pInstance->SetVolume(volume * baseVolumn);
		pInstance->Play(loop);
	}
}

void SoundInstance::Stop(bool immediate)
{
	if (pInstance)
		pInstance->Stop(immediate);
}

void SoundInstance::Continue()
{
	if (pInstance)
		pInstance->Resume();
	else
		Play(false);
}

float SoundInstance::GetVolume() const
{
	return volume;
}

void SoundInstance::SetVolume(float _volume)
{
	volume = _volume;
}

void SoundInstance::SetBaseVolumn(float _baseVolumn)
{
	baseVolumn = _baseVolumn;
}