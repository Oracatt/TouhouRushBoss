#pragma once
#include "Bomb.h"
#include "Source.h"
#include "UserComponent.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "Effect.h"
#include "SceneObjects.h"
#include "Item.h"
#include "UserRenderer.h"

class Reimu_Bomb :public Bomb
{
	class BackCanvas :public GameObject
	{
	public:

		BackCanvas() :frame(0), sr(AddComponent(new SpriteRender))
		{
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = { 0.25,0.5,0.5,0.8 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_charge.Get());
			AddComponent(new BlendStateSet::Setter(BLEND_STATE_DARKEN))->SetPriority(101, false);
			AddComponent(new BlendStateSet::Setter(BLEND_STATE_NORMAL))->SetPriority(99, false);
		}

	private:

		int frame;

		SpriteRender* sr;

		Material mat;

		void OnUpdate()
		{
			frame++;
			if (frame < 120)
				transform->scale += {50, 50, 0};
			if (frame >= 240)
			{
				mat.diffuse.a -= 0.04;
				sr->SetMaterial(mat);
			}
			if (frame == 260)
				Destroy();
		}

		void OnCreated() override
		{
			InsertBefore(GameSceneCom::locator[5].Get());
		}
	};

	class Sub_Reimu_Bomb :public Bomb
	{
		class Color_Bomb :public Bomb
		{
		public:

			MoveBody* mb;

			Color_Bomb(Bomb* _bomb, Player* _player, int _colorIndex) :
				Bomb(_player), frame(0), father(_bomb), sr(AddComponent(new SpriteRender)),
				mb(AddComponent(new MoveBody)), colorIndex(_colorIndex)
			{
				transform->Rotate({ 0,0,RandomAngle() }, true);
				mat.ambient = { 0,0,0,0 };
				mat.diffuse = { 0,0,0,1 };
				*(((float*)&mat.diffuse) + _colorIndex) = 1;
				mat.power = 0;
				mat.specular = { 0,0,0,0 };
				sr->SetMaterial(mat);
				sr->SetTexture(src_reimu_bomb.Get());
				mb->drag = { 1,1,1 };
			}

		private:

			ComPtr<Bomb> father;

			int frame;

			SpriteRender* sr;

			Material mat;

			int colorIndex;

			void OnUpdate() override
			{
				frame++;
				transform->Rotate({ 0,0,frame / 30.0f }, true);
				if (frame < 30)
					transform->scale += Vector3(5, 5, 0);
				transform->location = (transform->location * 4 + father->transform->location) * 0.2;
				if (!father->IsValid())
					OnDead();
			}

			void OnDestroy() override
			{
				father.Reset();
			}

			void OnDead()
			{
				scene->AttachActor(new Reimu_Color_Bomb_Break(colorIndex))->transform->location = transform->location;
				Destroy();
			}
		};

	public:

		Sub_Reimu_Bomb(Player* _player, int _revindex, int _coldFrame) :
			Bomb(_player), frame(0), index(_revindex), revolution((float)M_PI * 2 * index / 8.0f), dmgPoint(0),
			coldFrame(_coldFrame), mb(AddComponent(new MoveBody)), sr(AddComponent(new SpriteRender))
		{
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = { 1,1,1,0.75 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_reimu_bomb.Get());
		}

	private:

		int frame;

		int index;

		float revolution;

		int coldFrame;

		int dmgPoint;

		Vector3 targetLoc;

		MoveBody* mb;

		SpriteRender* sr;

		Material mat;

		void OnCreated() override
		{
			__super::OnCreated();
			for (int i = 0; i < 3; i++)
			{
				auto g = scene->AttachActor(new Color_Bomb(this, player, i));
				g->transform->location = player->transform->location;
				g->mb->velocity = GetVector(RandomAngle(), 200);
			}
		}

