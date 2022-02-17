#include "Enemy.h"
#include "SpellCard.h"
#include "EngineGlobal.h"
#include "Mathf.h"
#include "GameScene.h"
#include "GameGlobal.h"

using namespace Global;

Enemy::Enemy(const AnimationGroup& _animGroup, float _hp) :
	Character(_animGroup), attribution({ _hp,_hp,20,5 }), moveSpeed(0), maxSpeed(0),
	minSpeed(0), enemyFlags(0), spellCard(0), targetMaxDistance(0) {}

SpellCard* Enemy::GetSpellCard()
{
	return spellCard;
}

void Enemy::SetSpellCard(SpellCard* _spellCard)
{
	spellCard = _spellCard;
}

void Enemy::SetEnemyFlags(DWORD flags)
{
	enemyFlags = flags;
}

DWORD Enemy::GetEnemyFlags() const
{
	return enemyFlags;
}

EnemyAttribution& Enemy::GetAttribution()
{
	return attribution;
}

void Enemy::Move(const Vector2& pos, float maxspeed, float minspeed)
{
	targetPosition = pos;
	maxSpeed = maxspeed;
	minSpeed = minspeed;
	moveSpeed = maxSpeed;
	enemyFlags |= EnemyFlag::EF_Moving;
	targetMaxDistance = (targetPosition - transform->location).GetLength();
}

void Enemy::Move(const Vector2& pos, float minspeed)
{
	Move(pos, fmax((transform->location - pos).GetLength(), minspeed), minspeed);
}

void Enemy::TakeDamage(float dmg, BYTE dmgType)
{
	if (attribution.hp > 0)
	{
		if (dmgType == DMG_BULLET)
		{
			if ((spellCard && spellCard->GetHP() < 50 + 50 * playerData.power / 100) || attribution.hp < 50 + 50 * playerData.power / 100)
				se_damage01.Play();
			se_damage00.Play();
		}
		if (spellCard)
			spellCard->TakeDamage(dmg, dmgType);
		attribution.hp -= dmg;
		if (attribution.hp <= 0)
			OnDeath();
	}
}

void Enemy::Kill()
{
	OnDeath();
}

void Enemy::OnDeath()
{
	Destroy();
}

void Enemy::OnUpdate()
{
	if (player && (!(player->GetCharacterFlags() & CF_ImmuneDamage)) && (enemyFlags & EF_Checking))
	{
		float distance = Vector2Distance(player->transform->location, transform->location);
		if (distance * distance < player->GetAttribution().checkRadius * player->GetAttribution().checkRadius + attribution.tsRadius * attribution.tsRadius)
		{
			player->Miss();
		}
	}
	if (enemyFlags & EnemyFlag::EF_Moving)
	{
		float distance = (targetPosition - transform->location).GetLength();
		if (distance < moveSpeed * timer->GetDeltaTime())
		{
			transform->location = targetPosition;
			enemyFlags &= ~EnemyFlag::EF_Moving;
			animGroup.SetAnimIndex(0);
		}
		else
		{
			transform->location += (targetPosition - transform->location).GetNormalized() * moveSpeed * timer->GetDeltaTime();
			float value = 1 - distance / targetMaxDistance;
			moveSpeed = Lerp(maxSpeed, minSpeed, value);
			float threshold = 1 - 0.025 * animGroup.GetFrameCount(3);
			if (value > threshold)
			{
				if (targetPosition.x - transform->location.x < 0)
				{
					animGroup.SetAnimIndex(3);
					animGroup.SetFrame(Min((int)(animGroup.GetFrameCount(3) * (value - threshold) / (1 - threshold)), animGroup.GetFrameCount(3) - 1));
				}
				else
				{
					animGroup.SetAnimIndex(4);
					animGroup.SetFrame(Min((int)(animGroup.GetFrameCount(4) * (value - threshold) / (1 - threshold)), animGroup.GetFrameCount(4) - 1));
				}
			}
			else
			{
				if (targetPosition.x - transform->location.x < 0)
				{
					animGroup.SetAnimIndex(1);
				}
				else
				{
					animGroup.SetAnimIndex(2);
				}
			}
		}
	}
	__super::OnUpdate();
}

void Enemy::OnCreated()
{
	InsertBefore(GameSceneCom::locator[10].Get());
}