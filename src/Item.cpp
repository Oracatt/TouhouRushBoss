#include "Item.h"
#include "GameScene.h"
#include "GameGlobal.h"
#include "Mathf.h"
#include "UserComponent.h"

Item::Item(bool _highLight, bool _roll, bool gravity, bool _isPlayerDrop) :
	sr(AddComponent(new SpriteRender)), mb(AddComponent(new MoveBody)), highLight(_highLight),
	roll(_roll), rollFrame(24), checkRadius(0), following(false), tag(0), isPlayerDrop(_isPlayerDrop)
{
	mat.ambient = { 0,0,0,0 };
	mat.diffuse = { 1,1,1,1 };
	mat.power = 0;
	mat.specular = { 0,0,0,0 };
	sr->SetMaterial(mat);
	if (gravity)
		mb->force = { 0,-100,0 };
	mb->drag = { 0,1,0 };
}

void Item::OnCreated()
{
	if (highLight)
		InsertBefore(GameSceneCom::locator[15].Get());
	else
		InsertBefore(GameSceneCom::locator[11].Get());
}

void Item::OnDestroy()
{
	if (tag)
	{
		tag->Destroy();
		tag = 0;
	}
}

void Item::OnUpdate()
{
	if (isPlayerDrop)
	{
		const float spd = 200.0f;
		if (spd * timer->GetDeltaTime() > Vector2Distance(target, transform->location))
		{
			transform->location = target;
			isPlayerDrop = false;
			mb->SetActive(true);
		}
		else
		{
			transform->location += (target - transform->location).GetNormalized() * spd * timer->GetDeltaTime();
			mb->SetActive(false);
		}
	}

	if (transform->location.y > 240)
	{
		if (!tag)
			tag = OnShowTag();
		else
			tag->transform->location.x = transform->location.x;
	}
	else
	{
		if (tag)
		{
			tag->Destroy();
			tag = 0;
		}
	}

	if (roll)
	{
		if (rollFrame > 0)
		{
			rollFrame--;
			transform->Rotate({ 0,0,(float)(-1 * M_PI / 4) }, false);
			if (rollFrame == 0)
				transform->SetEulerAngles({ 0,0,0 });
		}
		else
		{
			roll = false;
		}
	}
	else
	{
		float distance = Vector2Distance(player->transform->location, transform->location);
		if (player && (!(player->GetPlayerFlags() & PlayerFlag::PF_Dying)) && (!(player->GetPlayerFlags() & PlayerFlag::PF_Respawning)))
		{
			if (distance < player->GetAttribution().itemRadius)
				following = true;
		}

		if (following)
		{
			const float spd = 400.0f;

			mb->SetActive(false);
			transform->location += (player->transform->location - transform->location).GetNormalized() * spd * timer->GetDeltaTime();
			if (distance * distance < 225 + checkRadius * checkRadius)
			{
				OnEat();
				Destroy();
			}
		}
	}
	if (transform->location.y < -250)
		Destroy();
}

void CollectAllItems()
{
	vector<Item*> items;
	for (LPACTOR g = GameSceneCom::locator[10].Get(); g != static_cast<LPACTOR>(GameSceneCom::locator[11].Get()); g = g->GetNext())
	{
		if (dynamic_cast<Item*>(g.Get()))
			items.push_back(static_cast<Item*>(g.Get()));
	}
	for (LPACTOR g = GameSceneCom::locator[14].Get(); g != static_cast<LPACTOR>(GameSceneCom::locator[15].Get()); g = g->GetNext())
	{
		if (dynamic_cast<Item*>(g.Get()))
			items.push_back(static_cast<Item*>(g.Get()));
	}
	for (auto e : items)
	{
		e->following = true;
	}
}