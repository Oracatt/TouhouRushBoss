#pragma once
#include "VirtualLib.h"
#include "GameScene.h"
#include "Source.h"
#include "VertexBuffer.h"
#include "Rand.h"
#include "Mathf.h"
#include "UserComponent.h"
#include "EngineGlobal.h"
#include "SoundEffect.h"
#include "EnemyBullet.h"
#include "EffectColor.h"

class Effect :public GameObject
{
public:

	Effect(bool _highLight) :highLight(_highLight) {}

	void OnCreated() override
	{
		if (highLight)
			InsertBefore(GameSceneCom::locator[13].Get());
		else
			InsertBefore(GameSceneCom::locator[17].Get());
	}

protected:

	bool highLight;
};

class EnemyBulletBreakEffect :public Effect
{
public:

	EnemyBulletBreakEffect(const Color& color, int startframe = 0) :Effect(true), sr(AddComponent(new SpriteRender)), frame(startframe)
	{
		Vector3 col = color;
		col = col.GetNormalized();
		Material mat;
		mat.ambient = { col.x,col.y,col.z,0.1f };
		mat.diffuse = { 0,0,0,1.0f };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_etbreak.Get());
		sr->SetActive(false);
	}

	void OnUpdate() override
	{
		frame++;
		if (frame >= 0)
		{
			sr->SetActive(true);
			int index = frame / 4;
			if (index < 4)
				sr->SetSourceRect({ index / 4.0f,0,0.25f,0.5f });
			else
				sr->SetSourceRect({ (index - 4) / 4.0f,0.5f,0.25f,0.5f });
		}
		else
		{
			sr->SetActive(false);
		}
		if (frame == 31)
			Destroy();
	}

protected:

	SpriteRender* sr;

	int frame;
};

class EnemyBulletBreakCharge :public Effect
{
public:

	EnemyBulletBreakCharge(const Color& color) :Effect(true), sr(AddComponent(new SpriteRender)), frame(0)
	{
		Vector3 col = color;
		col = col.GetNormalized();
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { col.x,col.y,col.z,2.0f };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_maple.Get());
		sr->SetSourceRect({ 0.5,0.5,0.5,0.5 });
	}

	void OnUpdate() override
	{
		frame++;
		transform->scale *= 0.98f;
		mat.diffuse.a -= 0.05f * frame;
		sr->SetMaterial(mat);
		if (frame == 40)
			Destroy();
	}

protected:

	SpriteRender* sr;

	Material mat;

	int frame;
};

class PlayerDeadCircle :public Effect
{
public:

	PlayerDeadCircle() :Effect(false), sr(AddComponent(new SpriteRender)), frame(0)
	{
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,1 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_deadcircle.Get());
		sr->SetSourceRect({ 0,0,0.5f,0.5f });
		transform->scale = { 0,0,1 };
	}

	void OnUpdate() override
	{
		frame++;
		transform->scale += Vector3(20, 20, 0);
		mat.diffuse.a -= 0.05f;
		sr->SetMaterial(mat);
		if (frame == 20)
			Destroy();
	}

protected:

	SpriteRender* sr;

	Material mat;

	int frame;
};

class InvColorEffect :public Effect
{
public:

