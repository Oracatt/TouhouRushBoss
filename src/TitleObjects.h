#pragma once
#include "VirtualLib.h"
#include "LevelResources.h"
#include "TitleScene.h"
#include "SceneObjects.h"
#include "Rand.h"
#include "UserComponent.h"
#include "EngineGlobal.h"
#include "BaseObject.h"
#include "UserRenderer.h"

using namespace Global;

class TDelayInvoke :public GameObject
{
public:

	TDelayInvoke(IDelegate* _delegate, int _frame) :
		del(_delegate), limit(_frame), frame(0)
	{
	}

private:

	ComPtr<IDelegate> del;

	int frame;

	int limit;

	void OnUpdate() override
	{
		frame++;
		if (frame >= limit)
		{
			del->Invoke();
			Destroy();
		}
	}

	void OnDestroy() override
	{
		if (del)
		{
			del.Reset();
		}
	}
};

class TDelayCreate :public GameObject
{
public:

	TDelayCreate(GameObject* _g, int _frame) :g(_g), frame(0), limit(_frame)
	{
	}

private:

	int frame;

	int limit;

	LPGOBJ g;

	void OnUpdate() override
	{
		frame++;
		if (frame >= limit)
		{
			scene->AttachActor(g.Get());
			g.Reset();
			Destroy();
		}
	}

	void OnDestroy() override
	{
		if (g)
		{
			g->Destroy();
			g.Reset();
		}
	}
};

class TitleBackground :public AnimObject
{
public:

	TitleBackground()
	{
		transform->scale = { 640,480,1 };
		sr->SetTexture(src_titlebg.Get());
	}

	void OnCreated() override
	{
		InsertBefore(TitleSceneCom::locator[0].Get());
	}
};

class BlackCanvas :public AnimObject
{
public:

	BlackCanvas() : frame(0)
	{
		transform->scale = { 640,480,1 };
		mat.diffuse = { 0,0,0,1 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_dummy.Get());
	}

	void OnCreated() override
	{
		InsertBefore(TitleSceneCom::locator[5].Get());
	}

	void OnUpdate() override
	{
		frame++;
		if (frame <= 40)
		{
			mat.diffuse.a -= 1 / 40.0f;
			sr->SetMaterial(mat);
		}
		else if (frame > 120)
		{
			Destroy();
		}
	}

private:

	int frame;
};

class TitleTag_1 :public AnimObject
{
public:

	TitleTag_1() : frame(0)
	{
		transform->location = { -224,185,0 };
		transform->scale = { 198,129,1 };
		mat.diffuse = { 1,1,1,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_title_1.Get());
	}

private:

	int frame;

	void OnCreated() override
	{
		InsertBefore(TitleSceneCom::locator[1].Get());
		scene->AttachActor(new BlendStateSet(BLEND_STATE_BRIGHTEN))->InsertBefore(this);
		scene->AttachActor(new BlendStateSet(BLEND_STATE_NORMAL))->InsertAfter(this);
	}

	void OnUpdate() override
	{
		frame++;
		if (frame <= 40)
		{
			transform->Translate({ 1,0,0 });
		}

		if (frame <= 20)
		{
			mat.diffuse.a += 0.05;
			sr->SetMaterial(mat);
		}

		transform->Translate({ 0,0.05f * (float)sin(frame / 30.0f),0 });
	}
};

class TitleTag_2 :public AnimObject
{
public:

	TitleTag_2() : frame(0)
	{
		transform->location = { -36,107,0 };
		transform->scale = { 246,66,1 };
		mat.diffuse = { 1,1,1,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_title_2.Get());
	}

private:

	int frame;

	void OnCreated() override
	{
		InsertBefore(TitleSceneCom::locator[1].Get());
		scene->AttachActor(new BlendStateSet(BLEND_STATE_BRIGHTEN))->InsertBefore(this);
		scene->AttachActor(new BlendStateSet(BLEND_STATE_NORMAL))->InsertAfter(this);
	}

	void OnUpdate() override
	{
		frame++;
		if (frame <= 40)
		{
			transform->Translate({ -1,0,0 });
		}

		if (frame <= 20)
		{
			mat.diffuse.a += 0.05;
			sr->SetMaterial(mat);
		}

		transform->Translate({ 0,0.05f * (float)sin(frame / 30.0f),0 });
	}
};

class Title :public MovingObject
{
public:

	Vector3 target;

	bool selected;

	Title(int rectIndex, const Vector3& targetLoc) :
		 frame(0), rolling(false), rollFrame(0), target(targetLoc), selected(false)
	{
		Move(target, 1200, 150);
		transform->scale = { 135,31,1 };
		mat.ambient = { 0.5,0.5,0.5,0 };
		mat.diffuse = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_title.Get());
		sr->SetLocalTransform(XMMatrixTranslation(0.5, 0, 0));
		sr->SetSourceRect({ 22 / 512.0f,(16 + 30 * rectIndex) / 512.0f,135 / 512.0f,31 / 512.0f });
	}

	void Roll()
	{
		rolling = true;
		rollFrame = 0;
	}

private:

	int frame;

	bool rolling;

	int rollFrame;

	void OnUpdate() override
	{
		frame++;
		if (frame <= 40)
		{
			mat.diffuse.a += 0.025;
		}
		if (rolling)
		{
			rollFrame++;
			if (rollFrame <= 15)
			{
				transform->SetEulerAngles({ -(float)M_PI * 2 * rollFrame / 15.0f,0,0 });
			}
			else
			{
				rolling = false;
				transform->SetEulerAngles({ 0,0,0 });
			}
		}
		if (selected)
		{
			mat.ambient.r = mat.ambient.g = mat.ambient.b = 0.88f + 0.12f * (float)sin(6 * timer->GetGameTime());
		}
		else
		{
			mat.ambient.r = mat.ambient.g = mat.ambient.b = 0.5;
		}
		sr->SetMaterial(mat);
	}

	void OnCreated() override
	{
		InsertBefore(TitleSceneCom::locator[1].Get());
	}
};


