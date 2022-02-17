#pragma once
#include "VirtualLib.h"
#include "SceneObjects.h"
#include "Boss.h"
#include "GameScene.h"
#include "UserComponent.h"
#include "Mathf.h"
#include "Source.h"
#include "SoundEffect.h"

class BossShowing :public GameObject
{
	class Particle :public GameObject
	{
	public:

		//0lt 1rt 2lb 3 rb
		Particle(const Color& color, const Vector3& loc, int _index) :
			frame(0), sr(AddComponent(new SpriteRender)), mb(AddComponent(new MoveBody)), target(loc)
		{
			transform->scale = { 0.4,0.4,1 };
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = color;
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_aura.Get());
			sr->SetSourceRect({ (Random(0,3) + 12) / 16.0f,0,1 / 16.0f,1 / 8.0f });

			mb->drag = { 45,45,45 };
			switch (_index)
			{
			case 0:
				transform->location = loc + Vector3(-130, -100, 0) + GetVector(RandomAngle(), Random(0.0f, 100.0f));
				mb->velocity = GetVector((float)M_PI_2 + Random(0.6f, 1.8f), Random(10.0f, 14.0f));
				break;
			case 1:
				transform->location = loc + Vector3(130, -100, 0) + GetVector(RandomAngle(), Random(0.0f, 100.0f));
				mb->velocity = GetVector((float)M_PI_2 - Random(0.6f, 1.8f), Random(10.0f, 14.0f));
				break;
			case 2:
				transform->location = loc + Vector3(0, -360, 0) + GetVector(RandomAngle(), Random(0.0f, 100.0f));
				mb->velocity = GetVector((float)M_PI_2 + Random(0.3f, 0.9f), Random(8.0f, 12.0f));
				break;
			case 3:
				transform->location = loc + Vector3(0, -360, 0) + GetVector(RandomAngle(), Random(0.0f, 100.0f));
				mb->velocity = GetVector((float)M_PI_2 - Random(0.3f, 0.9f), Random(8.0f, 12.0f));
				break;
			}
			transform->location = ScreenToWorld(transform->location, 0.99f, GameSceneCom::fixed3dcam.Get(), { 640,480 });
		}

	private:

		int frame;

		MoveBody* mb;

		SpriteRender* sr;

		Material mat;

		Vector3 target;

		void OnUpdate() override
		{
			frame++;
			mb->force = (ScreenToWorld(target, 0.99, GameSceneCom::fixed3dcam.Get(), { 640,480 }) - transform->location).GetNormalized() * 20;
			if (frame <= 10)
				mat.diffuse.a += 0.03;
			sr->SetMaterial(mat);
			if (frame == 120)
				Destroy();
		}
	};

public:

	BossShowing(Boss* _boss) :
		frame(0), boss(_boss)
	{
	}

private:

	int frame;

	LPGOBJ g;

	ComPtr<Boss> boss;

	void OnCreated() override
	{
		se_ch02.Play();
		InsertBefore(GameSceneCom::locator[18].Get());
		g = scene->AttachActor(new BlendStateSet(BLEND_STATE_DARKEN));
		g->InsertBefore(this);
		boss->SetCharacterFlags(boss->GetCharacterFlags() | CF_NoDraw);
	}

	void OnUpdate() override
	{
		frame++;
		if (frame <= 60)
		{
			for (int n = 0; n < 10; n++)
			{
				for (int i = 0; i < 4; i++)
				{
					if (Random(0, 1) == 1)
						scene->AttachActor(new Particle({ 0.25,0.5,0.5,0 }, boss->transform->location + GetVector(RandomAngle(), Random(-25.0f, 25.0f)), i))->InsertBefore(this);
					else
						scene->AttachActor(new Particle({ 0.5,0.5,0.25,0 }, boss->transform->location + GetVector(RandomAngle(), Random(-25.0f, 25.0f)), i))->InsertBefore(this);
				}
			}
		}
		if (frame == 150)
			boss->SetCharacterFlags(boss->GetCharacterFlags() & ~CF_NoDraw);
		if (frame == 200)
			Destroy();
	}

	void OnDestroy() override
	{
		if (g)
		{
			g->Destroy();
			g.Reset();
		}
		if (boss)
		{
			boss.Reset();
		}
	}
};


