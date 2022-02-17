#pragma once
#include "VirtualLib.h"
#include "GameScene.h"
#include "Source.h"
#include "GameGlobal.h"

class UI :public GameObject
{
public:

	UI() :sr(AddComponent(new SpriteRender))
	{
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,1 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
	}

protected:

	Material mat;

	SpriteRender* sr;

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::locator[20].Get());
	}
};