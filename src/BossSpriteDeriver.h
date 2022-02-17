#pragma once
#include "BossSprite.h"
#include "Source.h"

class Artia_Sprite :public BossSprite
{
public:

	Artia_Sprite() :
		BossSprite(MT_RIGHTTOLEFT)
	{
		transform->scale = { 270,480,1 };
		sr->SetTexture(src_artiaface_ct.Get());
	}
};

class Monstone_Sprite :public BossSprite
{
public:

	Monstone_Sprite() :
		BossSprite(MT_RIGHTTOLEFT)
	{
		transform->scale = { 476,404,1 };
		sr->SetTexture(src_monstone_ct.Get());
	}
};

class SunnyMilk_Sprite :public BossSprite
{
public:

	SunnyMilk_Sprite() :
		BossSprite(MT_RIGHTTOLEFT)
	{
		transform->scale = { 383,448,1 };
		sr->SetTexture(src_sunny_ct.Get());
	}
};