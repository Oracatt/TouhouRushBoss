#include "BossSpellCard.h"
#include "Boss.h"
#include "UIDeriver.h"
#include "Replay.h"

BossSpellCard::BossSpellCard(float _hp, double time, int _score, int id, bool _isTimeSpell) :
	SpellCard(_hp), enemy(0), boss(0), maxTime(time), remainTime(time), cardID(id),
	score(_score), maxScore(_score), isTimeSpell(_isTimeSpell), timeUp(false), startTime(0),
	isFinalSpell(false), finalSpellDead(false), deadFrame(60) {}

void BossSpellCard::TakeDamage(float dmg, BYTE dmgType)
{
	if (!isDead)
	{
		if (remainTime / maxTime > 0.7)
			dmg *= max(0, min(1, Lerp(0.0f, 1.0f, (maxTime - remainTime) / (maxTime * 0.3f))));

		if (lifebar)
			lifebar->SetPercent(fmax(hp - dmg, 0) / maxhp);

		AddScore(10);

		if (hp > 0)
		{
			hp -= dmg;
			if (hp <= 0)
				OnDying();
		}
	}
}

void BossSpellCard::CreateLifeBar(float minValue, float maxValue, bool startfull, bool showTag)
{
	lifebar = boss->GetScene()->AttachActor(new LifeBar(boss, minValue, maxValue, startfull, showTag, remainTime, isTimeSpell));
}

void BossSpellCard::OnDying()
{
	if (!isDead)
	{
		isDead = true;
		if (!isFinalSpell)
		{
			static_cast<Enemy*>(gameObject.Get())->SetSpellCard(0);
			OnSpellEnd();
			gameObject->RemoveComponent(this);
		}
		else
		{
			finalSpellDead = true;
			OnBossFinalSpellDead();
		}
	}
}

void BossSpellCard::OnAwake()
{
	scene = gameObject->GetScene();
	boss = static_cast<Boss*>(gameObject.Get());
	boss->SetSpellCard(this);
}

void BossSpellCard::OnSpellStart()
{
	spellFailed = false;
	startTime = GetTickCount64();
	OnBossSpellStart();
}

void BossSpellCard::OnSpellEnd()
{
	if (cardID != -1)
	{
		if (!isReplaying)
		{
			SCHistory::SetHistoryTotal(cardID, difficulty, SCHistory::GetHistoryTotal(cardID, difficulty) + 1);
			if (!spellFailed)
				SCHistory::SetHistoryGot(cardID, difficulty, SCHistory::GetHistoryGot(cardID, difficulty) + 1);
		}
		if (spellFailed)
		{
			scene->AttachActor(new BonusFailed(timeUp, maxTime - remainTime, (GetTickCount64() - startTime) / 1000.0f));
		}
		else
		{
			scene->AttachActor(new GetSpellCardBonus(score, maxTime - remainTime, (GetTickCount64() - startTime) / 1000.0f));
			AddScore(score);
		}
		bossLife--;
	}
	OnBossSpellEnd();
}

void BossSpellCard::OnRemoved()
{
	__super::OnRemoved();
	boss.Reset();
	if (lifebar)
	{
		lifebar->Destroy();
		lifebar.Reset();
	}
}

void BossSpellCard::OnUpdate()
{
	if (!finalSpellDead)
		remainTime -= timer->GetDeltaTime();
	else if (lifebar)
		lifebar->TimeStop();
	if (remainTime < 0)
		remainTime = 0;
	if (cardID != -1 && maxTime - remainTime > 5 && !isTimeSpell && !finalSpellDead)
	{
		score -= (int)(maxScore / (60 * (maxTime - 5)) * 2 / 3);
		score -= score % 10;
	}
	if (!isFinalSpell)
	{
		if (remainTime <= 0)
		{
			timeUp = true;
			if (!isTimeSpell)
				spellFailed = true;
			OnDying();
		}
		else
		{
			__super::OnUpdate();
		}
	}
	else
	{
		if (remainTime <= 0 && !finalSpellDead)
		{
			timeUp = true;
			if (!isTimeSpell)
				spellFailed = true;
			OnDying();
		}
		else if (finalSpellDead)
		{
			deadFrame--;
			if (deadFrame <= 0)
			{
				static_cast<Enemy*>(gameObject.Get())->SetSpellCard(0);
				OnSpellEnd();
				boss->Kill();
			}
		}
		else
		{
			__super::OnUpdate();
		}
	}
}

void BossSpellCard::OnBossSpellStart()
{
}

void BossSpellCard::OnBossSpellEnd()
{
}

void BossSpellCard::OnBossFinalSpellDead()
{
}