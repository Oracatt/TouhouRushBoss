#pragma once
#include "BossSpellCard.h"
#include "Boss.h"
#include "Effect.h"
#include "BossParticle.h"
#include "SoundEffect.h"
#include "EnemyBulletDeriver.h"
#include "CardBackgroundDeriver.h"
#include "BossSpriteDeriver.h"
#include "ItemDeriver.h"
#include "GSObjects.h"
#include "Shooter.h"
#include "Dialog.h"

class Artia_SC_13 :public BossSpellCard
{
	class LaserCurve_1 :public LaserCurve
	{
	public:

		int frame;

		int type;

		LaserCurve_1(int _type) :LaserCurve(90), frame(0), type(_type)
		{
			mb->drag = { 0.2,0.2,0.2 };
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (frame < 60)
			{
				if (type == 0)
					mb->force = GetVector(GetAngle(mb->velocity) + M_PI_2, 170);
				else if (type == 1)
					mb->force = GetVector(GetAngle(mb->velocity) - M_PI_2, 170);
			}
		}
	};

	class Laser_1 :public Laser
	{
	public:

		int frame;

		Laser_1() :Laser(), frame(0)
		{
			width = 2;
			length = 1000;
			checking = false;
			SetColor(2);
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (frame > 20 && frame <= 25)
			{
				width += 5.0f;
			}
			if (frame == 25)
			{
				checking = true;
			}
			if (frame == 70 + 10 * difficulty)
			{
				checking = false;
			}
			if (frame > 70 + 10 * difficulty)
			{
				width -= 5.0f;
			}
			if (width <= 2.1f && frame > 70)
			{
				Destroy();
			}
		}
	};

	class Laser_2 :public Laser
	{
	public:

		int frame;

		Laser_2() :Laser(), frame(0)
		{
			width = 2;
			length = 1000;
			checking = false;
			SetColor(2);
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (frame > 60 && frame <= 80)
			{
				width += 1.2f;
			}
			if (frame == 70)
			{
				checking = true;
			}
			if (frame == 120)
			{
				checking = false;
			}
			if (frame > 120)
			{
				width -= 1.2f;
			}
			if (frame == 140)
			{
				Destroy();
			}
		}
	};

	class Laser_3 :public Laser
	{
	public:

		int frame;

		Laser_3() :Laser(), frame(0)
		{
			width = 2;
			length = 1000;
			checking = false;
			SetColor(4);
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (frame > 30 && frame <= 40)
			{
				width += 2.0f;
			}
			if (frame == 40)
			{
				checking = true;
			}
			if (frame == 90)
			{
				checking = false;
			}
			if (frame > 90)
			{
				width -= 1.0f;
			}
			if (frame == 110)
			{
				Destroy();
			}
		}
	};

	class Shooter_1 :public Shooter
	{
	public:

		float angle;

		int nextShoot;

		Shooter_1() :angle(M_PI_2), nextShoot(0)
		{
		}

		void OnShoot() override
		{
			if (nextShoot <= 0)
			{
				auto d = new Laser_1;
				d->cleanOnOutOfRange = false;
				d->SetColor(4);
				d->transform->location = { 400 * cos(angle),400 * sin(angle),0 };
				d->angle = (float)(GetAngle(d->transform->location) + M_PI);
				scene->AttachActor(new LaserFog(d, 50, 70 + 10 * difficulty));
				se_lazer00.Play();
				nextShoot = max(60 - frame / 4, 4);
				angle -= 0.1295;
			}
			nextShoot--;
			if (frame == 672)
				Destroy();
		}
	};

	class Shooter_2 :public Shooter
	{
		void OnShoot() override
		{
			if (frame % 60 == 1)
			{
				auto cnt = 4 + difficulty;
				for (int i = 0; i < cnt; i++)
				{
					auto d = new Laser_3;
					d->transform->location = { -320.0f + Random(0.0f,640.0f / cnt) + i * 640.0f / cnt,400,0 };
					d->SetColor(4);
					d->angle = -M_PI_2;
					scene->AttachActor(new LaserFog(d, 50, 120));
				}
				se_lazer00.Play();
			}
			if (frame == 601)
				Destroy();
		}
	};

	class Shooter_3 :public Shooter
	{
		void OnShoot() override
		{
			if (frame % 60 == 1)
			{
				auto cnt = 4 + difficulty;
				for (int i = 0; i < cnt; i++)
				{
					auto d = new Laser_3;
					d->transform->location = { -400,-240.0f + Random(0.0f,480.0f / cnt) + i * 480.0f / cnt,0 };
					d->SetColor(4);
					d->angle = 0;
					scene->AttachActor(new LaserFog(d, 50, 120));
				}
				se_lazer00.Play();
			}
			if (frame == 601)
				Destroy();
		}
	};

	class Shooter_4 :public Shooter
	{
	public:

		float startrad;

		Shooter_4(float _startrad) :startrad(_startrad)
		{}

		void OnShoot() override
		{
			ShootAround<ZhenDan>(12, transform->location, 180, startrad + frame * 0.02f, 20 - 2 * frame, [=](ZhenDan* d)->void {
				d->SetColor(4);
				});

			if (frame == 5 + difficulty)
				Destroy();
		}
	};

	class Shooter_5 :public Shooter
	{
	public:

		float angle;

		int nextShoot;

		Shooter_5() :angle(0), nextShoot(12)
		{}

		void OnShoot() override
		{
			if (nextShoot == 0)
			{
				ShootAround<XingDanS>(6 + 3 * difficulty, transform->location, 120 + frame / 50, angle, 10, [=](XingDanS* d)->void {
					d->SetColor(Random(0, 15));
					});
				angle += M_PI * 2 / (6 + 3 * difficulty) * 0.37f;
				se_tan00.Play();
				nextShoot = 12 - (frame / 200);
			}
			nextShoot--;
			if (frame % 30 == 0)
			{
				scene->AttachActor(new Shooter_4(frame))->transform->location = transform->location;
			}
		}
	};

public:

	Artia_SC_13() :BossSpellCard(1500, 60, 5000000, 16, true) {}

	void OnBossSpellStart() override
	{
		isFinalSpell = true;
		boss->Move({ 0,100 });
		boss->SetCharacterFlags(boss->GetCharacterFlags() | CharacterFlag::CF_ImmuneDamage);
		boss->ShowMaple({ 0.75,0.5,1,0 }, 1, true, false);
		CreateLifeBar(0, 0.15, true, false);
		scene->AttachActor(new Artia_CDBG(this));
		scene->AttachActor(new CardCircle(this));
		scene->AttachActor(new SpellCardAttack);
		scene->AttachActor(new Artia_Sprite);
		scene->AttachActor(new SpellCardText(this, L"±ØÉ±¡¸°¢‹Xæ«Ö®Å­¡¹"));
	}

	void OnThink() override
	{
		if (frame == 75)
		{
			ShootAround<LaserCurve_1, int>(0, 18 + 6 * difficulty, boss->transform->location, 300, 0, 10, [=](LaserCurve_1* d)->void {
				d->SetColor(6);
				d->width = 6;
				});
			se_lazer00.Play();
		}
		if (frame == 150)
		{
			ShootAround<LaserCurve_1, int>(1, 18 + 6 * difficulty, boss->transform->location, 300, 0, 10, [=](LaserCurve_1* d)->void {
				d->SetColor(4);
				d->width = 6;
				});
			for (int i = 0; i < 12 + 4 * difficulty; i++)
			{
				auto d = new Laser_2;
				d->transform->location = boss->transform->location;
				d->angle = M_PI * 2 * i / (12 + 4 * difficulty);
				scene->AttachActor(new LaserFog(d, 50, 120));
				se_lazer01.Play();
			}
		}
		if (frame == 245)
		{
			boss->ShowMaple({ 0.75,0.5,1,0 }, 1, true, false);
		}
		if (frame == 320)
		{
			scene->AttachActor(new Shooter_1);
		}
		if (frame == 1140)
		{
			boss->ShowMaple({ 0.75,0.5,1,0 }, 1, true, false);
		}
		if (frame == 1215)
		{
			scene->AttachActor(new Shooter_2);
			scene->AttachActor(new Shooter_3);
		}
		if (frame == 1860)
		{
			boss->ShowMaple({ 0.75,0.5,1,0 }, 3, true, true);
		}
		if (frame == 1965)
		{
			scene->AttachActor(new Shooter_5)->transform->location = boss->transform->location;
		}
	}

	void OnBossFinalSpellDead() override
	{
		se_enep00.Play();
		CleanAuto(CLEAN_FINALSPELL, boss->transform->location);
		scene->AttachActor(new SetPresentTime(2, 130));
	}

	void OnBossSpellEnd() override
	{
		se_enep01.Play();
		boss->SetCharacterFlags(boss->GetCharacterFlags() & ~CharacterFlag::CF_ImmuneDamage);

		ClearLevel();
	}
};

class Artia_SC_12 :public BossSpellCard
{
	class FreezingFog :public Effect
	{
	public:

		MoveBody* mb;

		FreezingFog() :
			Effect(true), sr(AddComponent(new SpriteRender)), scale(0), entered(false), frame(0), mb(AddComponent(new MoveBody))
		{
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = { 0,1,1,1 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_fog.Get());
			transform->scale = { scale,scale,1 };
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (frame <= 128)
			{
				scale = Lerp(0.0f, 256.0f, pow(frame / 128.0f, 0.3f));
				transform->scale = { scale,scale,1 };
			}
			if (!entered)
			{
				if ((player->transform->location - transform->location).GetLength() < scale / 2)
				{
					entered = true;
					player->GetAttribution().moveSpeed /= 1.8f;
					player->GetAttribution().slowMoveSpeed /= 1.8f;
				}
			}
			else
			{
				if ((player->transform->location - transform->location).GetLength() > scale / 2)
				{
					entered = false;
					player->GetAttribution().moveSpeed *= 1.8f;
					player->GetAttribution().slowMoveSpeed *= 1.8f;
				}
			}
			if (frame >= 300)
			{
				mat.diffuse.a -= 0.05f;
				sr->SetMaterial(mat);
			}
			if (frame == 320)
			{
				Destroy();
			}
		}

		void OnDestroy() override
		{
			__super::OnDestroy();
			if (entered)
			{
				entered = false;
				if (player)
				{
					player->GetAttribution().moveSpeed *= 1.8f;
					player->GetAttribution().slowMoveSpeed *= 1.8f;
				}
			}
		}

	private:

		bool entered;

		float scale;

		SpriteRender* sr;

		Material mat;

		int frame;
	};

	class ZhenDan_1 :public ZhenDan
	{
	public:

		int frame;

		float angle;

		ZhenDan_1(int startFrame) :frame(startFrame)
		{
		}

		void OnCreated() override
		{
			__super::OnCreated();
			if ((transform->location - player->transform->location).GetLength() < 20)
				Destroy();
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (frame == 35)
			{
				mb->force = GetVector(angle, 120 + 20 * difficulty);
			}
		}
	};

	class ZhenDan_2 :public ZhenDan
	{
	public:

		int frame;

		int type;

		ZhenDan_2(int _type) :frame(0), type(_type)
		{
			mb->drag = { 2,2,2 };
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (frame == 60)
			{
				mb->force = GetVector(GetAngle(mb->velocity) + (type == 0 ? 0.5f : -0.5f), 450);
			}
		}
	};

	class ZhenDan_3 :public ZhenDan
	{
	public:

		int frame;

		float angle;

		ZhenDan_3() :frame(0)
		{
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (frame == 60)
			{
				mb->force = GetVector(angle, 180);
			}
		}
	};

	class Shooter_1 :public Shooter
	{
	public:

		int type;

		Shooter_1(int _type) :type(_type)
		{
		}

		void OnShoot() override
		{
			if (frame < 25)
			{
				ShootAround<ZhenDan_1, int>(frame, 24 + 8 * difficulty, transform->location, 0.01f, (type == 0 ? 1.0f : -1.0f) * (float)((0.37f + 0.02f * difficulty) * M_PI * 2 * frame / 6 / (24 + 8 * difficulty)), frame * 20.0f, [=](ZhenDan_1* d)->void {
					d->SetColor(6);
					d->angle = GetAngle(d->mb->velocity);
					});
				se_tan00.Play();
			}
			else
				Destroy();
		}
	};

	class Shooter_2 :public Shooter
	{
	public:

		void OnShoot() override
		{
			if (frame % (6 - difficulty) == 0)
			{
				ShootAround<HuanYu>(3, transform->location, 125, frame/12.0f, 10, [=](HuanYu* d)->void {
					d->SetColor(6);
					});
				ShootAround<HuanYu>(3, transform->location, 110, frame / 12.0f, 10, [=](HuanYu* d)->void {
					d->SetColor(6);
					});
				se_tan01.Play();
			}
			if (frame == 120)
				Destroy();
		}
	};

public:

	Vector3 bossLoc[2];

	int moveStep;

	vector<ComPtr<FreezingFog>> fogs;

	float plMs;

	float plSms;

	Artia_SC_12() :BossSpellCard(1100, 40, 5200000, 15),
		bossLoc{ {0,100,0},{35,120,0} }, moveStep(0),plMs(0),plSms(0)
	{}

	void OnBossSpellStart() override
	{
		plMs = player->GetAttribution().moveSpeed;
		plSms = player->GetAttribution().slowMoveSpeed;

		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		CreateLifeBar(0, 0.15, true, false);
		scene->AttachActor(new Artia_CDBG(this));
		scene->AttachActor(new CardCircle(this));
		scene->AttachActor(new SpellCardAttack);
		scene->AttachActor(new Artia_Sprite);
		scene->AttachActor(new SpellCardText(this, L"¼«º®¡¸Ø¨¹Åº®Ëª¡¹"));
	}

