#pragma once
#include "Player.h"
#include "Source.h"
#include "GameGlobal.h"
#include "PlayerBulletDeriver.h"
#include "SoundEffect.h"
#include "BombDeriver.h"

//[slowMode][power][index]
Vector2 marisa_weapon_location[2][5][4] =
{
	{
		{},
		{{0, 30}},
		{{-15, 30}, {15, 30}},
		{{-32, 0},{0, 30}, {32, 0}},
		{{-32, 0}, {-15, 30}, {15, 30}, {32, 0}}
	},
	{
		{},
		{{0, 32}},
		{{-8, 36}, {8, 36}},
		{{-16, 32},{0, 36}, {16, 32}},
		{{-24, 24}, {-8, 36}, {8, 36}, {24, 24}}
	}
};

class Marisa :public Player
{
	class Weapon :public GameObject
	{
		vector<LPGOBJ> lasers;

	public:

		Weapon() :type(0), sr(AddComponent(new SpriteRender)), power(0), index(0)
		{
			transform->scale = { 16,16,1 };
			Material mat;
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = { 1,1,1,1 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_player_marisa.Get());
			sr->SetSourceRect({ 5 / 16.0f,10 / 16.0f,1 / 16.0f,1 / 16.0f });
		}

		int GetType()
		{
			return type;
		}

		void SetType(int index)
		{
			type = index;
		}

		void SetPower(int _power)
		{
			power = _power;
		}

		void SetIndex(int _index)
		{
			index = _index;
		}

		void SetTargetLocation(const Vector3& loc)
		{
			target = loc;
		}

		void Shoot()
		{
			if (type == 0)
			{
				se_lazer02.Play();
				auto g = scene->AttachActor(new Marisa_Bullet_2);
				g->transform->location = transform->location;
				lasers.push_back(g);
				auto h = scene->AttachActor(new Marisa_Bullet_2_Shoot);
				h->transform->location = transform->location;
				lasers.push_back(h);
				if (power == 3)
				{
					if (index == 0)
						g->SetAngle((float)M_PI_2 + 0.1);
					if (index == 2)
						g->SetAngle((float)M_PI_2 - 0.1);
				}
				if (power == 4)
				{
					if (index == 0)
						g->SetAngle((float)M_PI_2 + 0.1);
					if (index == 3)
						g->SetAngle((float)M_PI_2 - 0.1);
				}
			}
			else if (type == 1)
			{
				se_msl.Play();
				scene->AttachActor(new Marisa_Bullet_3)->transform->location = transform->location;
			}
		}

		void OnUpdate() override
		{
			transform->scale = { (float)(16 * (1 + 0.1 * sin(timer->GetGameTime() * 12))),(float)(16 * (1 + 0.1 * sin(timer->GetGameTime() * 12))) ,1 };
			transform->location = (transform->location + target) * 0.5;
			for (auto it = lasers.begin(); it != lasers.end();)
			{
				if (!(*it)->IsValid())
					it = lasers.erase(it);
				else
				{
					(*it)->transform->location = transform->location;
					it++;
				}
			}
		}

		void OnCreated() override
		{
			InsertBefore(GameSceneCom::locator[9].Get());
		}

	private:

		int type;

		Vector3 target;

		SpriteRender* sr;

		int power;

		int index;
	};

public:

	Marisa() :Player([]()->AnimationGroup {
		Animation anims[5];
		anims[0].interval = 5;
		anims[1].interval = 5;
		anims[2].interval = 5;
		anims[3].interval = 1;
		anims[4].interval = 1;
		for (int i = 0; i < 8; i++)
			anims[0].rects.push_back({ i / 8.0f, 0, 1 / 8.0f, 48 / 256.0f });
		for (int i = 4; i < 8; i++)
			anims[1].rects.push_back({ i / 8.0f, 48 / 256.0f, 1 / 8.0f, 48 / 256.0f });
		for (int i = 4; i < 8; i++)
			anims[2].rects.push_back({ i / 8.0f, 96 / 256.0f, 1 / 8.0f, 48 / 256.0f });
		for (int i = 3; i >= 0; i--)
			anims[3].rects.push_back({ i / 8.0f, 48 / 256.0f, 1 / 8.0f, 48 / 256.0f });
		for (int i = 3; i >= 0; i--)
			anims[4].rects.push_back({ i / 8.0f, 96 / 256.0f, 1 / 8.0f, 48 / 256.0f });
		AnimationGroup ag(src_player_marisa.Get());
		for (int i = 0; i < 5; i++)
			ag.AddAnimation(anims[i]);
		return ag;
		}(), { 300,120,100,2.5,25,40,{0,0,0} }), shootFrame(0), weaponFrame(0)
	{
		transform->scale = { 32,48,1 };
	}

private:

