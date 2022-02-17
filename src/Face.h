#pragma once
#include "VirtualLib.h"
#include "GameScene.h"
#include "Source.h"
#include "SceneObjects.h"

enum FaceType
{
	FACE_HAPPY = 0,
	FACE_DISAPPOINT = 1,
	FACE_SWEAT = 2,
	FACE_ANGRY = 3,
	FACE_ANGRY2 = 4,
	FACE_SURPRISE = 5,
	FACE_PUZZLED = 6,
	FACE_NOTICE = 7,
	FACE_NOTICE2 = 8,
	FACE_LOSE = 9
};

class Face :public GameObject
{
public:

	class SubFace :public GameObject
	{
	public:

		SpriteRender* sr;

		Material mat;

		SubFace() :sr(AddComponent(new SpriteRender))
		{
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = { 0.64,0.64,0.64,1 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
		}
	};

	Face(bool _isLeft) :isLeft(_isLeft), leaveFrame(-1), actFrame(-1), activated(false)
	{
	}

	void SetFaceType(FaceType type)
	{
		OnSetFaceType(type);
	}

	virtual void Leave()
	{
		leaveFrame = 15;
	}

	virtual void Activate()
	{
		if (!activated && IsValid())
		{
			while (actFrame > 0)
			{
				actFrame--;
				if (isLeft)
				{
					face->transform->location.x -= 1.5;
					face->transform->location.y -= 0.5;
					body->transform->location.x -= 1.5;
					body->transform->location.y -= 0.5;
				}
				else
				{
					face->transform->location.x += 1.5;
					face->transform->location.y -= 0.5;
					body->transform->location.x += 1.5;
					body->transform->location.y -= 0.5;
				}
				if (actFrame >= 6)
				{
					face->mat.diffuse.r = face->mat.diffuse.g = face->mat.diffuse.b -= 0.06;
					face->sr->SetMaterial(face->mat);
					body->mat.diffuse.r = body->mat.diffuse.g = body->mat.diffuse.b -= 0.06;
					body->sr->SetMaterial(body->mat);
				}
			}
			activated = true;
			actFrame = 12;
		}
	}

	virtual void Deactivate()
	{
		if (activated && IsValid())
		{
			while (actFrame > 0)
			{
				actFrame--;
				if (isLeft)
				{
					face->transform->location.x += 1.5;
					face->transform->location.y += 0.5;
					body->transform->location.x += 1.5;
					body->transform->location.y += 0.5;
				}
				else
				{
					face->transform->location.x -= 1.5;
					face->transform->location.y += 0.5;
					body->transform->location.x -= 1.5;
					body->transform->location.y += 0.5;
				}
				if (actFrame >= 6)
				{
					face->mat.diffuse.r = face->mat.diffuse.g = face->mat.diffuse.b += 0.06;
					face->sr->SetMaterial(face->mat);
					body->mat.diffuse.r = body->mat.diffuse.g = body->mat.diffuse.b += 0.06;
					body->sr->SetMaterial(body->mat);
				}
			}
			activated = false;
			actFrame = 12;
		}
	}

protected:

	bool isLeft;

	bool activated;

	ComPtr<SubFace> body;

	ComPtr<SubFace> face;

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::locator[21].Get());
		body = scene->AttachActor(new SubFace);
		face = scene->AttachActor(new SubFace);
		body->InsertBefore(this);
		face->InsertBefore(this);
	}

	void OnDestroy() override
	{
		if (body)
		{
			body->Destroy();
			body.Reset();
		}
		if (face)
		{
			face->Destroy();
			face.Reset();
		}
	}

	virtual void OnSetFaceType(FaceType type) = 0;