	void OnThink() override
	{
		if (frame == 70)
		{
			boss->ShowMaple({ 0.75,0.5,1,0 }, 1, true, false);
		}
		if (frame == 80)
		{
			for (int i = 0; i < 12; i++)
			{
				auto fog = scene->AttachActor(new FreezingFog);
				fog->transform->location = boss->transform->location;
				fog->mb->velocity = GetVector(i * M_PI * 2 / 12, 100);
				fogs.push_back(fog);
			}
		}
		if (frame == 100)
		{
			for (int i = 0; i < 12; i++)
			{
				auto fog = scene->AttachActor(new FreezingFog);
				fog->transform->location = boss->transform->location;
				fog->mb->velocity = GetVector(i * M_PI * 2 / 12, 92);
				fogs.push_back(fog);
			}
		}
		if (frame == 120)
		{
			for (int i = 0; i < 12; i++)
			{
				auto fog = scene->AttachActor(new FreezingFog);
				fog->transform->location = boss->transform->location;
				fog->mb->velocity = GetVector(i * M_PI * 2 / 12, 84);
				fogs.push_back(fog);
			}
		}
		if (frame == 155)
		{
			scene->AttachActor(new Shooter_1(0))->transform->location = boss->transform->location;
		}
		if (frame == 305)
		{
			for (int i = 0; i < 12; i++)
			{
				auto fog = scene->AttachActor(new FreezingFog);
				fog->transform->location = boss->transform->location;
				fog->mb->velocity = GetVector(i * M_PI * 2 / 12, 100);
				fogs.push_back(fog);
			}
		}
		if (frame == 335)
		{
			for (int i = 0; i < 12; i++)
			{
				auto fog = scene->AttachActor(new FreezingFog);
				fog->transform->location = boss->transform->location;
				fog->mb->velocity = GetVector(i * M_PI * 2 / 12, 92);
				fogs.push_back(fog);
			}
		}
		if (frame == 305)
		{
			ShootAround<ZhenDan_2, int>(0, 24 + 8 * difficulty, boss->transform->location, 120, 0, 10, [=](ZhenDan_2* d)->void {
				d->SetColor(4);
				});
			ShootAround<ZhenDan_2, int>(1, 24 + 8 * difficulty, boss->transform->location, 120, M_PI / (24 + 8 * difficulty), 10, [=](ZhenDan_2* d)->void {
				d->SetColor(4);
				});
			se_tan00.Play();
		}
		if (frame == 330)
		{
			ShootAround<ZhenDan_2, int>(0, 24 + 8 * difficulty, boss->transform->location, 120, 0, 10, [=](ZhenDan_2* d)->void {
				d->SetColor(4);
				});
			ShootAround<ZhenDan_2, int>(1, 24 + 8 * difficulty, boss->transform->location, 120, M_PI / (24 + 8 * difficulty), 10, [=](ZhenDan_2* d)->void {
				d->SetColor(4);
				});
			se_tan00.Play();
		}
		if (frame == 355)
		{
			ShootAround<ZhenDan_2, int>(0, 24 + 8 * difficulty, boss->transform->location, 120, 0, 10, [=](ZhenDan_2* d)->void {
				d->SetColor(4);
				});
			ShootAround<ZhenDan_2, int>(1, 24 + 8 * difficulty, boss->transform->location, 120, M_PI / (24 + 8 * difficulty), 10, [=](ZhenDan_2* d)->void {
				d->SetColor(4);
				});
			se_tan00.Play();
		}
		if (frame == 390)
		{
			scene->AttachActor(new Shooter_1(1))->transform->location = boss->transform->location;
		}
		if (frame == 500)
		{
			boss->ShowMaple({ 0.75,0.5,1,0 }, 1, true, false);
			for (int i = 0; i < 12; i++)
			{
				auto fog = scene->AttachActor(new FreezingFog);
				fog->transform->location = boss->transform->location;
				fog->mb->velocity = GetVector(i * M_PI * 2 / 12, 75);
				fogs.push_back(fog);
			}
		}
		if (frame == 575)
		{
			scene->AttachActor(new Shooter_2)->transform->location = boss->transform->location;
		}
		if (frame == 660)
		{
			float ang = RandomAngle();
			ShootAround<ZhenDan_3>(12 + 4 * difficulty, { 220,160,0 }, 120, ang, 10, [=](ZhenDan_3* d)->void {
				d->mb->drag = { 1,1,1 };
				d->angle = GetAngle(d->mb->velocity);
				d->SetColor(6);
				});
			ShootAround<ZhenDan_3>(12 + 4 * difficulty, { 220,160,0 }, 80, ang, 10, [=](ZhenDan_3* d)->void {
				d->mb->drag = { 1,1,1 };
				d->angle = GetAngle(d->mb->velocity);
				d->SetColor(6);
				});
			ang = RandomAngle();
			ShootAround<ZhenDan_3>(12 + 4 * difficulty, { -220,160,0 }, 120, ang, 10, [=](ZhenDan_3* d)->void {
				d->mb->drag = { 1,1,1 };
				d->angle = GetAngle(d->mb->velocity);
				d->SetColor(6);
				});
			ShootAround<ZhenDan_3>(12 + 4 * difficulty, { -220,160,0 }, 80, ang, 10, [=](ZhenDan_3* d)->void {
				d->mb->drag = { 1,1,1 };
				d->angle = GetAngle(d->mb->velocity);
				d->SetColor(6);
				});
			ang = RandomAngle();
			ShootAround<ZhenDan_3>(12 + 4 * difficulty, { 140,40,0 }, 120, ang, 10, [=](ZhenDan_3* d)->void {
				d->mb->drag = { 1,1,1 };
				d->angle = GetAngle(d->mb->velocity);
				d->SetColor(6);
				});
			ShootAround<ZhenDan_3>(12 + 4 * difficulty, { 140,40,0 }, 80, ang, 10, [=](ZhenDan_3* d)->void {
				d->mb->drag = { 1,1,1 };
				d->angle = GetAngle(d->mb->velocity);
				d->SetColor(6);
				});
			ang = RandomAngle();
			ShootAround<ZhenDan_3>(12 + 4 * difficulty, { -140,40,0 }, 120, ang, 10, [=](ZhenDan_3* d)->void {
				d->mb->drag = { 1,1,1 };
				d->angle = GetAngle(d->mb->velocity);
				d->SetColor(6);
				});
			ShootAround<ZhenDan_3>(12 + 4 * difficulty, { -140,40,0 }, 80, ang, 10, [=](ZhenDan_3* d)->void {
				d->mb->drag = { 1,1,1 };
				d->angle = GetAngle(d->mb->velocity);
				d->SetColor(6);
				});
			se_tan00.Play();
		}
		if (frame == 800)
			frame = 0;
	}

	void OnBossSpellEnd() override
	{
		for (auto fog : fogs)
		{
			fog->Destroy();
		}
		fogs.clear();

		player->GetAttribution().moveSpeed = plMs;
		player->GetAttribution().slowMoveSpeed = plSms;

		se_enep02.Play();
		CleanAuto(CLEAN_SPELL, boss->transform->location);
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		DropItem<Power>(20, boss->transform->location, 70);
		DropItem<Point>(30, boss->transform->location, 70);
		DropItem<BombItem>(1, boss->transform->location, 0);

		ComPtr<Boss> tempBoss(boss);
		scene->AttachActor(new DelayInvoke(Delegate([tempBoss]()->void {tempBoss->AddComponent(new Artia_SC_13); }), 60));
	}
};

class Artia_SC_11 :public BossSpellCard
{
	class ZhaDan_1 :public ZhaDan
	{
	public:

		int frame;

		ZhaDan_1() :frame(0)
		{
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (frame == 20 + difficulty)
			{
				mb->velocity = mb->velocity.GetNormalized() * 80;
			}
		}
	};

	class Shooter_1 :public Shooter
	{
	public:

		LPGOBJ follow;

		Vector3 offset;

		Shooter_1(LPGOBJ _follow, const Vector3& _offset) :follow(_follow), offset(_offset)
		{
		}

		void OnShoot() override
		{
			transform->location = follow->transform->location + offset;
			if (frame % 5 == 0)
			{
				ShootAround<ZhaDan_1>(3 + difficulty, transform->location, 400, RandomAngle(), 20, [=](ZhaDan_1* d)->void {
					d->SetColor(5);
					});
				se_tan00.Play();
			}
		}

		void OnDestroy() override
		{
			__super::OnDestroy();
			follow.Reset();
		}
	};

public:

	Artia_SC_11() :BossSpellCard(520, 39) {}

	void OnBossSpellStart() override
	{
		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		boss->ShowMaple({ 0.75,0.5,1,0 }, 1, true, false);
		CreateLifeBar(0.15, 1, false, true);
	}

	void OnThink() override
	{
		if (frame == 240)
			boss->Move({ 0,100 });

		if (frame == 540)
			boss->Move({ 45,104 });

		if (frame == 840)
			boss->Move({ -81,98 });

		if (frame == 1140)
			boss->Move({ 102,107 });

		if (frame == 75)
		{
			scene->AttachActor(new Shooter_1(boss, { -50,-50,0 }));
			scene->AttachActor(new Shooter_1(boss, { 0,-50,0 }));
			scene->AttachActor(new Shooter_1(boss, { 50,-50,0 }));
		}

		if (frame == 1400)
			frame = 200;
	}

	void OnBossSpellEnd() override
	{
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		CleanAuto(CLEAN_NOSPELL);
		boss->AddComponent(new Artia_SC_12);
	}
};

class Artia_SC_10 :public BossSpellCard
{
	class Shooter_1 :public Shooter
	{
		void OnShoot() override
		{
			int d[] = { 6,7,9,9 };
			ShootAround<ZhenDan>(d[difficulty], transform->location, 100 + 10 * difficulty, 1.1f * sin(frame / 12.0f), 10, [=](ZhenDan* d)->void {
				d->SetColor(6);
				});
			se_tan00.Play();
		}
	};

public:

	Vector3 bossLoc[2];

	int moveStep;

	Artia_SC_10() :BossSpellCard(1150, 46, 4800000, 14),
		bossLoc{ {0,100,0},{35,120,0} }, moveStep(0)
	{}

	void OnBossSpellStart() override
	{
		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		CreateLifeBar(0, 0.15, true, false);
		scene->AttachActor(new Artia_CDBG(this));
		scene->AttachActor(new CardCircle(this));
		scene->AttachActor(new SpellCardAttack);
		scene->AttachActor(new Artia_Sprite);
		scene->AttachActor(new SpellCardText(this, L"´Ì¹Ç¡¸Îè¶¯µÄ±ù×¶¡¹"));
	}

	void OnThink() override
	{
		if (frame == 1)
		{
			boss->ShowMaple({ 0.75,0.5,1,0 }, 1, true, false);
		}
		if (frame == 75)
		{
			scene->AttachActor(new Shooter_1)->transform->location = boss->transform->location;
		}
	}

	void OnBossSpellEnd() override
	{
		se_enep02.Play();
		CleanAuto(CLEAN_SPELL, boss->transform->location);
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		DropItem<Power>(20, boss->transform->location, 70);
		DropItem<Point>(30, boss->transform->location, 70);
		DropItem<BombItem>(1, boss->transform->location, 0);

		ComPtr<Boss> tempBoss(boss);
		scene->AttachActor(new DelayInvoke(Delegate([tempBoss]()->void {tempBoss->AddComponent(new Artia_SC_11); }), 60));
	}
};

class Artia_SC_9 :public BossSpellCard
{
	class Shooter_1 :public Shooter
	{
		void OnShoot() override
		{
			if (frame % 6 == 0)
			{
				for (int i = 3 + difficulty / 2; i >= 0; i--)
					ShootAround<ZhaDan>(6 + 2 * difficulty, transform->location, 210 + 10 * difficulty, 0.27f * M_PI * 2 * frame / 6 / (6 + 2 * difficulty), 68 - 12 * i, [=](ZhaDan* d)->void {
					d->SetColor(5);
						});
				ShootAround<XiaoYu>(6 + 2 * difficulty, transform->location, 210 + 10 * difficulty, 0.27f * M_PI * 2 * frame / 6 / (6 + 2 * difficulty), 80, [=](XiaoYu* d)->void {
					d->SetColor(5);
					});
				se_tan00.Play();
			}
		}
	};

public:

	Artia_SC_9() :BossSpellCard(700, 48){}

	void OnBossSpellStart() override
	{
		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		CreateLifeBar(0.15, 1, false, true);
	}

	void OnThink() override
	{
		if (frame == 1)
		{
			boss->ShowMaple({ 0.75,0.5,1,0 }, 1, true, false);
		}
		if (frame == 75)
		{
			scene->AttachActor(new Shooter_1)->transform->location = boss->transform->location;
		}
	}

	void OnBossSpellEnd() override
	{
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		CleanAuto(CLEAN_NOSPELL);
		boss->AddComponent(new Artia_SC_10);
	}
};

class Artia_SC_8 :public BossSpellCard
{
	class Mirror :public EnemyBullet
	{
		class XingDanS_1 :public XingDanS
		{
		public:

			Vector3 vec;

			int frame;

			XingDanS_1() :frame(0) {}

			void OnUpdate() override
			{
				__super::OnUpdate();
				frame++;
				if (IsValid() && frame == 30)
				{
					mb->force = vec * 180;
				}
			}
		};

		class GuangYuL_1 :public GuangYuL
		{
		public:

			int frame;

			Vector3 lastLoc;

			GuangYuL_1() :frame(0) {}

			void OnUpdate() override
			{
				__super::OnUpdate();
				frame++;
				if (frame > 90)
				{
					if (frame % (8 - difficulty) == 0)
					{
						auto d = new XingDanS_1;
						d->vec = (transform->location - lastLoc).GetNormalized();
						d->transform->location = transform->location;
						d->mb->drag = { 1,1,1 };
						d->SetColor(Random(0, 15));
						scene->AttachActor(new EnemyBulletFog(d));
						se_tan00.Play();
					}
				}
				lastLoc = transform->location;
			}
		};

	public:

		ComPtr<GuangYuL> guangYu[3];

		ComPtr<Laser> laser[6];

		int frame;

		float radius;

		float lenToCenter;

		float angle;

		float subAngle;

		LPGOBJ centerObj;

		Mirror(LPGOBJ _centerObj, float _angle, float _subAngle) :EnemyBullet(AR_NONE, true), frame(0),
			radius(0), lenToCenter(0), angle(_angle), subAngle(_subAngle), centerObj(_centerObj)
		{
			breakSize = 0;
			breakColor = breakColor8;
			checkRadius = 0;
			transform->location = centerObj->transform->location;
			cleanOnHit = false;
			cleanOnBomb = false;
			cleanOnOutOfRange = false;
		}

		void OnCheck() override
		{}

		void OnCreated() override
		{
			__super::OnCreated();
			for (int i = 0; i < 3; i++)
			{
				guangYu[i] = scene->AttachActor(new GuangYuL_1);
				guangYu[i]->cleanOnHit = false;
				guangYu[i]->cleanOnBomb = false;
				guangYu[i]->cleanOnOutOfRange = false;
				guangYu[i]->SetColor(4);
				guangYu[i]->transform->location = transform->location;
			}
			for (int i = 0; i < 6; i++)
			{
				int srcIndex = i / 2;
				int dstIndex = (srcIndex + ((i % 2 == 0) ? 1 : 2)) % 3;
				laser[i] = new Laser;
				laser[i]->cleanOnHit = false;
				laser[i]->cleanOnBomb = false;
				laser[i]->cleanOnOutOfRange = false;
				laser[i]->SetColor(4);
				laser[i]->transform->location = guangYu[srcIndex]->transform->location;
				laser[i]->length = (guangYu[dstIndex]->transform->location - guangYu[srcIndex]->transform->location).GetLength();
				laser[i]->width = 1;
				laser[i]->angle = GetAngle(guangYu[dstIndex]->transform->location - guangYu[srcIndex]->transform->location);
				scene->AttachActor(laser[i].Get());
			}
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			subAngle += 0.05;
			angle -= 0.05;
			if (frame <= 120)
			{
				radius += 0.5;
				lenToCenter += 1;
			}
			transform->location = centerObj->transform->location + GetVector(angle, lenToCenter);
			for (int i = 0; i < 3; i++)
			{
				guangYu[i]->transform->location = transform->location + GetVector(subAngle + i * M_PI * 2.0f / 3.0f, radius);
			}
			for (int i = 0; i < 6; i++)
			{
				int srcIndex = i / 2;
				int dstIndex = (srcIndex + ((i % 2 == 0) ? 1 : 2)) % 3;
				laser[i]->transform->location = guangYu[srcIndex]->transform->location;
				laser[i]->length = (guangYu[dstIndex]->transform->location - guangYu[srcIndex]->transform->location).GetLength();;
				laser[i]->width = 1 + radius / 60.0f * 19.0f;
				laser[i]->angle = GetAngle(guangYu[dstIndex]->transform->location - guangYu[srcIndex]->transform->location);
			}
		}

		void OnDestroy() override
		{
			__super::OnDestroy();
			for (int i = 0; i < 3; i++)
			{
				guangYu[i].Reset();
			}
			for (int i = 0; i < 6; i++)
			{
				laser[i].Reset();
			}
			centerObj.Reset();
		}

		void SetColor(int col) override
		{
			colorIndex = col;
		}
	};

	class Laser_1 :public Laser
	{
	public:

		int frame;

		Laser_1() :Laser(), frame(0)
		{
			width = 2;
			length = 1000;
			checking = false;
			SetColor(4);
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (frame > 60 && frame <= 80)
			{
				width += 1.5f;
			}
			if (frame == 80)
			{
				checking = true;
			}
			if (frame == 140)
			{
				checking = false;
			}
			if (frame > 140)
			{
				width -= 1.5f;
			}
			if (frame == 160)
			{
				Destroy();
			}
		}
	};

	class Shooter_1 :public Shooter
	{
	public:

		float angle;

		int spawn;

		Shooter_1(float _angle) :angle(_angle), spawn(0)
		{
		}

		void OnShoot() override
		{
			if (frame == 1)
			{
				auto d = new Laser_1;
				d->transform->location = transform->location;
				d->angle = angle;
				scene->AttachActor(new LaserFog(d, 50, 140));
				se_lazer00.Play();
			}
			if (frame > 80 && frame % 2 == 0)
			{
				spawn++;
				auto d = new YanDan;
				d->SetColor(1);
				d->transform->location = transform->location + GetVector(angle, (30 - (3 * difficulty)) * spawn);
				d->mb->force = GetVector(RandomAngle(), 50);
				d->mb->drag = { 0.3,0.3,0.3 };
				scene->AttachActor(new EnemyBulletFog(d));
				if (IsOutOfRange(d->transform->location, 10))
					Destroy();
			}
		}
	};

