#pragma once
#include "VirtualLib.h"
#include "Source.h"
#include "GameScene.h"
#include "UserRenderer.h"
#include "GameGlobal.h"
#include "Mathf.h"
#include "SceneObjects.h"
#include "SoundEffect.h"

class LifeBar :public GameObject
{
public:

	LifeBar(LPGOBJ _follow, float _minValue, float _maxValue, bool _startfull, bool _showTag, double time, bool _nodraw) :
		follow(_follow), hide(false), frame(0), remainTime(time), minValue(_minValue), maxValue(_maxValue),
		value(maxValue), startfull(_startfull), showTag(_showTag), border(AddComponent(new CircleDrawer(60, 65, 1))),
		bar(AddComponent(new CircleDrawer(61, 64, 1))), tag_sr(0), timeOut(10), timeStop(false), setTimeStop(false), nodraw(_nodraw)
	{
		if (follow)
			transform->location = follow->transform->location;

		transform->SetEulerAngles({ 0,0,M_PI_2 });
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,nodraw ? 0.0f : 1.0f };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		border->SetMaterial(mat);
		border->SetTexture(src_lifebar.Get());
		border->SetSourceRect({ 0.25,0,0.25,1 });

		bar->SetMaterial(mat);
		bar->SetTexture(src_lifebar.Get());
		bar->SetSourceRect({ 0,0,0.25,1 });
	}

	void SetPercent(float percent)
	{
		value = minValue + (maxValue - minValue) * percent;
	}

	void TimeStop()
	{
		setTimeStop = true;
	}