	InvColorEffect(int destroyFrame, int startframe = 0, bool scaling = true, int coldFrame = 0, bool special = false) :
		Effect(false), frame(startframe), remain(destroyFrame), scaleChange(scaling), coldRemain(coldFrame), spe(special)
	{
		SpriteRender* sr = AddComponent(new Drawer);
		Material mat;
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,1 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_dummy.Get());
		transform->scale = { 3,3,1 };
	}

	void OnCreated() override
	{
		if (spe)
			InsertAfter(GameSceneCom::locator[19].Get());
		else
			InsertAfter(GameSceneCom::locator[17].Get());
	}

	void OnUpdate() override
	{
		frame++;
		remain--;
		if (scaleChange && (--coldRemain) <= 0)
		{
			transform->scale += Vector3((6 + frame) * (6 + frame) * (6 + frame), (6 + frame) * (6 + frame) * (6 + frame), 0) * 0.0002f;
		}
		else if (coldRemain <= 0)
		{
			transform->scale.x *= 0.9f;
			transform->scale.y *= 0.9f;
		}
		if (remain <= 0)
			Destroy();
	}

	class Drawer :public SpriteRender
	{
		void OnRender() override
		{
			UINT stride = 32;
			UINT offset = 0;

			ComPtr<ID3D11BlendState> state;
			float factor[4];
			UINT mask;
			pContext->OMGetBlendState(&state, factor, &mask);
			renderer->SetBlendState(BLEND_STATE_INVDEST);
			pContext->IASetVertexBuffers(0, 1, vb_InvColor.GetAddressOf(), &stride, &offset);
			renderer->SetWorldMatrix(transform);
			renderer->SetTextureMatrix(mTexTransform);
			if (mTexture)
				renderer->SendShaderData(SD_TEXTURE, mTexture->GetShaderResourceView(), 0);
			else
				renderer->SendShaderData(SD_TEXTURE, 0, 0);
			renderer->SendShaderData(SD_MATERIAL, &mMaterial, 0);
			renderer->StartRender();
			pContext->Draw(768, 0);
			pContext->OMSetBlendState(state.Get(), factor, mask);
		}
	};

protected:

	int frame;

	int remain;

	int coldRemain;

	bool scaleChange;

	bool spe;
};

class BossInvColorEffect :public Effect
{
public:

	BossInvColorEffect(int destroyFrame, int startframe = 0, bool scaling = true, int coldFrame = 0, bool special = false) :
		Effect(false), frame(startframe), remain(destroyFrame), scaleChange(scaling), coldRemain(coldFrame), spe(special)
	{
		SpriteRender* sr = AddComponent(new Drawer);
		Material mat;
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,1 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_dummy.Get());
		transform->scale = { 3,3,1 };
	}

	void OnCreated() override
	{
		if (spe)
			InsertAfter(GameSceneCom::locator[19].Get());
		else
			InsertAfter(GameSceneCom::locator[17].Get());
	}

	void OnUpdate() override
	{
		frame++;
		remain--;
		if (scaleChange && (--coldRemain) <= 0)
		{
			transform->scale += Vector3((6 + frame) * (6 + frame) * (6 + frame), (6 + frame) * (6 + frame) * (6 + frame), 0) * 0.0003f;
		}
		else if (coldRemain <= 0)
		{
			transform->scale.x *= 0.9f;
			transform->scale.y *= 0.9f;
		}
		if (remain <= 0)
			Destroy();
	}

	class Drawer :public SpriteRender
	{
		void OnRender() override
		{
			UINT stride = 32;
			UINT offset = 0;

			ComPtr<ID3D11BlendState> state;
			float factor[4];
			UINT mask;
			pContext->OMGetBlendState(&state, factor, &mask);
			renderer->SetBlendState(BLEND_STATE_INVDEST);
			pContext->IASetVertexBuffers(0, 1, vb_InvColor.GetAddressOf(), &stride, &offset);
			renderer->SetWorldMatrix(transform);
			renderer->SetTextureMatrix(mTexTransform);
			if (mTexture)
				renderer->SendShaderData(SD_TEXTURE, mTexture->GetShaderResourceView(), 0);
			else
				renderer->SendShaderData(SD_TEXTURE, 0, 0);
			renderer->SendShaderData(SD_MATERIAL, &mMaterial, 0);
			renderer->StartRender();
			pContext->Draw(768, 0);
			pContext->OMSetBlendState(state.Get(), factor, mask);
		}
	};

protected:

	int frame;

	int remain;

	int coldRemain;

	bool scaleChange;

	bool spe;
};

class PlayerDeadSmallStar :public Effect
{
public:

	PlayerDeadSmallStar() :
		Effect(true), mb(AddComponent(new MoveBody)), sr(AddComponent(new SpriteRender)), frame(0)
	{
		mb->velocity = GetVector(RandomAngle(), Random(200, 600));
		mb->drag = { 0.1,0.1,0.1 };
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 0.7,0.4,0.5,0.6 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_base.Get());
		sr->SetSourceRect({ 0.25f * Random(0,3),0.0,0.25,0.25 });
		transform->scale = { 32,32,1 };
	}

	void OnUpdate() override
	{
		frame++;
		transform->scale -= Vector3(1, 1, 0);
		mat.diffuse.a -= 0.005f;
		sr->SetMaterial(mat);
		if (frame > 20)
			Destroy();
	}

