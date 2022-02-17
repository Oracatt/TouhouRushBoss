#pragma once
#include "Bullet.h"
#include "Enemy.h"

class PlayerBullet :public Bullet
{
public:

	PlayerBullet(BYTE _autoRotateMode);

protected:

	float checkRadius;

	float damage;

	vector<Enemy*> damagedEnemy;

	void Clean(BYTE cleanType) override;

	void OnCreated() override;

	void OnCheck() override;

	void OnHit(Character* character) override;
};