		void OnUpdate() override
		{
			frame++;
			if (frame < 30)
				transform->scale += Vector3(5, 5, 0);
			transform->Rotate({ 0,0,frame / 30.0f }, true);
			if (frame < 60)
			{
				revolution -= 0.05 + frame / 1200.0f;
				targetLoc = player->transform->location + GetVector(revolution, frame * 2);
				transform->location = (transform->location + targetLoc) * 0.5;
			}
			else if (frame < 80 + coldFrame)
			{
				revolution -= 0.1;
				targetLoc = player->transform->location + GetVector(revolution, 130);
				transform->location = (transform->location * 3 + targetLoc) * 0.25;
			}
			else if (frame == 80 + coldFrame)
			{
				mb->velocity = GetVector(revolution - (float)M_PI_2, 1500);
				mb->drag = { 1,1,1 };
			}
			else
			{
				vector<Enemy*> enemies;
				for (LPACTOR g = GameSceneCom::locator[6].Get(); g != static_cast<LPACTOR>(GameSceneCom::locator[7].Get()); g = g->GetNext())
				{
					if (dynamic_cast<Enemy*>(g.Get()))
						enemies.push_back(static_cast<Enemy*>(g.Get()));
				}
				for (LPACTOR g = GameSceneCom::locator[9].Get(); g != static_cast<LPACTOR>(GameSceneCom::locator[10].Get()); g = g->GetNext())
				{
					if (dynamic_cast<Enemy*>(g.Get()))
						enemies.push_back(static_cast<Enemy*>(g.Get()));
				}
				float min_distance = FLT_MAX;
				Enemy* target = 0;
				for (auto e : enemies)
				{
					if (!(e->GetCharacterFlags() & CF_ImmuneDamage))
					{
						float distance = Vector2(e->transform->location - transform->location).GetLength();
						if (distance < min_distance)
						{
							min_distance = distance;
							target = e;
						}
					}
				}
				if (target)
				{
					mb->force = (target->transform->location - transform->location).GetNormalized() * 1500;
				}
			}
			OnCheck();
			if (frame > 240 || (dmgPoint >= (60 + coldFrame) && frame > 80))
			{
				OnDead();
			}
		}

		void OnCheck()
		{
			vector<Enemy*> enemies;
			for (LPACTOR g = GameSceneCom::locator[6].Get(); g != static_cast<LPACTOR>(GameSceneCom::locator[7].Get()); g = g->GetNext())
			{
				if (dynamic_cast<Enemy*>(g.Get()))
					enemies.push_back(static_cast<Enemy*>(g.Get()));
			}
			for (LPACTOR g = GameSceneCom::locator[9].Get(); g != static_cast<LPACTOR>(GameSceneCom::locator[10].Get()); g = g->GetNext())
			{
				if (dynamic_cast<Enemy*>(g.Get()))
					enemies.push_back(static_cast<Enemy*>(g.Get()));
			}
			for (auto e : enemies)
			{
				if (!(e->GetCharacterFlags() & CF_ImmuneDamage))
				{
					float distance = Vector2(e->transform->location - transform->location).GetLength();
					if (distance < min(10 + frame, 70))
					{
						e->TakeDamage(1, DMG_BOMB);
						dmgPoint++;
					}
				}
			}
			vector<EnemyBullet*> bullets;
			for (LPACTOR g = GameSceneCom::locator[11].Get(); g != static_cast<LPACTOR>(GameSceneCom::locator[12].Get()); g = g->GetNext())
			{
				if (dynamic_cast<EnemyBullet*>(g.Get()))
					bullets.push_back(static_cast<EnemyBullet*>(g.Get()));
			}
			for (LPACTOR g = GameSceneCom::locator[15].Get(); g != static_cast<LPACTOR>(GameSceneCom::locator[16].Get()); g = g->GetNext())
			{
				if (dynamic_cast<EnemyBullet*>(g.Get()))
					bullets.push_back(static_cast<EnemyBullet*>(g.Get()));
			}
			for (auto e : bullets)
			{
				float distance = Vector2(e->transform->location - transform->location).GetLength();
				if (distance < min(10 + frame, 70))
				{
					e->Clean(CLEAN_BOMB);
				}
			}
		}

		void OnDead()
		{
			se_bombtan.Play();
			scene->AttachActor(new Reimu_Bomb_Break)->transform->location = transform->location;
			GameSceneCom::gamecam->Shake(6, 60);
			Destroy();
		}
	};

public:

	Reimu_Bomb(Player* _player) :Bomb(_player), frame(0) {}

	void OnCreated() override
	{
		__super::OnCreated();
		vector<int> coldFrame;
		for (int i = 0; i < 8; i++)
			coldFrame.push_back(i * 8);
		RandomSort(coldFrame);
		for (int i = 0; i < 8; i++)
			scene->AttachActor(new Sub_Reimu_Bomb(player, i, coldFrame[i]))->transform->location = player->transform->location;
		scene->AttachActor(new BackCanvas)->transform->location = player->transform->location;
		CollectAllItems();
	}

	void OnUpdate() override
	{
		frame++;
		if (frame == 240)
		{
			player->SetPlayerFlags(player->GetPlayerFlags() & ~PF_UsingBomb);
			Destroy();
		}
	}

private:

	int frame;
};

class Marisa_Bomb :public Bomb
{
	class BackCanvas :public GameObject
	{
	public:

