#pragma once
#include "Item.h"
#include "GameScene.h"
#include "Source.h"
#include "SoundEffect.h"
#include "UIDeriver.h"

class ItemTag :public GameObject
{
public:

	ItemTag(ITexture* source, const RectangleF& rect) :sr(AddComponent(new SpriteRender))
	{
		Material mat;
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,1 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(source);
		sr->SetSourceRect(rect);
		sr->SetLocalTransform(XMMatrixTranslation(0, -0.5, 0));
	}

protected:

	SpriteRender* sr;

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::locator[11].Get());
	}

};

class Power :public Item
{
public:

	Power(bool isPlayerDrop = false) :Item(false, true, true, isPlayerDrop)
	{
		transform->scale = { 16,16,1 };
		sr->SetTexture(src_item.Get());
		sr->SetSourceRect({ 12 / 16.0f,0,1 / 16.0f,1 / 4.0f });
	}

protected:

	void OnEat() override
	{
		se_eat.Play();
		if (playerData.power % 100 >= 95)
			se_powerup.Play();
		if (playerData.power < 400 && playerData.power >= 395)
			scene->AttachActor(new FullPowerUp);
		playerData.power += 5;
		if (playerData.power >= 400)
		{
			AddScore(10000);
			scene->AttachActor(new ItemNumber(10000, transform->location, player->transform->location.y >= player->GetAttribution().itemLine));
			playerData.power = 400;
		}
		else
		{
			AddScore(100);
		}
	}

	GameObject* OnShowTag() override
	{
		GameObject* g = scene->AttachActor(new ItemTag(src_item.Get(), { 12 / 16.0f,1 / 4.0f,1 / 16.0f,1 / 4.0f }));
		g->transform->location = { transform->location.x ,240,0 };
		g->transform->scale = { 16,16,1 };
		return g;
	}
};

class ScorePoint :public Item
{
public:

	ScorePoint(bool _autoEat = false) :Item(true, false, false, false), frame(0), autoEat(_autoEat)
	{
		transform->scale = { 16,16,1 };
		mat.diffuse.a = 0.5;
		sr->SetMaterial(mat);
		sr->SetTexture(src_item.Get());
		sr->SetSourceRect({ 13 / 16.0f,1 / 2.0f,1 / 16.0f,1 / 4.0f });
		mb->velocity = GetVector(Random((float)M_PI_2 - 0.2f, (float)M_PI_2 + 0.2f), 120);
		mb->drag = { 1,1,1 };
	}

protected:

	int frame;

	bool autoEat;

	void OnEat() override
	{
		se_eat.Play();
		AddScore(2000);
	}

	GameObject* OnShowTag() override
	{
		return 0;
	}

	void OnUpdate() override
	{
		frame++;
		float distance = Vector2Distance(player->transform->location, transform->location);
		if (player && frame > 30)
		{
			following = true;
		}

		if (following)
		{
			const float spd = 600.0f;

			mb->SetActive(false);
			transform->location += (player->transform->location - transform->location).GetNormalized() * spd * timer->GetDeltaTime();
			if (distance * distance < 225 + checkRadius * checkRadius || autoEat)
			{
				OnEat();
				Destroy();
			}
		}
		if (transform->location.y < -250 || transform->location.y > 250 || transform->location.x < -330 || transform->location.x > 330)
			Destroy();
	}
};

class Point :public Item
{
public:

	Point() :Item(false, true, true, false)
	{
		transform->scale = { 16,16,1 };
		sr->SetTexture(src_item.Get());
		sr->SetSourceRect({ 13 / 16.0f,0,1 / 16.0f,1 / 4.0f });
	}

protected:

	void OnEat() override
	{
		se_eat.Play();
		float value = fmin((player->transform->location.y + 240) / (player->GetAttribution().itemLine + 240), 1.0f);
		int score = 10 * Lerp(playerData.maxPoint / 40, playerData.maxPoint / 10, value);
		AddScore(score);
		scene->AttachActor(new ItemNumber(score, transform->location, value == 1.0f));
	}

	GameObject* OnShowTag() override
	{
		GameObject* g = scene->AttachActor(new ItemTag(src_item.Get(), { 13 / 16.0f,1 / 4.0f,1 / 16.0f,1 / 4.0f }));
		g->transform->location = { transform->location.x ,240,0 };
		g->transform->scale = { 16,16,1 };
		return g;
	}
};

class LifeItem :public Item
{
public:

	LifeItem() :Item(false, false, true, false)
	{
		transform->scale = { 32,32,1 };
		sr->SetTexture(src_item.Get());
		sr->SetSourceRect({ 2 / 8.0f,0,1 / 8.0f,1 / 2.0f });
	}

protected:

	void OnEat() override
	{
		se_eat.Play();
		if (playerData.life < 9)
		{
			playerData.life++;
			scene->AttachActor(new Extend);
		}
		else
		{
			AddScore(100000);
			scene->AttachActor(new ItemNumber(100000, transform->location, player->transform->location.y >= player->GetAttribution().itemLine));
		}
	}

	GameObject* OnShowTag() override
	{
		GameObject* g = scene->AttachActor(new ItemTag(src_item.Get(), { 2 / 8.0f,1 / 2.0f,1 / 8.0f,1 / 2.0f }));
		g->transform->location = { transform->location.x ,240,0 };
		g->transform->scale = { 32,32,1 };
		return g;
	}

	void OnCreated() override
	{
		__super::OnCreated();
		se_bonus2.Play();
	}
};

class BombItem :public Item
{
public:

	BombItem() :Item(false, false, true, false)
	{
		transform->scale = { 32,32,1 };
		sr->SetTexture(src_item.Get());
		sr->SetSourceRect({ 4 / 8.0f,0,1 / 8.0f,1 / 2.0f });
	}

protected:

	void OnEat() override
	{
		se_eat.Play();
		if (playerData.bomb < 9)
		{
			playerData.bomb++;
		}
		else
		{
			AddScore(50000);
			scene->AttachActor(new ItemNumber(50000, transform->location, player->transform->location.y >= player->GetAttribution().itemLine));
		}
	}

	GameObject* OnShowTag() override
	{
		GameObject* g = scene->AttachActor(new ItemTag(src_item.Get(), { 4 / 8.0f,1 / 2.0f,1 / 8.0f,1 / 2.0f }));
		g->transform->location = { transform->location.x ,240,0 };
		g->transform->scale = { 32,32,1 };
		return g;
	}
};

class FullPower :public Item
{
public:

	FullPower(bool isPlayerDrop = false) :Item(false, true, true, isPlayerDrop)
	{
		transform->scale = { 32,32,1 };
		sr->SetTexture(src_item.Get());
		sr->SetSourceRect({ 5 / 8.0f,0,1 / 8.0f,1 / 2.0f });
	}

protected:

	void OnEat() override
	{
		se_eat.Play();
		if (playerData.power < 400)
			scene->AttachActor(new FullPowerUp);
		AddScore(10000);
		scene->AttachActor(new ItemNumber(10000, transform->location, player->transform->location.y >= player->GetAttribution().itemLine));
		playerData.power = 400;
	}

	GameObject* OnShowTag() override
	{
		GameObject* g = scene->AttachActor(new ItemTag(src_item.Get(), { 5 / 8.0f,1 / 2.0f,1 / 8.0f,1 / 2.0f }));
		g->transform->location = { transform->location.x ,240,0 };
		g->transform->scale = { 32,32,1 };
		return g;
	}
};