	class LaserCurve_1 :public LaserCurve
	{
	public:

		int frame;

		float baseAngle;

		float offsetAngle;

		int type;

		LaserCurve_1(float _baseAngle, float _offsetAngle, int _type) :
			LaserCurve(90), frame(0), baseAngle(_baseAngle), offsetAngle(_offsetAngle), type(_type)
		{
			width = 8;
		}

		void OnCreated() override
		{
			__super::OnCreated();
			SetColor(type == 0 ? 4 : 6);
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			mb->velocity = GetVector(baseAngle + offsetAngle * cos(frame / 10.0f) * (type == 0 ? 1 : -1), 350);
		}
	};

	class Shooter_2 :public Shooter
	{
	public:

		float angle;

		int spawn;

		Shooter_2(float _angle) :angle(_angle), spawn(0)
		{
		}

		void OnShoot() override
		{
			if (frame == 1)
			{
				auto d = new Laser_1;
				d->transform->location = transform->location;
				d->angle = angle;
				scene->AttachActor(new LaserFog(d, 50, 140));
				se_lazer00.Play();
			}
			if (frame == 30)
			{
				auto d = new LaserCurve_1(angle,0.36f,0);
				d->transform->location = transform->location;
				scene->AttachActor(new EnemyBulletFog(d));
				d = new LaserCurve_1(angle, 0.36f, 1);
				d->transform->location = transform->location;
				scene->AttachActor(new EnemyBulletFog(d));
				Destroy();
			}
		}
	};

public:

	Vector3 bossLoc[3];

	int moveStep;

	Artia_SC_8() :BossSpellCard(1150, 44, 4500000, 13),
		bossLoc{ {0,100,0},{-35,105,0},{33,88,0} }, moveStep(0)
	{}

	void OnBossSpellStart() override
	{
		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		CreateLifeBar(0, 0.15, true, false);
		scene->AttachActor(new Artia_CDBG(this));
		scene->AttachActor(new CardCircle(this));
		scene->AttachActor(new SpellCardAttack);
		scene->AttachActor(new Artia_Sprite);
		scene->AttachActor(new SpellCardText(this, L"Òì¹â¡¸ÒìÊÀ½çÀâ¾µ¡¹"));
	}

	void OnThink() override
	{
		if (frame == 60)
		{
			boss->ShowMaple({ 0.75,0.5,1,0 }, 3, true, false);
		}
		if (frame == 135)
		{
			for (int i = 0; i < 3; i++)
				scene->AttachActor(new Mirror(boss, i * M_PI * 2.0f / 3, i * M_PI * 2.0f / 3));
		}
		if (frame == 210)
		{
			for (int i = 0; i < 6 + 2 * difficulty; i++)
				scene->AttachActor(new Shooter_1(M_PI * 2 * i / (6 + 2 * difficulty)))->transform->location = boss->transform->location;
		}
		if (frame == 420)
		{
			moveStep = (moveStep + 1) % 3;
			boss->Move(bossLoc[moveStep]);
		}
		if (frame == 560)
		{
			for (int i = 0; i < 6 + 2 * difficulty; i++)
				scene->AttachActor(new Shooter_2(GetAngle(player->transform->location - boss->transform->location) + M_PI * 2 * i / (6 + 2 * difficulty)))->transform->location = boss->transform->location;
		}
		if (frame == 770)
		{
			moveStep = (moveStep + 1) % 3;
			boss->Move(bossLoc[moveStep]);
		}
		if (frame == 810)
		{
			boss->ShowMaple({ 0.75,0.5,1,0 }, 1, true, false);
		}
		if (frame == 860)
		{
			frame = 180;
		}
	}

	void OnBossSpellEnd() override
	{
		se_enep02.Play();
		CleanAuto(CLEAN_SPELL, boss->transform->location);
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		DropItem<Power>(20, boss->transform->location, 70);
		DropItem<Point>(30, boss->transform->location, 70);
		DropItem<LifeItem>(1, boss->transform->location, 0);

		ComPtr<Boss> tempBoss(boss);
		scene->AttachActor(new DelayInvoke(Delegate([tempBoss]()->void {tempBoss->AddComponent(new Artia_SC_9); }), 60));
	}
};

class Artia_SC_7 :public BossSpellCard
{
	class Shooter_1 :public Shooter
	{
	public:

		int type;

		LPGOBJ follow;

		Vector3 offset;

		Shooter_1(int _type, LPGOBJ _follow, const Vector3& _offset) :type(_type), follow(_follow), offset(_offset)
		{
		}

		void OnShoot() override
		{
			transform->location = follow->transform->location + offset;
			if (frame % 5 == 0)
			{
				ShootAround<ZhaDan>(3 + difficulty, transform->location, 100 + 5 * difficulty, frame / (12 + 2.0f * difficulty) * (type == 0 ? 1 : -1), 30, [=](ZhaDan* d)->void {
					d->SetColor(type == 0 ? 5 : 8);
					});
				se_tan00.Play();
			}
		}

		void OnDestroy() override
		{
			__super::OnDestroy();
			follow.Reset();
		}
	};

public:

	Vector3 bossLoc[7];

	int moveStep;

	Artia_SC_7() :BossSpellCard(745, 53),
		bossLoc{ {0,100,0},{-24,143,0},{35,129,0},{101,85,0},{44,78,0},{-53,105,0},{-94,113,0} }, moveStep(0){}

	void OnBossSpellStart() override
	{
		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		CreateLifeBar(0.15, 1, false, true);
	}

	void OnThink() override
	{
		if (frame == 1)
		{
			boss->ShowMaple({ 0.75,0.5,1,0 }, 1, true, false);
		}
		if (frame == 75)
		{
			for (int i = 0; i < 3; i++)
			{
				scene->AttachActor(new Shooter_1(0, boss, GetVector(M_PI_2 + i * M_PI * 2 / 3, 75)));
			}
		}
		if (frame == 300)
		{
			moveStep = (moveStep + 1) % 7;
			boss->Move(bossLoc[moveStep]);
			frame = 100;
		}
	}

	void OnBossSpellEnd() override
	{
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		CleanAuto(CLEAN_NOSPELL);
		boss->AddComponent(new Artia_SC_8);
	}
};

class Artia_SC_6 :public BossSpellCard
{
	class LaserCurve_1 :public LaserCurve
	{
	public:

		int frame;

		int type;

		LaserCurve_1(int _type) :LaserCurve(90), frame(0), type(_type)
		{
			mb->drag = { 0.2,0.2,0.2 };
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (frame < 60)
			{
				if (type == 0)
					mb->force = GetVector(GetAngle(mb->velocity) + M_PI_2, 150);
				else if(type==1)
					mb->force = GetVector(GetAngle(mb->velocity) - M_PI_2, 150);
			}
		}
	};

	class XingDanL_1 :public XingDanL
	{
	public:

		int frame;

		int type;

		float angle;

		XingDanL_1(int _type) : frame(0), type(_type)
		{
			mb->drag = { 1.0,1.0,1.0 };
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (frame == 30)
			{
				if (type == 0)
					mb->force = GetVector(angle + (float)M_PI / 3, 220);
				else if (type == 1)
					mb->force = GetVector(angle - (float)M_PI / 3, 220);
			}
			if (frame > 60 && frame < 120)
			{
				if (type == 0)
					mb->force = GetVector(GetAngle(mb->velocity) + (float)M_PI / 3, 220);
				else if (type == 1)
					mb->force = GetVector(GetAngle(mb->velocity) - (float)M_PI / 3, 220);
			}
			if (frame == 120)
			{
				mb->force = { 0,0,0 };
				mb->drag = { 0,0,0 };
			}
		}
	};

	class Laser_1 :public Laser
	{
	public:

		int frame;

		Laser_1() :Laser(), frame(0)
		{
			width = 2;
			length = 1000;
			checking = false;
			SetColor(4);
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (frame > 60 && frame <= 80)
			{
				width += 1.5f;
			}
			if (frame == 80)
			{
				checking = true;
			}
			if (frame == 140)
			{
				checking = false;
			}
			if (frame > 140)
			{
				width -= 1.5f;
			}
			if (frame == 160)
			{
				Destroy();
			}
		}
	};

	class Shooter_1 :public Shooter
	{
	public:

		int type;

		Shooter_1(int _type) :type(_type)
		{
		}

		void OnShoot() override
		{
			if (frame % (3 - difficulty / 2) == 0)
			{
				auto d = new XingDanL_1(type);
				d->angle = frame / 5.0f * (type == 0 ? 1 : -1);
				d->mb->velocity = GetVector(d->angle, 200);
				d->transform->location = transform->location;
				d->SetColor(type == 0 ? 6 : 5);
				scene->AttachActor(new EnemyBulletFog(d));
				se_tan00.Play();
			}
			if (frame == 120)
				Destroy();
		}
	};

public:

	Vector3 bossLoc[2];

	int moveStep;

	Artia_SC_6() :BossSpellCard(1050, 39, 4300000, 12), 
		bossLoc{ {0,100,0},{35,120,0} }, moveStep(0)
	{}

	void OnBossSpellStart() override
	{
		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		CreateLifeBar(0, 0.15, true, false);
		scene->AttachActor(new Artia_CDBG(this));
		scene->AttachActor(new CardCircle(this));
		scene->AttachActor(new SpellCardAttack);
		scene->AttachActor(new Artia_Sprite);
		scene->AttachActor(new SpellCardText(this, L"Á÷¹â¡¸Á÷Ó°º®ÐÇ¡¹"));
	}

	void OnThink() override
	{
		if (frame == 1)
		{
			boss->ShowMaple({ 0.75,0.5,1,0 }, 1, true, false);
		}
		if (frame == 75)
		{
			scene->AttachActor(new Shooter_1(0))->transform->location = boss->transform->location;
		}
		if (frame == 135)
		{
			scene->AttachActor(new Shooter_1(1))->transform->location = boss->transform->location;
		}
		if (frame == 150)
		{
			ShootAround<LaserCurve_1, int>(0, 12 + 4 * difficulty, boss->transform->location, 300, 0, 10, [=](LaserCurve_1* d)->void {
				d->SetColor(6);
				d->width = 6;
				});
			ShootAround<LaserCurve_1, int>(1, 12 + 4 * difficulty, boss->transform->location, 300, 0, 10, [=](LaserCurve_1* d)->void {
				d->SetColor(4);
				d->width = 6;
				});
			se_lazer00.Play();
		}
		if (frame == 230)
		{
			auto d = new Laser_1;
			d->transform->location = boss->transform->location;
			d->angle = GetAngle(player->transform->location - boss->transform->location);
			scene->AttachActor(new LaserFog(d, 50, 140));
			se_lazer01.Play();
		}
		if ((frame == 210 || frame == 230 || frame == 250))
		{
			ShootAround<XingDanS>(24 + 8 * difficulty, boss->transform->location, 100 + difficulty * 10, 0, 10, [=](XingDanS* d)->void {
				d->SetColor(Random(0, 15));
				});
			se_tan01.Play();
		}
		if (frame == 275)
		{
			scene->AttachActor(new Shooter_1(0))->transform->location = boss->transform->location;
		}
		if (frame == 335)
		{
			scene->AttachActor(new Shooter_1(1))->transform->location = boss->transform->location;
		}
		if (frame == 380)
		{
			ShootAround<LaserCurve_1, int>(0, 12 + 4 * difficulty, boss->transform->location, 300, 0, 10, [=](LaserCurve_1* d)->void {
				d->SetColor(6);
				d->width = 6;
				});
			ShootAround<LaserCurve_1, int>(1, 12 + 4 * difficulty, boss->transform->location, 300, 0, 10, [=](LaserCurve_1* d)->void {
				d->SetColor(4);
				d->width = 6;
				});
			se_lazer00.Play();
		}
		if (frame == 450)
		{
			moveStep = (moveStep + 1) % 2;
			boss->Move(bossLoc[moveStep]);
		}
		if (frame == 600)
		{
			frame = 0;
		}
	}

	void OnBossSpellEnd() override
	{
		se_enep02.Play();
		CleanAuto(CLEAN_SPELL, boss->transform->location);
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		DropItem<Power>(20, boss->transform->location, 70);
		DropItem<Point>(30, boss->transform->location, 70);
		DropItem<LifeItem>(1, boss->transform->location, 0);

		ComPtr<Boss> tempBoss(boss);
		scene->AttachActor(new DelayInvoke(Delegate([tempBoss]()->void {tempBoss->AddComponent(new Artia_SC_7); }), 60));
	}
};

class Artia_SC_5 :public BossSpellCard
{
	class Shooter_1 :public Shooter
	{
	public:

		int offset;

		Shooter_1() :offset(80)
		{
		}

		void OnShoot() override
		{
			if (frame % 6 == 0)
			{
				ShootAround<ZhaDan>(12 + 4 * difficulty, transform->location, 120, frame * 0.05f * M_PI * 2 / (12 + 4 * difficulty), offset, [=](ZhaDan* d)->void {
					d->SetColor(5);
					});
				se_tan00.Play();
				offset -= 10;
				if (offset == -90)
					Destroy();
			}
		}
	};

	class Shooter_2 :public Shooter
	{
	public:

		LPGOBJ follow;

		Shooter_2(LPGOBJ _follow) :follow(_follow)
		{
		}

		void OnShoot() override
		{
			if (frame % 10 == 0)
			{
				ShootAround<ZhenDan>(18 + 6 * difficulty, follow->transform->location, 190 - frame, 0, 10, [=](ZhenDan* d)->void {
					d->SetColor(4);
					});
				se_tan00.Play();
			}
			if (frame == 60)
				Destroy();
		}

		void OnDestroy() override
		{
			__super::OnDestroy();
			follow.Reset();
		}
	};

public:

	Vector3 bossLoc[4];

	int moveStep;

	Artia_SC_5() :BossSpellCard(600, 45),
		bossLoc{ {0,100,0},{-135,79,0},{22,88,0},{100,111,0} }, moveStep(0){}

	void OnBossSpellStart() override
	{
		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		CreateLifeBar(0.15, 1, false, true);
	}

	void OnThink() override
	{
		if (frame == 1)
		{
			boss->ShowMaple({ 0.75,0.5,1,0 }, 1, true, false);
		}
		if (frame == 80)
		{
			scene->AttachActor(new Shooter_1)->transform->location = boss->transform->location;
		}
		if (frame == 200)
		{
			moveStep = (moveStep + 1) % 4;
			boss->Move(bossLoc[moveStep]);
			scene->AttachActor(new Shooter_2(boss));
		}
		if (frame == 300)
		{
			scene->AttachActor(new Shooter_1)->transform->location = boss->transform->location;
		}
		if (frame == 420)
		{
			moveStep = (moveStep + 1) % 4;
			boss->Move(bossLoc[moveStep]);
			scene->AttachActor(new Shooter_2(boss));
		}
		if (frame == 520)
		{
			scene->AttachActor(new Shooter_1)->transform->location = boss->transform->location;
		}
		if (frame == 640)
		{
			moveStep = (moveStep + 1) % 4;
			boss->Move(bossLoc[moveStep]);
			scene->AttachActor(new Shooter_2(boss));
		}
		if (frame == 740)
		{
			scene->AttachActor(new Shooter_1)->transform->location = boss->transform->location;
		}
		if (frame == 860)
		{
			moveStep = (moveStep + 1) % 4;
			boss->Move(bossLoc[moveStep]);
			scene->AttachActor(new Shooter_2(boss));
		}
		if (frame == 990)
			frame = 0;
	}

	void OnBossSpellEnd() override
	{
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		CleanAuto(CLEAN_NOSPELL);
		boss->AddComponent(new Artia_SC_6);
	}
};

class Artia_SC_4 :public BossSpellCard
{
	class HuanYu_1 :public HuanYu
	{
	public:

		float angle;

		int frame;

		HuanYu_1() :frame(0), angle(0)
		{
			mb->drag = { 1,1,1 };
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (frame == 60)
			{
				mb->force = GetVector(angle, 60 + 5 * difficulty);
			}
		}
	};

	class GuangYuL_1 :public GuangYuL
	{
	public:

		int frame;

		float startAngle;

