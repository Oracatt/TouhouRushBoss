#pragma once
#include "Character.h"

#define DMG_BULLET 0
#define DMG_BOMB 1

/********************
AnimGroup:
0:��ֹ
1:����
2:����
3:����ֹͣ
4:����ֹͣ
********************/

struct EnemyAttribution
{
	float hp; //����ֵ
	float maxhp; //�������ֵ
	float checkRadius; //�ܻ��ж�
	float tsRadius; //�����ж�
};

enum EnemyFlag
{
	EF_Moving = 1,
	EF_Checking = 2
};

class Enemy :public Character
{
public:

	Enemy(const AnimationGroup& _animGroup, float _hp);

	virtual void Move(const Vector2& pos, float maxspeed, float minspeed);

	virtual void Move(const Vector2& pos, float minspeed = 50);

	virtual void TakeDamage(float dmg, BYTE dmgType);

	class SpellCard* GetSpellCard();

	void SetSpellCard(class SpellCard* spellCard);

	void SetEnemyFlags(DWORD flags);

	DWORD GetEnemyFlags() const;

	EnemyAttribution& GetAttribution();

	void Kill();

protected:

	float moveSpeed;

	float maxSpeed;

	float minSpeed;

	Vector2 targetPosition;

	float targetMaxDistance;

	DWORD enemyFlags;

	EnemyAttribution attribution;

	SpellCard* spellCard;

	virtual void OnDeath();

	void OnUpdate() override;

	void OnCreated() override;
};