class TitleStage :public MovingObject
{
public:

	Vector3 target;

	bool selected;

	TitleStage(int rectIndex, const Vector3& targetLoc) :
		frame(0), rolling(false), rollFrame(0), target(targetLoc), selected(false)
	{
		Move(target, 1200, 150);
		transform->scale = { 69,31,1 };
		mat.ambient = { 0.5,0.5,0.5,0 };
		mat.diffuse = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_title.Get());
		sr->SetSourceRect({ 174 / 512.0f,(16 + 30 * rectIndex) / 512.0f,69 / 512.0f,31 / 512.0f });
	}

	void Roll()
	{
		rolling = true;
		rollFrame = 0;
	}

private:

	int frame;

	bool rolling;

	int rollFrame;

	void OnUpdate() override
	{
		frame++;
		if (frame <= 40)
		{
			mat.diffuse.a += 0.025;
		}
		if (rolling)
		{
			rollFrame++;
			if (rollFrame <= 15)
			{
				transform->SetEulerAngles({ -(float)M_PI * 2 * rollFrame / 15.0f,0,0 });
			}
			else
			{
				rolling = false;
				transform->SetEulerAngles({ 0,0,0 });
			}
		}
		if (selected)
		{
			mat.ambient.r = mat.ambient.g = mat.ambient.b = 0.88f + 0.12f * (float)sin(6 * timer->GetGameTime());
		}
		else
		{
			mat.ambient.r = mat.ambient.g = mat.ambient.b = 0.5;
		}
		sr->SetMaterial(mat);
	}

	void OnCreated() override
	{
		InsertBefore(TitleSceneCom::locator[4].Get());
	}
};

class TitleParticle :public GameObject
{
public:

	SpriteRender* sr;

	Material mat;

	MoveBody* mb;

	TitleParticle() :
		sr(0), mb(AddComponent(new MoveBody)), frame(0)
	{
		float size = Random(4.0f, 12.0f);
		transform->scale = { size,size,1 };
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,0.75 };
		mat.specular = { 0,0,0,0 };
		mat.power = 0;
		AddComponent(new BlendStateSet::Setter(BLEND_STATE_BRIGHTEN));
		sr = AddComponent(new SpriteRender);
		sr->SetMaterial(mat);
		sr->SetTexture(src_light.Get());
		sr->SetSourceRect({ 0,0.5f,0.18f,0.36f });
		AddComponent(new BlendStateSet::Setter(BLEND_STATE_NORMAL));
		mb->drag = { 0.5,0.5,0.5 };
		mb->velocity = { Random(-25.0f,25.0f),Random(25.0f,35.0f),0 };
		mb->force = { Random(-15.0f,15.0f),Random(25.0f,35.0f) ,0 };
	}

private:

	int frame;

	void OnCreated() override
	{
		InsertBefore(TitleSceneCom::locator[0].Get());
	}

	void OnUpdate() override
	{
		frame++;
		if (frame > 270)
		{
			mat.diffuse.a -= 0.025;
			sr->SetMaterial(mat);
		}
		if (frame == 300)
		{
			Destroy();
		}
	}
};

class TitleParticleSpawner :public GameObject
{
public:

	int frame;

	TitleParticleSpawner() :frame(0)
	{
	}

	void OnCreated() override
	{
		InsertBefore(TitleSceneCom::locator[0].Get());
	}

	void OnUpdate() override
	{
		frame++;
		if (frame % 2 == 0)
			scene->AttachActor(new TitleParticle)->transform->location = { Random(-360.0f,360.0f),-252,0 };
	}
};

class SelectTag :public MovingObject
{
public:

	SelectTag(int index)
	{
		transform->scale = { 577 * 0.5f,785 * 0.5f / 3.0,1 };
		sr->SetTexture(src_select.Get());
		sr->SetSourceRect({ 0,index / 3.0f,1,1 / 3.0f });
	}
};

class DifficultyTagAnim :public TransformingObject
{
public:

	int d;

	DifficultyTagAnim(int _d) :d(_d)
	{
		transform->scale = { 477 * 0.8f,170 * 0.56f,1 };
		mat.diffuse.a = 0.5;
		sr->SetMaterial(mat);
		sr->SetSourceRect({ 0,d / 4.0f,1,7 / 40.0f });
		sr->SetTexture(src_difficulty.Get());
	}

	void Leave()
	{
		__super::Leave(15, 0.1f);
		Transform({ 477 * 0.8f,170 * 0.56f,1 }, { 20,0,0 }, 15);
	}

private:

	void OnCreated() override
	{
		InsertBefore(TitleSceneCom::locator[4].Get());
		Enter(15, 1);
		Transform({ 477 * 0.4f,170 * 0.28f,1 }, { 0,-200,0 }, 15);
	}
};

class SpriteTextTitle :public SimpleMovingObject
{
public:

	int frame;

	SpriteTextRender* sr;

	SpriteTextTitle() : frame(0), sr(AddComponent(new SpriteTextRender))
	{
		transform->scale = { 15,15,1 };
		mb->SetUpdateLevel(1);
	}

	void OnCreated() override
	{
		InsertBefore(TitleSceneCom::locator[4].Get());
	}
};