#include "Boss.h"
#include "GameScene.h"
#include "Source.h"
#include "VertexBuffer.h"
#include "Mathf.h"
#include "GameGlobal.h"
#include "Rand.h"
#include "EffectColor.h"
#include "EngineGlobal.h"
#include "SceneObjects.h"
#include "GameGlobal.h"
#include "Effect.h"
#include "BossParticle.h"
#include "SoundEffect.h"
#include "GSObjects.h"

class Boss::MagicSquare :public GameObject, public ICamera
{
public:
	
	MagicSquare(Boss* _boss) :sr(AddComponent(new MagicSquareRender(this))), boss(_boss), show(false), frame(0), size(0)
	{
		transform->scale = { 0,0,1 };
		transform->rotation = { -0.002681, -0.446869, 0.893746 ,-0.039161 };
		Material mat;
		mat.ambient = { 1,1,1,1 };
		mat.diffuse = { 0,0,0,0.5 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_magicsquare.Get());
	}

	void Show()
	{
		show = true;
	}

	bool IsValid() override
	{
		return GameObject::IsValid();
	}

	void Destroy() override
	{
		return GameObject::Destroy();
	}

	void* GetUserData() override
	{
		return GameObject::GetUserData();
	}

	void SetUserData(void* pdata) override
	{
		return GameObject::SetUserData(pdata);
	}

	HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override
	{
		return GameObject::QueryInterface(riid, ppvObject);
	}

	ULONG __stdcall AddRef() override
	{
		return GameObject::AddRef();
	}

	ULONG __stdcall Release() override
	{
		return GameObject::Release();
	}

protected:

	int frame;

	float size;

	bool show;
	
	SpriteRender* sr;

	Boss* boss;
	
	void OnCreated() override
	{
		InsertBefore(GameSceneCom::locator[6].Get());
	}
	
	void OnUpdate() override
	{
		if (show)
		{
			frame++;
			if (frame <= 30)
			{
				float size = Lerp(0.0f, 5.0f, pow(frame / 30.0f, 0.33));
				transform->scale = { size,size,1 };
			}
			if (frame < 13)
				for (int i = 0; i < 5; i++)
					transform->Rotate({ 0.00f ,-0.006f,0.012f }, true);
			else if (frame < 27)
				for (int i = 0; i < 4; i++)
					transform->Rotate({ 0.00f ,-0.006f,0.012f }, true);
			else if (frame < 40)
				for (int i = 0; i < 2; i++)
					transform->Rotate({ 0.00f ,-0.006f,0.012f }, true);
			else
				transform->Rotate({ 0.00f ,-0.006f,0.012f }, true);
		}
		if (boss->IsValid())
			transform->location = ScreenToWorld(boss->transform->location, 0.99f, this, { 640,480 });
		else
			GameObject::Destroy();
	}
	
private:
	
	float GetFov() override { return M_PI_4; }

	void SetFov(float fovY) override {}

	Vector3 GetLocation() override { return { 0,0,0 }; }

	Quaternion GetRotation() override { return QuaternionRotateRollPitchYaw(0, 0, 0); };

	CAMERA_TYPE GetType() override { return CAMERA_TYPE_PROJECTIVE; };

	void SetType(CAMERA_TYPE type) override {};

	USize GetOrthogonalSize() { return { 640,480 }; };

	void SetOrthogonalSize(const USize& size) override {};

	class MagicSquareRender :public SpriteRender
	{
	public:

		MagicSquareRender(ICamera* _cam) :cam(_cam) {}

		void OnFetchData() override
		{
			__super::OnFetchData();
			m_cam = cam;
			m_gamecam = GameSceneCom::gamecam.Get();
		}

