#pragma once
#include "Player.h"
#include "Source.h"
#include "GameGlobal.h"
#include "PlayerBulletDeriver.h"
#include "SoundEffect.h"
#include "BombDeriver.h"

//[slowMode][power][index]
Vector2 reimu_weapon_location[2][5][4] =
{
	{
		{},
		{{0, -44}},
		{{-20, -36}, {20, -36}},
		{{-28, -32},{0, -44}, {28, -32}},
		{{-40, -8}, {-20, -36}, {20, -36}, {40, -8}}
	},
	{
		{},
		{{0, 32}},
		{{-8, 36}, {8, 36}},
		{{-16, 32},{0, 36}, {16, 32}},
		{{-24, 24}, {-8, 36}, {8, 36}, {24, 24}}
	}
};

class Reimu :public Player
{
	class Weapon :public GameObject
	{
	public:

		Weapon() :type(0), sr(AddComponent(new SpriteRender))
		{
			transform->scale = { 16,16,1 };
			Material mat;
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = { 1,1,1,1 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_player_reimu.Get());
			sr->SetSourceRect({ 3 / 8.0f,9 / 16.0f,1 / 16.0f,1 / 16.0f });
		}

		int GetType()
		{
			return type;
		}

		void SetType(int index)
		{
			type = index;
		}

		void SetTargetLocation(const Vector3& loc)
		{
			target = loc;
		}

		void Shoot()
		{
			if (type == 0)
				scene->AttachActor(new Reimu_Bullet_2)->transform->location = transform->location;
			else if (type == 1)
			{
				int t = Random(0, 1);
				int s = Random(0, 1);
				scene->AttachActor(new Reimu_Bullet_3(t ^ 1, s))->transform->location = transform->location + Vector2(-4, 0);
				scene->AttachActor(new Reimu_Bullet_3(t ^ 0, s))->transform->location = transform->location + Vector2(4, 0);
			}
		}

		void OnUpdate() override
		{
			transform->SetEulerAngles({ 0,0,(float)timer->GetGameTime() * 6 });
			transform->location = (transform->location + target) * 0.5;
		}

		void OnCreated() override
		{
			InsertBefore(GameSceneCom::locator[9].Get());
		}

	private:

		int type;

		Vector3 target;

		SpriteRender* sr;
	};

public:

	Reimu() :Player([]()->AnimationGroup {
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
		AnimationGroup ag(src_player_reimu.Get());
		for (int i = 0; i < 5; i++)
			ag.AddAnimation(anims[i]);
		return ag;
		}(), { 270,120,120,2,25,40,{0,0,0} }), shootFrame(0), weaponFrame(0)
	{
		transform->scale = { 32,48,1 };
	}

private:

	ComPtr<Weapon> weapon[4];

	int shootFrame;

	int weaponFrame;

	void OnShoot() override
	{
		if (!shootFrame)
		{
			shootFrame = 3;
			se_plst.Play();
			scene->AttachActor(new Reimu_Bullet_1)->transform->location = transform->location + Vector2{ -10,0 };
			scene->AttachActor(new Reimu_Bullet_1)->transform->location = transform->location + Vector2{ 10,0 };
		}
		if (!weaponFrame)
		{
			if (playerFlags & PlayerFlag::PF_SlowMode)
			{
				weaponFrame = 3;
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
				weaponFrame = 5;
				for (int i = 0; i < 4; i++)
				{
					if (weapon[i])
					{
						weapon[i]->SetType(0);
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
						weapon[i]->SetTargetLocation(transform->location + reimu_weapon_location
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
		AddGodModeFrame(300);
		scene->AttachActor(new Reimu_Bomb(this));
	}
};