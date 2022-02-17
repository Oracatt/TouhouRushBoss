#pragma once
#include "BackGround.h"
#include "Source.h"
#include "LevelResources.h"
#include "SceneObjects.h"

using namespace Global;

class FrozenForest :public BackGround
{
public:

	LPGOBJ ground;

	LPGOBJ tree;

	LPGOBJ snow;

	LPGOBJ canvas;

	FrozenForest() :groundz(0), treez(0), frame(0) {}
	
protected:

	float groundz;

	float treez;

	int frame;

	void OnCreated() override
	{
		__super::OnCreated();

		//SetFogColor({ 0.8,0.8,0.9,1 });
		SetFogColor({ 0,0,0,1 });
		SetFogStart(4);
		SetFogRange(35);
		SetFogEnabled(true);

		GameSceneCom::backcam->transform->location = { 0,0,0 };
		GameSceneCom::backcam->transform->SetEulerAngles({ (float)M_PI / 7.0f,0,0 });

		ground = CreateObject(new GameObject);
		tree = CreateObject(new GameObject);
		snow = CreateObject(new GameObject);
		canvas = CreateObject(new GameObject);
		ground->InsertBefore(GameSceneCom::locator[1].Get());
		tree->InsertBefore(GameSceneCom::locator[1].Get());
		snow->InsertBefore(GameSceneCom::locator[2].Get());
		canvas->InsertBefore(GameSceneCom::locator[3].Get());
	}

	void OnUpdate() override
	{
		if (frame == 0)
		{
			auto g = CreateObject(new Canvas);
			g->InsertAfter(canvas.Get());
		}

		frame++;

		GameSceneCom::backcam->transform->location.z += (float)(5 * timer->GetDeltaTime());

		while (groundz - GameSceneCom::backcam->transform->location.z < 100)
		{
			for (int x = -3; x <= 3; x++)
			{
				auto g = CreateObject(new Ground);
				g->InsertAfter(ground.Get());
				g->transform->location = { 10.0f * x,-12,groundz };
			}
			groundz += 10;
		}

		while (treez - GameSceneCom::backcam->transform->location.z < 100)
		{
			float x = Random(-14.0f, -3.0f);
			GameObject* g = CreateObject(new Grass);
			g->InsertAfter(tree.Get());
			g->transform->location = { x,-12,treez };
			g = CreateObject(new Leaf);
			g->InsertAfter(tree.Get());
			g->transform->location = { x,-12,treez };
			g = CreateObject(new Tree);
			g->InsertAfter(tree.Get());
			g->transform->location = { x,-12,treez };
			x = Random(3.0f, 14.0f);
			g = CreateObject(new Grass);
			g->InsertAfter(tree.Get());
			g->transform->location = { x,-12,treez + 3 };
			g = CreateObject(new Leaf);
			g->InsertAfter(tree.Get());
			g->transform->location = { x,-12,treez + 3 };
			g = CreateObject(new Tree);
			g->InsertAfter(tree.Get());
			g->transform->location = { x,-12,treez + 3 };
			treez += 6;
		}

		auto g = CreateObject(new Snow);
		g->InsertAfter(snow.Get());
		g->transform->location = GameSceneCom::backcam->transform->location + Vector3(Random(-30.0f, 30.0f), Random(2.0f, 3.0f), Random(5.0f, 45.0f));
	}

	void OnDestroy() override
	{
		__super::OnDestroy();
		ground.Reset();
		tree.Reset();
		snow.Reset();
		canvas.Reset();
	}

	class Ground :public GameObject
	{
	public:

		Ground() :sr(AddComponent(new SpriteRender))
		{
			transform->Rotate({ M_PI_2,0,0 }, false);
			transform->scale = { 10,10,1 };
			Material mat;
			mat.ambient = { 1,1,1,1 };
			mat.diffuse = { 0,0,0,1 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_snow_ground.Get());
		}

		void OnUpdate() override
		{
			if (transform->location.z - GameSceneCom::backcam->transform->location.z < -10)
				Destroy();
		}

	protected:

		SpriteRender* sr;
	};

	class Tree :public GameObject
	{
	public:

		Tree() :sr(AddComponent(new SpriteRender))
		{
			transform->scale = {10,12.5,1 };
			Material mat;
			mat.ambient = { 1,1,1,1 };
			mat.diffuse = { 0,0,0,1 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(Random(0, 1) == 0 ? src_tree1.Get() : src_tree2.Get());
			sr->SetLocalTransform(XMMatrixTranslation(0, 0.5, 0));
		}

		void OnUpdate() override
		{
			if (transform->location.z - GameSceneCom::backcam->transform->location.z < -10)
				Destroy();
		}

	protected:

		SpriteRender* sr;
	};

