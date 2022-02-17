#pragma once
#include "VirtualLib.h"
#include "GameScene.h"
#include "SceneObjects.h"
#include "Source.h"
#include "EngineGlobal.h"

class BalloonPart :public GameObject
{
public:

	BalloonPart() :sr(AddComponent(new SpriteRender))
	{
		Material mat;
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,1 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetLocalTransform(XMMatrixTranslation(0.5, -0.5, 0));
	}

protected:

	SpriteRender* sr;
};

class Balloon :public GameObject
{
public:
	
	Balloon(const Vector2& _balloonLoc, int _balloonLen, bool _inv = false) :
		sr(AddComponent(new SpriteRender)), balloonLoc(_balloonLoc), balloonLen(_balloonLen),
		inv(_inv), text_tr(0), rect({ 0,0,100,100 })
	{
		transform->scale = { 640,480,1 };
		Material mat;
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,1 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(GameSceneCom::balloonTarget.Get());
	}

	void SetText(const wstring& _str)
	{
		if (text_tr)
			text_tr->SetText(_str.c_str());
		else
			str = _str;
	}

	void SetRect(const D2D1_RECT_F& _rect)
	{
		if (text_tr)
			text_tr->SetRect(_rect);
		else
			rect = _rect;
	}

protected:

	LPGOBJ rtSet[2];

	LPGOBJ highLightSet[2]; //气泡都插入到1前

	LPGOBJ text;

	TextRender2D* text_tr;

	SpriteRender* sr;

	Vector2 balloonLoc;

	int balloonLen;

	bool inv;

	wstring str;

	D2D1_RECT_F rect;

	vector<ComPtr<BalloonPart>> parts;

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::locator[22].Get());
		rtSet[0] = scene->AttachActor(new RenderTargetSet(GameSceneCom::balloonTarget.Get(), 0));
		rtSet[0]->InsertBefore(this);
		highLightSet[0] = scene->AttachActor(new BlendStateSet(BLEND_STATE_BRIGHTEN));
		highLightSet[0]->InsertBefore(this);
		highLightSet[1] = scene->AttachActor(new BlendStateSet(BLEND_STATE_NORMAL));
		highLightSet[1]->InsertBefore(this);
		text = scene->AttachActor(new GameObject);
		text->transform2D->location = { balloonLoc.x + 320,240 - balloonLoc.y };
		text_tr = text->AddComponent(new TextRender2D(false));
		text_tr->SetTextFormat(tf_wryh_big.Get());
		text_tr->SetBodyBrush(sb_black.Get());
		text_tr->SetText(str);
		text_tr->SetRect(rect);
		text->InsertBefore(this);
		rtSet[1] = scene->AttachActor(new RenderTargetSet(GameSceneCom::gameTarget.Get(), 0, false));
		rtSet[1]->InsertBefore(this);
	}

	void OnDestroy() override
	{
		for (int i = 0; i < 2; i++)
		{
			if (rtSet[i])
			{
				rtSet[i]->Destroy();
				rtSet[i].Reset();
			}
			if (highLightSet[i])
			{
				highLightSet[i]->Destroy();
				highLightSet[i].Reset();
			}
		}
		for (auto part : parts)
		{
			if (part)
				part->Destroy();
		}
		if (text)
		{
			text->Destroy();
			text.Reset();
		}
		parts.clear();
	}

	BalloonPart* AddPart(BalloonPart* part)
	{
		auto p = scene->AttachActor(part);
		p->InsertBefore(highLightSet[1].Get());
		parts.push_back(p);
		return p;
	}
};

class Balloon_1 :public Balloon
{
	class PartL :public BalloonPart
	{
	public:

		PartL(bool inv)
		{
			transform->scale = { inv ? -74.0f : 74.0f,96,1 };
			sr->SetTexture(src_balloon_1.Get());
			sr->SetSourceRect({ 96 / 256.0f,0,74 / 256.0f,96 / 1024.0f });
		}
	};

	class PartM :public BalloonPart
	{
	public:

		PartM(bool inv)
		{
			transform->scale = { inv ? -32.0f : 32.0f,64,1 };
			sr->SetTexture(src_balloon.Get());
			sr->SetSourceRect({ 0, 32 / 1024.0f, 1, 64 / 1024.0f });
		}
	};

	class PartR :public BalloonPart
	{
	public:

