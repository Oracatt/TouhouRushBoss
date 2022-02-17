#include "LoadingScene.h"
#include "EngineGlobal.h"
#include "Callback.h"
#include "GameScene.h"
#include "UserComponent.h"
#include "Mathf.h"
#include "Rand.h"
#include "SceneObjects.h"
#include "Source.h"
#include "TitleScene.h"
#include <thread>

using namespace Global;

extern void LoadGlobalResources();
extern bool resourcesLoaded;

class JumpTitle :public GameObject
{
public:

	JumpTitle() :sr(0), frame(0)
	{
		transform->scale = { 640,480,1 };
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 0,0,0,0 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		AddComponent(new BlendStateSet::Setter(BLEND_STATE_NORMAL));
		sr = AddComponent(new SpriteRender);
		sr->SetMaterial(mat);
		sr->SetTexture(src_dummy.Get());
	}

private:

	int frame;

	SpriteRender* sr;

	Material mat;

	void OnUpdate() override
	{
		frame++;
		if (frame <= 20)
		{
			mat.diffuse.a += 1 / 20.0f;
			sr->SetMaterial(mat);
		}
		if (frame == 30)
		{
			scene->Destroy();
			ComPtr<IScene> mscene;
			engine->CreateScene(0, &mscene);
			mscene->AddComponent(new TitleSceneCom);
			engine->SetNowScene(mscene.Get());
		}
	}
};

class LoadingTagWithSnow :public GameObject
{
public:

	class Snow :public GameObject
	{
	public:

		Snow(ITexture* tex) :
			sr(AddComponent(new SpriteRender)), mb(AddComponent(new MoveBody)), frame(0)
		{
			float size = Random(6.0f, 18.0f);
			transform->scale = { size,size,1 };
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = { 1,1,1,0 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(tex);
			sr->SetSourceRect({ 0,0.5,0.25,0.25 });
			mb->velocity = GetVector(M_PI + Random((float)M_PI * 3.5f / 8, (float)M_PI * 4.5f / 8), 120);
		}

	private:

		int frame;

		MoveBody* mb;

		SpriteRender* sr;

		Material mat;

		void OnUpdate() override
		{
			frame++;
			if (frame <= 8)
			{
				mat.diffuse.a += 0.1;
				sr->SetMaterial(mat);
			}
			else if (frame >= 25)
			{
				mat.diffuse.a -= 0.075;
				sr->SetMaterial(mat);
			}
			else if (frame == 35)
			{
				Destroy();
			}
		}
	};

	LoadingTagWithSnow(ITexture* tex) :
		sr_1(AddComponent(new SpriteRender)), sr_2(AddComponent(new SpriteRender)), presrc_loading(tex), frame(0), isLeft(false)
	{
		transform->scale = { 128,32,1 };
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,1 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr_1->SetMaterial(mat);
		sr_1->SetTexture(presrc_loading.Get());
		sr_1->SetSourceRect({ 0,0,1,0.25 });
		sr_2->SetMaterial(mat);
		sr_2->SetTexture(presrc_loading.Get());
		sr_2->SetSourceRect({ 0,0.25,1,0.25 });
		sr_2->SetLocalTransform(XMMatrixTranslation(0, -0.5, 0));
	}

	void Leave()
	{
		isLeft = true;
	}

private:

	int frame;

	SpriteRender* sr_1;

	SpriteRender* sr_2;

	ComPtr<ITexture> presrc_loading;

	Material mat;

	bool isLeft;

	void OnUpdate() override
	{
		frame++;
		if (!isLeft)
		{
			if (frame % 3 == 0)
			{
				auto g = scene->AttachActor(new Snow(presrc_loading.Get()));
				g->transform->location = { 140 + ((frame / 3) % 3) * 140.0f / 3 + Random(0.0f,140.0f / 3),-140,0 };
			}
			mat.diffuse.a = 0.75f + 0.25f * sin(timer->GetGameTime() * 6);
			sr_1->SetMaterial(mat);
			sr_2->SetMaterial(mat);
			if (resourcesLoaded)
			{
				Leave();
				scene->AttachActor(new JumpTitle);
			}
		}
		else
		{
			mat.diffuse.a -= 0.12f;
			sr_1->SetMaterial(mat);
			sr_2->SetMaterial(mat);
			if (mat.diffuse.a <= 0)
				Destroy();
		}
	}
};

void LoadingSceneCom::OnLoad()
{
	engine->CreateTextureFromFile("image\\loading\\sig.png", &presrc_sig);
	engine->CreateTextureFromFile("image\\loading\\loading.png", &presrc_loading);

	renderer->Invoke(Delegate([=]() {
		Light light;
		light.diffuse = { 1,1,1,1 };
		light.ambient = { 1,1,1,1 };
		light.direction = Vector3(0, 0, 1).GetNormalized();
		light.type = LT_DIRECTIONAL;
		light.specular = { 1,1,1,0 };

		renderer->SendShaderData(SD_LIGHT, (LPVOID)0, &light);
		renderer->SendShaderData(SD_LIGHTENABLED, (LPVOID)0, (LPVOID)true);
		renderer->SendShaderData(SD_SAMPLER, spAnis.Get(), 0);

		pContext->OMSetDepthStencilState(dsState.Get(), 0);
		}));

	camera = scene->AttachActor(new Camera(CAMERA_TYPE_ORTHOGONAL, { 640,480 }));
	camera->transform->location = { 0,0,-10 };

	scene->AttachActor(new BlendStateSet(BLEND_STATE_NORMAL));

	auto backGround = scene->AttachActor(new GameObject);
	backGround->transform->scale = { 640,480,1 };
	SpriteRender* bg_sr = new SpriteRender;
	Material mat;
	mat.ambient = { 0,0,0,0 };
	mat.diffuse = { 1,1,1,1 };
	mat.power = 0;
	mat.specular = { 0,0,0,0 };
	bg_sr->SetMaterial(mat);
	bg_sr->SetTexture(presrc_sig.Get());
	backGround->AddComponent(bg_sr);

	auto loadingTag = scene->AttachActor(new LoadingTagWithSnow(presrc_loading.Get()));
	loadingTag->transform->location = { 210,-178,0 };

	scene->AttachActor(new BlendStateSet(BLEND_STATE_BRIGHTEN));

	thread(LoadGlobalResources).detach();
}

void LoadingSceneCom::OnClear()
{
	camera.Reset();
	presrc_sig.Reset();
	presrc_loading.Reset();
}