	class Grass :public GameObject
	{
	public:

		Grass() :sr(AddComponent(new SpriteRender))
		{
			transform->scale = { 6,3,1 };
			Material mat;
			mat.ambient = { 1,1,1,1 };
			mat.diffuse = { 0,0,0,0.9 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_forest1.Get());
			sr->SetLocalTransform(XMMatrixTranslation(0, 0.4, 0));
			sr->SetSourceRect({ 0,Random(0,1) == 1 ? 0 : 0.5f,1,0.5f });
		}

		void OnUpdate() override
		{
			if (transform->location.z - GameSceneCom::backcam->transform->location.z < -10)
				Destroy();
		}

	protected:

		SpriteRender* sr;
	};

	class Leaf :public GameObject
	{
	public:

		Leaf() :sr(AddComponent(new SpriteRender))
		{
			transform->scale = { 10,5,1 };
			Material mat;
			mat.ambient = { 1,1,1,1 };
			mat.diffuse = { 0,0,0,0.5 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_forest1.Get());
			sr->SetLocalTransform(XMMatrixTranslation(0, 1.9, 0));
			sr->SetSourceRect({ 0,Random(0,1) == 1 ? 0 : 0.5f,1,0.5f });
		}

		void OnUpdate() override
		{
			if (transform->location.z - GameSceneCom::backcam->transform->location.z < -10)
				Destroy();
		}

	protected:

		SpriteRender* sr;
	};

	class Snow :public GameObject
	{
	public:

		Vector3 rotateSpeed;

		float moveSpeed;

		Snow() :
			sr(AddComponent(new SpriteRender)),
			rotateSpeed(Random(-0.2f, 0.2f), Random(-0.2f, 0.2f), Random(-4.0f, 4.0f)),
			moveSpeed(Random(5.0f, 6.0f))
		{
			transform->scale = { 0.65,0.65,1 };
			Material mat;
			mat.ambient = { 1,1,1,1 };
			mat.diffuse = { 0,0,0,0.5 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_snow.Get());
		}

		void OnUpdate() override
		{
			transform->Rotate(rotateSpeed * timer->GetDeltaTime(), true);
			transform->Translate({ 0,(float)(-moveSpeed * timer->GetDeltaTime()),(float)(-0.0f * timer->GetDeltaTime()) });
			if (transform->location.z - GameSceneCom::backcam->transform->location.z < -10)
				Destroy();
			else if (transform->location.y < -13)
				Destroy();
		}

	protected:

		SpriteRender* sr;
	};

	class Canvas :public GameObject
	{
	public:

		Canvas() :sr(AddComponent(new SpriteRender))
		{
			transform->scale = { 800,600,1 };
			mat.ambient = { 0,0,0,1 };
			mat.diffuse = { 1,1,1,1 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_dummy.Get());
		}

		void OnUpdate() override
		{
			mat.diffuse.a -= 0.02;
			sr->SetMaterial(mat);
			if (mat.diffuse.a <= 0)
				Destroy();
		}

	protected:

		SpriteRender* sr;

		Material mat;
	};
};

class RiverSide :public BackGround
{
public:

	LPGOBJ ground;

	LPGOBJ water;

	LPGOBJ cloud;

	RiverSide() :groundz(0), waterz(0), cloudz(10) {}

protected:

	float groundz;

	float waterz;

	float cloudz;

	void OnCreated() override
	{
		__super::OnCreated();

		SetFogColor({ 0.8,0.8,0.9,1 });
		SetFogStart(4);
		SetFogRange(35);
		SetFogEnabled(true);

		GameSceneCom::backcam->transform->location = { 0,0,0 };
		GameSceneCom::backcam->transform->SetEulerAngles({ (float)M_PI / 4.0f,0,0 });

		ground = CreateObject(new GameObject);
		water = CreateObject(new GameObject);
		cloud = CreateObject(new GameObject);
		ground->InsertBefore(GameSceneCom::locator[1].Get());
		water->InsertBefore(GameSceneCom::locator[2].Get());
		cloud->InsertBefore(GameSceneCom::locator[2].Get());

		CreateObject(new CallbackSet(&waterCallback, [=]()->void {float t = timer->GetGameTime() * 4.0f; renderer->SendShaderData(SD_TIMER, &t, 0); }))->InsertBefore(ground.Get());
		CreateObject(new CallbackSet(&mainCallback))->InsertAfter(ground.Get());
	}