		PartR(bool inv)
		{
			transform->scale = { inv ? -32.0f : 32.0f,64,1 };
			sr->SetTexture(src_balloon_1.Get());
			sr->SetSourceRect({ 0,32 / 1024.0f,32 / 256.0f,64 / 1024.0f });
		}
	};

public:

	Balloon_1(const Vector2& loc, int len, bool _inv = false) :Balloon(loc, len, _inv)
	{
		if (!inv)
			SetRect({ 32,44,75 + 32.0f * len,100 });
		else
			SetRect({ -75 - 32.0f * len,44,-32,100 });
	}

private:

	void OnCreated() override
	{
		__super::OnCreated();
		Vector2 baseLoc = balloonLoc;
		if (!inv)
		{
			AddPart(new PartL(inv))->transform->location = baseLoc;
			baseLoc.x += 70;
			baseLoc.y -= 32;
			for (int i = 0; i < balloonLen; i++)
			{
				AddPart(new PartM(inv))->transform->location = baseLoc;
				baseLoc.x += 32;
			}
			AddPart(new PartR(inv))->transform->location = baseLoc;
		}
		else
		{
			AddPart(new PartL(inv))->transform->location = baseLoc;
			baseLoc.x -= 70;
			baseLoc.y -= 32;
			for (int i = 0; i < balloonLen; i++)
			{
				AddPart(new PartM(inv))->transform->location = baseLoc;
				baseLoc.x -= 32;
			}
			AddPart(new PartR(inv))->transform->location = baseLoc;
		}
	}
};

class Balloon_2 :public Balloon
{
	class PartL :public BalloonPart
	{
	public:

		PartL(bool inv)
		{
			transform->scale = { inv ? -92.0f : 92.0f,112,1 };
			sr->SetTexture(src_balloon_1.Get());
			sr->SetSourceRect({ 78 / 256.0f,96/1024.0f,92 / 256.0f,112 / 1024.0f });
		}
	};

	class PartM :public BalloonPart
	{
	public:

		PartM(bool inv)
		{
			transform->scale = { inv ? -32.0f : 32.0f,112,1 };
			sr->SetTexture(src_balloon.Get());
			sr->SetSourceRect({ 0, 111 / 1024.0f, 1, 112.0f / 1024.0f });
		}
	};

	class PartR :public BalloonPart
	{
	public:
		PartR(bool inv)
		{
			transform->scale = { inv ? -48.0f : 48.0f,99,1 };
			sr->SetTexture(src_balloon_1.Get());
			sr->SetSourceRect({ 0,112 / 1024.0f,48 / 256.0f,99 / 1024.0f });
		}
	};

public:

	Balloon_2(const Vector2& loc, int len, bool _inv = false) :Balloon(loc, len, _inv)
	{
		if (!inv)
			SetRect({ 40,44,90 + 32.0f * len,100 });
		else
			SetRect({ -90 - 32.0f * len,44,-40,100 });
	}

private:

	void OnCreated() override
	{
		__super::OnCreated();
		Vector2 baseLoc = balloonLoc;
		if (!inv)
		{
			AddPart(new PartL(inv))->transform->location = baseLoc;
			baseLoc.x += 88;
			baseLoc.y -= 15;
			for (int i = 0; i < balloonLen; i++)
			{
				AddPart(new PartM(inv))->transform->location = baseLoc;
				baseLoc.x += 32;
			}
			baseLoc.y -= 1;
			AddPart(new PartR(inv))->transform->location = baseLoc;
		}
		else
		{
			AddPart(new PartL(inv))->transform->location = baseLoc;
			baseLoc.x -= 88;
			baseLoc.y -= 15;
			for (int i = 0; i < balloonLen; i++)
			{
				AddPart(new PartM(inv))->transform->location = baseLoc;
				baseLoc.x -= 32;
			}
			baseLoc.y -= 1;
			AddPart(new PartR(inv))->transform->location = baseLoc;
		}
	}
};

class Balloon_3 :public Balloon
{
	class PartL :public BalloonPart
	{
	public:

		PartL(bool inv)
		{
			transform->scale = { inv ? -80.0f : 80.0f,96,1 };
			sr->SetTexture(src_balloon_1.Get());
			sr->SetSourceRect({ 90 / 256.0f,231 / 1024.0f,80 / 256.0f,96 / 1024.0f });
		}
	};

	class PartM :public BalloonPart
	{
	public:

