#include "EnemyBullet.h"
#include "GameGlobal.h"
#include "Mathf.h"
#include "GameScene.h"
#include "Effect.h"
#include "Rand.h"
#include "ItemDeriver.h"

EnemyBullet::EnemyBullet(BYTE _autoRotateMode, bool _highLight) :
	Bullet(_autoRotateMode), grazed(false), checkRadius(0),
	highLight(_highLight), colorIndex(-1), breakColor(0), breakSize(0.4) {}

void EnemyBullet::OnCreated()
{
	if (colorIndex == -1)
		SetColor(0);
	if (highLight)
		InsertBefore(GameSceneCom::locator[12].Get());
	else
		InsertBefore(GameSceneCom::locator[16].Get());
}

void EnemyBullet::OnCheck()
{
	if (player && (!(player->GetCharacterFlags() & CF_ImmuneDamage)))
	{
		float distance = Vector2Distance(player->transform->location, transform->location);
		if (distance * distance < player->GetAttribution().checkRadius * player->GetAttribution().checkRadius + checkRadius * checkRadius)
		{
			OnHit(player.Get());
		}
		else if ((!grazed) && (distance * distance < player->GetAttribution().grazeRadius * player->GetAttribution().grazeRadius + checkRadius * checkRadius))
		{
			OnGrazed();
		}
	}
}

void EnemyBullet::OnHit(Character* character)
{
	static_cast<Player*>(character)->Miss();
	if (cleanOnHit)
		Clean(CLEAN_HIT);
}

void EnemyBullet::OnGrazed()
{
	grazed = true;
	playerData.maxPoint += 10;
	auto g = scene->AttachActor(new PlayerGrazeStar);
	g->transform->location = player->transform->location;
	g->mb->velocity = GetVector(RandomAngle(), Random(360, 400));
	se_graze.Play();
	scene->AttachActor(new ScorePoint(true))->transform->location = transform->location;
}

void EnemyBullet::Clean(BYTE cleanType)
{
	if (!IsValid())
		return;
	if (cleanOnHit&&cleanType == CLEAN_HIT)
	{
		Destroy();
		float angleZ[2] = { Random(0.0, (float)M_PI * 2), Random(0.0, (float)M_PI * 2) };
		int cw = Random(0, 1);
		for (int i = 0; i < 2; i++)
		{
			auto eff = scene->AttachActor(new EnemyBulletBreakEffect(breakColor[colorIndex], i * 4));
			eff->transform->location = transform->location;
			eff->transform->scale = { 64 - 16.0f * i ,64 - 16.0f * i,1 };
			eff->transform->scale *= breakSize;
			eff->transform->SetEulerAngles({ 0,((i == cw) ? 0.0f : (float)M_PI), angleZ[i] });
		}
		auto eff = scene->AttachActor(new EnemyBulletBreakCharge(breakColor[colorIndex]));
		eff->transform->location = transform->location;
		eff->transform->scale = { 32,32 ,1 };
		eff->transform->scale *= breakSize;
	}
	else if (cleanOnOutOfRange && cleanType == CLEAN_OUTOFRANGE)
	{
		Destroy();
	}
	else if (cleanOnBomb && cleanType == CLEAN_BOMB)
	{
		Destroy();
		float angleZ[2] = { Random(0.0, (float)M_PI * 2), Random(0.0, (float)M_PI * 2) };
		int cw = Random(0, 1);
		for (int i = 0; i < 2; i++)
		{
			auto eff = scene->AttachActor(new EnemyBulletBreakEffect(breakColor[colorIndex], i * 4));
			eff->transform->location = transform->location;
			eff->transform->scale = { 64 - 16.0f * i ,64 - 16.0f * i,1 };
			eff->transform->scale *= breakSize;
			eff->transform->SetEulerAngles({ 0,((i == cw) ? 0.0f : (float)M_PI), angleZ[i] });
		}
		auto eff = scene->AttachActor(new EnemyBulletBreakCharge(breakColor[colorIndex]));
		eff->transform->location = transform->location;
		eff->transform->scale = { 32,32 ,1 };
		eff->transform->scale *= breakSize;
		scene->AttachActor(new ScorePoint)->transform->location = transform->location;
	}
	else if (cleanType == CLEAN_BONUS)
	{
		Destroy();
		float angleZ[2] = { Random(0.0, (float)M_PI * 2), Random(0.0, (float)M_PI * 2) };
		int cw = Random(0, 1);
		for (int i = 0; i < 2; i++)
		{
			auto eff = scene->AttachActor(new EnemyBulletBreakEffect(breakColor[colorIndex], i * 4));
			eff->transform->location = transform->location;
			eff->transform->scale = { 64 - 16.0f * i ,64 - 16.0f * i,1 };
			eff->transform->scale *= breakSize;
			eff->transform->SetEulerAngles({ 0,((i == cw) ? 0.0f : (float)M_PI), angleZ[i] });
		}
		auto eff = scene->AttachActor(new EnemyBulletBreakCharge(breakColor[colorIndex]));
		eff->transform->location = transform->location;
		eff->transform->scale = { 32,32 ,1 };
		eff->transform->scale *= breakSize;
		scene->AttachActor(new ScorePoint)->transform->location = transform->location;
	}
}

float EnemyBullet::GetCheckRadius()
{
	return checkRadius;
}

class BulletCleaner :public GameObject
{
public:

	BulletCleaner(UINT _cleanType, UINT durframe, float _rangeSpeed) :
		cleanType(_cleanType), remainFrame(durframe), range(0), rangeSpeed(_rangeSpeed)
	{
	}

private:

	UINT cleanType;

	int remainFrame;

	float range;

	float rangeSpeed;

	void OnUpdate() override
	{
		remainFrame--;
		range += rangeSpeed;
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
			if ((e->transform->location - transform->location).GetLength() <= range)
				e->Clean(cleanType);
		}
		if (remainFrame <= 0)
			Destroy();
	}

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::locator[24].Get());
	}
};

void CleanAllEnemyBullets(UINT cleanType, UINT durframe, float rangeSpeed, const Vector2& loc)
{
	if (durframe == 0)
	{
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
			e->Clean(cleanType);
	}
	else
	{
		player->GetScene()->AttachActor(new BulletCleaner(cleanType, durframe, rangeSpeed))->transform->location = loc;
	}
}

void CleanAuto(UINT autoCleanType, const Vector2& loc)
{
	if (autoCleanType == CLEAN_NOSPELL)
	{
		CleanAllEnemyBullets(CLEAN_BONUS);
	}
	else if (autoCleanType == CLEAN_SPELL)
	{
		CleanAllEnemyBullets(CLEAN_BONUS, 90, 20, loc);
	}
	else if (autoCleanType == CLEAN_FINALSPELL)
	{
		CleanAllEnemyBullets(CLEAN_BONUS, 90, 10, loc);
	}
}