	void OnUpdate() override
	{
		GameSceneCom::backcam->transform->location.z += (float)(4 * timer->GetDeltaTime());

		while (groundz - GameSceneCom::backcam->transform->location.z < 100)
		{
			for (int x = -2; x <= 2; x++)
			{
				auto g = CreateObject(new Ground);
				g->InsertAfter(ground.Get());
				g->transform->location = { 10.0f * x,-12,groundz };
				if (abs(x) == 1)
					g->transform->scale.x *= -1;
			}
			groundz += 10;
		}

		while (waterz - GameSceneCom::backcam->transform->location.z < 100)
		{
			for (int x = -2; x <= 2; x++)
			{
				auto g = CreateObject(new Water);
				g->InsertAfter(water.Get());
				g->transform->location = { 10.0f * x,-11,waterz };
			}
			waterz += 10;
		}

		while (cloudz - GameSceneCom::backcam->transform->location.z < 100)
		{
			if (Random(0, 1))
			{
				auto g = CreateObject(new Cloud_1);
				g->InsertAfter(cloud.Get());
				g->transform->location = { Random(-8.0f,8.0f),-4,cloudz };
			}
			else
			{
				auto g = CreateObject(new Cloud_2);
				g->InsertAfter(cloud.Get());
				g->transform->location = { Random(-8.0f,8.0f),-4,cloudz };
			}
			cloudz += 2.5;
		}
	}

	void OnDestroy() override
	{
		__super::OnDestroy();
		ground.Reset();
		water.Reset();
		cloud.Reset();
	}

	class Ground :public GameObject
	{
	public:

		Ground() :sr(AddComponent(new SpriteRender))
		{
			transform->Rotate({ M_PI_2,0,0 }, false);
			transform->scale = { 10,10,1 };
			Material mat;
			mat.ambient = { 1,1,1,1 };
			mat.diffuse = { 0,0,0,1 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_river_ground.Get());
		}

		void OnUpdate() override
		{
			if (transform->location.z - GameSceneCom::backcam->transform->location.z < -10)
				Destroy();
		}

	protected:

		SpriteRender* sr;
	};

	class Water :public GameObject
	{
	public:

		Water() :sr(AddComponent(new SpriteRender))
		{
			transform->Rotate({ M_PI_2,0,0 }, false);
			transform->scale = { 10,10,1 };
			Material mat;
			mat.ambient = { 1,1,1,1 };
			mat.diffuse = { 0,0,0,0.6 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_water.Get());
		}

		void OnUpdate() override
		{
			sr->SetTextureTransform(XMMatrixTranslation(-timer->GetGameTime() * 0.1f, timer->GetGameTime() * 0.1f, 0));

			if (transform->location.z - GameSceneCom::backcam->transform->location.z < -10)
				Destroy();
		}

	protected:

		SpriteRender* sr;
	};

	class Cloud_1 :public GameObject
	{
	public:

		Cloud_1() :sr(AddComponent(new SpriteRender))
		{
			transform->Rotate({ M_PI / 4.0f,0,0 }, false);
			transform->scale = { 10,10,1 };
			Material mat;
			mat.ambient = { 1,1,1,1 };
			mat.diffuse = { 0,0,0,0.15 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_cloud_1.Get());
		}

		void OnUpdate() override
		{
			transform->location.z -= 0.05f;

			if (transform->location.z - GameSceneCom::backcam->transform->location.z < -10)
				Destroy();
		}

	protected:

		SpriteRender* sr;
	};

	class Cloud_2 :public GameObject
	{
	public:

		Cloud_2() :sr(AddComponent(new SpriteRender))
		{
			transform->Rotate({ M_PI / 4.0f,0,0 }, false);
			transform->scale = { 10,10,1 };
			Material mat;
			mat.ambient = { 1,1,1,1 };
			mat.diffuse = { 0,0,0,0.15 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_cloud_1.Get());
		}

		void OnUpdate() override
		{
			transform->location.z -= 0.05f;

			if (transform->location.z - GameSceneCom::backcam->transform->location.z < -10)
				Destroy();
		}

	protected:

		SpriteRender* sr;
	};
};

class GrassLand :public BackGround
{
public:

