#pragma once
#include "SpellCard.h"
#include "History.h"

class BossSpellCard :public SpellCard
{
public:

	int cardID;

	int score;

	double maxTime;

	double remainTime;

	BossSpellCard(float _hp, double time, int score = 0, int id = -1, bool _isTimeSpell = false);

	void TakeDamage(float dmg, BYTE dmgType) override;

	virtual void CreateLifeBar(float minValue, float maxValue, bool startfull, bool showTag);

	virtual void OnDying();

protected:

	ComPtr<LifeBar> lifebar;

	ComPtr<class Boss> boss;

	bool isDead;

	int deadFrame;

	bool isTimeSpell;

	bool isFinalSpell;

private:

	class Enemy* enemy;

	int maxScore;

	bool timeUp;

	bool finalSpellDead;

	ULONGLONG startTime;

	void OnAwake() final;

	void OnSpellStart() final;

	void OnSpellEnd() final;

	void OnRemoved() override;

	void OnUpdate() override;

	virtual void OnBossSpellStart();

	virtual void OnBossSpellEnd();

	virtual void OnBossFinalSpellDead();
};