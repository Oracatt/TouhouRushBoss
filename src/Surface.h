#pragma once
#include "VirtualLib.h"
#include "GameScene.h"
#include "EngineGlobal.h"
#include "SceneObjects.h"
#include "Replay.h"

using namespace Global;

class LifeAndBomb :public GameObject
{
public:

	LifeAndBomb() :sr(AddComponent(new SpriteRender)), hide(false)
	{
		transform->location = { 285,-300,0 };
		transform->scale = { 120,96,1 };
		RenderTargetDesc desc;
		desc.width = 320;
		desc.height = 240;
		desc.msaaEnabled = false;
		desc.support2D = true;
		desc.hasDepth = true;
		desc.multiSampleCount = 0;
		engine->CreateRenderTarget(desc, &target);
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,1 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(target.Get());
		sr->SetLocalTransform(XMMatrixTranslation(0.5, 0.5, 0));
	}

protected:

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::locator[20].Get());
		scene->AttachActor(new RenderTargetSet(target.Get(), 0))->InsertBefore(this);

		LPGOBJ g = scene->AttachActor(new GameObject);
		g->transform->location = { -320,240 ,0 };
		g->transform->scale = { 80,80,1 };
		g->transform2D->location = { 50,0 };
		g->transform2D->scale = { 2.8,2.8 };
		g->InsertBefore(this);
		SpriteRender* g_sr = new SpriteRender;
		Material sr_mat;
		sr_mat.ambient = { 0,0,0,0 };
		sr_mat.diffuse = { 1,1,1,1 };
		sr_mat.power = 0;
		sr_mat.specular = { 0,0,0,0 };
		g_sr->SetMaterial(sr_mat);
		g_sr->SetTexture(src_front.Get());
		g_sr->SetSourceRect({ 31 / 32.0f,0,1 / 32.0f,1 / 32.0f });
		g_sr->SetLocalTransform(XMMatrixTranslation(0.5, -0.5, 0));
		g->AddComponent(g_sr);

		g_sr = new SpriteRender;
		g_sr->SetMaterial(sr_mat);
		g_sr->SetTexture(src_front.Get());
		g_sr->SetSourceRect({ 31 / 32.0f,42 / 1024.0f,1 / 32.0f,1 / 32.0f });
		g_sr->SetLocalTransform(XMMatrixTranslation(0.5, -1.6, 0));
		g->AddComponent(g_sr);

		lifeText = new TextRender2D(false);
		lifeText->SetTextFormat(tf_digit.Get());
		lifeText->SetOutlineEnabled(true);
		lifeText->SetOutlineWidth(2);
		g->AddComponent(lifeText.Get());

		bombText = new TextRender2D(false);
		bombText->SetTextFormat(tf_digit.Get());
		bombText->SetOutlineEnabled(true);
		bombText->SetOutlineWidth(2);
		bombText->SetRect({ 0,16,100,116 });
		g->AddComponent(bombText.Get());

		scene->AttachActor(new RenderTargetSet(GameSceneCom::gameTarget.Get(), 0, false))->InsertBefore(this);
	}

	void OnUpdate() override
	{
		wchar_t text[32];
		wsprintf(text, L"%d", playerData.life);
		lifeText->SetText(text);
		wsprintf(text, L"%d", playerData.bomb);
		bombText->SetText(text);
		if (player->transform->location.x > 245 && player->transform->location.y < -175)
			hide = true;
		else
			hide = false;
		if (hide)
		{
			mat.diffuse.a -= 0.05;
			if (mat.diffuse.a < 0.2)
				mat.diffuse.a = 0.2;
		}
		else
		{
			mat.diffuse.a += 0.05;
			if (mat.diffuse.a > 1.0)
				mat.diffuse.a = 1.0;
		}
		sr->SetMaterial(mat);
	}

private:

	Material mat;

	bool hide;

	ComPtr<IRenderTarget> target;

	SpriteRender* sr;

	ComPtr<TextRender2D> lifeText;

	ComPtr<TextRender2D> bombText;
};

class PowerAndPoint :public GameObject
{
public:

