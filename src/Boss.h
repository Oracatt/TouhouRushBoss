#pragma once
#include "Enemy.h"

class Boss :public Enemy
{
public:

	class MagicSquare;

	class Aura;

	class AuraFire;

	MagicSquare* magicSquare;

	Boss(const AnimationGroup& _animGroup, int colorKey);

	void ShowMagicSquare();

	void ShowMaple(const Color& color, int storetimes, bool blast, bool shakeScreen = false);

protected:

	bool showMagicSquare;

	int frame;

	int warpFrame;

	int colorKey;

	void OnCreated() override;

	void OnUpdate() override;

	void OnDestroy() override;

	void OnDeath() override;
};