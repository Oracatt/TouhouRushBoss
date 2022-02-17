#pragma once
#include "PlayerBullet.h"
#include "Source.h"
#include "Effect.h"
#include "SceneObjects.h"

class Reimu_Bullet_1 :public PlayerBullet
{
public:

	Reimu_Bullet_1() :PlayerBullet(AR_NONE)
	{
		transform->scale = { 64,16,1 };
		transform->SetEulerAngles({ 0,0,M_PI_2 });
		Material mat = sr->GetMaterial();
		mat.diffuse.a = 0.5;
		sr->SetMaterial(mat);
		sr->SetTexture(src_player_reimu.Get());
		sr->SetSourceRect({ 3 / 4.0f,9 / 16.0f,1 / 4.0f,1 / 16.0f });
		mb->velocity = { 0,2000,0 };
	}

private:

	void OnHit(Character* character) override
	{
		scene->AttachActor(new Reimu_Bullet_1_Hit)->transform->location = transform->location;
		__super::OnHit(character);
	}
};

class Reimu_Bullet_2 :public PlayerBullet
{
public:

	Reimu_Bullet_2() :PlayerBullet(AR_VELOCITY)
	{
		damage = 2;
		checkRadius = 25;
		transform->scale = { 16,16,1 };
		Material mat = sr->GetMaterial();
		mat.diffuse.a = 0.5;
		sr->SetMaterial(mat);
		sr->SetTexture(src_player_reimu.Get());
		sr->SetSourceRect({ 0 / 4.0f,10 / 16.0f,1 / 16.0f,1 / 16.0f });
		mb->velocity = { 0,1500,0 };
		mb->drag = { 1,1,1 };
		mb->force = { 0,1500,0 };
	}

private:

	void OnCheck() override
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
			if (min_distance * min_distance < target->GetAttribution().checkRadius * target->GetAttribution().checkRadius + checkRadius * checkRadius)
			{
				OnHit(target);
			}
		}
	}

	void OnHit(Character* character) override
	{
		auto eff = scene->AttachActor(new Reimu_Bullet_2_Hit);
		eff->transform->location = transform->location;
		eff->mb->velocity = mb->velocity.GetNormalized() * Random(60.0f, 150.0f);
		__super::OnHit(character);
	}
};

class Reimu_Bullet_3 :public PlayerBullet
{
public:

	Reimu_Bullet_3(int _type, int _seed) :
		PlayerBullet(AR_NONE), type(_type), seed(_seed)
	{
		transform->scale = { 128,16,1 };
		transform->SetEulerAngles({ 0,0,M_PI_2 });
		Material mat = sr->GetMaterial();
		mat.diffuse.a = 0.5;
		sr->SetMaterial(mat);
		sr->SetTexture(src_player_reimu.Get());
		sr->SetSourceRect({ 1 / 4.0f,11 / 16.0f,1 / 4.0f,1 / 16.0f });
		mb->velocity = { 0,2000,0 };
	}

private:

	int type;

	int seed;

	void OnUpdate() override
	{
		Material mat = sr->GetMaterial();
		mat.diffuse.a += 0.06f;
		if (mat.diffuse.a > 0.8)
			mat.diffuse.a = 0.8;
		sr->SetMaterial(mat);
		__super::OnUpdate();
	}

	void OnHit(Character* character) override
	{
		auto g = scene->AttachActor(new Reimu_Bullet_3_Hit(type, seed));
		g->transform->location = transform->location;
		g->transform->location.y = character->transform->location.y;
		__super::OnHit(character);
	}
};

class Marisa_Bullet_1 :public PlayerBullet
{
public:

	Marisa_Bullet_1() :PlayerBullet(AR_NONE)
	{
		transform->scale = { 32,16,1 };
		transform->SetEulerAngles({ 0,0,M_PI_2 });
		Material mat = sr->GetMaterial();
		mat.diffuse.a = 0.5;
		sr->SetMaterial(mat);
		sr->SetTexture(src_player_marisa.Get());
		sr->SetSourceRect({ 0 / 4.0f,9 / 16.0f,1 / 8.0f,1 / 16.0f });
		mb->velocity = { 0,2000,0 };
	}

private:

	void OnHit(Character* character) override
	{
		scene->AttachActor(new Marisa_Bullet_1_Hit)->transform->location = transform->location;
		__super::OnHit(character);
	}
};

class Marisa_Bullet_2 :public PlayerBullet
{
public:

	Marisa_Bullet_2() :PlayerBullet(AR_NONE), dframe(0), angle(0)
	{
		cleanOnHit = false;
		transform->scale = { 256 * 3,16,1 };
		SetAngle(M_PI_2);
		Material mat = sr->GetMaterial();
		mat.diffuse.a = 0.5;
		sr->SetMaterial(mat);
		sr->SetTexture(src_player_marisa.Get());
		sr->SetLocalTransform(XMMatrixTranslation(0.5, 0, 0));
		sr->SetSourceRect({ -(float)timer->GetGameTime() * 1.5f,11 / 16.0f,3,1 / 16.0f });
		AddComponent(new BlendStateSet::Setter(BLEND_STATE_BRIGHTEN))->SetPriority(101, false);
		AddComponent(new BlendStateSet::Setter(BLEND_STATE_NORMAL))->SetPriority(99, false);
		SortComponents();
	}

	void SetAngle(float _angle)
	{
		angle = _angle;
		transform->SetEulerAngles({ 0,0,angle });
	}

private:

	int dframe;

	float angle;

	void OnUpdate() override
	{
		__super::OnUpdate();
		dframe++;
		if (dframe == 1)
			Destroy();
	}

	void OnCheck() override
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
				float x = transform->location.x;
				float y = transform->location.y;
				float k = tan(angle);
				if (abs(k) > 999999)
					k = 999999;
				float b = y - k * x;
				float ex = e->transform->location.x;
				float ey = e->transform->location.y;
				if (abs(k * ex - ey + b) / sqrt(k * k + 1) < e->GetAttribution().checkRadius)
				{
					float eangle = atan2(ey - y, ex - x);
					if (eangle < 0)
						eangle += (float)M_PI * 2;
					if (AngleSubAngle(angle, eangle) < M_PI_2)
						OnHit(e);
				}
			}
		}
	}

	void OnHit(Character* character) override
	{
		float x = transform->location.x;
		float y = transform->location.y;
		float k = tan(angle);
		if (abs(k) > 999999)
			k = 999999;
		float b = y - k * x;
		float ex = character->transform->location.x;
		float ey = character->transform->location.y;
		float kk = -1 / k;
		float bb = ey - kk * ex;
		float tx = (bb - b) / (k - kk);
		float ty = k * tx + b;
		scene->AttachActor(new Marisa_Bullet_2_Hit)->transform->location = { tx,ty,0 };
		__super::OnHit(character);
	}
};

class Marisa_Bullet_3 :public PlayerBullet
{
public:

	Marisa_Bullet_3() :
		PlayerBullet(AR_NONE)
	{
		damage = 8;
		transform->scale = { 32,16,1 };
		transform->SetEulerAngles({ 0,0,M_PI_2 });
		sr->SetTexture(src_player_marisa.Get());
		sr->SetSourceRect({ 10 / 16.0f,12 / 16.0f,1 / 8.0f,1 / 16.0f });
		mb->velocity = { 0,200,0 };
		mb->force = { 0,2500,0 };
	}

private:

	void OnHit(Character* character) override
	{
		scene->AttachActor(new Marisa_Bullet_3_Hit)->transform->location = transform->location;
		__super::OnHit(character);
	}
};