protected:

	MoveBody* mb;

	SpriteRender* sr;

	int frame;

	Material mat;
};

class PlayerDeadBigStar :public Effect
{
public:

	PlayerDeadBigStar() :
		Effect(true), mb(AddComponent(new MoveBody)), sr(AddComponent(new SpriteRender)), frame(0), size(0)
	{
		mb->velocity = GetVector(RandomAngle(), Random(50, 1000));
		mb->drag = { 0.55,0.55,0.55 };
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 0.7,0.4,0.5,0.55 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_base.Get());
		sr->SetSourceRect({ 0.25f * Random(0,3),0.0,0.25,0.25 });
		transform->scale = { size,size,1 };
	}

	void OnUpdate() override
	{
		frame++;
		if (frame < 7)
			size += 30;
		/*
		else if (frame >= 18)
			size -= 8;
		else
			size -= 2;
			*/
		else
			size /= (1 + 0.0085 * (frame - 7));
		transform->scale = { size,size,1 };

		if (frame >= 35)
			Destroy();	
	}

protected:

	MoveBody* mb;

	SpriteRender* sr;

	float size;

	int frame;

	Material mat;
};

class Reimu_Bullet_1_Hit :public Effect
{
public:

	Reimu_Bullet_1_Hit() :Effect(true), frame(0), sr(AddComponent(new SpriteRender))
	{
		transform->scale = { 32,16,1 };
		transform->SetEulerAngles({ 0,0,M_PI_2 });
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 0.9,0.9,0.9,1 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_player_reimu.Get());
		sr->SetSourceRect({ 3 / 16.0f,9 / 16.0f,1 / 16.0f,1 / 16.0f });
	}

protected:

	int frame;

	Material mat;

	SpriteRender* sr;

	void OnUpdate() override
	{
		frame++;
		transform->location.y += 2.5f;
		mat.diffuse.a -= 0.033f;
		sr->SetMaterial(mat);
		if (frame == 30)
			Destroy();
	}
};

class Reimu_Bullet_2_Hit :public Effect
{
public:

	MoveBody* mb;

	Reimu_Bullet_2_Hit() :
		Effect(true), frame(0), sr(AddComponent(new SpriteRender)), 
		mb(AddComponent(new MoveBody)), rotateSpeed(Random(-0.15f, 0.15f))
	{
		transform->scale = { 16,16,1 };
		transform->SetEulerAngles({ 0,0,RandomAngle() });
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,1 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_player_reimu.Get());
		sr->SetSourceRect({ 2 / 16.0f,10 / 16.0f,1 / 16.0f,1 / 16.0f });
	}

protected:

	int frame;

	float rotateSpeed;

	Material mat;

	SpriteRender* sr;

	void OnUpdate() override
	{
		frame++;
		if (frame < 6)
			transform->scale += {9.6, 9.6, 0};
		else
			transform->scale -= {4.1, 4.1, 0};
		transform->Rotate({ 0,0,rotateSpeed }, true);
		mat.diffuse.a -= 0.05f;
		sr->SetMaterial(mat);
		if (frame == 20)
			Destroy();
	}
};

class Reimu_Bullet_3_Hit :public Effect
{
public:

	Reimu_Bullet_3_Hit(int type,int seed) :
		Effect(true), frame(0), sr(AddComponent(new SpriteRender))
	{
		transform->scale = { type ? 150.0f : 96.0f,12,1 };
		transform->SetEulerAngles({ 0,0,(float)M_PI_2 + Random(0.0f,0.2f) * (seed == type ? 1 : -1) });
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,1 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_player_reimu.Get());
		sr->SetSourceRect({ 0,(11 + type) / 16.0f,(type ? 100 / 256.0f : 64 / 256.0f),1 / 16.0f });
	}

protected:

	int frame;

	Material mat;

	SpriteRender* sr;

	void OnUpdate() override
	{
		frame++;
		transform->location.y += 2.0f;
		mat.diffuse.a -= 0.05f;
		sr->SetMaterial(mat);
		if (frame == 20)
			Destroy();
	}
};