private:

	int leaveFrame;

	int actFrame;

	void OnUpdate() override
	{
		if (actFrame > 0)
		{
			actFrame--;
			if (activated)
			{
				if (isLeft)
				{
					face->transform->location.x += 1.5;
					face->transform->location.y += 0.5;
					body->transform->location.x += 1.5;
					body->transform->location.y += 0.5;
				}
				else
				{
					face->transform->location.x -= 1.5;
					face->transform->location.y += 0.5;
					body->transform->location.x -= 1.5;
					body->transform->location.y += 0.5;
				}
				if (actFrame >= 6)
				{
					face->mat.diffuse.r = face->mat.diffuse.g = face->mat.diffuse.b += 0.06;
					face->sr->SetMaterial(face->mat);
					body->mat.diffuse.r = body->mat.diffuse.g = body->mat.diffuse.b += 0.06;
					body->sr->SetMaterial(body->mat);
				}
			}
			else
			{
				if (isLeft)
				{
					face->transform->location.x -= 1.5;
					face->transform->location.y -= 0.5;
					body->transform->location.x -= 1.5;
					body->transform->location.y -= 0.5;
				}
				else
				{
					face->transform->location.x += 1.5;
					face->transform->location.y -= 0.5;
					body->transform->location.x += 1.5;
					body->transform->location.y -= 0.5;
				}
				if (actFrame >= 6)
				{
					face->mat.diffuse.r = face->mat.diffuse.g = face->mat.diffuse.b -= 0.06;
					face->sr->SetMaterial(face->mat);
					body->mat.diffuse.r = body->mat.diffuse.g = body->mat.diffuse.b -= 0.06;
					body->sr->SetMaterial(body->mat);
				}
			}
		}
		if (leaveFrame > 0)
		{
			leaveFrame--;
			if (isLeft)
			{
				face->transform->location.x -= 2;
				body->transform->location.x -= 2;
			}
			else
			{
				face->transform->location.x += 2;
				body->transform->location.x += 2;
			}
			face->mat.diffuse.a -= 0.07;
			face->sr->SetMaterial(face->mat);
			body->mat.diffuse.a -= 0.07;
			body->sr->SetMaterial(body->mat);
			if (!leaveFrame)
				Destroy();
		}
	}
};

class Face_Reimu :public Face
{
public:

	Face_Reimu() :Face(true) {}

	void OnCreated() override
	{
		__super::OnCreated();
		body->transform->location = { -180,-60,0 };
		body->transform->scale = { 337.5,400,1 }; //0.6x
		face->transform->location = { -218,26,0 };
		face->transform->scale = { 132.5,117.5,1 };
	}

	void OnSetFaceType(FaceType type) override
	{
		body->sr->SetTexture(src_reimuface_bs.Get());
		face->sr->SetTexture(src_reimuface[type].Get());
	}
};

class Face_Marisa :public Face
{
public:

	Face_Marisa() :Face(true) {}

	void OnCreated() override
	{
		__super::OnCreated();
		body->transform->location = { -168,-95,0 };
		body->transform->scale = { 340,355,1 }; //0.5x
		face->transform->location = { -189,28,0 };
		face->transform->scale = { 131,108.5,1 }; 
	}

	void OnSetFaceType(FaceType type) override
	{
		body->sr->SetTexture(src_marisaface_bs.Get());
		face->sr->SetTexture(src_marisaface[type].Get());
	}
};

class Face_Artia :public Face
{
public:

	Face_Artia() :Face(false) {}

	void OnCreated() override
	{
		__super::OnCreated();
		body->transform->location = { 196,-95,0 };
		body->transform->scale = { 259.2,360,1 }; //0.36x
		face->sr->SetActive(false);
	}

	void OnSetFaceType(FaceType type) override
	{
		body->sr->SetTexture(src_artiaface[type].Get());
	}
};

class Face_Monstone :public Face
{
public:

	Face_Monstone() :Face(false) {}

	void OnCreated() override
	{
		__super::OnCreated();
		body->transform->location = { 196,-105,0 };
		body->transform->scale = { 476 * 0.84f,404 * 0.84f,1 };
		face->sr->SetActive(false);
	}

	void OnSetFaceType(FaceType type) override
	{
		body->sr->SetTexture(src_monstone_ct.Get());
	}
};

class Face_SunnyMilk :public Face
{
public:

	Face_SunnyMilk() :Face(false) {}

	void OnCreated() override
	{
		__super::OnCreated();
		body->transform->location = { 196,80,0 };
		body->transform->scale = { 403 * 0.85f,416 * 0.85f,1 };
		body->sr->SetLocalTransform(XMMatrixTranslation(0, -0.5, 0));
		face->transform->location = { 196,80,0 };
		face->transform->scale = { 403 * 0.85f,288 * 0.85f,1 };
		face->sr->SetLocalTransform(XMMatrixTranslation(0, -0.5, 0));
	}

	void OnSetFaceType(FaceType type) override
	{
		body->sr->SetTexture(src_sunnyface_bs.Get());
		if (type == FACE_LOSE)
			face->transform->scale = { 403 * 0.85f,416 * 0.85f,1 };
		else
			face->transform->scale = { 403 * 0.85f,288 * 0.85f,1 };
		face->sr->SetTexture(src_sunnyface[type].Get());
	}
};