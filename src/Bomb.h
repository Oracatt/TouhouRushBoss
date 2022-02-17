#pragma once
#include "VirtualLib.h"
#include "GameScene.h"
#include "Player.h"

class Bomb :public GameObject
{
public:

	Bomb(Player* _player) :
		player(_player) {}

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::locator[14].Get());
	}

protected:

	Player* player;
};