class Reimu_Bomb_Break :public Effect
{
public:

	Reimu_Bomb_Break() :
		Effect(true), sr(AddComponent(new SpriteRender)), frame(0)
	{
		transform->scale = { 150,150,1 };
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,0.75 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_reimu_bomb.Get());
	}

protected:

	void OnUpdate() override
	{
		frame++;
		transform->scale += {75, 75, 0};
		mat.diffuse.a -= 0.05;
		sr->SetMaterial(mat);
		if (frame == 15)
			Destroy();
	}

	int frame;

	Material mat;

	SpriteRender* sr;
};

class Reimu_Color_Bomb_Break :public Effect
{
public:

	Reimu_Color_Bomb_Break(int colorIndex) :
		Effect(true), sr(AddComponent(new SpriteRender)), mb(AddComponent(new MoveBody)), frame(0)
	{
		transform->scale = { 150,150,1 };
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 0,0,0,0.8 };
		*(((float*)&mat.diffuse) + colorIndex) = 1;
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_reimu_bomb.Get());
		mb->velocity = GetVector(RandomAngle(), Random(150, 400));
	}

protected:

	void OnUpdate() override
	{
		frame++;
		if (frame > 25)
			transform->scale -= {6, 6, 0};
		else
			transform->scale -= {1, 1, 0};
		mat.diffuse.a -= 0.015;
		sr->SetMaterial(mat);
		if (frame == 40)
			Destroy();
	}

	int frame;

	Material mat;

	SpriteRender* sr;

	MoveBody* mb;
};

class Marisa_Bullet_1_Hit :public Effect
{
public:

	Marisa_Bullet_1_Hit() :Effect(true), frame(0), sr(AddComponent(new SpriteRender))
	{
		transform->scale = { 32,16,1 };
		transform->SetEulerAngles({ 0,0,M_PI_2 });
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 0.9,0.9,0.9,1 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_player_marisa.Get());
		sr->SetSourceRect({ 1 / 8.0f,9 / 16.0f,1 / 8.0f,1 / 16.0f });
	}

protected:

	int frame;

	Material mat;

	SpriteRender* sr;

	void OnUpdate() override
	{
		frame++;
		transform->location.y += 2.5f;
		mat.diffuse.a -= 0.033f;
		sr->SetMaterial(mat);
		if (frame == 30)
			Destroy();
	}
};

class Marisa_Bullet_2_Shoot :public Effect
{
public:

	Marisa_Bullet_2_Shoot() :Effect(true), frame(0), sr(AddComponent(new SpriteRender))
	{
		transform->scale = { 16,16,1 };
		transform->SetEulerAngles({ 0,0,RandomAngle() });
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,1 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_player_marisa.Get());
		sr->SetSourceRect({ 7 / 8.0f,9 / 16.0f,1 / 8.0f,1 / 8.0f });
	}

protected:

	int frame;

	Material mat;

	SpriteRender* sr;

	void OnUpdate() override
	{
		frame++;
		if (frame == 2)
			Destroy();
	}
};

class Marisa_Bullet_2_Hit :public Effect
{
public:

	Marisa_Bullet_2_Hit() :Effect(true), frame(0), sr(AddComponent(new SpriteRender))
	{
		transform->scale = { 32,32,1 };
		transform->SetEulerAngles({ 0,0,M_PI_2 });
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 0.45,0.25,0.45,1 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_player_marisa.Get());
		sr->SetSourceRect({ 1 / 8.0f,12 / 16.0f,1 / 8.0f,1 / 8.0f });
	}

protected:

	int frame;

	Material mat;

	SpriteRender* sr;

	void OnUpdate() override
	{
		frame++;
		transform->location.y += 2.5f;
		mat.diffuse.a -= 0.05f;
		sr->SetMaterial(mat);
		if (frame == 20)
			Destroy();
	}
};

class Marisa_Bullet_3_Hit :public Effect
{
	class BreakEffect :public Effect
	{
	public:

