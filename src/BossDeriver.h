#pragma once
#include "Boss.h"
#include "Source.h"
#include "GameGlobal.h"

class Artia :public Boss
{
public:

	Artia() :Boss([]()->AnimationGroup {
		Animation anims[5];
		anims[0].interval = 7;
		anims[1].interval = 7;
		anims[2].interval = 7;
		anims[3].interval = 1000;
		anims[4].interval = 1000;
		for (int i = 0; i < 4; i++)
			anims[0].rects.push_back({ i / 6.0f, 0, 1 / 6.0f, 1 / 3.0f });
		for (int i = 3; i < 6; i++)
			anims[1].rects.push_back({ (i + 1) / 6.0f, 1 / 3.0f, -1 / 6.0f, 1 / 3.0f });
		for (int i = 3; i < 6; i++)
			anims[2].rects.push_back({ i / 6.0f, 1 / 3.0f, 1 / 6.0f, 1 / 3.0f });
		for (int i = 2; i >= 0 ; i--)
			anims[3].rects.push_back({ (i + 1) / 6.0f, 1 / 3.0f, -1 / 6.0f, 1 / 3.0f });
		for (int i = 2; i >= 0; i--)
			anims[4].rects.push_back({ i / 6.0f, 1 / 3.0f, 1 / 6.0f, 1 / 3.0f });
		AnimationGroup ag(src_artia.Get());
		for (int i = 0; i < 5; i++)
			ag.AddAnimation(anims[i]);
		return ag;
		}(), 1) {
		transform->scale = { 64,74,1 };
		characterFlags |= CF_Floating;
	}

	void OnCreated() override
	{
		__super::OnCreated();
		bossName = L"Artia";
	}
};

class Monstone :public Boss
{
public:

	Monstone() :Boss([]()->AnimationGroup {
		Animation anims[5];
		anims[0].interval = 7;
		anims[1].interval = 7;
		anims[2].interval = 7;
		anims[3].interval = 1000;
		anims[4].interval = 1000;

		anims[0].rects.push_back({ 0,0,1 / 3.0f,1 });
		anims[1].rects.push_back({ 1 / 3.0f,0,1 / 3.0f,1 });
		anims[2].rects.push_back({ 2 / 3.0f,0,1 / 3.0f,1 });
		anims[3].rects.push_back({ 1 / 3.0f,0,1 / 3.0f,1 });
		anims[4].rects.push_back({ 2 / 3.0f,0,1 / 3.0f,1 });

		AnimationGroup ag(src_monstone.Get());
		for (int i = 0; i < 5; i++)
			ag.AddAnimation(anims[i]);
		return ag;
		}(), 0) {
		transform->scale = { 64,64,1 };
		characterFlags |= CF_Floating;
	}

	void OnCreated() override
	{
		__super::OnCreated();
		bossName = L"Monstone";
	}
};

/*
	Anim[5]:Ê©·¨
*/
class SunnyMilk :public Boss
{
public:

	SunnyMilk() :Boss([]()->AnimationGroup {
		Animation anims[6];
		anims[0].interval = 7;
		anims[1].interval = 7;
		anims[2].interval = 7;
		anims[3].interval = 1000;
		anims[4].interval = 1000;
		anims[5].interval = 1000;

		for (int i = 0; i < 4; i++)
			anims[0].rects.push_back({ 96 * i / 512.0f,0,96 / 512.0f,96 / 512.0f });
		for (int i = 1; i < 4; i++)
			anims[1].rects.push_back({ 96 * (i + 1) / 512.0f,96 / 512.0f,-96 / 512.0f,96 / 512.0f });
		for (int i = 1; i < 4; i++)
			anims[2].rects.push_back({ 96 * i / 512.0f,96 / 512.0f,96 / 512.0f,96 / 512.0f });
		anims[3].rects.push_back({ 96 / 512.0f,96 / 512.0f,-96 / 512.0f,96 / 512.0f });
		anims[4].rects.push_back({ 0 / 512.0f,96 / 512.0f,96 / 512.0f,96 / 512.0f });
		anims[5].rects.push_back({ 96 * 2 / 512.0f,192 / 512.0f,96 / 512.0f,96 / 512.0f });

		AnimationGroup ag(src_sunnymilk.Get());
		for (int i = 0; i < 6; i++)
			ag.AddAnimation(anims[i]);
		return ag;
		}(), 2) {
		transform->scale = { 64,64,1 };
		characterFlags |= CF_Floating;
	}

	void OnCreated() override
	{
		__super::OnCreated();
		bossName = L"Sunny Milk";
	}
};