#include "SpellCard.h"
#include "Enemy.h"

SpellCard::SpellCard(float _hp) :hp(_hp), maxhp(_hp), enemy(0), frame(0), scene(0) {}

float SpellCard::GetHP()
{
	return hp;
}

void SpellCard::TakeDamage(float dmg, BYTE dmgType)
{
	if (hp > 0)
	{
		hp -= dmg;
		if (hp <= 0)
		{
			static_cast<Enemy*>(gameObject.Get())->SetSpellCard(0);
			OnSpellEnd();
			gameObject->RemoveComponent(this);
		}
	}
}

void SpellCard::OnThink() {}

void SpellCard::OnSpellStart() {}

void SpellCard::OnSpellEnd() {}

void SpellCard::OnStart() 
{
	OnSpellStart();
}

void SpellCard::OnAwake()
{
	scene = gameObject->GetScene();
	enemy = static_cast<Enemy*>(gameObject.Get());
	enemy->SetSpellCard(this);
}

void SpellCard::OnUpdate()
{
	frame++;
	OnThink();
}

void SpellCard::OnRemoved()
{
	enemy.Reset();
}