		BreakEffect() :Effect(true), frame(0), sr(AddComponent(new SpriteRender))
		{
			transform->scale = { 32,32,1 };
			transform->SetEulerAngles({ 0,0,M_PI_2 });
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = { 1,1,1,0.5 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_player_marisa.Get());
			sr->SetSourceRect({ 0 / 8.0f,7 / 8.0f,1 / 8.0f,1 / 8.0f });
		}

	private:

		int frame;

		Material mat;

		SpriteRender* sr;

		void OnUpdate() override
		{
			frame++;
			if (frame <= 15)
				sr->SetSourceRect({ (frame / 2) / 8.0f,7 / 8.0f,1 / 8.0f,1 / 8.0f });
			else
				Destroy();
		}
	};

public:

	Marisa_Bullet_3_Hit() :Effect(true), frame(0)
	{
	}

protected:

	int frame;

	void OnUpdate() override
	{
		frame++;
		if (frame == 1)
		{
			scene->AttachActor(new BreakEffect)->transform->location = transform->location;
		}
		if (frame <= 8 && frame % 2 == 0)
		{
			scene->AttachActor(new BreakEffect)->transform->location = transform->location + GetVector(RandomAngle(), Random(5.0f, 30.0f));
		}
		if (frame == 13)
			Destroy();
	}
};

class MapleCircle_1 :public Effect
{
public:

	MapleCircle_1(LPGOBJ _follow, const Color& color, bool playSound = true) :
		Effect(true), sr(AddComponent(new SpriteRender)), frame(0), ps(playSound), follow(_follow)
	{
		transform->location = follow->transform->location;
		transform->scale = { 800,800,1 };
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = color;
		mat.diffuse.a = 0;
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_maple.Get());
		sr->SetSourceRect({ 0,0.5,0.5,0.5 });
	}

private:

	LPGOBJ follow;

	int frame;

	Material mat;

	SpriteRender* sr;

	bool ps;

	void OnUpdate() override
	{
		frame++;
		transform->location = follow->transform->location;
		if (frame <= 5)
		{
			mat.diffuse.a += 0.06;
			sr->SetMaterial(mat);
		}
		transform->scale -= {20, 20, 0};
		if (frame == 40)
			Destroy();
	}

	void OnCreated() override
	{
		if (ps)
			se_ch02.Play();
		__super::OnCreated();
	}

	void OnDestroy() override
	{
		__super::OnDestroy();
		follow.Reset();
	}
};

class MapleCircle_2 :public Effect
{
public:

	MapleCircle_2(LPGOBJ _follow, const Color& color) :
		Effect(true), sr(AddComponent(new SpriteRender)), frame(0), follow(_follow)
	{
		transform->location = follow->transform->location;
		transform->scale = { 0,0,1 };
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = color;
		mat.diffuse.a = 0.8;
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_maple.Get());
		sr->SetSourceRect({ 0,0.5,0.5,0.5 });
	}

private:

	LPGOBJ follow;

	int frame;

	Material mat;

	SpriteRender* sr;

	void OnUpdate() override
	{
		frame++;
		transform->location = follow->transform->location;
		if (frame >= 0)
		{
			mat.diffuse.a -= 0.8f/35.0f;
			sr->SetMaterial(mat);
		}
		transform->scale += {25, 25, 0};
		if (frame == 35)
			Destroy();
	}

	void OnCreated() override
	{
		se_enep02.Play();
		__super::OnCreated();
	}

	void OnDestroy() override
	{
		__super::OnDestroy();
		follow.Reset();
	}
};

class EnemyBulletFog :public Effect
{
public:

	EnemyBulletFog(EnemyBullet* _g, int _frame = 15) :
		Effect(false), g(_g), frame(0), limit(_frame), sr(AddComponent(new SpriteRender))
	{
		transform->location = _g->transform->location;
		transform->scale = _g->transform->scale * 3;
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,0.1 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_bullet_1.Get());
		sr->SetSourceRect({ 0,0.5,0.5,0.5 });

		if (_g->breakColor == breakColor16)
			SetColor(bulletFog1Index16[_g->colorIndex]);
		else if (_g->breakColor == breakColor8)
			SetColor(bulletFog1Index8[_g->colorIndex]);
		else if (_g->breakColor == breakColor4)
			SetColor(bulletFog1Index4[_g->colorIndex]);
		else if (_g->breakColor == breakColorYanDan)
			SetColor(bulletFog1IndexYanDan[_g->colorIndex]);
	}