	LPGOBJ grass;

	LPGOBJ leaf;

	LPGOBJ sunlight;

	GrassLand() :grassz(0), leafz(0), sunlightz(26) {}

protected:

	float grassz;

	float leafz;

	float sunlightz;

	void OnCreated() override
	{
		__super::OnCreated();

		SetFogColor({ 0.3,0.3,0.15,1 });
		SetFogStart(2);
		SetFogRange(35);
		SetFogEnabled(true);

		GameSceneCom::backcam->transform->location = { 0,0,0 };
		GameSceneCom::backcam->transform->SetEulerAngles({ (float)M_PI / 4.0f,0,0 });

		grass = CreateObject(new GameObject);
		leaf = CreateObject(new GameObject);
		sunlight = CreateObject(new GameObject);
		grass->InsertBefore(GameSceneCom::locator[1].Get());
		leaf->InsertBefore(GameSceneCom::locator[1].Get());
		sunlight->InsertBefore(GameSceneCom::locator[2].Get());
	}

	void OnUpdate() override
	{
		GameSceneCom::backcam->transform->location.z += (float)(2 * timer->GetDeltaTime());
		GameSceneCom::backcam->transform->SetEulerAngles({ (float)M_PI / 4.0f,0,-0.06f * sin((float)timer->GetGameTime() / 3.0f) });

		while (grassz - GameSceneCom::backcam->transform->location.z < 100)
		{
			for (int x = -2; x <= 2; x++)
			{
				auto g = CreateObject(new Grass);
				g->InsertAfter(grass.Get());
				g->transform->location = { 30.0f * x,-12,grassz };
			}
			grassz += 30;
		}

		while (leafz - GameSceneCom::backcam->transform->location.z < 100)
		{
			for (int x = -2; x <= 2; x++)
			{
				auto g = CreateObject(new Leaf);
				g->InsertAfter(leaf.Get());
				g->transform->location = { 14.0f * x,-10,leafz };
			}
			leafz += 14;
		}

		while (sunlightz - GameSceneCom::backcam->transform->location.z < 100)
		{
			auto g = CreateObject(new SunLight);
			g->InsertAfter(sunlight.Get());
			g->transform->location = { 3.0f,-6,sunlightz };

			sunlightz += 30;
		}
	}

	void OnDestroy() override
	{
		__super::OnDestroy();
		grass.Reset();
		leaf.Reset();
		sunlight.Reset();
	}

	class Grass :public GameObject
	{
	public:

		Grass() :sr(AddComponent(new SpriteRender))
		{
			transform->Rotate({ M_PI_2,0,0 }, false);
			transform->scale = { 30,30,1 };
			Material mat;
			mat.ambient = { 1,1,1,1 };
			mat.diffuse = { 0,0,0,1 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_grassland.Get());
		}

		void OnUpdate() override
		{
			if (transform->location.z - GameSceneCom::backcam->transform->location.z < -30)
				Destroy();
		}

	protected:

		SpriteRender* sr;
	};

	class Leaf :public GameObject
	{
	public:

		Leaf() :sr(AddComponent(new SpriteRender))
		{
			transform->Rotate({ M_PI_2,0,0 }, false);
			transform->scale = { 14,14,1 };
			Material mat;
			mat.ambient = { 1,1,1,1 };
			mat.diffuse = { 0,0,0,1 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_leaf.Get());
			AddComponent(new BlendStateSet::Setter(BLEND_STATE_DARKEN))->SetPriority(101);
			AddComponent(new BlendStateSet::Setter(BLEND_STATE_NORMAL))->SetPriority(99);
		}

		void OnUpdate() override
		{
			if (transform->location.z - GameSceneCom::backcam->transform->location.z < -10)
				Destroy();
		}

	protected:

		SpriteRender* sr;
	};

	class SunLight :public GameObject
	{
	public:

		SunLight() :sr(AddComponent(new SpriteRender))
		{
			transform->Rotate({ 0,(float)M_PI / 3.5f,0 }, false);
			transform->scale = { 20,20,1 };
			Material mat;
			mat.ambient = { 1,1,1,1 };
			mat.diffuse = { 0,0,0,1 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_sunlight.Get());
			sr->SetSourceRect({ 0.01f,0,0.99f,1 });
		}

		void OnUpdate() override
		{
			if (transform->location.z - GameSceneCom::backcam->transform->location.z < -10)
				Destroy();
		}

	protected:

		SpriteRender* sr;
	};
};