		GuangYuL_1(float _startAngle) :frame(0), startAngle(_startAngle)
		{
			cleanOnHit = false;
			SetColor(3);
			mb->velocity = { 0,-300,0 };
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (frame % (5 - difficulty / 2) == 0)
			{
				auto d = new HuanYu_1;
				d->angle = frame / 5.0f + startAngle;
				d->transform->location = transform->location;
				d->SetColor(5);
				scene->AttachActor(new EnemyBulletFog(d));
				se_tan00.Play();
			}
		}
	};

	class Shooter_1 :public Shooter
	{
	public:

		float x;

		float startAngle;

		Shooter_1() :x(304), startAngle(RandomAngle())
		{
		}

		void OnShoot() override
		{
			if (frame % 10 == 0)
			{
				auto d = new GuangYuL_1(startAngle);
				d->transform->location = { x,240,0 };
				scene->AttachActor(new EnemyBulletFog(d));
				if (abs(x) > 0.1f)
				{
					d = new GuangYuL_1(startAngle);
					d->transform->location = { -x,240,0 };
					scene->AttachActor(new EnemyBulletFog(d));
					x -= 608.0f / (5 + difficulty);
				}
			}
			const int desFrame[] = { 30,40,40,50 };
			if (difficulty >= 0 && difficulty <= 3)
			{
				if (frame == desFrame[difficulty])
					Destroy();
			}
			else
			{
				Destroy();
			}
		}
	};

public:

	Artia_SC_4() :BossSpellCard(1000, 36, 3400000, 11)
	{}

	void OnBossSpellStart() override
	{
		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		CreateLifeBar(0, 0.15, true, false);
		scene->AttachActor(new Artia_CDBG(this));
		scene->AttachActor(new CardCircle(this));
		scene->AttachActor(new SpellCardAttack);
		scene->AttachActor(new Artia_Sprite);
		scene->AttachActor(new SpellCardText(this, L"¶³·û¡¸±ùÍÁÏÂµÄÇ§Äê¿Ý¹Ç¡¹"));
	}

	void OnThink() override
	{
		if (frame == 1)
		{
			boss->ShowMaple({ 0.75,0.5,1,0 }, 1, true, false);
		}
		if (frame == 75)
		{
			scene->AttachActor(new Shooter_1);
		}
		if (frame >= 270 && frame <= 330 && frame % (3 - difficulty / 2) == 0)
		{
			auto d = new ZhongYu;
			d->SetColor(3);
			d->transform->location = { Random(-310.0f,310.0f),245,0 };
			d->mb->force = GetVector(-M_PI_2 + Random(-0.3f, 0.3f), Random(30.0f, 50.0f) + 5 * difficulty);
			d->mb->velocity = d->mb->force.GetNormalized() * (120 + 5 * difficulty);
			d->mb->drag = { 0.05,0.05,0.05 };
			scene->AttachActor(new EnemyBulletFog(d));
			se_tan01.Play();
		}
		if (frame >= 300 && frame <= 330 && frame % (6 - 2 * (difficulty / 2)) == 0)
		{
			auto d = new DaYu;
			d->SetColor(1);
			d->transform->location = { Random(-310.0f,310.0f),245,0 };
			d->mb->force = GetVector(-M_PI_2 + Random(-0.3f, 0.3f), Random(20.0f, 40.0f) + 5 * difficulty);
			d->mb->velocity = d->mb->force.GetNormalized() * (110 + 5 * difficulty);
			d->mb->drag = { 0.05,0.05,0.05 };
			scene->AttachActor(new EnemyBulletFog(d));
		}
		if (frame == 480)
			frame = 0;
	}

	void OnBossSpellEnd() override
	{
		se_enep02.Play();
		CleanAuto(CLEAN_SPELL, boss->transform->location);
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		DropItem<Power>(20, boss->transform->location, 70);
		DropItem<Point>(30, boss->transform->location, 70);
		DropItem<BombItem>(1, boss->transform->location, 0);

		ComPtr<Boss> tempBoss(boss);
		scene->AttachActor(new DelayInvoke(Delegate([tempBoss]()->void {tempBoss->AddComponent(new Artia_SC_5); }), 60));
	}
};

class Artia_SC_3 :public BossSpellCard
{
	class Shooter_3 :public Shooter
	{
	public:

		float angle;

		int type;

		Vector3 loc;

		Shooter_3(float _angle, int _type, const Vector3& _loc) :angle(_angle), type(_type), loc(_loc)
		{
		}

		void OnShoot() override
		{
			int f = 3 + difficulty / 2;
			ShootSnipe<ZhenDan>(3, loc, 240 - 2 * frame, angle + (frame / (f * 2.0f) - 0.5f) * (0.28f * f / 7) * (type == 0 ? 1 : -1), 0.28, 0, [=](ZhenDan* d)->void {
				d->SetColor(4);
				});
			if (frame == f * 2)
				Destroy();
		}
	};

	class Shooter_2 :public Shooter
	{
	public:

		LPGOBJ follow;

		Shooter_2(LPGOBJ _follow) :follow(_follow) {}

		void OnShoot() override
		{
			if (frame == 25)
			{
				float angle = GetAngle(player->transform->location - follow->transform->location);
				for (int i = 0; i < 6; i++)
					scene->AttachActor(new Shooter_3((float)(angle + i * M_PI * 2 / 6), 0, follow->transform->location));
				se_slash.Play();
			}
			if (frame == 50)
			{
				float angle = GetAngle(player->transform->location - follow->transform->location);
				for (int i = 0; i < 6; i++)
					scene->AttachActor(new Shooter_3((float)(angle + i * M_PI * 2 / 6), 1, follow->transform->location));
				se_slash.Play();
			}
			if (frame == 75)
			{
				float angle = GetAngle(player->transform->location - follow->transform->location);
				for (int i = 0; i < 6; i++)
					scene->AttachActor(new Shooter_3((float)(angle + i * M_PI * 2 / 6), 0, follow->transform->location));
				se_slash.Play();
			}
			if (frame == 100)
			{
				float angle = GetAngle(player->transform->location - follow->transform->location);
				for (int i = 0; i < 6; i++)
					scene->AttachActor(new Shooter_3((float)(angle + i * M_PI * 2 / 6), 1, follow->transform->location));
				se_slash.Play();
			}
			
			if (frame == 75 + 25 * (difficulty / 2))
				Destroy();
		}

		void OnDestroy() override
		{
			__super::OnDestroy();
			follow.Reset();
		}
	};

	class Shooter_1 :public Shooter
	{
	public:

		LPGOBJ follow;

		int type;

		Shooter_1(LPGOBJ _follow, int _type) :type(_type), follow(_follow)
		{
		}

		void OnShoot() override
		{
			transform->location = follow->transform->location;
			if (frame % 5 == 0)
			{
				ShootAround<ZhaDan>(3 + difficulty, transform->location, (120 + 5 * difficulty) * (type == 0 ? 0.85f : 1.15f), frame / (18.0f) * (type == 0 ? 1 : -1), 25, [=](ZhaDan* d)->void {
					d->SetColor(type == 0 ? 5 : 8);
					});
				se_tan00.Play();
			}
		}

		void OnDestroy() override
		{
			__super::OnDestroy();
			follow.Reset();
		}
	};

public:

	Artia_SC_3() :BossSpellCard(500, 40) {}

	void OnBossSpellStart() override
	{
		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		boss->ShowMaple({ 0.75,0.5,1,0 }, 1, true, false);
		CreateLifeBar(0.15, 1, false, true);
	}

	void OnThink() override
	{
		if (frame == 70)
		{
			scene->AttachActor(new Shooter_1(boss, 0));
			scene->AttachActor(new Shooter_1(boss, 1));
		}

		if (frame == 500)
		{
			boss->Move({ 120,90 });
		}

		if (frame == 1000)
		{
			boss->Move({ 50,120 });
		}

		if (frame == 1500)
		{
			boss->Move({ -90,80 });
		}

		if (frame == 2000)
		{
			boss->Move({ -30,135 });
		}

		if (frame % 250 == 0)
			scene->AttachActor(new Shooter_2(boss));
	}

	void OnBossSpellEnd() override
	{
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		CleanAuto(CLEAN_NOSPELL);
		boss->AddComponent(new Artia_SC_4);
	}
};

class Artia_SC_2 :public BossSpellCard
{
	class ZhenDan_1 :public ZhenDan
	{
	public:

		int frame;

		Vector3 normal;

		ZhenDan_1() :frame(0)
		{
			autoRotateMode = AR_NONE;
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (frame == 90)
			{
				mb->force = normal * 75;
			}
			else if (mb->velocity.GetLength() < 2 && frame < 90)
			{
				mb->velocity = { 0,0,0 };
				mb->force = { 0,0,0 };
			}
		}
	};

	class ZhenDan_2 :public ZhenDan
	{
	public:

		int frame;

		float angle;

		ZhenDan_2() :frame(0)
		{
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (frame == 300)
			{
				mb->drag = { 0.2,0.2,0.2 };
				mb->force = GetVector(angle + Random(-0.18f - 0.01f * difficulty, 0.18f + 0.01f * difficulty), 50 * (Random(0, 1) == 1 ? 1 : -1));
			}
		}
	};

	class Shooter_1 :public Shooter
	{
	public:

		Shooter_1()
		{
			cleanOnOutOfRange = false;
		}

		void OnShoot() override
		{
			if (frame <= 60)
			{
				int maxrow = 5 + 1 * (difficulty / 2);
				for (int i = 0; i < maxrow; i++)
				{
					auto d = new ZhenDan_2;
					d->angle = -M_PI_4;
					d->cleanOnHit = false;
					d->cleanOnBomb = false;
					d->transform->location = { -810 + frame * 500.0f / 60 + (i + 1) * 1120.0f / (maxrow + 1),250 - frame * 500.0f / 60,0 };
					d->mb->velocity = { 0.0001,-0.0001,0 };
					d->SetColor(5);
					scene->AttachActor(new EnemyBulletFog(d));
				}
				se_tan00.Play();
			}
			if (frame == 60)
				Destroy();
		}
	};

	class Shooter_2 :public Shooter
	{
	public:

		Shooter_2()
		{
			cleanOnOutOfRange = false;
		}

		void OnShoot() override
		{
			if (frame <= 60)
			{
				int maxrow = 5 + 1 * (difficulty / 2);
				for (int i = 0; i < maxrow; i++)
				{
					auto d = new ZhenDan_2;
					d->angle = -M_PI_4 * 3;
					d->cleanOnHit = false;
					d->cleanOnBomb = false;
					d->transform->location = { 810 - frame * 500.0f / 60 - (i + 1) * 1120.0f / (maxrow + 1),250 - frame * 500.0f / 60,0 };
					d->mb->velocity = { -0.0001,-0.0001,0 };
					d->SetColor(5);
					scene->AttachActor(new EnemyBulletFog(d));
				}
				se_tan00.Play();
			}
			if (frame == 60)
				Destroy();
		}
	};

public:

	Vector3 bossLoc[4];

	int moveStep;

	Artia_SC_2() :BossSpellCard(1300, 48, 4000000, 10),
		bossLoc{ { 0,100,0 }, { -50,75,0 }, { 88,89,0 }, { 64,127,0 } }, moveStep(0) {}

	void OnBossSpellStart() override
	{
		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		CreateLifeBar(0, 0.15, true, false);
		scene->AttachActor(new Artia_CDBG(this));
		scene->AttachActor(new CardCircle(this));
		scene->AttachActor(new SpellCardAttack);
		scene->AttachActor(new Artia_Sprite);
		scene->AttachActor(new SpellCardText(this, L"±ù·û¡¸±ùÄ§·¨·âÓ¡¡¹"));
	}

	void OnThink() override
	{
		if (frame == 1)
		{
			boss->ShowMaple({ 0.75,0.5,1,0 }, 1, true, false);
		}

		if (frame == 100)
		{
			for (int i = 0; i < 8 + 2 * difficulty; i++)
			{
				ShootAround<ZhenDan_1>(12 + 4 * difficulty, boss->transform->location, sqrt(4000 + 4000.0f * i), (float)(M_PI * 2 / (12 + 4.0 * difficulty) / 3.5 * i), 10, [=](ZhenDan_1* d)->void {
					d->normal = d->mb->velocity.GetNormalized();
					d->mb->force = d->normal * -150;
					d->transform->SetEulerAngles({ 0,0,atan2(d->mb->velocity.y, d->mb->velocity.x) - (float)M_PI_2 });
					d->SetColor(8);
					});
			}
			se_tan00.Play();
		}

		if (frame == 190)
		{
			scene->AttachActor(new Shooter_1());
			scene->AttachActor(new Shooter_2());
		}

		if (frame == 330)
		{
			for (int i = 0; i < 4 + difficulty; i++)
			{
				ShootAround<ZhaDan>(16 + 8 * difficulty, boss->transform->location - Vector3(50, 0, 0), sqrt((float)2500 + 500 * difficulty + (2500 + 500 * difficulty) * i), 0, 10, [=](ZhaDan* d)->void {
					d->outOfRangeTolerance = 80;
					d->mb->force = d->mb->velocity.GetNormalized() * (-135 + 5 * difficulty);
					d->transform->SetEulerAngles({ 0,0,atan2(d->mb->velocity.y, d->mb->velocity.x) - (float)M_PI_2 });
					d->SetColor(5);
					});
			}
			se_tan01.Play();
		}

		if (frame == 350)
		{
			for (int i = 0; i < 4 + difficulty; i++)
			{
				ShootAround<ZhaDan>(16 + 8 * difficulty, boss->transform->location - Vector3(0, 50, 0), sqrt(6000 + 6000.0f * i), 0, 10, [=](ZhaDan* d)->void {
					d->outOfRangeTolerance = 80;
					d->mb->force = d->mb->velocity.GetNormalized() * (-135 + 5 * difficulty);
					d->transform->SetEulerAngles({ 0,0,atan2(d->mb->velocity.y, d->mb->velocity.x) - (float)M_PI_2 });
					d->SetColor(5);
					});
			}
			se_tan01.Play();
		}

		if (frame == 370)
		{
			for (int i = 0; i < 4 + difficulty; i++)
			{
				ShootAround<ZhaDan>(16 + 8 * difficulty, boss->transform->location - Vector3(-50, 0, 0), sqrt(6000 + 6000.0f * i), 0, 10, [=](ZhaDan* d)->void {
					d->outOfRangeTolerance = 80;
					d->mb->force = d->mb->velocity.GetNormalized() * (-135 + 5 * difficulty);
					d->transform->SetEulerAngles({ 0,0,atan2(d->mb->velocity.y, d->mb->velocity.x) - (float)M_PI_2 });
					d->SetColor(5);
					});
			}
			se_tan01.Play();
		}

		if (frame == 490)
		{
			moveStep = (moveStep + 1) % 4;
			boss->Move(bossLoc[moveStep]);
		}

		if (frame == 800)
			frame = 0;
	}

	void OnBossSpellEnd() override
	{
		se_enep02.Play();
		CleanAuto(CLEAN_SPELL, boss->transform->location);
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		DropItem<Power>(20, boss->transform->location, 70);
		DropItem<Point>(30, boss->transform->location, 70);
		DropItem<BombItem>(1, boss->transform->location, 0);

		ComPtr<Boss> tempBoss(boss);
		scene->AttachActor(new DelayInvoke(Delegate([tempBoss]()->void {tempBoss->AddComponent(new Artia_SC_3); }), 60));
	}
};

class Artia_SC_1 :public BossSpellCard
{
	class Shooter_1 :public Shooter
	{
	public:

		int type;

		Shooter_1(int _type) :type(_type)
		{
		}

		void OnShoot() override
		{
			if (frame % 3 == 0)
			{
				ShootAround<ZhaDan>(3 + difficulty, transform->location, 100 + 5 * difficulty, frame / (12 + 2.0f * difficulty) * (type == 0 ? 1 : -1), 60 - frame, [=](ZhaDan* d)->void {
					d->SetColor(type == 0 ? 5 : 8);
					});
				se_tan00.Play();
			}
			if (frame == 50)
				Destroy();
		}
	};

public:

	Artia_SC_1() :BossSpellCard(450, 40) {}

	void OnBossSpellStart() override
	{
		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		boss->ShowMaple({ 0.75,0.5,1,0 }, 1, true, false);
		CreateLifeBar(0.15, 1, false, true);
	}