	PowerAndPoint() :sr(AddComponent(new SpriteRender)), hide(false)
	{
		transform->location = { -320,-300,0 };
		transform->scale = { 120,96,1 };
		RenderTargetDesc desc;
		desc.width = 320;
		desc.height = 240;
		desc.msaaEnabled = false;
		desc.support2D = true;
		desc.hasDepth = true;
		desc.multiSampleCount = 0;
		engine->CreateRenderTarget(desc, &target);
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,1 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(target.Get());
		sr->SetLocalTransform(XMMatrixTranslation(0.5, 0.5, 0));
	}

protected:

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::locator[20].Get());
		scene->AttachActor(new RenderTargetSet(target.Get(), 0))->InsertBefore(this);

		LPGOBJ g = scene->AttachActor(new GameObject);
		g->transform->location = { -320,240 ,0 };
		g->transform->scale = { 96,96,1 };
		g->transform2D->location = { 50,0 };
		g->transform2D->scale = { 2.8,2.8 };
		g->InsertBefore(this);
		SpriteRender* g_sr = new SpriteRender;
		Material sr_mat;
		sr_mat.ambient = { 0,0,0,0 };
		sr_mat.diffuse = { 1,1,1,1 };
		sr_mat.power = 0;
		sr_mat.specular = { 0,0,0,0 };
		g_sr->SetMaterial(sr_mat);
		g_sr->SetTexture(src_item.Get());
		g_sr->SetSourceRect({ 12 / 16.0f,0,1 / 16.0f,1 / 4.0f });
		g_sr->SetLocalTransform(XMMatrixTranslation(0.5, -0.5, 0));
		g->AddComponent(g_sr);

		g_sr = new SpriteRender;
		g_sr->SetMaterial(sr_mat);
		g_sr->SetTexture(src_item.Get());
		g_sr->SetSourceRect({ 13 / 16.0f,0,1 / 16.0f,1 / 4.0f });
		g_sr->SetLocalTransform(XMMatrixTranslation(0.5, -1.4, 0));
		g->AddComponent(g_sr);

		powerText = new TextRender2D(false);
		powerText->SetTextFormat(tf_digit.Get());
		powerText->SetOutlineEnabled(true);
		powerText->SetOutlineWidth(2);
		g->AddComponent(powerText.Get());

		pointText = new TextRender2D(false);
		pointText->SetTextFormat(tf_digit.Get());
		pointText->SetOutlineEnabled(true);
		pointText->SetOutlineWidth(2);
		pointText->SetRect({ 0,16,100,116 });
		g->AddComponent(pointText.Get());

		scene->AttachActor(new RenderTargetSet(GameSceneCom::gameTarget.Get(), 0, false))->InsertBefore(this);
	}

	void OnUpdate() override
	{
		wchar_t text[32];
		wsprintf(text, L"%d.%02d/4.00", playerData.power / 100, playerData.power % 100);
		powerText->SetText(text);
		if (playerData.maxPoint < 1000000)
			wsprintf(text, L"%d,%03d", playerData.maxPoint / 1000, playerData.maxPoint % 1000);
		else
			wsprintf(text, L"%d,%03d,%03d", playerData.maxPoint / 1000000, (playerData.maxPoint % 1000000) / 1000, playerData.maxPoint % 1000);
		pointText->SetText(text);
		if (player->transform->location.x < -200 && player->transform->location.y < -175)
			hide = true;
		else
			hide = false;
		if (hide)
		{
			mat.diffuse.a -= 0.05;
			if (mat.diffuse.a < 0.2)
				mat.diffuse.a = 0.2;
		}
		else
		{
			mat.diffuse.a += 0.05;
			if (mat.diffuse.a > 1.0)
				mat.diffuse.a = 1.0;
		}
		sr->SetMaterial(mat);
	}

private:

	Material mat;

	bool hide;

	ComPtr<IRenderTarget> target;

	SpriteRender* sr;

	ComPtr<TextRender2D> powerText;

	ComPtr<TextRender2D> pointText;
};

class BossName :public GameObject
{
public:

	BossName() :tr(AddComponent(new TextRender2D(false)))
	{
		transform2D->location = { 2,0 };
		tr->SetOutlineEnabled(true);
		tr->SetTextFormat(tf_wryh.Get());
		tr->SetRect({ 0,0,200,100 });
	}

	void OnUpdate() override
	{
		wchar_t text[32];
		wsprintf(text, L"%ls", bossName.c_str());
		tr->SetText(text);
	}

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::locator[20].Get());
	}

private:

	TextRender2D* tr;
};

class Score :public GameObject
{
public:

