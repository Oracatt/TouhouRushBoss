#include "PlayerBullet.h"
#include "GameScene.h"
#include "Enemy.h"

PlayerBullet::PlayerBullet(BYTE _autoRotateMode) :
	Bullet(_autoRotateMode), checkRadius(15), damage(1)
{}

void PlayerBullet::Clean(BYTE cleanType)
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
}

void PlayerBullet::OnCreated()
{
	InsertBefore(GameSceneCom::locator[8].Get());
}

void PlayerBullet::OnCheck()
{
	vector<Enemy*> enemies;
	for (LPACTOR g = GameSceneCom::locator[6].Get(); g != static_cast<LPACTOR>(GameSceneCom::locator[7].Get()); g = g->GetNext())
	{
		if (dynamic_cast<Enemy*>(g.Get()))
			enemies.push_back(static_cast<Enemy*>(g.Get()));
	}
	for (LPACTOR g = GameSceneCom::locator[9].Get(); g != static_cast<LPACTOR>(GameSceneCom::locator[10].Get()); g = g->GetNext())
	{
		if (dynamic_cast<Enemy*>(g.Get()))
			enemies.push_back(static_cast<Enemy*>(g.Get()));
	}
	if (cleanOnHit)
	{
		float min_distance = FLT_MAX;
		Enemy* target = 0;
		for (auto e : enemies)
		{
			if (!(e->GetCharacterFlags() & CF_ImmuneDamage))
			{
				float distance = Vector2(e->transform->location - transform->location).GetLength();
				if (abs(e->transform->location.x-transform->location.x)<e->GetAttribution().checkRadius+checkRadius&&
					abs(e->transform->location.y - transform->location.y) < e->GetAttribution().checkRadius + checkRadius
					)
				{
					if (distance < min_distance)
					{
						min_distance = distance;
						target = e;
					}
				}
			}
		}
		if (target)
		{
			OnHit(target);
		}
	}
	else
	{
		for (auto e : enemies)
		{
			if (!(e->GetCharacterFlags() & CF_ImmuneDamage))
			{
				float distance = Vector2(e->transform->location - transform->location).GetLength();
				if (distance * distance < e->GetAttribution().checkRadius * e->GetAttribution().checkRadius + checkRadius * checkRadius)
				{
					bool damaged = false;
					for (auto de : damagedEnemy)
					{
						if (de == e)
						{
							damaged = true;
							break;
						}
					}
					if (damaged)
						continue;
					damagedEnemy.push_back(e);
					OnHit(e);
				}
			}
		}
	}
}

void PlayerBullet::OnHit(Character* character)
{
	static_cast<Enemy*>(character)->TakeDamage(damage, DMG_BULLET);
	if (cleanOnHit)
		Clean(CLEAN_HIT);
}