	void OnThink() override
	{
		if (frame == 40)
			boss->Move({ 0,100 });

		if (frame == 240)
			boss->Move({ 185,110 });

		if (frame == 440)
			boss->Move({ -50,80 });

		if (frame == 640)
			boss->Move({ -200,140 });

		if (frame % 200 == 100)
		{
			for (int i = 0; i < 3; i++)
			{
				auto st = scene->AttachActor(new Shooter_1(0));
				st->transform->location = boss->transform->location + GetVector(M_PI_2 + i * M_PI * 2 / 3, 75);
			}
		}

		if (frame % 200 == 180)
		{
			for (int i = 0; i < 3; i++)
			{
				auto st = scene->AttachActor(new Shooter_1(1));
				st->transform->location = boss->transform->location + GetVector(M_PI_2 * 3 + i * M_PI * 2 / 3, 75);
			}
		}

		if (frame == 800)
			frame = 0;
	}

	void OnBossSpellEnd() override
	{
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		CleanAuto(CLEAN_NOSPELL);
		boss->AddComponent(new Artia_SC_2);
	}
};

class Monstone_SC_9 :public BossSpellCard
{
	class XiaoYu_1 :public XiaoYu
	{
	public:

		int frame;

		XiaoYu_1():frame(0)
		{
			cleanOnBomb = false;
			cleanOnHit = false;
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (IsValid())
			{
				if (frame == 60)
				{
					float angle = RandomAngle();
					for (int i = 0; i < 3; i++)
					{
						auto d = new DianDan;
						d->SetColor(13);
						d->transform->location = transform->location;
						d->mb->velocity = GetVector(angle + i * M_PI * 2 / 3, 90 + 10 * difficulty);
						scene->AttachActor(new EnemyBulletFog(d));
					}
					se_kira00.Play();
					Destroy();
				}
			}
		}
	};

	template <class T>
	class ReboundBullet :public T
	{
		void OnUpdate() override
		{
			__super::OnUpdate();
			if (this->transform->location.x < -320)
				this->mb->velocity.x = abs(this->mb->velocity.x) / 2;
			if (this->transform->location.x > 320)
				this->mb->velocity.x = -abs(this->mb->velocity.x) / 2;
		}
	};

	class Shooter_1 :public Shooter
	{
	public:

		ComPtr<Boss> boss;
	
		Shooter_1(ComPtr<Boss> _boss) :boss(_boss) {}

		void OnShoot() override
		{
			if (frame < 90)
			{
				boss->transform->location.x = Lerp(boss->transform->location.x, player->transform->location.x, 0.09);
			}
			if (frame == 90)
			{
				boss->Move({ boss->transform->location.x ,120 }, 50);
				boss->ShowMaple({ 0.25,0.75,0.75,0 }, 1, false, false);
			}
			if (frame == 150)
			{
				boss->Move({ boss->transform->location.x ,-220 }, 700, 1400);
			}
			if (frame == 175)
			{
				for (int i = 0; i < 18 + 6 * difficulty; i++)
				{
					auto d = new ReboundBullet<DaYu>;
					d->SetColor(3);
					d->transform->location = boss->transform->location;
					d->mb->velocity = GetVector((float)M_PI_2 + Random(-0.3f, 0.3f), Random(300.f, 660 + 20.0f * difficulty));
					d->mb->force = { 0,-220 - 10.0f * difficulty,0 };
					d->mb->drag = { 0,0.25,0 };
					scene->AttachActor(new EnemyBulletFog(d));
				}
				for (int i = 0; i < 24 + 8 * difficulty; i++)
				{
					auto d = new ReboundBullet<XiaoYu>;
					d->SetColor(13);
					d->transform->location = boss->transform->location;
					d->mb->velocity = GetVector((float)M_PI_2 + Random(-0.6f, 0.6f), Random(280.f, 620 + 20.0f * difficulty));
					d->mb->force = { 0,-220 - 10.0f * difficulty,0 };
					d->mb->drag = { 0,0.25,0 };
					scene->AttachActor(new EnemyBulletFog(d));
				}
				for (int i = 0; i < 30 + 10 * difficulty; i++)
				{
					auto d = new ReboundBullet<MiDan>;
					d->SetColor(13);
					d->transform->location = boss->transform->location;
					d->mb->velocity = GetVector((float)M_PI_2 + Random(-0.9f, 0.9f), Random(260.f, 580 + 20.0f * difficulty));
					d->mb->force = { 0,-220 - 10.0f * difficulty,0 };
					d->mb->drag = { 0,0.25,0 };
					scene->AttachActor(new EnemyBulletFog(d));
				}
				se_tan00.Play();
				GameSceneCom::gamecam->Shake(30, 16);
			}
			if (frame == 240)
			{
				boss->Move({ boss->transform->location.x,100 }, 300, 150);
			}
			if (frame >= 240 && frame % 3 == 0)
			{
				for (int i = 0; i < difficulty + 1; i++)
				{
					auto d = new XiaoYu_1;
					d->transform->location = { Random(-320.0f,320.0f),240,0 };
					d->mb->velocity = { 0,Random(-200.0f,-75.0f),0 };
					d->SetColor(13);
					scene->AttachActor(new EnemyBulletFog(d));
				}
				se_tan00.Play();
			}
			if (frame == 300)
				Destroy();
		}

		void Destroy() override
		{
			__super::Destroy();
			boss.Reset();
		}
	};

public:

	Monstone_SC_9() :BossSpellCard(1300, 55, 4500000, 9, false) {}

	void OnBossSpellStart() override
	{
		isFinalSpell = true;
		boss->GetAttribution().tsRadius = 20;
		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		CreateLifeBar(0, 1, false, false);
		scene->AttachActor(new Monstone_CDBG(this));
		scene->AttachActor(new CardCircle(this));
		scene->AttachActor(new SpellCardAttack);
		scene->AttachActor(new Monstone_Sprite);
		scene->AttachActor(new SpellCardText(this, L"¾ÞÊ¯¡¸Ì©É½Ñ¹¶¥¡¹"));
	}

	void OnThink() override
	{
		if (frame == 75)
		{
			scene->AttachActor(new Shooter_1(boss));
		}
		if (frame == 400)
		{
			frame = 0;
		}
	}

	void OnBossFinalSpellDead() override
	{
		se_enep00.Play();
		CleanAuto(CLEAN_FINALSPELL, boss->transform->location);
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
	}

	void OnBossSpellEnd() override
	{
		se_enep01.Play();
		ShowEndDialog_Monstone();
	}
};

class Monstone_SC_8 :public BossSpellCard
{
	class Shadow :public GameObject
	{
		class Part :public GameObject
		{
		public:

			SpriteRender* sr;

			Material mat;

			LPGOBJ follow;

			Part(LPGOBJ _follow, float _alpha) :sr(AddComponent(new SpriteRender)), follow(_follow)
			{
				transform->location = follow->transform->location;
				transform->scale = follow->transform->scale;
				SpriteRender* fsr = follow->GetComponent<SpriteRender>();
				if (fsr)
				{
					mat = fsr->GetMaterial();
					mat.diffuse.a = _alpha;
					sr->SetTexture(fsr->GetTexture());
					sr->SetMaterial(mat);
					sr->SetLocalTransform(fsr->GetLocalTransform());
					sr->SetTextureTransform(fsr->GetTextureTransform());
				}
			}

			void OnCreated() override
			{
				InsertBefore(follow.Get());
			}

			void OnUpdate() override
			{
				mat.diffuse.a -= 0.02f;
				sr->SetMaterial(mat);
				if (mat.diffuse.a <= 0)
					Destroy();
			}

			void OnDestroy() override
			{
				follow.Reset();
			}
		};

	public:

		LPGOBJ follow;

		int interval;

		float alpha;

		int frame;

		Shadow(LPGOBJ _follow, int _interval, float _alpha) :
			follow(_follow), interval(_interval), alpha(_alpha), frame(0) {}

		void OnCreated() override
		{
			InsertBefore(follow.Get());
		}

		void OnUpdate() override
		{
			if (follow->IsValid())
			{
				frame++;
				if (frame % interval == 0)
				{
					scene->AttachActor(new Part(follow, alpha));
				}
			}
			else
			{
				Destroy();
			}
		}

		void OnDestroy() override
		{
			follow.Reset();
		}
	};

	class EnemyBullet_Monstone :public EnemyBullet
	{
	public:

		EnemyBullet_Monstone() :EnemyBullet(AR_NONE, false)
		{
			cleanOnHit = false;
			cleanOnBomb = false;
			cleanOnOutOfRange = false;
			breakSize = 0.7;
			breakColor = breakColor8;
			checkRadius = 7;
			transform->scale = { 64,64,1 };
			sr->SetTexture(src_monstone.Get());
			mat.diffuse.a = 0.75;
			sr->SetMaterial(mat);
		}

		void SetColor(int index) override
		{
			colorIndex = index;
			sr->SetSourceRect({ 0,0,1 / 3.0f,1 });
		}

		void Clean(BYTE cleanType) override
		{
			if (!IsValid())
				return;
			if (cleanOnHit && cleanType == CLEAN_HIT)
			{
				Destroy();
			}
			else if (cleanOnOutOfRange && cleanType == CLEAN_OUTOFRANGE)
			{
				Destroy();
			}
			else if (cleanOnBomb && cleanType == CLEAN_BOMB)
			{
				Destroy();
			}
			else if (cleanType == CLEAN_BONUS)
			{
				Destroy();
			}
		}
	};

	template <class T>
	class ShadowedObject :public T
	{
		void OnCreated() override
		{
			__super::OnCreated();
			player->GetScene()->AttachActor(new Shadow(this, 2, 0.5));
		}
	};

	class EnemyBullet_Monstone_1 :public EnemyBullet_Monstone
	{
	public:

		int frame;

		LPGOBJ follow;

		float length;

		float angle;

		float targetLength;

		int type;

		Vector3 playerLoc;

		EnemyBullet_Monstone_1(LPGOBJ _follow, int _type) :
			frame(0), follow(_follow), length(0), angle(0), targetLength(100), type(_type)
		{
			switch (type)
			{
			case 0:
				mat.diffuse = { 1,1,0.5,0.75 };
				break;
			case 1:
				mat.diffuse = { 1,0.5,0.5,0.75 };
				break;
			case 2:
				mat.diffuse = { 0.5,1,1,0.75 };
				break;
			case 3:
				mat.diffuse = { 0.25,0.25,1,0.75 };
				break;
			}
			sr->SetMaterial(mat);
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			length = Lerp(length, targetLength, 0.02f);
			angle += 0.04f * (length / targetLength);
			transform->location = follow->transform->location + GetVector(angle, length);
			if (frame == 900)
				targetLength = 150;
			if (frame >= 1800)
			{
				targetLength = 200 + 20 * sin((frame - 1800) / 20.0f);
			}

			if (frame > 60)
			{
				switch (type)
				{
				case 0:
					if (frame % 60 == 0)
					{
						ShootAround<ShadowedObject<MiDan>>(6 + 3 * difficulty, transform->location, (0.7f + 0.1f * difficulty) * 150, RandomAngle(), 10, [&](MiDan* d)->void {
							d->SetColor(13);
							});
						se_tan00.Play();
					}
					break;
				case 1:
					if ((frame / 30) % 2 == 0 && frame % 2 == 0 && (frame % 30 <= 12 + 6 * difficulty))
					{
						if (frame % 30 == 0)
						{
							playerLoc = player->transform->location;
							se_slash.Play();
						}
						ShootSnipe<ShadowedObject<XiaoYu>>(1, transform->location, (0.7f + 0.1f * difficulty) * 200, GetAngle(playerLoc - transform->location), 0, 0, [&](XiaoYu* d)->void {
							d->SetColor(1);
							});
					}
					break;
				case 2:
					if (frame % (7 - difficulty) == 0)
					{
						ShootSnipe<ShadowedObject<MiDan>>(1, transform->location, (0.7f + 0.1f * difficulty) * 200, GetAngle(follow->transform->location - transform->location), 0, 0, [&](MiDan* d)->void {
							d->SetColor(8);
							});
						se_tan01.Play();
					}
					break;
				case 3:
					if (frame % 90 == 0)
					{
						ShootAround<ShadowedObject<DaYu>>(4 + 1 * difficulty, transform->location, (0.7f + 0.1f * difficulty) * 120, RandomAngle(), 10, [&](DaYu* d)->void {
							d->SetColor(1);
							});
						se_kira00.Play();
					}
					break;
				}
			}
		}

		void OnCreated() override
		{
			__super::OnCreated();
			scene->AttachActor(new Shadow(this, 2, 0.5));
		}

		void OnDestroy() override
		{
			__super::OnDestroy();
			follow.Reset();
		}
	};

public:

	LPGOBJ playerShadow;

	LPGOBJ bossShadow;

	Monstone_SC_8() :BossSpellCard(1500, 40, 4000000, 8, true)
	{}

	void OnBossSpellStart() override
	{
		playerShadow = scene->AttachActor(new Shadow(player, 2, 0.5));
		bossShadow = scene->AttachActor(new Shadow(boss, 2, 0.5));
		boss->Move({ 0,100 });
		boss->SetCharacterFlags(boss->GetCharacterFlags() | CF_ImmuneDamage);
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		CreateLifeBar(0, 0.15, true, false);
		scene->AttachActor(new Monstone_CDBG(this));
		scene->AttachActor(new CardCircle(this));
		scene->AttachActor(new SpellCardAttack);
		scene->AttachActor(new Monstone_Sprite);
		scene->AttachActor(new SpellCardText(this, L"»ÃÊ¯¡¸¶àÖØ»ÃÓ°¡¹"));
	}

	void OnThink() override
	{
		if (frame == 1)
		{
			boss->ShowMaple({ 0.25,0.75,0.75,0 }, 1, true, false);
		}

		if (frame == 75)
		{
			for (int i = 0; i < 4; i++)
				scene->AttachActor(new EnemyBullet_Monstone_1(boss, i))->angle = M_PI_2 * i;
		}

		if (frame == 900)
		{
			boss->Move({ 0,50 });
		}

		if (frame > 1800)
		{
			boss->transform->location = { fmin((frame - 1800) / 1.5f,60.0f) * sin((frame - 1800) / 25.0f),50,0 };
		}
	}

	void OnBossSpellEnd() override
	{
		playerShadow->Destroy();
		playerShadow.Reset();
		bossShadow->Destroy();
		bossShadow.Reset();

		se_enep02.Play();
		CleanAuto(CLEAN_SPELL, boss->transform->location);
		boss->SetCharacterFlags(boss->GetCharacterFlags() & ~CF_ImmuneDamage);
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		DropItem<Power>(20, boss->transform->location, 70);
		DropItem<Point>(30, boss->transform->location, 70);
		DropItem<BombItem>(1, boss->transform->location, 0);

		ComPtr<Boss> tempBoss(boss);
		scene->AttachActor(new DelayInvoke(Delegate([tempBoss]()->void {tempBoss->AddComponent(new Monstone_SC_9); }), 60));
	}
};

class Monstone_SC_7 :public BossSpellCard
{
	class MiDan_1 :public MiDan
	{
	public:

		int frame;

		float angle;

		int type;

		float force;

		MiDan_1(int _type) :frame(0), angle(0), type(_type), force(0)
		{
			mb->drag = { 0.5,0.5,0.5 };
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (frame < 60)
			{
				mb->velocity *= 0.95;
			}
			if (frame == 60)
			{
				if (type == 0)
				{
					mb->force = GetVector(angle + (float)M_PI / 4.0f, (0.7f + 0.1f * difficulty) * force);
					mb->velocity = GetVector(angle, (0.7f + 0.1f * difficulty) * force);
				}
				else
				{
					mb->force = GetVector(angle - (float)M_PI / 4.0f, (0.7f + 0.1f * difficulty) * force);
					mb->velocity = GetVector(angle, (0.7f + 0.1f * difficulty) * force);
				}
			}
		}
	};

	class Shooter_1 :public Shooter
	{
	public:

		LPGOBJ follow;

		float angle;

		Shooter_1(LPGOBJ _follow) :follow(_follow), angle(0) {}