		BackCanvas(Player* _player) :frame(0), sr(AddComponent(new CircleDrawer(0, 1, 1))), player(_player)
		{
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = { 0.2,0.2,0.1,1.0 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_dummy.Get());
			AddComponent(new BlendStateSet::Setter(BLEND_STATE_DARKEN))->SetPriority(101, false);
			AddComponent(new BlendStateSet::Setter(BLEND_STATE_NORMAL))->SetPriority(99, false);
			SortComponents();
		}

	private:

		int frame;

		CircleDrawer* sr;

		Material mat;

		ComPtr<Player> player;

		void OnUpdate()
		{
			transform->location = player->transform->location;
			frame++;
			if (frame < 120)
			{
				mat.diffuse.r += 0.1f / 120.0f;
				mat.diffuse.g += 0.1f / 120.0f;
				sr->SetMaterial(mat);
				transform->scale = { (float)(frame * frame),(float)(frame * frame),1 };
			}
			if (frame >= 160)
			{
				transform->scale = { (float)((280 - frame) * (280 - frame)) ,(float)((280 - frame) * (280 - frame)) ,1 };
				mat.diffuse.r -= 0.2f / 120.0f;
				mat.diffuse.b += 0.2f / 120.0f;
				mat.diffuse.a -= 0.003;
				sr->SetMaterial(mat);
			}
			if (frame == 280)
				Destroy();
		}

		void OnCreated() override
		{
			InsertBefore(GameSceneCom::locator[5].Get());
		}

		void OnDestroy()
		{
			__super::OnDestroy();
			player.Reset();
		}
	};

	class Spark :public Bomb
	{
	public:

		Spark(Player* _player, const Color& color, float _baseAngle) :
			Bomb(_player), sr(AddComponent(new SpriteRender)), frame(0), angle(0), baseAngle(_baseAngle)
		{
			SetAngle(baseAngle);
			transform->scale = { 768,0,1 };
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = color;
			mat.specular = { 0,0,0,0 };
			mat.power = 0;
			sr->SetMaterial(mat);
			sr->SetTexture(src_marisa_bomb.Get());
			sr->SetSourceRect({ 0.001,0,0.999,1 });
			sr->SetLocalTransform(XMMatrixTranslation(0.5, 0, 0));
		}

		void SetAngle(float _angle)
		{
			angle = _angle;
			transform->SetEulerAngles({ 0,0,angle });
		}

		float GetAngle()
		{
			return angle;
		}

	private:

		float baseAngle;

		float angle;

		int frame;

		SpriteRender* sr;

		Material mat;

		void OnUpdate() override
		{
			if (player->GetAttribution().direction.x < 0)
				SetAngle(angle + 0.003f);
			if (player->GetAttribution().direction.x > 0)
				SetAngle(angle - 0.003f);
			SetAngle(Lerp<float>(angle, baseAngle, 0.04));

			frame++;
			transform->location = player->transform->location;
			if (frame < 250 && frame > 5 && frame % 10 == 0 && abs(baseAngle - M_PI_2) < 0.001)
			{
				scene->AttachActor(new Wave(this, player, { 0.8,0.8,0.3,1 }, angle))->transform->location = player->transform->location;
			}
			if (frame <= 15)
			{
				transform->scale.y += 20;
			}
			if (frame >= 255)
			{
				transform->scale.y -= 20;
			}
			if (frame <= 250)
			{
				GameSceneCom::gamecam->Shake(20, 7);
			}

			OnCheck();

			if (frame == 270)
				Destroy();
		}