class MapleSpawner_1 :public GameObject
{
public:

	class Maple :public GameObject
	{
	public:

		Maple(const Color& color, const Vector3& center, const Vector3& _start) :
			frame(0), sr(0),
			rotateSpeed({ RandomAngle(),RandomAngle(),RandomAngle() }), target(center), start(_start)
		{
			transform->location = start;
			AddComponent(new BlendStateSet::Setter(BLEND_STATE_BRIGHTEN));
			transform->scale = { 0.25,0.25,1 };
			transform->SetEulerAngles({ RandomAngle(),RandomAngle(),RandomAngle() });
			mat.ambient = color;
			mat.diffuse = { 0,0,0,0 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr = AddComponent(new SpriteRender);
			sr->SetMaterial(mat);
			sr->SetTexture(src_maple.Get());
			sr->SetSourceRect({ 0.5,0,0.5,0.5 });
		}

	private:

		int frame;

		Material mat;

		SpriteRender* sr;

		Euler rotateSpeed;

		Vector3 target;

		Vector3 start;

		void OnUpdate() override
		{
			frame++;
			if (frame <= 30)
				transform->location = Lerp(start, target, frame / 30.0f);
			transform->Rotate(rotateSpeed * timer->GetDeltaTime(), true);
			if (frame <= 4)
			{
				mat.diffuse.a += 0.125;
				sr->SetMaterial(mat);
			}
			if (frame >= 32)
			{
				mat.diffuse.a -= 0.1;
				sr->SetMaterial(mat);
			}
			if (frame == 37)
				Destroy();
		}
	};

	MapleSpawner_1(LPGOBJ _follow, const Color& _color) :
		frame(0), color(_color), follow(_follow)
	{
	}

private:

	int frame;

	Color color;

	LPGOBJ follow;

	void OnUpdate() override
	{
		frame++;
		Vector3 start = ScreenToWorld(follow->transform->location, 0.9, GameSceneCom::fixed3dcam.Get(), { 640,480 });
		Vector3 target = ScreenToWorld(follow->transform->location, 0.95, GameSceneCom::fixed3dcam.Get(), { 640,480 });
		scene->AttachActor(new Maple(color, target, start + GetVector((float)M_PI_2 * (frame % 4) + Random(0.0f, (float)M_PI_2), Random(0.45f, 0.5f))))->InsertBefore(this);
		if (frame == 30)
			Destroy();
	}

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::locator[18].Get());
	}

	void OnDestroy() override
	{
		__super::OnDestroy();
		follow.Reset();
	}
};

class MapleSpawner_2 :public GameObject
{
public:

	class Maple :public GameObject
	{
	public:

		Maple(const Color& color, const Vector3& center, const Vector3& _start) :
			frame(0), sr(0),
			rotateSpeed({ RandomAngle(),RandomAngle(),RandomAngle() }), target(center), start(_start)
		{
			transform->location = target;
			AddComponent(new BlendStateSet::Setter(BLEND_STATE_BRIGHTEN));
			transform->scale = { 0.4,0.4,1 };
			transform->SetEulerAngles({ RandomAngle(),RandomAngle(),RandomAngle() });
			mat.ambient = color;
			mat.diffuse = { 0,0,0,0.5 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr = AddComponent(new SpriteRender);
			sr->SetMaterial(mat);
			sr->SetTexture(src_maple.Get());
			sr->SetSourceRect({ 0.5,0,0.5,0.5 });
		}

	private:

		int frame;

		Material mat;

		SpriteRender* sr;

		Euler rotateSpeed;

		Vector3 target;

		Vector3 start;

		void OnUpdate() override
		{
			frame++;
			if (frame <= 45)
			{
				transform->location = Lerp(target, start, pow(frame / 45.0f, 0.75f));
				transform->Rotate(rotateSpeed * timer->GetDeltaTime() * Lerp(2.0f, 1.0f, (frame / 45.0f)), true);
			}
			else
			{
				transform->Rotate(rotateSpeed * timer->GetDeltaTime(), true);
			}
			if (frame >= 25)
			{
				transform->scale -= { 0.01,0.01,0 };
				mat.diffuse.a -= 0.02;
				sr->SetMaterial(mat);
			}
			if (frame == 50)
				Destroy();
		}
	};