		void OnShoot() override
		{
			transform->location = follow->transform->location;
			angle += 0.04f;
			if (frame % (7 - difficulty) == 0)
			{
				ShootAround<MiDan_1, int>(0, 6, transform->location, 500, -angle, 10, [&](MiDan_1* d)->void {
					d->SetColor(6);
					d->angle = GetAngle(d->mb->velocity);
					d->force = 50 + 3 * (frame % 30);
					});
				ShootAround<MiDan_1, int>(1, 6, transform->location, 500, angle, 10, [&](MiDan_1* d)->void {
					d->SetColor(13);
					d->angle = GetAngle(d->mb->velocity);
					d->force = 50 + 3 * (frame % 30);
					});
				se_tan00.Play();
			}

			if (frame == 200)
				Destroy();
		}

		void OnDestroy() override
		{
			__super::OnDestroy();
			follow.Reset();
		}
	};

public:

	Vector3 bossLoc[4];

	int moveStep;

	Monstone_SC_7() :BossSpellCard(560, 44),
		bossLoc{ {0,100,0},{50,108,0},{20,124,0},{-40,103,0} }, moveStep(0){}

	void OnBossSpellStart() override
	{
		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		boss->ShowMaple({ 0.25,0.75,0.75,0 }, 1, true, false);
		CreateLifeBar(0.15, 1, false, true);
	}

	void OnThink() override
	{
		if (frame == 75)
			scene->AttachActor(new Shooter_1(boss));

		if (frame == 280)
		{
			moveStep = (moveStep + 1) % 4;
			boss->Move(bossLoc[moveStep]);
			frame = 0;
		}
	}

	void OnBossSpellEnd() override
	{
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		CleanAuto(CLEAN_NOSPELL);
		boss->AddComponent(new Monstone_SC_8);
	}
};

class Monstone_SC_6 :public BossSpellCard
{
	class XiaoYu_1 :public XiaoYu
	{
	public:

		int frame;

		XiaoYu_1() :frame(0) {}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (frame == 200)
			{
				Clean(CLEAN_HIT);
			}
		}
	};

	class Shooter_1 :public Shooter
	{
		void OnShoot() override
		{
			if (frame % 2 == 0)
			{
				if((player->transform->location - transform->location).GetLength() > 20)
				{
					auto d = new XiaoYu_1;
					d->SetColor(8);
					d->transform->location = transform->location + GetVector(RandomAngle(), Random(0.0f, 30.0f));
					scene->AttachActor(new EnemyBulletFog(d));
				}
				se_tan00.Play();
			}
			if (IsOutOfRange(transform->location, 10))
				Destroy();
		}
	};

	class Shooter_2 :public Shooter
	{
		void OnShoot() override
		{
			if (frame % 2 == 0)
			{
				float angle = GetAngle(player->transform->location - transform->location);
				auto d = new XiaoYu;
				d->SetColor(13);
				d->transform->location = transform->location;
				d->mb->velocity = GetVector(angle + frame / 60.0f * Random(-0.3f, 0.3f), 20 * difficulty + 180 - frame);
				d->mb->force = GetVector(angle, 50);
				d->mb->drag = { 0.1,0.1,0.1 };
				scene->AttachActor(new EnemyBulletFog(d));
				se_tan00.Play();
			}
			if (frame == 54 + 12 * difficulty)
				Destroy();
		}
	};

	class ZhongYu_1 :public ZhongYu
	{
	public:

		ZhongYu_1()
		{
			cleanOnHit = false;
			cleanOnBomb = false;
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			if (IsValid())
			{
				if (transform->location.y > 240)
				{
					scene->AttachActor(new Shooter_2)->transform->location = transform->location;
					Destroy();
				}
			}
		}
	};

	class ZhongYu_2 :public ZhongYu
	{
	public:

	public:

		int frame;

		ZhongYu_2() :frame(0)
		{
			cleanOnHit = false;
			cleanOnBomb = false;
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			mb->velocity *= 0.95f;
			if (frame == 90)
			{
				for (int i = 0; i < 12 + 6 * difficulty; i++)
				{
					auto d = new MiDan;
					d->outOfRangeTolerance = 70;
					d->SetColor(13);
					d->transform->location = transform->location;
					d->mb->velocity = GetVector((float)M_PI_2 + Random(-0.5f, 0.5f), Random(100.f, 240.0f));
					d->mb->force = { 0,-120 - 20.0f * difficulty,0 };
					d->mb->drag = { 0,0.25,0 };
					scene->AttachActor(new EnemyBulletFog(d));
				}
				se_kira00.Play();
				Destroy();
			}
		}
	};

	class Shooter_3 :public Shooter
	{
	public:

		float offset;

		int type;

		Shooter_3(int _type) :offset(0), type(_type) {}

		void OnShoot() override
		{
			if (frame % 10 == 1)
			{
				auto d = new ZhongYu_2;
				d->SetColor(6);
				d->transform->location = transform->location;
				if (type == 0)
					d->mb->velocity = GetVector(offset + (float)(M_PI / 8 + (frame / 10) * 3 * M_PI / 16), 350);
				else
					d->mb->velocity = GetVector(offset + (float)(M_PI / 8 + (4 - frame / 10) * 3 * M_PI / 16), 350);
				scene->AttachActor(new EnemyBulletFog(d));
				se_tan01.Play();
			}
			if (frame == 41)
				Destroy();
		}
	};

public:

	Monstone_SC_6() :BossSpellCard(1100, 48, 3600000, 7)
	{}

	void OnBossSpellStart() override
	{
		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		CreateLifeBar(0, 0.15, true, false);
		scene->AttachActor(new Monstone_CDBG(this));
		scene->AttachActor(new CardCircle(this));
		scene->AttachActor(new SpellCardAttack);
		scene->AttachActor(new Monstone_Sprite);
		scene->AttachActor(new SpellCardText(this, L"ÂÒÊ¯¡¸ÆÆ¿Õ¡¹"));
	}

	void OnThink() override
	{
		if (frame == 1)
		{
			boss->ShowMaple({ 0.25,0.75,0.75,0 }, 1, true, false);
		}

		if (frame == 80)
		{
			for (int i = 0; i < 3; i++)
			{
				auto d = new ZhongYu_1;
				d->transform->location = boss->transform->location;
				d->SetColor(6);
				d->mb->velocity = { -150 + 120.0f * i,20,0 };
				d->mb->force = { 0,150,0 };
				scene->AttachActor(new EnemyBulletFog(d));
			}
			se_tan00.Play();
		}

		if (frame == 260)
		{
			for (int i = 0; i < 6 + 2 * difficulty; i++)
			{
				auto st = scene->AttachActor(new Shooter_1);
				st->transform->location = boss->transform->location;
				st->mb->velocity = GetVector(M_PI * 2 * i / (6 + 2 * difficulty), 300);
			}
		}

		if (frame == 280)
		{
			boss->Move({ 80,50});
		}

		if (frame == 400)
		{
			auto st = scene->AttachActor(new Shooter_3(1));
			st->transform->location = boss->transform->location;
			st->offset = 0.3;
		}

		if (frame == 600)
		{
			for (int i = 0; i < 3; i++)
			{
				auto d = new ZhongYu_1;
				d->transform->location = boss->transform->location;
				d->SetColor(6);
				d->mb->velocity = { -150 + 120.0f * i,20,0 };
				d->mb->force = { 0,150,0 };
				scene->AttachActor(new EnemyBulletFog(d));
			}
			se_tan00.Play();
		}

		if (frame == 780)
		{
			for (int i = 0; i < 6 + 2 * difficulty; i++)
			{
				auto st = scene->AttachActor(new Shooter_1);
				st->transform->location = boss->transform->location;
				st->mb->velocity = GetVector(M_PI * 2 * i / (6 + 2 * difficulty), 300);
			}
		}

		if (frame == 800)
		{
			boss->Move({ -35,70 });
		}

		if (frame == 920)
		{
			auto st = scene->AttachActor(new Shooter_3(0));
			st->transform->location = boss->transform->location;
			st->offset = -0.3;
		}

		if (frame == 1000)
		{
			boss->Move({ 0,100 });
		}

		if (frame == 1060)
		{
			frame = 0;
		}
	}

	void OnBossSpellEnd() override
	{
		se_enep02.Play();
		CleanAuto(CLEAN_SPELL, boss->transform->location);
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		DropItem<Power>(20, boss->transform->location, 70);
		DropItem<Point>(30, boss->transform->location, 70);
		DropItem<BombItem>(1, boss->transform->location, 0);

		ComPtr<Boss> tempBoss(boss);
		scene->AttachActor(new DelayInvoke(Delegate([tempBoss]()->void {tempBoss->AddComponent(new Monstone_SC_7); }), 60));
	}
};

class Monstone_SC_5 :public BossSpellCard
{
	class Shooter_1 :public Shooter
	{
	public:

		float startrad;

		LPGOBJ follow;

		Shooter_1(LPGOBJ _follow) :startrad(0), follow(_follow) {}

		void OnShoot() override
		{
			transform->location = follow->transform->location;
			if (frame % (27 - 3 * difficulty) == 0)
			{
				for (int i = 0; i < 4; i++)
				{
					ShootAround<MiDan>(8 + difficulty, transform->location, (0.7f + 0.1f * difficulty) * (200 - 5 * i), startrad - 0.02f * i, 10, [=](MiDan* d)->void {
						d->SetColor(13);
						});
					ShootAround<MiDan>(8 + difficulty, transform->location, (0.7f + 0.1f * difficulty) * (200 - 5 * i), startrad + 0.02f * i, 10, [=](MiDan* d)->void {
						d->SetColor(13);
						});
				}
				startrad += 0.01f * (27 - 3 * difficulty);
				se_tan00.Play();
			}

			if (frame % (81 - 9 * difficulty) == 0)
			{
				for (int i = 0; i < 24 + 12 * difficulty; i++)
				{
					float angle = M_PI * 2 * i / (24 + 12 * difficulty);
					auto d = new MiDan;
					d->transform->location = transform->location + GetVector(angle + startrad, 80);
					d->SetColor(6);
					d->mb->velocity = GetVector(angle + startrad, (0.7f + 0.1f * difficulty) * (60 + 150 * pow(abs((fmod(fmod(angle, M_PI * 2), (M_PI / 2)) / (M_PI / 2)) - 0.5), 2)));
					scene->AttachActor(new EnemyBulletFog(d));
				}
			}
		}

		void OnDestroy() override
		{
			__super::OnDestroy();
			follow.Reset();
		}
	};

public:

	Monstone_SC_5() :BossSpellCard(540, 45) {}

	void OnBossSpellStart() override
	{
		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		boss->ShowMaple({ 0.25,0.75,0.75,0 }, 1, true, false);
		CreateLifeBar(0.15, 1, false, true);
	}

	void OnThink() override
	{
		if (frame == 75)
		{
			scene->AttachActor(new Shooter_1(boss));
		}
	}

	void OnBossSpellEnd() override
	{
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		CleanAuto(CLEAN_NOSPELL);
		boss->AddComponent(new Monstone_SC_6);
	}
};

class Monstone_SC_4 :public BossSpellCard
{
	class XiaoYu_1 :public XiaoYu
	{
	public:

		float angle;

		int frame;

		XiaoYu_1() :frame(0), angle(0) {}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (frame == 60)
			{
				mb->drag = { 0.4,0.4,0.4 };
				mb->force = GetVector(angle, 50);
			}
		}
	};

	class Shooter_1 :public Shooter
	{
	public:

		Shooter_1()
		{
			mb->drag = { 0.5,0.5,0.5 };
		}

		void OnShoot() override
		{
			mb->force = GetVector(GetAngle(mb->velocity) + M_PI / 4.0f, 300);
			if (frame % (6 - difficulty) == 0)
			{
				if ((transform->location - player->transform->location).GetLength() > 20)
				{
					auto d = new XiaoYu_1;
					d->transform->location = transform->location;
					d->SetColor(13);
					d->angle = frame / 9.0f + GetAngle(mb->velocity);
					scene->AttachActor(new EnemyBulletFog(d));
				}
				se_tan00.Play();
			}

			if (IsOutOfRange(transform->location, 10))
				Destroy();
		}
	};

public:

	Monstone_SC_4() :BossSpellCard(760, 44, 3200000, 6)
	{}

	void OnBossSpellStart() override
	{
		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		CreateLifeBar(0, 0.15, true, false);
		scene->AttachActor(new Monstone_CDBG(this));
		scene->AttachActor(new CardCircle(this));
		scene->AttachActor(new SpellCardAttack);
		scene->AttachActor(new Monstone_Sprite);
		scene->AttachActor(new SpellCardText(this, L"Á¦·û¡¸ÂùÊ¯¹ÖÁ¦¡¹"));
	}

	void OnThink() override
	{
		if (frame == 1)
		{
			boss->Move({ 0,0 });
			boss->ShowMaple({ 0.25,0.75,0.75,0 }, 1, true, false);
		}

		if (frame == 100)
		{
			float startangle = RandomAngle();
			for (int i = 0; i < 6 + difficulty; i++)
			{
				auto st = scene->AttachActor(new Shooter_1);
				st->transform->location = boss->transform->location;
				st->mb->velocity = GetVector(startangle + M_PI * 2 * i / (6 + difficulty), 250);
			}
		}

		if (frame == 360)
			frame = 0;
	}

	void OnBossSpellEnd() override
	{
		se_enep02.Play();
		CleanAuto(CLEAN_SPELL, boss->transform->location);
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		DropItem<Power>(20, boss->transform->location, 70);
		DropItem<Point>(30, boss->transform->location, 70);
		DropItem<LifeItem>(1, boss->transform->location, 0);

		ComPtr<Boss> tempBoss(boss);
		scene->AttachActor(new DelayInvoke(Delegate([tempBoss]()->void {tempBoss->AddComponent(new Monstone_SC_5); }), 60));
	}
};

class Monstone_SC_3 :public BossSpellCard
{
	class ZhongYu_1 :public ZhongYu
	{
	public:

		int frame;

		ZhongYu_1() :frame(0)
		{
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (frame == 22 + difficulty)
				mb->velocity = mb->velocity.GetNormalized() * 75;
		}
	};

	class Shooter_1 :public Shooter
	{
	public:

		LPGOBJ follow;

		float angle;

		Shooter_1(LPGOBJ _follow) :follow(_follow), angle(0) {}

		void OnShoot() override
		{
			transform->location = GetVector(angle, 90) + follow->transform->location;
			if (frame % (6 - difficulty) == 0)
			{
				auto d = new MiDan;
				d->transform->location = transform->location;
				d->SetColor(6);
				d->mb->velocity = GetVector(angle - M_PI_2, 120);
				scene->AttachActor(new EnemyBulletFog(d));
				d = new MiDan;
				d->transform->location = transform->location;
				d->SetColor(13);
				d->mb->velocity = GetVector(angle - M_PI_2 - 0.1f, 100);
				scene->AttachActor(new EnemyBulletFog(d));
				se_tan00.Play();
				angle += 0.05f * (6 - difficulty);
			}
		}

		void OnDestroy() override
		{
			__super::OnDestroy();
			follow.Reset();
		}
	};

public:

	Vector3 bossLoc[4];

	int moveStep;

	Monstone_SC_3() :BossSpellCard(490, 39),
		bossLoc{ {0,100,0},{35,108,0},{12,94,0},{-20,103,0} }, moveStep(0){}

	void OnBossSpellStart() override
	{
		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		boss->ShowMaple({ 0.25,0.75,0.75,0 }, 1, true, false);
		CreateLifeBar(0.15, 1, false, true);
	}

	void OnThink() override
	{
		if (frame == 75)
		{
			scene->AttachActor(new Shooter_1(boss))->angle = 0;
			scene->AttachActor(new Shooter_1(boss))->angle = M_PI;
		}

		if (frame % 200 == 0)
		{
			moveStep = (moveStep + 1) % 4;
			boss->Move(bossLoc[moveStep]);
		}

		if (frame % 50 == 0 && frame > 80)
		{
			ShootAround<ZhongYu_1>(12 + 4 * difficulty, boss->transform->location, 500, RandomAngle(), 10, [=](ZhongYu* d)->void {
				d->SetColor(6);
				});
			se_tan01.Play();
		}
	}

	void OnBossSpellEnd() override
	{
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		CleanAuto(CLEAN_NOSPELL);
		boss->AddComponent(new Monstone_SC_4);
	}
};

