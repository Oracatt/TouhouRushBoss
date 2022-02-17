#pragma once
#include "VirtualLib.h"
#include "LifeBar.h"

class SpellCard :public AComponent
{
public:

	SpellCard(float _hp);

	float GetHP();

	virtual void TakeDamage(float dmg, BYTE dmgType);

	virtual void OnThink();

protected:

	int frame;

	float hp;

	float maxhp;

	IScene* scene;

	ComPtr<class Enemy> enemy;

	virtual void OnSpellStart();

	virtual void OnSpellEnd();

	void OnStart() override;

	void OnAwake() override;

	void OnUpdate() override;

	void OnRemoved() override;
};