		PartM(bool inv)
		{
			transform->scale = { inv ? -32.0f : 32.0f,68,1 };
			sr->SetTexture(src_balloon.Get());
			sr->SetSourceRect({ 0, 254 / 1024.0f, 1, 68.0f / 1024.0f });
		}
	};

	class PartR :public BalloonPart
	{
	public:
		PartR(bool inv)
		{
			transform->scale = { inv ? -38.0f : 38.0f,68,1 };
			sr->SetTexture(src_balloon_1.Get());
			sr->SetSourceRect({ 0,253 / 1024.0f,38 / 256.0f,68 / 1024.0f });
		}
	};

public:

	Balloon_3(const Vector2& loc, int len, bool _inv = false) :Balloon(loc, len, _inv)
	{
		if (!inv)
			SetRect({ 26,38,90 + 32.0f * len,100 });
		else
			SetRect({ -90 - 32.0f * len,38,-26,100 });
	}

private:

	void OnCreated() override
	{
		__super::OnCreated();
		Vector2 baseLoc = balloonLoc;
		if (!inv)
		{
			AddPart(new PartL(inv))->transform->location = baseLoc;
			baseLoc.x += 76;
			baseLoc.y -= 23;
			for (int i = 0; i < balloonLen; i++)
			{
				AddPart(new PartM(inv))->transform->location = baseLoc;
				baseLoc.x += 32;
			}
			baseLoc.y += 1;
			AddPart(new PartR(inv))->transform->location = baseLoc;
		}
		else
		{
			AddPart(new PartL(inv))->transform->location = baseLoc;
			baseLoc.x -= 76;
			baseLoc.y -= 23;
			for (int i = 0; i < balloonLen; i++)
			{
				AddPart(new PartM(inv))->transform->location = baseLoc;
				baseLoc.x -= 32;
			}
			baseLoc.y += 1;
			AddPart(new PartR(inv))->transform->location = baseLoc;
		}
	}
};

class Balloon_4 :public Balloon
{
	class PartL :public BalloonPart
	{
	public:
		
		PartL(bool inv)
		{
			transform->scale = { inv ? -80.0f : 80.0f,96,1 };
			sr->SetTexture(src_balloon_1.Get());
			sr->SetSourceRect({ 90 / 256.0f,343 / 1024.0f,80 / 256.0f,96 / 1024.0f });
		}
	};

	class PartM :public BalloonPart
	{
	public:

		PartM(bool inv)
		{
			transform->scale = { inv ? -32.0f : 32.0f,68,1 };
			sr->SetTexture(src_balloon.Get());
			sr->SetSourceRect({ 0, 366 / 1024.0f, 1, 68.0f / 1024.0f });
		}
	};

	class PartR :public BalloonPart
	{
	public:
		PartR(bool inv)
		{
			transform->scale = { inv ? -38.0f : 38.0f,68,1 };
			sr->SetTexture(src_balloon_1.Get());
			sr->SetSourceRect({ 0,365 / 1024.0f,38 / 256.0f,68 / 1024.0f });
		}
	};

public:

	Balloon_4(const Vector2& loc, int len, bool _inv = false) :Balloon(loc, len, _inv)
	{
		if (!inv)
			SetRect({ 26,38,90 + 32.0f * len,100 });
		else
			SetRect({ -90 - 32.0f * len,38,-26,100 });
	}

private:

	void OnCreated() override
	{
		__super::OnCreated();
		Vector2 baseLoc = balloonLoc;
		if (!inv)
		{
			AddPart(new PartL(inv))->transform->location = baseLoc;
			baseLoc.x += 76;
			baseLoc.y -= 23;
			for (int i = 0; i < balloonLen; i++)
			{
				AddPart(new PartM(inv))->transform->location = baseLoc;
				baseLoc.x += 32;
			}
			baseLoc.y += 1;
			AddPart(new PartR(inv))->transform->location = baseLoc;
		}
		else
		{
			AddPart(new PartL(inv))->transform->location = baseLoc;
			baseLoc.x -= 76;
			baseLoc.y -= 23;
			for (int i = 0; i < balloonLen; i++)
			{
				AddPart(new PartM(inv))->transform->location = baseLoc;
				baseLoc.x -= 32;
			}
			baseLoc.y += 1;
			AddPart(new PartR(inv))->transform->location = baseLoc;
		}
	}
};