	MapleSpawner_2(LPGOBJ _follow, const Color& _color) :
		frame(0), color(_color), follow(_follow)
	{
	}

private:

	int frame;

	Vector3 target;

	Vector3 start;

	Color color;

	LPGOBJ follow;

	void OnUpdate() override
	{
		frame++;
		Vector3 target = ScreenToWorld(follow->transform->location, 0.95, GameSceneCom::fixed3dcam.Get(), { 640,480 });
		Vector3 start = ScreenToWorld(follow->transform->location, 0.95, GameSceneCom::fixed3dcam.Get(), { 640,480 });
		scene->AttachActor(new Maple(color, target, start + GetVector((float)M_PI_2 * (frame % 4) + Random(0.0f, (float)M_PI_2), Random(0.8f, 1.1f))))->InsertBefore(this);
		if (frame == 24)
			Destroy();
	}

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::locator[18].Get());
	}

	void OnDestroy() override
	{
		__super::OnDestroy();
		follow.Reset();
	}
};

class BossDeadMaple :public GameObject
{
public:

	class Maple :public GameObject
	{
	public:

		Maple(const Color& color, const Vector3& center, const Vector3& _start) :
			frame(0), sr(0),
			rotateSpeed({ RandomAngle(),RandomAngle(),RandomAngle() }), target(center), start(_start)
		{
			transform->location = target;
			AddComponent(new BlendStateSet::Setter(BLEND_STATE_BRIGHTEN));
			transform->scale = { 0.4,0.4,1 };
			transform->SetEulerAngles({ RandomAngle(),RandomAngle(),RandomAngle() });
			mat.ambient = color;
			mat.diffuse = { 0,0,0,0.5 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr = AddComponent(new SpriteRender);
			sr->SetMaterial(mat);
			sr->SetTexture(src_maple.Get());
			sr->SetSourceRect({ 0.5,0,0.5,0.5 });
		}

	private:

		int frame;

		Material mat;

		SpriteRender* sr;

		Euler rotateSpeed;

		Vector3 target;

		Vector3 start;

		void OnUpdate() override
		{
			frame++;
			if (frame <= 60)
			{
				transform->location = Lerp(target, start, pow(frame / 60.0f, 0.75f));
				transform->Rotate(rotateSpeed * timer->GetDeltaTime() * Lerp(2.0f, 1.0f, (frame / 60.0f)), true);
			}
			else
			{
				transform->Rotate(rotateSpeed * timer->GetDeltaTime(), true);
			}
			if (frame >= 40)
			{
				transform->scale -= { 0.01, 0.01, 0 };
				mat.diffuse.a -= 0.02;
				sr->SetMaterial(mat);
			}
			if (frame == 65)
				Destroy();
		}
	};

	BossDeadMaple(const Color& _color, const Vector3& center) :
		frame(0), target(ScreenToWorld(center, 0.95, GameSceneCom::fixed3dcam.Get(), { 640,480 })),
		color(_color), start(ScreenToWorld(center, 0.95, GameSceneCom::fixed3dcam.Get(), { 640,480 }))
	{
	}

private:

	int frame;

	Vector3 target;

	Vector3 start;

	Color color;

	void OnUpdate() override
	{
		frame++;
		for (int i = 0; i < 4; i++)
			scene->AttachActor(new Maple(color, target, start + GetVector((float)M_PI_2 * (frame % 4) + Random(0.0f, (float)M_PI_2), Random(0.4f, 1.8f))))->InsertBefore(this);
		if (frame == 36)
			Destroy();
	}

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::locator[18].Get());
	}
};