		void OnRender() override
		{
			UINT stride = 32;
			UINT offset = 0;

			pContext->IASetVertexBuffers(0, 1, vb_Sprite.GetAddressOf(), &stride, &offset);

			renderer->SetWorldMatrix(transform);
			renderer->SetTextureMatrix(mTexTransform);
			if (mTexture)
				renderer->SendShaderData(SD_TEXTURE, mTexture->GetShaderResourceView(), 0);
			else
				renderer->SendShaderData(SD_TEXTURE, 0, 0);
			renderer->SendShaderData(SD_MATERIAL, &mMaterial, 0);
			renderer->SendShaderData(SD_UPDATEMATRIX, m_cam.Get(), 0);
			renderer->StartRender();
			pContext->Draw(6, 0);
			renderer->SendShaderData(SD_UPDATEMATRIX, m_gamecam.Get(), 0);

			m_cam.Reset();
			m_gamecam.Reset();
		}

	private:

		ICamera* cam;

		ComPtr<ICamera> m_cam;

		ComPtr<ICamera> m_gamecam;
	};
};

class Boss::Aura :public GameObject
{
public:

	Aura(GameObject* _follow, int colorKey) :
		sr(AddComponent(new SpriteRender)), frame(0), follow(_follow), angle(RandomAngle())
	{
		speed = Random(3.2f, 4.8f);
		transform->location = (follow->transform->location);
		transform->Rotate({ 0,0,angle }, true);
		transform->scale = { 0,0,1 };
		mat.ambient = auraColorKey[colorKey];
		mat.diffuse = { 0,0,0,1 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_aura.Get());
		sr->SetSourceRect({ 52 / 128.0f,6 / 64.0f,39 / 128.0f,39 / 64.0f });
	}

protected:

	float angle;

	float speed;

	Material mat;

	int frame;

	SpriteRender* sr;

	LPGOBJ follow;

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::locator[6].Get());
	}

	void OnDestroy() override
	{
		follow.Reset();
	}

	void OnUpdate() override
	{
		transform->location = (follow->transform->location);
		frame++;
		if (frame < 30)
			transform->scale += {speed, speed, 0};
		if (frame > 0)
		{
			mat.diffuse.a -= 0.033;
			sr->SetMaterial(mat);
		}
		if (frame == 30)
			Destroy();
	}
};

class Boss::AuraFire :public GameObject
{
public:

	AuraFire(GameObject* _follow, int colorKey) :sr(AddComponent(new SpriteRender)), frame(0), follow(_follow)
	{
		speed = 7.0f;
		transform->location = (follow->transform->location);
		transform->Rotate({ 0,0,Random(-0.05f,0.05f) }, true);
		transform->scale = { 40,32,1 };
		mat.ambient = auraColorKey[colorKey];
		mat.diffuse = { 0,0,0,1 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_aura.Get());
		sr->SetLocalTransform(XMMatrixTranslation(0, 0.5, 0));
		sr->SetSourceRect({ 7 / 128.0f,3 / 64.0f,37 / 128.0f,42 / 64.0f });
	}

protected:

	float speed;

	Material mat;

	int frame;

	SpriteRender* sr;

	LPGOBJ follow;

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::locator[6].Get());
	}

	void OnDestroy() override
	{
		follow.Reset();
	}

	void OnUpdate() override
	{
		transform->location = (follow->transform->location);
		frame++;
		if (frame < 20)
			transform->scale += {0.5, speed, 0};
		if (frame > 5)
		{
			mat.diffuse.a -= 0.07;
			sr->SetMaterial(mat);
		}
		if (frame == 20)
			Destroy();
	}
};

Boss::Boss(const AnimationGroup& _animGroup, int _colorKey) :
	Enemy(_animGroup, FLT_MAX), magicSquare(0), showMagicSquare(false), frame(0), colorKey(_colorKey), warpFrame(0) {}

void Boss::ShowMagicSquare()
{
	showMagicSquare = true;
	magicSquare->Show();
}

