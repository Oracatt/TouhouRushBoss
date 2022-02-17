#pragma once
#include "Bullet.h"
#include "Character.h"

class EnemyBullet :public Bullet
{
public:

	const Color* breakColor;

	int colorIndex;

	EnemyBullet(BYTE _autoRotateMode, bool _highLight);

	virtual void SetColor(int index) = 0;

	void Clean(BYTE cleanType) override;

	float GetCheckRadius();

protected:

	bool grazed;

	float checkRadius;

	float breakSize;

	const bool highLight;

	void OnCreated() override;

	void OnCheck() override;

	void OnHit(Character* character) override;

	virtual void OnGrazed();
};

void CleanAllEnemyBullets(UINT cleanType, UINT durframe = 20, float rangeSpeed = 10000, const Vector2& loc = {});

void CleanAuto(UINT autoCleanType, const Vector2& loc = {});