private:

	class TimeText;

	bool nodraw;

	bool timeStop;

	bool setTimeStop;

	bool hide;

	int frame;

	double remainTime;

	float minValue;

	float maxValue;

	float value;

	bool startfull;

	bool showTag;

	CircleDrawer* border;

	CircleDrawer* bar;

	Material mat;

	LPGOBJ follow;

	LPGOBJ tag;

	SpriteRender* tag_sr;

	ComPtr<TimeText> timeText;

	int timeOut;

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::locator[20].Get());
		if (showTag)
		{
			const float angle = (float)(54.0 / 180.0 * M_PI);
			tag = scene->AttachActor(new GameObject);
			tag->transform->scale = { 5,6,1 };
			tag->transform->SetEulerAngles({ 0,0,angle });
			tag->InsertBefore(GameSceneCom::locator[20].Get());
			tag_sr = new SpriteRender;
			tag_sr->SetMaterial(mat);
			tag_sr->SetTexture(src_lifebar.Get());
			tag_sr->SetSourceRect({ 9 / 16.0f, 0,5 / 16.0f,3 / 8.0f });
			tag->AddComponent(tag_sr);
			tag->transform->location = transform->location + GetVector((float)(144 / 180.0 * M_PI), 62.5);
		}
		timeText = scene->AttachActor(new TimeText);
		timeText->InsertBefore(GameSceneCom::locator[20].Get());
	}

	void OnUpdate() override
	{
		frame++;
		if (!timeStop)
			remainTime -= timer->GetDeltaTime();
		if (remainTime < 0)
			remainTime = 0;
		if (remainTime < timeOut)
		{
			se_timeout.Play();
			timeOut--;
			timeText->SetTimeOut();
		}
		if (!timeStop)
			timeText->SetTime(remainTime);
		if (!startfull && frame <= 60)
			bar->SetPercent(frame / 60.0f);
		else
			bar->SetPercent(value);
		if (follow)
		{
			transform->location = follow->transform->location;
			if (tag)
				tag->transform->location = transform->location + GetVector((float)(144 / 180.0 * M_PI), 62.5);
		}
		if ((player->transform->location - transform->location).GetLength() < 100)
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
		if (nodraw)
			mat.diffuse.a = 0;
		border->SetMaterial(mat);
		bar->SetMaterial(mat);
		if (tag)
			tag_sr->SetMaterial(mat);
		if (setTimeStop)
			timeStop = true;
	}

	void OnDestroy() override
	{
		follow.Reset();
		if (tag)
		{
			tag->Destroy();
			tag.Reset();
		}
		if (timeText)
		{
			timeText->Destroy();
			timeText.Reset();
		}
	}

	class TimeText :public GameObject
	{
		class TimeRender :public SpriteRender
		{
		public:

			TimeRender() :t(0), m_t(0), size(1), m_size(1) {}

			void SetTime(double _t)
			{
				t = _t;
			}

			void SetSize(float _size)
			{
				size = _size;
			}

		private:

			double t;

			double m_t;

			float size;

			float m_size;

			void OnUpdate() override
			{
				if (size > 1)
					size -= 0.1;
				if (size < 1)
					size = 1;
			}

			void OnFetchData() override
			{
				m_t = t;
				m_size = size;
				if (m_t > 99.99)
					m_t = 99.99;
				__super::OnFetchData();
			}

			void OnRender() override
			{
				UINT stride = 32;
				UINT offset = 0;

				pContext->IASetVertexBuffers(0, 1, vb_Sprite.GetAddressOf(), &stride, &offset);

				if (mTexture)
					renderer->SendShaderData(SD_TEXTURE, mTexture->GetShaderResourceView(), 0);
				else
					renderer->SendShaderData(SD_TEXTURE, 0, 0);
				renderer->SendShaderData(SD_MATERIAL, &mMaterial, 0);

				XMMATRIX matW = XMMatrixScaling(m_size, m_size, 1) * XMMatrixScaling(72, 72, 1) * XMMatrixTranslation(-100, 0, 0);
				renderer->SetWorldMatrix(matW);
				renderer->SetTextureMatrix(GetTextureMat(((int)(m_t / 10)) % 10));
				renderer->StartRender();
				pContext->Draw(6, 0);

				matW = XMMatrixScaling(m_size, m_size, 1) * XMMatrixScaling(72, 72, 1) * XMMatrixTranslation(-45, 0, 0);
				renderer->SetWorldMatrix(matW);
				renderer->SetTextureMatrix(GetTextureMat(((int)(m_t)) % 10));
				renderer->StartRender();
				pContext->Draw(6, 0);

				matW = XMMatrixScaling(72, 72, 1) * XMMatrixTranslation(10, 0, 0);
				renderer->SetWorldMatrix(matW);
				renderer->SetTextureMatrix(GetTextureMat(11));
				renderer->StartRender();
				pContext->Draw(6, 0);

				matW = XMMatrixScaling(48, 48, 1) * XMMatrixTranslation(40, -12, 0);
				renderer->SetWorldMatrix(matW);
				renderer->SetTextureMatrix(GetTextureMat(((int)(m_t * 10)) % 10));
				renderer->StartRender();
				pContext->Draw(6, 0);

				matW = XMMatrixScaling(48, 48, 1) * XMMatrixTranslation(80, -12, 0);
				renderer->SetWorldMatrix(matW);
				renderer->SetTextureMatrix(GetTextureMat(((int)(m_t * 100)) % 10));
				renderer->StartRender();
				pContext->Draw(6, 0);
			}

			XMMATRIX GetTextureMat(BYTE n)
			{
				SetSourceRect({ (32 * n) / 512.0f,288 / 512.0f,32 / 512.0f,32 / 512.0f });
				return texTransform;
			}
		};

	public:

		TimeText() :sr(0), hide(false),tr(0)
		{
			transform->scale = { 160,120,1 };
			transform->location = { 0,200,0 };
			transform2D->location = { 80,60 };

			RenderTargetDesc desc;
			desc.width = 160;
			desc.height = 120;
			desc.msaaEnabled = false;
			desc.support2D = true;
			desc.hasDepth = true;
			desc.multiSampleCount = 0;
			engine->CreateRenderTarget(desc, &target);
			AddComponent(new RenderTargetSet::Setter(target.Get(), 0));

			tr = new TimeRender;
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = { 1,1,1,1 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			tr->SetMaterial(mat);
			tr->SetTexture(src_ascii.Get());
			AddComponent(tr);

			AddComponent(new RenderTargetSet::Setter(GameSceneCom::gameTarget.Get(), 0, false));

			sr = new SpriteRender;
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = { 1,1,1,1 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(target.Get());
			AddComponent(sr);
		}

		void OnUpdate() override
		{
			if ((player->transform->location - transform->location).GetLength() < 70)
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

		void SetTimeOut()
		{
			mat.diffuse.g = 0;
			mat.diffuse.b = 0;
		}

		void SetTime(double t)
		{
			tr->SetTime(t);
			if ((int)(t + timer->GetDeltaTime()) > (int)t&& t <= 5)
				tr->SetSize(2);
		}

	private:

		ComPtr<IRenderTarget> target;

		Material mat;

		SpriteRender* sr;

		TimeRender* tr;

		bool hide;
	};
};