void Boss::ShowMaple(const Color& color, int storetimes, bool blast, bool shakeScreen)
{
	int delay = 0;
	if (storetimes == 1)
	{
		scene->AttachActor(new MapleCircle_1(this, color));
		scene->AttachActor(new MapleSpawner_1(this, color));
		delay += 70;
	}
	else
	{
		se_ch00.Play();
		scene->AttachActor(new MapleCircle_1(this, color, false));
		scene->AttachActor(new MapleSpawner_1(this, color));
		for (int i = 0; i < storetimes - 1; i++)
		{
			delay += 24;
			auto maple_1 = new MapleCircle_1(this, color, false);
			scene->AttachActor(new DelayCreate(maple_1, delay));
			auto ms_1 = new MapleSpawner_1(this, color);
			scene->AttachActor(new DelayCreate(ms_1, delay));
		}
		delay += 70;
	}
	if (blast)
	{
		auto maple_2 = new MapleCircle_2(this, color);
		scene->AttachActor(new DelayCreate(maple_2, delay));
		auto ms_2 = new MapleSpawner_2(this, color);
		scene->AttachActor(new DelayCreate(ms_2, delay));
	}
	if (shakeScreen)
	{
		GameSceneCom::gamecam->Shake(delay - 10, 12);
	}
}

void Boss::OnCreated()
{
	nowBoss = this;
	warpColor = colorKey;
	warpLimit = 10000;
	magicSquare = scene->AttachActor(new MagicSquare(this));
	InsertBefore(GameSceneCom::locator[7].Get());
}

void Boss::OnUpdate()
{
	frame++;
	if (showMagicSquare)
	{
		warpFrame++;
		if (warpFrame <= 20)
			warpLimit = Lerp(100, 28, (float)pow((warpFrame / 20.0f), 0.3));
		if (frame % 2 == 0)
			scene->AttachActor(new Aura(this, colorKey));
		if (frame % 6 == 0 && frame > 10)
			scene->AttachActor(new AuraFire(this, colorKey));
	}
	warpCenter = transform->location;
	__super::OnUpdate();
}

void Boss::OnDestroy()
{
	if (nowBoss.Get() == this)
	{
		nowBoss.Reset();
		bossName = L"";
		warpCenter = { 10000,10000 };
		warpLimit = 10000;
		bossLife = 0;
	}
	magicSquare->Destroy();
}

void Boss::OnDeath()
{
	scene->AttachActor(new BossDeadMaple(auraColorKey[colorKey], transform->location));

	scene->AttachActor(new BossInvColorEffect(120, 0, true))->transform->location = transform->location + Vector3(-40, 0, 0);
	scene->AttachActor(new BossInvColorEffect(120, 0, true))->transform->location = transform->location + Vector3(40, 0, 0);
	scene->AttachActor(new BossInvColorEffect(120, 0, true))->transform->location = transform->location + Vector3(0, -40, 0);
	scene->AttachActor(new BossInvColorEffect(120, 0, true))->transform->location = transform->location + Vector3(0, 40, 0);
	scene->AttachActor(new BossInvColorEffect(120, 0, true))->transform->location = transform->location;
	scene->AttachActor(new BossInvColorEffect(40, 0, false))->transform->location = transform->location + Vector3(-40, 0, 0);
	scene->AttachActor(new BossInvColorEffect(40, 0, false))->transform->location = transform->location + Vector3(40, 0, 0);
	scene->AttachActor(new BossInvColorEffect(40, 0, false))->transform->location = transform->location + Vector3(0, -40, 0);
	scene->AttachActor(new BossInvColorEffect(40, 0, false))->transform->location = transform->location + Vector3(0, 40, 0);
	scene->AttachActor(new BossInvColorEffect(40, 0, false, 0, true))->transform->location = transform->location;
	scene->AttachActor(new BossInvColorEffect(120, -20, true, 24, false))->transform->location = transform->location;

	GameSceneCom::gamecam->Shake(40, 60);

	Destroy();
}