		void OnCheck()
		{
			float x = transform->location.x;
			float y = transform->location.y;
			float k = tan(angle);
			if (abs(k) > 999999)
				k = 999999;
			float b = y - k * x;
			vector<Enemy*> enemies;
			for (LPACTOR g = GameSceneCom::locator[6].Get(); g != static_cast<LPACTOR>(GameSceneCom::locator[7].Get()); g = g->GetNext())
			{
				if (dynamic_cast<Enemy*>(g.Get()))
					enemies.push_back(static_cast<Enemy*>(g.Get()));
			}
			for (LPACTOR g = GameSceneCom::locator[9].Get(); g != static_cast<LPACTOR>(GameSceneCom::locator[10].Get()); g = g->GetNext())
			{
				if (dynamic_cast<Enemy*>(g.Get()))
					enemies.push_back(static_cast<Enemy*>(g.Get()));
			}
			for (auto e : enemies)
			{
				if (!(e->GetCharacterFlags() & CF_ImmuneDamage))
				{
					float ex = e->transform->location.x;
					float ey = e->transform->location.y;
					float kk = -1 / k;
					float bb = ey - kk * ex;
					float tx = (bb - b) / (k - kk);
					float ty = k * tx + b;
					const float a = 85;
					if (abs(k * ex - ey + b) / sqrt(k * k + 1) - e->GetAttribution().checkRadius <
						(transform->scale.y / 300) * fmin(140, sqrt((Vector2(tx, ty) - transform->location).GetLength() * a)))
					{
						float eangle = atan2(ey - y, ex - x);
						if (AngleSubAngle(angle, eangle) < M_PI / 2)
							e->TakeDamage(1, DMG_BOMB);
					}
				}
			}
			vector<EnemyBullet*> bullets;
			for (LPACTOR g = GameSceneCom::locator[11].Get(); g != static_cast<LPACTOR>(GameSceneCom::locator[12].Get()); g = g->GetNext())
			{
				if (dynamic_cast<EnemyBullet*>(g.Get()))
					bullets.push_back(static_cast<EnemyBullet*>(g.Get()));
			}
			for (LPACTOR g = GameSceneCom::locator[15].Get(); g != static_cast<LPACTOR>(GameSceneCom::locator[16].Get()); g = g->GetNext())
			{
				if (dynamic_cast<EnemyBullet*>(g.Get()))
					bullets.push_back(static_cast<EnemyBullet*>(g.Get()));
			}
			for (auto e : bullets)
			{
				float ex = e->transform->location.x;
				float ey = e->transform->location.y;
				float kk = -1 / k;
				float bb = ey - kk * ex;
				float tx = (bb - b) / (k - kk);
				float ty = k * tx + b;
				const float a = 85;
				if (abs(k * ex - ey + b) / sqrt(k * k + 1) - e->GetCheckRadius() <
					(transform->scale.y / 300) * fmin(140, sqrt((Vector2(tx, ty) - transform->location).GetLength() * a)))
				{
					float eangle = atan2(ey - y, ex - x);
					if (AngleSubAngle(angle, eangle) < M_PI / 2)
						e->Clean(CLEAN_BOMB);
				}
			}
		}
	};

	class Wave :public Bomb
	{
	public:

		Wave(Spark* _spark, Player* _player, const Color& color, float _baseAngle) :
			Bomb(_player), sr(AddComponent(new SpriteRender)), frame(0), baseAngle(_baseAngle), spark(_spark), length(0)
		{
			SetAngle(baseAngle);
			transform->scale = { 96 * 330 / 256.0f,0,1 };
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = color;
			mat.specular = { 0,0,0,0 };
			mat.power = 0;
			sr->SetMaterial(mat);
			sr->SetTexture(src_marisa_bomb_2.Get());
			sr->SetSourceRect({ 0.001,0,0.999,1 });
			sr->SetLocalTransform(XMMatrixTranslation(0.5, 0, 0));
		}

		void SetAngle(float _angle)
		{
			transform->SetEulerAngles({ 0,0,_angle });
		}

	private:

		float baseAngle;

		int frame;

		SpriteRender* sr;

		Material mat;

		ComPtr<Spark> spark;

		float length;

		void OnUpdate() override
		{
			frame++;
			length += 20;
			transform->location = player->transform->location + GetVector(spark->GetAngle(), length);
			if (frame <= 3)
			{
				transform->scale.y += 110;
			}
			if (frame == 120)
				Destroy();
		}

		void OnDestroy() override
		{
			__super::OnDestroy();
			spark.Reset();
		}
	};

public:

	Marisa_Bomb(Player* _player) :Bomb(_player), frame(0) {}

	void OnCreated() override
	{
		__super::OnCreated();
		player->GetAttribution().moveSpeed /= 4;
		player->GetAttribution().slowMoveSpeed /= 4;
		auto g = scene->AttachActor(new Spark(player, { 0.75,0.75,0.75,1 }, M_PI_2));
		g->transform->location = player->transform->location;
		g = scene->AttachActor(new Spark(player, { 0.75,0,0.75,1 }, M_PI_2 - 0.02f));
		g->transform->location = player->transform->location;
		g = scene->AttachActor(new Spark(player, { 0.75,0.75,0,1 }, M_PI_2 + 0.02f));
		g->transform->location = player->transform->location;
		scene->AttachActor(new BackCanvas(player))->transform->location = player->transform->location;
		CollectAllItems();
	}

	void OnUpdate() override
	{
		frame++;
		if (frame == 270)
		{
			player->GetAttribution().moveSpeed = 300;
			player->GetAttribution().slowMoveSpeed = 120;
		}
		if (frame == 290)
		{
			player->SetPlayerFlags(player->GetPlayerFlags() & ~PF_UsingBomb);
			Destroy();
		}
	}

private:

	int frame;
};