	Score() :tr(AddComponent(new TextRender2D(false)))
	{
		transform2D->location = { 495,0 };
		tr->SetOutlineEnabled(true);
		tr->SetTextFormat(tf_digit.Get());
		tr->SetRect({ 0,0,200,100 });
	}

	void OnUpdate() override
	{
		wchar_t text[32];
		wsprintf(text, L"SCORE  %05d%05d", playerData.score / 100000, playerData.score % 100000);
		tr->SetText(text);
	}

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::locator[20].Get());
	}

private:

	TextRender2D* tr;
};

class HiScore :public GameObject
{
public:

	HiScore() :tr(AddComponent(new TextRender2D(false)))
	{
		transform2D->location = { 322,0 };
		tr->SetOutlineEnabled(true);
		tr->SetTextFormat(tf_digit.Get());
		tr->SetRect({ 0,0,200,100 });
	}

	void OnUpdate() override
	{
		if (!isReplaying)
			showHighScore = playerData.hiscore;
		wchar_t text[32];
		wsprintf(text, L"HI SCORE  %05d%05d", showHighScore / 100000, showHighScore % 100000);
		tr->SetText(text);
	}

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::locator[20].Get());
	}

private:

	TextRender2D* tr;
};

class BossLife :public GameObject
{
	class Drawer :public SpriteRender
	{
	public:

		Drawer() :m_bossLife(0) {}

	private:

		int m_bossLife;

		void OnFetchData() override
		{
			m_bossLife = bossLife;
			__super::OnFetchData();
		}

		void OnRender() override
		{
			UINT stride = 32;
			UINT offset = 0;

			pContext->IASetVertexBuffers(0, 1, vb_Sprite.GetAddressOf(), &stride, &offset);

			XMMATRIX matW = mlocalTransform * transform;
			renderer->SetTextureMatrix(mTexTransform);
			if (mTexture)
				renderer->SendShaderData(SD_TEXTURE, mTexture->GetShaderResourceView(), 0);
			else
				renderer->SendShaderData(SD_TEXTURE, 0, 0);
			renderer->SendShaderData(SD_MATERIAL, &mMaterial, 0);
			for (int i = 0; i < m_bossLife; i++)
			{
				renderer->SetWorldMatrix(matW);
				renderer->StartRender();
				pContext->Draw(6, 0);
				matW = XMMatrixTranslation(1, 0, 0) * matW;
			}
		}
	};

public:

	BossLife() :sr(AddComponent(new Drawer))
	{
		transform->scale = { 10.5,10.5,1 };
		transform->location = { -312,218,0 };
		Material mat;
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,1 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_front.Get());
		sr->SetSourceRect({ 786 / 1024.0f,159 / 1024.0f,21 / 1024.0f,21 / 1024.0f });
	}

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::locator[20].Get());
	}

private:

	SpriteRender* sr;
};

class Surface:public GameObject
{
protected:

	void OnCreated() override
	{
		ComPtr<IRenderTarget> target;
		RenderTargetDesc desc;
		desc.width = 640;
		desc.height = 480;
		desc.msaaEnabled = false;
		desc.support2D = true;
		desc.hasDepth = true;
		desc.multiSampleCount = 0;
		engine->CreateRenderTarget(desc, &target);

		scene->AttachActor(new RenderTargetSet(target.Get(), 0, true))->InsertBefore(GameSceneCom::locator[20].Get());
		scene->AttachActor(new BossName);
		scene->AttachActor(new BossLife);
		scene->AttachActor(new HiScore);
		scene->AttachActor(new Score);
		scene->AttachActor(new RenderTargetSet(GameSceneCom::gameTarget.Get(), 0, false))->InsertBefore(GameSceneCom::locator[20].Get());
		
		auto g = scene->AttachActor(new GameObject);
		g->transform->scale = { 640,480,1 };
		SpriteRender* g_sr = new SpriteRender;
		Material mat;
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,1 };
		mat.specular = { 0,0,0,0 };
		mat.power = 0;
		g_sr->SetMaterial(mat);
		g_sr->SetTexture(target.Get());
		g->AddComponent(g_sr);
		g->InsertBefore(GameSceneCom::locator[20].Get());

		scene->AttachActor(new PowerAndPoint);
		scene->AttachActor(new LifeAndBomb);
		Destroy();
	}
};