private:

	int frame;

	int limit;

	LPGOBJ g;

	SpriteRender* sr;

	Material mat;

	void OnUpdate() override
	{
		frame++;
		if (frame >= limit)
		{
			scene->AttachActor(g.Get());
			g.Reset();
			Destroy();
		}
		else
		{
			transform->scale = g->transform->scale * Lerp(4.0f, 1.0f, frame / (float)limit);
			mat.diffuse.a = Lerp(0.0f, 1.0f, frame / (float)limit);
			sr->SetMaterial(mat);
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

	void SetColor(int index)
	{
		sr->SetSourceRect({ (index * 32 + 1) / 256.0f,209 / 256.0f,30 / 256.0f,30 / 256.0f });
	}
};

class LaserFog :public Effect
{
public:

	LaserFog(EnemyBullet* _g, float _scale, int _frame) :
		Effect(false), g(_g), scale(_scale), frame(0), limit(_frame), sr(AddComponent(new SpriteRender)), isDead(false)
	{
		transform->location = _g->transform->location;
		transform->scale = { scale,scale,1 };
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,0 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_bullet_1.Get());
		sr->SetSourceRect({ 0,0.5,0.5,0.5 });

		if (_g->breakColor == breakColor16)
			SetColor(bulletFog1Index16[_g->colorIndex]);
		else if (_g->breakColor == breakColor8)
			SetColor(bulletFog1Index8[_g->colorIndex]);
		else if (_g->breakColor == breakColor4)
			SetColor(bulletFog1Index4[_g->colorIndex]);
		else if (_g->breakColor == breakColorYanDan)
			SetColor(bulletFog1IndexYanDan[_g->colorIndex]);
	}

private:

	int frame;

	int limit;

	float scale;

	bool isDead;

	LPGOBJ g;

	SpriteRender* sr;

	Material mat;

	void OnCreated() override
	{
		__super::OnCreated();
		if (g)
		{
			scene->AttachActor(g.Get());
		}
	}

	void OnUpdate() override
	{
		if (!g->IsValid() && !isDead)
		{
			isDead = true;
			frame = 0;
		}
		frame++;
		if (!isDead)
		{
			if (frame < 15)
			{
				transform->scale = Vector3(scale, scale, 3) * Lerp(0.0f, 1.0f, frame / 15.0f);
				mat.diffuse.a = Lerp(0.0f, 1.0f, frame / 15.0f);
				sr->SetMaterial(mat);
			}
			else if (frame >= limit)
			{
				isDead = true;
				frame = 0;
			}
		}
		else
		{
			transform->scale = Vector3(scale, scale, 3) * Lerp(1.0f, 0.0f, frame / 15.0f);
			mat.diffuse.a = Lerp(1.0f, 0.0f, frame / 15.0f);
			sr->SetMaterial(mat);
			if (frame==15)
			{
				Destroy();
			}
		}
	}

	void OnDestroy() override
	{
		if (g)
		{
			g.Reset();
		}
	}

	void SetColor(int index)
	{
		sr->SetSourceRect({ (index * 32 + 1) / 256.0f,209 / 256.0f,30 / 256.0f,30 / 256.0f });
	}
};

class PlayerGrazeStar :public Effect
{
public:

	MoveBody* mb;

	PlayerGrazeStar() :
		Effect(true), mb(AddComponent(new MoveBody)), sr(AddComponent(new SpriteRender)), frame(0)
	{
		mb->drag = { 0.1,0.1,0.1 };
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,0.8 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_base.Get());
		sr->SetSourceRect({ 0.25f * Random(0,3),0.0,0.25,0.25 });
		transform->scale = { 4,4,1 };
	}

	void OnUpdate() override
	{
		frame++;
		if (frame <= 5)
			transform->scale += Vector3(0.2, 0.2, 0);
		else
			transform->scale -= Vector3(0.2, 0.2, 0);
		if (frame >= 15)
			Destroy();
	}

protected:

	SpriteRender* sr;

	int frame;

	Material mat;
};