	ComPtr<Weapon> weapon[4];

	int shootFrame;

	int weaponFrame;

	void OnShoot() override
	{
		if (playerFlags & PlayerFlag::PF_UsingBomb)
			return;
		if (!shootFrame)
		{
			shootFrame = 3;
			se_plst.Play();
			scene->AttachActor(new Marisa_Bullet_1)->transform->location = transform->location + Vector2{ -10,0 };
			scene->AttachActor(new Marisa_Bullet_1)->transform->location = transform->location + Vector2{ 10,0 };
		}
		if (!weaponFrame)
		{
			if (playerFlags & PlayerFlag::PF_SlowMode)
			{
				weaponFrame = 12;
				for (int i = 0; i < 4; i++)
				{
					if (weapon[i])
					{
						weapon[i]->SetType(1);
						weapon[i]->Shoot();
					}
				}
			}
			else
			{
				weaponFrame = 1;
				int p = 0;
				for (int i = 0; i < 4; i++)
				{
					if (weapon[i])
						p++;
				}
				for (int i = 0; i < 4; i++)
				{
					if (weapon[i])
					{
						weapon[i]->SetType(0);
						weapon[i]->SetPower(p);
						weapon[i]->SetIndex(i);
						weapon[i]->Shoot();
					}
				}
			}
		}
	}

	void OnUpdate() override
	{
		if (shootFrame > 0)
			shootFrame--;
		if (weaponFrame > 0)
			weaponFrame--;

		int power = playerData.power / 100;
		if (playerFlags & PF_PeaceLoving)
		{
			for (int i = 0; i < power; i++)
			{
				if (weapon[i] && (weapon[i]->transform->location - transform->location).GetLength() < 10)
				{
					weapon[i]->Destroy();
					weapon[i].Reset();
				}
			}
		}

		if (!(playerFlags & PlayerFlag::PF_Respawning) && !(playerFlags & PlayerFlag::PF_Freezing))
		{
			if (!(playerFlags & PlayerFlag::PF_Dying))
			{
				for (int i = power; i < 4; i++)
				{
					if (weapon[i])
					{
						weapon[i]->Destroy();
						weapon[i].Reset();
					}
				}
				for (int i = 0; i < power; i++)
				{
					if (!weapon[i] && !(playerFlags & PF_PeaceLoving))
					{
						weapon[i] = scene->AttachActor(new Weapon);
						weapon[i]->transform->location = transform->location;
					}
				}
			}
			for (int i = 0; i < 4; i++)
			{
				if (weapon[i])
				{
					if (!(playerFlags & PF_PeaceLoving))
						weapon[i]->SetTargetLocation(transform->location + marisa_weapon_location
							[(playerFlags & PF_SlowMode) ? 1 : 0][power][i]);
					else
						weapon[i]->SetTargetLocation(transform->location);
				}
			}
		}
		__super::OnUpdate();
	}

	void OnDead() override
	{
		for (int i = 0; i < 4; i++)
		{
			if (weapon[i])
			{
				weapon[i]->Destroy();
				weapon[i].Reset();
			}
		}
		__super::OnDead();
	}

	void OnBomb() override
	{
		se_nep00.Play();
		se_slash.Play();
		AddGodModeFrame(330);
		scene->AttachActor(new Marisa_Bomb(this));
	}
};