#pragma once
#include "VirtualLib.h"
#include "GameScene.h"

#define MT_RIGHTTOLEFT 1

class BossSprite :public GameObject
{
public:

	BossSprite(BYTE _moveType) :
		sr(AddComponent(new SpriteRender)), frame(0), moveType(_moveType)
	{
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,0 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		if (moveType == MT_RIGHTTOLEFT)
			transform->location = { 200,30,0 };
	}

protected:

	SpriteRender* sr;

	Material mat;

	int frame;

	BYTE moveType;

private:

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::locator[21].Get());
	}

	void OnUpdate() override
	{
		frame++;
		if (moveType == MT_RIGHTTOLEFT)
		{
			if (frame <= 10)
			{
				mat.diffuse.a += 0.1;
				sr->SetMaterial(mat);
				transform->Translate({ -15,-3,0 });
			}
			else if (frame <= 80)
			{
				transform->Translate({ -1,-0.2,0 });
			}
			else if (frame <= 90)
			{
				mat.diffuse.a -= 0.1;
				sr->SetMaterial(mat);
				transform->Translate({ -15,-3,0 });
			}
			else
			{
				Destroy();
			}
		}
	}
};