class Monstone_SC_2 :public BossSpellCard
{
	class XiaoYu_1 :public XiaoYu
	{
	public:

		int frame;

		XiaoYu_1() :frame(0) 
		{
			cleanOnHit = false;
			cleanOnBomb = false;
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			mb->velocity *= 0.96;
			frame++;
			if (frame == 60)
			{
				ShootAround<DianDan>(6 + difficulty, transform->location, 120, RandomAngle(), 10, [=](DianDan* d)->void {
					d->SetColor(13);
					});
				se_kira00.Play();
				Destroy();
			}
		}
	};

	class ZhongYu_1 :public ZhongYu
	{
	public:

		int frame;

		ZhongYu_1() :frame(0) 
		{
			cleanOnHit = false;
			cleanOnBomb = false;
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			mb->velocity *= 0.96;
			frame++;
			if (frame == 60)
			{
				ShootAround<XiaoYu_1>(6 + difficulty, transform->location, 200, RandomAngle(), 10, [=](XiaoYu_1* d)->void {
					d->SetColor(13);
					});
				se_kira00.Play();
				Destroy();
			}
		}
	};

public:

	Vector3 bossLoc[7];

	int moveStep;

	Monstone_SC_2() :BossSpellCard(950, 40, 3200000, 5),
		bossLoc{ { 0,100,0 }, { 50,118,0 }, { 104,108,0 }, { 64,100,0 },{-29,110,0},{-130,105,0},{-71,130,0} }, moveStep(0) {}

	void OnBossSpellStart() override
	{
		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		CreateLifeBar(0, 0.15, true, false);
		scene->AttachActor(new Monstone_CDBG(this));
		scene->AttachActor(new CardCircle(this));
		scene->AttachActor(new SpellCardAttack);
		scene->AttachActor(new Monstone_Sprite);
		scene->AttachActor(new SpellCardText(this, L"Ê¯·û¡¸Ð¡ËéÊ¯¡¹"));
	}

	void OnThink() override
	{
		if (frame == 1)
		{
			boss->ShowMaple({ 0.25,0.75,0.75,0 }, 1, true, false);
		}

		if (frame == 75)
		{
			ShootAround<ZhongYu_1>(6 + difficulty, boss->transform->location, 200, RandomAngle(), 10, [=](ZhongYu_1* d)->void {
				d->SetColor(6);
				});
			se_tan00.Play();
		}

		if (frame == 280)
		{
			moveStep = (moveStep + 1) % 7;
			boss->Move(bossLoc[moveStep]);
		}

		if (frame == 360)
		{
			frame = 0;
		}
	}

	void OnBossSpellEnd() override
	{
		se_enep02.Play();
		CleanAuto(CLEAN_SPELL, boss->transform->location);
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		DropItem<Power>(20, boss->transform->location, 70);
		DropItem<Point>(30, boss->transform->location, 70);
		DropItem<BombItem>(1, boss->transform->location, 0);

		ComPtr<Boss> tempBoss(boss);
		scene->AttachActor(new DelayInvoke(Delegate([tempBoss]()->void {tempBoss->AddComponent(new Monstone_SC_3); }), 60));
	}
};

class Monstone_SC_1 :public BossSpellCard
{
	class MiDan_1 :public MiDan
	{
	public:

		int frame;

		int type;

		MiDan_1(int _type) :frame(0), type(_type) {}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (frame == 45)
			{
				mb->velocity = GetVector(GetAngle(mb->velocity) + 0.5f * (type == 0 ? 1 : -1), 70 + 10 * difficulty);
			}
		}
	};

	class Shooter_1 :public Shooter
	{
	public:

		int type;

		float angle;

		LPGOBJ follow;

		Shooter_1(LPGOBJ _follow, int _type) :type(_type), follow(_follow)
		{
		}

		void OnShoot() override
		{
			transform->location = follow->transform->location;
			if (frame % (3 - difficulty % 2) == 0)
			{
				auto d = new MiDan_1(type);
				d->transform->location = transform->location + GetVector(angle, 30);
				d->SetColor(6);
				d->mb->velocity = GetVector(angle, 300);
				scene->AttachActor(new EnemyBulletFog(d));
				angle += (0.11f - 0.01 * difficulty) * (difficulty % 2 == 0 ? 1.5f : 1.0f);
				se_tan00.Play();
			}
		}

		void OnDestroy() override
		{
			__super::OnDestroy();
			follow.Reset();
		}
	};

public:

	Vector3 bossLoc[4];

	int moveStep;

	Monstone_SC_1() :BossSpellCard(420, 36),
		bossLoc{ {0,100,0},{35,108,0},{12,94,0},{-20,103,0} }, moveStep(0){}

	void OnBossSpellStart() override
	{
		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		boss->ShowMaple({ 0.25,0.75,0.75,0 }, 1, true, false);
		CreateLifeBar(0.15, 1, false, true);
	}

	void OnThink() override
	{
		if (frame == 75)
		{
			if (difficulty < 2)
			{
				scene->AttachActor(new Shooter_1(boss, 1))->angle = 0;
				scene->AttachActor(new Shooter_1(boss, 0))->angle = M_PI;
			}
			else
			{
				scene->AttachActor(new Shooter_1(boss, 1))->angle = 0;
				scene->AttachActor(new Shooter_1(boss, 1))->angle = M_PI;
				scene->AttachActor(new Shooter_1(boss, 0))->angle = M_PI / 2;
				scene->AttachActor(new Shooter_1(boss, 0))->angle = M_PI * 3 / 2;
			}
		}

		if (frame % 200 == 0)
		{
			moveStep = (moveStep + 1) % 4;
			boss->Move(bossLoc[moveStep]);
		}
	}

	void OnBossSpellEnd() override
	{
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		CleanAuto(CLEAN_NOSPELL);
		boss->AddComponent(new Monstone_SC_2);
	}
};

class SunnyMilk_SC_7 :public BossSpellCard
{
	class Shooter_1 :public Shooter
	{
	public:

		int type;

		float angle;

		Shooter_1(int _type):type(_type)
		{
		}

		void OnShoot() override
		{
			if (frame == 1)
			{
				for (int i = 0; i < 20; i++)
				{
					auto d = new YanDan;
					d->SetColor(0);
					d->transform->location = transform->location;
					d->mb->velocity = GetVector(angle + Random(-0.05f, 0.05f), (400.0f + 20.0f * difficulty) * ((float)i / 20));
					d->mb->force = GetVector(angle, -300);
					d->mb->drag = { 0.2,0.2,0.2 };
					scene->AttachActor(new EnemyBulletFog(d));
				}
			}
			if (frame == 3)
			{
				Vector3 target = transform->location + GetVector(angle + M_PI_2 * (type == 0 ? 1 : -1), 20);
				if (!IsOutOfRange(target, 10))
				{
					auto st = scene->AttachActor(new Shooter_1(type));
					st->transform->location = target;
					st->angle = angle;
				}
				Destroy();
			}
		}
	};

	class Shooter_2 :public Shooter
	{
	public:

		LPGOBJ follow;

		Shooter_2(LPGOBJ _follow) :follow(_follow) {}

		void OnShoot() override
		{
			transform->location = follow->transform->location;
			if (frame % 5 == 0)
			{
				const int dnum = 24;
				float dangle = (float)M_PI * 4 / 3 / (dnum - 1);
				for (int i = 0; i < dnum; i++)
				{
					auto d = new ZhenDan;
					d->transform->location = transform->location;
					d->SetColor(13);
					d->mb->velocity = GetVector(dangle * i - (float)M_PI / 6 + (0.25f + 0.05f * difficulty) * sin(frame / 20.0f), 150);
					scene->AttachActor(new EnemyBulletFog(d));
				}
				se_tan00.Play();
			}
		}

		void OnDestroy() override
		{
			__super::OnDestroy();
			follow.Reset();
		}
	};

	class Shooter_3 :public Shooter
	{
	public:

		LPGOBJ follow;

		Shooter_3(LPGOBJ _follow) :follow(_follow) {}

		void OnShoot() override
		{
			transform->location = follow->transform->location;
			if (frame % 60 == 0)
			{
				ShootAround<DaYu>(6 + 3 * difficulty, transform->location, 100, RandomAngle(), 10, [=](DaYu* d)->void {
					d->SetColor(0);
					});
				se_kira00.Play();
			}
		}

		void OnDestroy() override
		{
			__super::OnDestroy();
			follow.Reset();
		}
	};

	class Laser_1 :public Laser
	{
	public:

		int frame;

		Laser_1() :Laser(), frame(0)
		{
			width = 2;
			length = 1000;
			angle = -M_PI_2;
			checking = false;
			SetColor(1);
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (frame == 40)
			{
				auto st = scene->AttachActor(new Shooter_1(0));
				st->transform->location = { transform->location.x - 10,-240,0 };
				st->angle = M_PI_2;
				st = scene->AttachActor(new Shooter_1(1));
				st->transform->location = { transform->location.x + 10,-240,0 };
				st->angle = M_PI_2;
			}
			if (frame > 40 && frame <= 60)
			{
				width += 1.5f;
			}
			if (frame == 60)
			{
				checking = true;
			}
			if (frame == 120)
			{
				checking = false;
			}
			if (frame > 120)
			{
				width -= 1.5f;
			}
			if (frame == 140)
			{
				Destroy();
			}
		}
	};

public:

	Vector3 bossLoc[6];

	int moveStep;

	SunnyMilk_SC_7() :BossSpellCard(1500, 60, 4000000, 4, false),
		bossLoc{ {0,100,0},{-44,100,0},{-20,100,0},{26,100,0},{61,100,0},{16,100,0} }, moveStep(0){}

	void OnBossSpellStart() override
	{
		isFinalSpell = true;
		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		CreateLifeBar(0, 1, false, false);
		scene->AttachActor(new SunnyMilk_CDBG(this));
		scene->AttachActor(new CardCircle(this));
		scene->AttachActor(new SpellCardAttack);
		scene->AttachActor(new SunnyMilk_Sprite);
		scene->AttachActor(new SpellCardText(this, L"µØÑ×¡¸ÎÞ¶ËÖ®»ð¡¹"));
	}

	void OnThink() override
	{
		if (frame == 1)
		{
			boss->ShowMaple({ 1,1,0.35,0 }, 1, true, false);
		}

		if (frame == 75)
		{
			scene->AttachActor(new Shooter_2(boss));
			scene->AttachActor(new Shooter_3(boss));
		}

		if (frame == 200)
		{
			boss->GetAnimationGroup().SetAnimIndex(5);
			auto d = new Laser_1;
			d->transform->location = boss->transform->location;
			scene->AttachActor(new LaserFog(d, 50, 120));
			se_lazer00.Play();
		}

		if (frame == 400)
		{
			moveStep = (moveStep + 1) % 6;
			boss->Move(bossLoc[moveStep]);
		}

		if (frame == 500)
		{
			frame = 150;
		}
	}

	void OnBossFinalSpellDead() override
	{
		se_enep00.Play();
		CleanAuto(CLEAN_FINALSPELL, boss->transform->location);
	}

	void OnBossSpellEnd() override
	{
		se_enep01.Play();
		ShowEndDialog_SunnyMilk();
	}
};

class SunnyMilk_SC_6 :public BossSpellCard
{
	class Shooter_1 :public Shooter
	{
	public:

		float angle;

		bool played;

		Shooter_1() :angle(0), played(false)
		{}

		void OnShoot() override
		{
			if (frame < 16 + 8 * difficulty)
			{
				auto d = new YanDan;
				d->SetColor(3);
				d->transform->location = transform->location;
				d->mb->velocity = GetVector(angle + Random(-0.1f, 0.1f), (190.0f + 20.0f * difficulty) * ((float)frame / (16 + 8 * difficulty)));
				d->mb->force = GetVector(angle, -50);
				d->mb->drag = { 0.2,0.2,0.2 };
				scene->AttachActor(new EnemyBulletFog(d));
				if (!played)
				{
					played = true;
					se_lazer01.Play();
				}
			}
			else
			{
				Destroy();
			}
		}
	};

	class FangDan_1 :public FangDan
	{
	public:

		FangDan_1()
		{
			cleanOnHit = false;
			cleanOnBomb = false;
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			if (IsValid())
			{
				if (transform->location.x < -320)
				{
					auto st = scene->AttachActor(new Shooter_1);
					st->transform->location = transform->location;
					st->angle = 0;
					Destroy();
				}
				else if (transform->location.x > 320)
				{
					auto st = scene->AttachActor(new Shooter_1);
					st->transform->location = transform->location;
					st->angle = M_PI;
					Destroy();
				}
				else if (transform->location.y < -240)
				{
					auto st = scene->AttachActor(new Shooter_1);
					st->transform->location = transform->location;
					st->angle = M_PI / 2;
					Destroy();
				}
				else if (transform->location.y > 240)
				{
					auto st = scene->AttachActor(new Shooter_1);
					st->transform->location = transform->location;
					st->angle = M_PI * 3 / 2;
					Destroy();
				}
			}
		}
	};

public:

	SunnyMilk_SC_6() :BossSpellCard(1250, 46, 3500000, 3) {}

	void OnBossSpellStart() override
	{
		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		CreateLifeBar(0, 0.15, true, false);
		scene->AttachActor(new SunnyMilk_CDBG(this));
		scene->AttachActor(new CardCircle(this));
		scene->AttachActor(new SpellCardAttack);
		scene->AttachActor(new SunnyMilk_Sprite);
		scene->AttachActor(new SpellCardText(this, L"¹â·û¡¸¼«¹â³å»÷¡¹"));
	}

	void OnThink() override
	{
		if (frame == 1)
		{
			boss->ShowMaple({ 1,1,0.35,0 }, 1, true, false);
		}

		if (frame == 75)
		{
			float angle = GetAngle(player->transform->location - boss->transform->location);
			ShootAround<LaserCurve, int>(60, 7 + difficulty, boss->transform->location, 50, angle, 10, [=](LaserCurve* d)->void {
				d->SetColor(13);
				d->mb->force = d->mb->velocity.GetNormalized() * 100;
				});
			ShootAround<FangDan_1>(7 + difficulty, boss->transform->location, 50, angle, 10, [=](FangDan_1* d)->void {
				d->SetColor(6);
				d->mb->force = d->mb->velocity.GetNormalized() * 100;
				});
			se_lazer00.Play();
		}

		if (frame == 200)
			frame = 0;
	}

	void OnBossSpellEnd() override
	{
		se_enep02.Play();
		CleanAuto(CLEAN_SPELL, boss->transform->location);
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		DropItem<Power>(20, boss->transform->location, 70);
		DropItem<Point>(30, boss->transform->location, 70);
		DropItem<BombItem>(1, boss->transform->location, 0);

		ComPtr<Boss> tempBoss(boss);
		scene->AttachActor(new DelayInvoke(Delegate([tempBoss]()->void {tempBoss->AddComponent(new SunnyMilk_SC_7); }), 60));
	}
};

class SunnyMilk_SC_5 :public BossSpellCard
{
	class ZhenDan_1 :public ZhenDan
	{
	public:

		int frame;

		float angle;

		float dangle;

		int type;

