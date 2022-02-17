#include "Shooter.h"
#include "Rand.h"

Shooter::Shooter(bool _highLight) :EnemyBullet(AR_NONE, _highLight), frame(0)
{
	cleanOnHit = false;
	cleanOnBomb = false;
	cleanOnOutOfRange = true;
}

void Shooter::Clean(BYTE cleanType)
{
	if (!IsValid())
		return;
	if (cleanOnHit && cleanType == CLEAN_HIT)
	{
		Destroy();
	}
	else if (cleanOnOutOfRange && cleanType == CLEAN_OUTOFRANGE)
	{
		Destroy();
	}
	else if (cleanOnBomb && cleanType == CLEAN_BOMB)
	{
		Destroy();
	}
	else if (cleanType == CLEAN_BONUS)
	{
		Destroy();
	}
}

void Shooter::OnCheck()
{
}

void Shooter::OnGrazed()
{
	grazed = true;
}

void Shooter::OnUpdate()
{
	__super::OnUpdate();
	frame++;
	OnShoot();
}

void Shooter::SetColor(int c)
{
}