		ZhenDan_1(int _type) :frame(0), angle(0), type(_type), dangle(0) {}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (IsValid())
			{
				if (frame == 60)
				{
					mb->drag = { 1,1,1 };
					if (type == 0)
						mb->velocity = GetVector(angle + (float)M_PI_2, (0.7f + 0.1 * difficulty) * 200);
					else if (type == 1)
						mb->velocity = GetVector(angle - (float)M_PI_2, (0.7f + 0.1 * difficulty) * 200);
				}
				if (frame >= 60)
				{
					if (type == 0)
						mb->force = GetVector(angle + (float)M_PI_2 + dangle, (0.7f + 0.1 * difficulty) * 300);
					else
						mb->force = GetVector(angle - (float)M_PI_2 - dangle, (0.7f + 0.1 * difficulty) * 300);
				}
			}
		}
	};

	class Shooter_1 :public Shooter
	{
	public:

		LPGOBJ follow;

		Shooter_1(LPGOBJ _follow) :follow(_follow) {}

		void OnShoot() override
		{
			transform->location = follow->transform->location;
			if (frame % 144 == 0)
			{
				const int cnumArray[] = { 2,3,4,4 };
				int dnum = 12 + 2 * difficulty;
				int cnum = cnumArray[difficulty];
				float dangle = (float)M_PI * 4.0f / 3.0f / (dnum - 1);
				for (int i = 0; i < dnum; i++)
				{
					float angle = -M_PI / 6 + dangle * i;
					for (int j = 0; j < cnum; j++)
					{
						auto d = new ZhenDan_1(0);
						d->angle = angle;
						d->dangle = M_PI_2 / 3.0f * (j + 1);
						d->SetColor(13);
						d->transform->location = transform->location;
						d->mb->velocity = GetVector(angle, sqrt(12000 + 12000 * j));
						d->mb->force = -d->mb->velocity;
						scene->AttachActor(new EnemyBulletFog(d));
						d = new ZhenDan_1(1);
						d->angle = angle;
						d->dangle = M_PI_2 / 3.0f * (j + 1);
						d->SetColor(13);
						d->transform->location = transform->location;
						d->mb->velocity = GetVector(angle, sqrt(12000 + 12000 * j));
						d->mb->force = -d->mb->velocity;
						scene->AttachActor(new EnemyBulletFog(d));
					}
				}
				se_kira00.Play();
			}
			if (frame % 24 == 0)
			{
				float ang = RandomAngle();
				ShootAround<YanDan>(6 + 2 * difficulty, transform->location, (0.7f + 0.1 * difficulty) * 180, ang, 20, [=](YanDan* d)->void {
					d->SetColor(0);
					d->mb->drag = { 0.5,0.5,0.5 };
					d->mb->force = GetVector(GetAngle(d->mb->velocity) + (float)M_PI / 6.0f, (0.7f + 0.1 * difficulty) * 200);
					});
				ShootAround<YanDan>(6 + 2 * difficulty, transform->location, (0.7f + 0.1 * difficulty) * 180, ang, 20, [=](YanDan* d)->void {
					d->SetColor(0);
					d->mb->drag = { 0.5,0.5,0.5 };
					d->mb->force = GetVector(GetAngle(d->mb->velocity) - (float)M_PI / 6.0f, (0.7f + 0.1 * difficulty) * 200);
					});
				se_tan00.Play();
			}
		}

		void OnDestroy() override
		{
			__super::OnDestroy();
			follow.Reset();
		}
	};

public:

	Vector3 bossLoc[3];

	int moveStep;

	SunnyMilk_SC_5() :BossSpellCard(460, 40),
		bossLoc{ {0,100,0},{24,110,0},{15,92,0}}, moveStep(0){}

	void OnBossSpellStart() override
	{
		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		CreateLifeBar(0.15, 1, false, true);
	}

	void OnThink() override
	{
		if (frame == 1 && moveStep == 0)
			boss->ShowMaple({ 1,1,0.35,0 }, 1, true, false);

		if (frame == 75)
		{
			auto st = scene->AttachActor(new Shooter_1(boss));
			st->transform->location = boss->transform->location;
		}

		if (frame == 120)
		{
			moveStep = (moveStep + 1) % 3;
			boss->Move(bossLoc[moveStep]);
		}

		if (frame == 300)
			frame = 100;
	}

	void OnBossSpellEnd() override
	{
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		CleanAuto(CLEAN_NOSPELL);
		boss->AddComponent(new SunnyMilk_SC_6);
	}
};

class SunnyMilk_SC_4 :public BossSpellCard
{
	class GuangYuL_1 :public GuangYuL
	{
	public:

		int frame;

		GuangYuL_1(int startframe) :frame(startframe) 
		{
			cleanOnHit = false;
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			frame++;
			if (IsValid())
			{
				if (frame >= 60)
				{
					for (int i = 0; i < 2 + 1 * difficulty; i++)
					{
						auto d = new YanDan;
						d->transform->location = transform->location;
						d->mb->velocity = GetVector(RandomAngle(), (0.8f + 0.2f * difficulty) * Random(120.0f, 150.0f));
						d->SetColor(0);
						scene->AttachActor(new EnemyBulletFog(d));
					}
					for (int i = 0; i < 1 + difficulty / 2; i++)
					{
						auto d = new DaYu;
						d->transform->location = transform->location;
						d->mb->velocity = GetVector(RandomAngle(), (0.8f + 0.2f * difficulty) * Random(80.0f, 120.0f));
						d->SetColor(0);
						scene->AttachActor(new EnemyBulletFog(d));
					}
					se_tan00.Play();
					Destroy();
				}
			}
		}
	};

public:

	int position;

	SunnyMilk_SC_4() :BossSpellCard(950, 41, 3200000, 2), position(0) {}

	void OnBossSpellStart() override
	{
		boss->Move({ 0,120 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		CreateLifeBar(0, 0.15, true, false);
		scene->AttachActor(new SunnyMilk_CDBG(this));
		scene->AttachActor(new CardCircle(this));
		scene->AttachActor(new SpellCardAttack);
		scene->AttachActor(new SunnyMilk_Sprite);
		scene->AttachActor(new SpellCardText(this, L"ÈÕ·û¡¸ÑôÑ×Èý½Ç¡¹"));
	}

	void OnThink() override
	{
		if (frame == 1)
		{
			boss->ShowMaple({ 1,1,0.35,0 }, 1, true, false);
			boss->GetAnimationGroup().SetAnimIndex(0);
		}

		if (frame >= 100 && frame < 1000)
		{
			if (!(boss->GetEnemyFlags() & EF_Moving))
			{
				position++;
				switch (position)
				{
				case 1:
					boss->Move({ 100,-20 }, 120.0f, 120.0f);
					break;
				case 2:
					boss->Move({ -100,-20 }, 120.0f, 120.0f);
					break;
				case 3:
					boss->Move({ 0,120 }, 120.0f, 120.0f);
					break;
				case 4:
					frame = 1000;
					break;
				}
			}

			if (frame != 1000)
			{
				if (frame % 20 == 0)
				{
					for (int i = 0; i < 4 + 2 * difficulty; i++)
					{
						auto d = new YanDan;
						d->transform->location = boss->transform->location;
						d->mb->velocity = GetVector(RandomAngle(), (0.8f + 0.2f * difficulty) * Random(120.0f, 150.0f));
						d->SetColor(0);
						scene->AttachActor(new EnemyBulletFog(d));
					}
					for (int i = 0; i < 4 + 2 * difficulty; i++)
					{
						auto d = new DianDan;
						d->transform->location = boss->transform->location;
						d->mb->velocity = GetVector(RandomAngle(), (0.8f + 0.2f * difficulty) * Random(150.0f, 185.0f));
						d->SetColor(0);
						scene->AttachActor(new EnemyBulletFog(d));
					}
					for (int i = 0; i < 2 + 1 * difficulty; i++)
					{
						auto d = new DaYu;
						d->transform->location = boss->transform->location;
						d->mb->velocity = GetVector(RandomAngle(), (0.8f + 0.2f * difficulty) * Random(80.0f, 120.0f));
						d->SetColor(0);
						scene->AttachActor(new EnemyBulletFog(d));
					}
				}

				if (frame % 10 == 0)
				{

					auto d = new GuangYuL_1(-360 + frame);
					d->SetColor(1);
					d->transform->location = boss->transform->location;
					d->mb->drag = { 1,1,1 };
					scene->AttachActor(new EnemyBulletFog(d));
					se_tan00.Play();
				}
			}
		}

		if (frame == 1060)
		{
			boss->GetAnimationGroup().SetAnimIndex(5);
		}

		if (frame == 1250)
		{
			position = 0;
			frame = 0;
		}
	}

	void OnBossSpellEnd() override
	{
		se_enep02.Play();
		CleanAuto(CLEAN_SPELL, boss->transform->location);
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		DropItem<Power>(20, boss->transform->location, 70);
		DropItem<Point>(30, boss->transform->location, 70);
		DropItem<LifeItem>(1, boss->transform->location, 0);

		ComPtr<Boss> tempBoss(boss);
		scene->AttachActor(new DelayInvoke(Delegate([tempBoss]()->void {tempBoss->AddComponent(new SunnyMilk_SC_5); }), 60));
	}
};

class SunnyMilk_SC_3 :public BossSpellCard
{
	class Shooter_1 :public Shooter
	{
	public:

		int type;

		float startrad;

		Shooter_1(int _type) :type(_type), startrad(0)
		{
		}

		void OnShoot() override
		{
			if (frame % 5 == 0 && frame <= 150)
			{
				ShootAround<ZhenDan>(8 + 4 * difficulty, transform->location, (0.7f + difficulty * 0.1f) * 200, (startrad) * (type == 0 ? 1.0f : -1.0f), 20, [=](ZhenDan* d)->void {
					d->SetColor(13);
					});
				se_tan00.Play();
				startrad += fmin(frame / 1000.0f, 0.15f);
			}
			if (frame > 160)
			{
				for (int i = 0; i < difficulty + 1; i++)
				{
					auto d = new YanDan;
					d->mb->velocity = GetVector(RandomAngle(), (0.7f + difficulty * 0.1f) * (350 - 5.0f * (frame - 160)));
					d->transform->location = transform->location;
					d->SetColor(0);
					scene->AttachActor(new EnemyBulletFog(d));
				}
				se_tan00.Play();
			}
			if (frame == 190)
				Destroy();
		}
	};

public:

	Vector3 bossLoc[3];

	int moveStep;

	int type;

	SunnyMilk_SC_3() :BossSpellCard(480, 42),
		bossLoc{ {0,100,0},{25,115,0},{-43,130,0}}, moveStep(0), type(0) {}

	void OnBossSpellStart() override
	{
		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		CreateLifeBar(0.15, 1, false, true);
	}

	void OnThink() override
	{
		if (frame == 1 && moveStep == 0)
			boss->ShowMaple({ 1,1,0.35,0 }, 1, true, false);

		if (frame == 75)
		{
			auto st = scene->AttachActor(new Shooter_1(type));
			st->transform->location = boss->transform->location;
			type = (type + 1) % 2;
		}

		if (frame == 240)
		{
			moveStep = (moveStep + 1) % 3;
			boss->Move(bossLoc[moveStep]);
		}

		if (frame == 260 + (moveStep == 0 ? 30 : 0))
		{
			frame = 0;
		}
	}

	void OnBossSpellEnd() override
	{
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		CleanAuto(CLEAN_NOSPELL);
		boss->AddComponent(new SunnyMilk_SC_4);
	}
};

class SunnyMilk_SC_2 :public BossSpellCard
{
	class YanDan_1 :public YanDan
	{
	public:

		bool bounced;

		YanDan_1() :bounced(false)
		{
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			if (IsValid())
			{
				if (!bounced)
				{
					if (transform->location.x < -320)
					{
						mb->velocity.x = abs(mb->velocity.x);
						SetColor(3);
						bounced = true;
					}
					if (transform->location.x > 320)
					{
						mb->velocity.x = -abs(mb->velocity.x);
						SetColor(3);
						bounced = true;
					}
					if (transform->location.y < -240)
					{
						mb->velocity.y = abs(mb->velocity.y);
						SetColor(3);
						bounced = true;
					}
					if (transform->location.y > 240)
					{
						mb->velocity.y = -abs(mb->velocity.y);
						SetColor(3);
						bounced = true;
					}
				}
			}
		}
	};

	class Shooter_1 :public Shooter
	{
	public:

		LPGOBJ follow;

		Shooter_1(LPGOBJ _follow) :follow(_follow) {}

		void OnShoot() override
		{
			if (frame % (65 - 5 * difficulty) == 0)
			{
				float ang = RandomAngle();
				for (int i = 0; i < 7 + difficulty; i++)
				{
					ShootAround<ZhenDan>(8 + difficulty, follow->transform->location, (0.7f + 0.1f * difficulty) * 150, ang + 0.03f * i, 15, [=](ZhenDan* d)->void {
						d->SetColor(13);
						});
				}
				ShootAround<YanDan_1>(5 + 2 * difficulty, follow->transform->location, (0.7f + 0.1f * difficulty) * 100, RandomAngle(), 20, [=](YanDan_1* d)->void {
					d->SetColor(0);
					});
				se_tan00.Play();
			}
		}

		void OnDestroy() override
		{
			__super::OnDestroy();
			follow.Reset();
		}
	};

public:

	Vector3 bossLoc[6];

	int moveStep;

	SunnyMilk_SC_2() :BossSpellCard(1000, 40, 2500000, 1),
		bossLoc{ { 0,100,0 }, { 112,110,0 }, { 88,88,0 }, { -21,105,0 },{-76,94,0},{-10,125,0} }, moveStep(0) {}

	void OnBossSpellStart() override
	{
		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		CreateLifeBar(0, 0.15, true, false);
		scene->AttachActor(new SunnyMilk_CDBG(this));
		scene->AttachActor(new CardCircle(this));
		scene->AttachActor(new SpellCardAttack);
		scene->AttachActor(new SunnyMilk_Sprite);
		scene->AttachActor(new SpellCardText(this, L"»ð·û¡¸»ð¾«ÁéÔ¾¶¯¡¹"));
	}

	void OnThink() override
	{
		if (frame == 1)
		{
			boss->ShowMaple({ 1,1,0.35,0 }, 1, true, false);
		}

		if (frame == 25)
		{
			scene->AttachActor(new Shooter_1(boss));
		}

		if (frame == 200)
		{
			moveStep = (moveStep + 1) % 6;
			boss->Move(bossLoc[moveStep]);
		}

		if (frame == 250)
		{
			frame = 50;
		}
	}

	void OnBossSpellEnd() override
	{
		se_enep02.Play();
		CleanAuto(CLEAN_SPELL, boss->transform->location);
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		DropItem<Power>(20, boss->transform->location, 70);
		DropItem<Point>(30, boss->transform->location, 70);
		DropItem<BombItem>(1, boss->transform->location, 0);

		ComPtr<Boss> tempBoss(boss);
		scene->AttachActor(new DelayInvoke(Delegate([tempBoss]()->void {tempBoss->AddComponent(new SunnyMilk_SC_3); }), 60));
	}
};

class SunnyMilk_SC_1 :public BossSpellCard
{
	class Shooter_1 :public Shooter
	{
	public:

		int type;

		float startrad;

		Shooter_1(int _type) :type(_type), startrad(RandomAngle())
		{
		}

		void OnShoot() override
		{
			if (frame % 1 == 0)
			{
				ShootAround<ZhenDan>(6 + 2 * difficulty, transform->location, (0.7f + difficulty * 0.1f) * (30 + 11 * frame), (startrad + frame / 9.0f) * (type == 0 ? 1.0f : -1.0f), 20, [=](ZhenDan* d)->void {
					d->SetColor(13);
					});
				se_tan00.Play();
			}
			if (frame == 18 + 2 * difficulty)
				Destroy();
		}
	};

public:

	Vector3 bossLoc[7];

	int moveStep;

	int type;

	SunnyMilk_SC_1() :BossSpellCard(450, 40),
		bossLoc{ {0,100,0},{-24,92,0},{-8,110,0},{28,115,0},{44,98,0},{10,88,0},{-16,114,0} }, moveStep(0), type(0) {}

	void OnBossSpellStart() override
	{
		boss->Move({ 0,100 });
		boss->SetEnemyFlags(boss->GetEnemyFlags() | EnemyFlag::EF_Checking);
		CreateLifeBar(0.15, 1, false, true);
	}

	void OnThink() override
	{
		if (frame == 1 && moveStep == 0)
			boss->ShowMaple({ 1,1,0.35,0 }, 1, true, false);

		if (frame == 75)
		{
			auto st = scene->AttachActor(new Shooter_1(type));
			st->transform->location = boss->transform->location;
			type = (type + 1) % 2;
		}

		if (frame == 120)
		{
			moveStep = (moveStep + 1) % 7;
			boss->Move(bossLoc[moveStep]);
		}

		if (frame == 140 + (moveStep == 0 ? 30 : 0))
		{
			frame = 0;
		}
	}

	void OnBossSpellEnd() override
	{
		boss->SetEnemyFlags(boss->GetEnemyFlags() & ~EnemyFlag::EF_Checking);
		CleanAuto(CLEAN_NOSPELL);
		boss->AddComponent(new SunnyMilk_SC_2);
	}
};