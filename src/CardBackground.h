#pragma once
#include "VirtualLib.h"
#include "GameScene.h"
#include "BossSpellCard.h"
#include "UserRenderer.h"

class CardBackground :public GameObject
{
protected:

	class CardSurface :public GameObject
	{
		enum CSTATE
		{
			Entering,
			Running,
			Leaving
		};

	public:

		SpriteRender* sr;

		Material mat;

		CardSurface(BLEND_STATE _blendState, float enterAlphaPerFrame, float leaveAlphaPerFrame, float maxAlpha = 1) :
			sr(0), state(Entering), ea(enterAlphaPerFrame),
			la(leaveAlphaPerFrame), ma(maxAlpha)
		{
			transform->scale = { 640,480,1 };
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = { 1,1,1,0 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr = new SpriteRender;
			sr->SetMaterial(mat);
			AddComponent(new BlendStateSet::Setter(_blendState));
			AddComponent(sr);
		}

		virtual void OnEnter()
		{
			state = Entering;
		}

		virtual void OnLeave()
		{
			state = Leaving;
		}

		void OnCreated() override
		{
			InsertBefore(GameSceneCom::locator[3].Get());
		}

		void OnUpdate() override
		{
			switch (state)
			{
			case Entering:
				mat.diffuse.a += ea;
				sr->SetMaterial(mat);
				if (mat.diffuse.a > ma)
				{
					mat.diffuse.a = ma;
					sr->SetMaterial(mat);
					state = Running;
				}
				break;
			case Leaving:
				mat.diffuse.a -= la;
				sr->SetMaterial(mat);
				if (mat.diffuse.a < 0)
				{
					mat.diffuse.a = 0;
					sr->SetMaterial(mat);
					Destroy();
				}
				break;
			}
		}

	private:

		CSTATE state;

		float ea;

		float la;

		float ma;
	};

public:

	vector<ComPtr<CardSurface>> cardSurface;

	ComPtr<BossSpellCard> spellCard;

	CardBackground(BossSpellCard* sc) :spellCard(sc)
	{
	}

protected:

	void AddSurface(CardSurface* cs)
	{
		cardSurface.push_back(cs);
		scene->AttachActor(cs);
		cs->OnEnter();
	}

	void OnCreated() final
	{
		InsertBefore(GameSceneCom::locator[3].Get());
		OnAddSurface();
	}

	void OnUpdate() override
	{
		if (!spellCard->IsValid())
		{
			for (auto e : cardSurface)
				e->OnLeave();
			Destroy();
		}
	}

	void OnDestroy() override
	{
		spellCard.Reset();
		cardSurface.clear();
	}

	virtual void OnAddSurface() = 0;
};

class CardCircle :public GameObject
{
	class Circle :public GameObject
	{
	public:

		CircleDrawer* cd;

		Material mat;

		Circle(bool _warp) :
			warp(_warp), cd(0)
		{
			if (warp)
			{
				cd = AddComponent(new CircleDrawer(0, 20, 3));
				cd->SetSourceRect({ 5 / 8.0f,0,1 / 8.0f,1 });
			}
			else
			{
				cd = AddComponent(new CircleDrawer(0, 0, 4));
				cd->SetSourceRect({ 3 / 8.0f,0,1 / 8.0f,1 });
			}
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = { 1,1,1,0.5f };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			cd->SetMaterial(mat);
			cd->SetTexture(src_line.Get());
		}

	private:

		bool warp;

		void OnCreated() override
		{
			if (warp)
				InsertBefore(GameSceneCom::locator[4].Get());
			else
				InsertBefore(GameSceneCom::locator[6].Get());
		}
	};

public:

	CardCircle(BossSpellCard* sc) :
		spellCard(sc), frame(0), speed(9.08), aspeed(-0.2)
	{
	}

private:

	int frame;

	float speed;

	float aspeed;

	ComPtr<Circle> circle_in;

	ComPtr<Circle> circle_out;

	ComPtr<BossSpellCard> spellCard;

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::locator[4].Get());
		circle_out = scene->AttachActor(new Circle(true));
		circle_in = scene->AttachActor(new Circle(false));
		circle_out->transform->location = spellCard->GetGameObject()->transform->location * 0.8;
		circle_in->transform->location = spellCard->GetGameObject()->transform->location;
	}

	void OnUpdate() override
	{
		if (spellCard->IsValid())
		{
			frame++;
			circle_out->transform->Rotate({ 0,0,2 }, true);
			circle_in->transform->Rotate({ 0,0,-2 }, true);
			if (frame < 60)
				circle_out->transform->location = spellCard->GetGameObject()->transform->location * 0.8;
			else
				circle_out->transform->location = Lerp(circle_out->transform->location, spellCard->GetGameObject()->transform->location * 0.8, 0.2);
			circle_in->transform->location = spellCard->GetGameObject()->transform->location;
			if (frame <= 60)
			{
				circle_in->cd->SetMaxRadius(Lerp(20.0f, 195.0f, sqrt(frame / 60.0f)));
				circle_in->cd->SetMinRadius(Lerp(0.0f, 175.0f, sqrt(frame / 60.0f)));
			}
			else
			{
				circle_in->cd->SetMaxRadius(Lerp(40.0f, 195.0f, (spellCard->remainTime + 1) / spellCard->maxTime));
				circle_in->cd->SetMinRadius(Lerp(20.0f, 175.0f, (spellCard->remainTime + 1) / spellCard->maxTime));
			}
			if (frame <= 70)
			{
				circle_out->cd->SetMaxRadius(circle_out->cd->GetMaxRadius() + speed);
				circle_out->cd->SetMinRadius(circle_out->cd->GetMinRadius() + speed);
				speed += aspeed;
			}
			else
			{
				float scale = Lerp(0.5f, 1.0f, ((spellCard->remainTime + 1) / spellCard->maxTime) * ((spellCard->remainTime + 1) / spellCard->maxTime));
				circle_out->transform->scale = { scale,scale,1 };
			}
		}
		else
		{
			Destroy();
		}
	}

	void OnDestroy() override
	{
		if (circle_in)
		{
			circle_in->Destroy();
			circle_in.Reset();
		}
		if (circle_out)
		{
			circle_out->Destroy();
			circle_out.Reset();
		}
	}
};

class SpellCardAttack :public GameObject
{
	class Unit :public GameObject
	{
	public:

		Unit(bool _moveToRight) :
			sr(AddComponent(new SpriteRender)), frame(0), moveToRight(_moveToRight)
		{
			transform->SetEulerAngles({ 0,0,(float)-M_PI / 3.0f });
			transform->scale = { 16,128,1 };
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = { 1,1,1,0 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_line.Get());
			sr->SetSourceRect({ 3 / 4.0f,0,1 / 8.0f,1 });
		}

	private:

		int frame;

		SpriteRender* sr;

		Material mat;

		bool moveToRight;

		void OnCreated() override
		{
			InsertBefore(GameSceneCom::locator[6].Get());
		}

		void OnUpdate() override
		{
			frame++;
			const Vector3 vec = Vector3(cos((float)M_PI / 6.0f), sin((float)M_PI / 6.0f), 0) * 2;
			if (moveToRight)
				transform->Translate(vec);
			else
				transform->Translate(-vec);
			if (frame <= 10)
			{
				mat.diffuse.a += 0.05f;
				sr->SetMaterial(mat);
			}
			else if (frame >= 50)
			{
				mat.diffuse.a -= 0.05f;
				sr->SetMaterial(mat);
			}
			if (frame == 60)
			{
				Destroy();
			}
		}
	};

public:

	SpellCardAttack()
	{
	}

	void OnCreated() override
	{
		se_cat00.Play();
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 12; j++)
			{
				const Vector3 vec = Vector3(cos((float)M_PI / 6.0f), sin((float)M_PI / 6.0f), 0) * 120;
				scene->AttachActor(new Unit(i % 2))->transform->location = vec * j + Vector3{ -440.0f,-390 + 40.0f * i,0 };
			}
		}
		Destroy();
	}
};

class SpellCardText :public GameObject
{
	class Text :public GameObject
	{
	public:

		TextRender2D* tr;

		Text() :tr(AddComponent(new TextRender2D(false))), frame(0)
		{
			transform2D->scale = { 2,2 };
			transform2D->location = { 615,405 };
			tr->SetTextFormat(tf_songti_spell.Get());
			tr->SetBodyBrush(sb_white.Get());
			tr->SetOutlineBrush(sb_black.Get());
			tr->SetOutlineEnabled(true);
			tr->SetText(L"测符「正在测试正在测试」");
			tr->SetOutlineWidth(4);
			tr->SetRect({ -300,-50,0,0 });
		}
		
		void OnUpdate() override
		{
			frame++;
			if (frame <= 25)
			{
				transform2D->scale -= Vector2{0.04, 0.04};
			}
		}

	private:

		int frame;
	};

	class Back :public GameObject
	{
	public:

		Back() :sr(AddComponent(new SpriteRender)), frame(0)
		{
			transform->location = { 140,-180,0 };
			transform->scale = { 384,67.5,1 };
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = { 1,1,1,0 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_ascii.Get());
			sr->SetSourceRect({ 0,390 / 512.0f,1,90 / 512.0f });
		}

	private:

		int frame;

		SpriteRender* sr;

		Material mat;

		void OnUpdate() override
		{
			frame++;
			if (frame > 15 && frame <= 35)
			{
				mat.diffuse.a += 0.05;
				sr->SetMaterial(mat);
				transform->location.y += 1;
			}
		}
	};

	class Bonus :public GameObject
	{
	public:

		Bonus() :sr(AddComponent(new SpriteRender)), frame(0)
		{
			transform->location = { 80,-180,0 };
			transform->scale = Vector3{ 57,19,1 }*0.65;
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = { 1,1,1,0 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_ascii.Get());
			sr->SetSourceRect({ 6/512.0f,364 / 512.0f,57/512.0f,19 / 512.0f });
		}

	private:

		int frame;

		SpriteRender* sr;

		Material mat;

		void OnUpdate() override
		{
			frame++;
			if (frame <= 10)
			{
				mat.diffuse.a += 0.1;
				sr->SetMaterial(mat);
			}
		}
	};

	class BonusNumber :public GameObject
	{
		class Drawer :public SpriteRender
		{
		public:

			Drawer() :number(0), m_number(0)
			{
			}

			void SetNumber(int _number)
			{
				number = _number;
			}

			void OnFetchData() override
			{
				m_failed = spellFailed;
				m_number = number;
				if (m_number > 99999999)
					m_number = 99999999;
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

				if (!m_failed)
				{
					XMMATRIX matW = XMMatrixScaling(10, 11.125, 1) * XMMatrixTranslation(100, -180, 0);
					int maxBit = GetMaxBit(m_number);
					for (int x = maxBit; x > 0; x--)
					{
						matW *= XMMatrixTranslation(8.5, 0, 0);
						renderer->SetWorldMatrix(matW);
						renderer->SetTextureMatrix(GetTextureMat(GetBitNum(number, x)));
						renderer->StartRender();
						pContext->Draw(6, 0);
					}
				}
				else
				{
					SetSourceRect({ 288 / 512.0f,365 / 512.0f,61 / 512.0f,18 / 512.0f });
					XMMATRIX matW = XMMatrixScaling(30.5 * 1.25f, 11.125, 1) * XMMatrixTranslation(124, -180, 0);

					renderer->SetWorldMatrix(matW);
					renderer->SetTextureMatrix(texTransform);
					renderer->StartRender();
					pContext->Draw(6, 0);
				}
			}

			XMMATRIX GetTextureMat(BYTE n)
			{
				SetSourceRect({ (16 * n) / 512.0f,320 / 512.0f,16 / 512.0f,18 / 512.0f });
				return texTransform;
			}

			BYTE GetMaxBit(int number)
			{
				if (number > 9999999)
					return 8;
				if (number > 999999)
					return 7;
				if (number > 99999)
					return 6;
				if (number > 9999)
					return 5;
				if (number > 999)
					return 4;
				if (number > 99)
					return 3;
				if (number > 9)
					return 2;
				return 1;
			}

			BYTE GetBitNum(int number, int bit) //bit:个位为1
			{
				return (number / PowOfTen(bit - 1)) % 10;
			}

			int PowOfTen(int x)
			{
				int v = 1;
				for (int i = 0; i < x; i++)
					v *= 10;
				return v;
			}

		private:

			int number;

			int m_number;

			bool m_failed;
		};

	public:

		Drawer* dr;

		Material mat;

		BonusNumber() :dr(AddComponent(new Drawer))
		{
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = { 1,1,1,1 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			dr->SetMaterial(mat);
			dr->SetTexture(src_ascii.Get());
		}
	};

	class History :public GameObject
	{
	public:

		History() :sr(AddComponent(new SpriteRender)), frame(0)
		{
			transform->location = { 200,-180,0 };
			transform->scale = Vector3{ 62,19,1 }*0.65;
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = { 1,1,1,0 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_ascii.Get());
			sr->SetSourceRect({ 189 / 512.0f,364 / 512.0f,62 / 512.0f,19 / 512.0f });
		}

	private:

		int frame;

		SpriteRender* sr;

		Material mat;

		void OnUpdate() override
		{
			frame++;
			if (frame <= 10)
			{
				mat.diffuse.a += 0.1;
				sr->SetMaterial(mat);
			}
		}
	};

	class HistoryNumber :public GameObject
	{
		class Drawer :public SpriteRender
		{
		public:

			Drawer() :got(0), total(0)
			{
			}

			void SetGot(int _number)
			{
				got = _number;
			}

			void SetTotal(int _number)
			{
				total = _number;
			}

			void OnFetchData() override
			{
				m_got = got;
				m_total = total;
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

				XMMATRIX matW = XMMatrixScaling(10, 11.125, 1) * XMMatrixTranslation(222, -180, 0);
				for (int x = 3; x > 0; x--)
				{
					matW *= XMMatrixTranslation(8.5, 0, 0);
					renderer->SetWorldMatrix(matW);
					renderer->SetTextureMatrix(GetTextureMat(GetBitNum(m_got, x)));
					renderer->StartRender();
					pContext->Draw(6, 0);
				}
				matW *= XMMatrixTranslation(8.5, 0, 0);
				renderer->SetWorldMatrix(matW);
				renderer->SetTextureMatrix(GetTextureMat(10));
				renderer->StartRender();
				pContext->Draw(6, 0);
				for (int x = 3; x > 0; x--)
				{
					matW *= XMMatrixTranslation(8.5, 0, 0);
					renderer->SetWorldMatrix(matW);
					renderer->SetTextureMatrix(GetTextureMat(GetBitNum(m_total, x)));
					renderer->StartRender();
					pContext->Draw(6, 0);
				}
			}

			XMMATRIX GetTextureMat(BYTE n)
			{
				SetSourceRect({ (16 * n) / 512.0f,320 / 512.0f,16 / 512.0f,18 / 512.0f });
				return texTransform;
			}

			BYTE GetMaxBit(int number)
			{
				if (number > 9999999)
					return 8;
				if (number > 999999)
					return 7;
				if (number > 99999)
					return 6;
				if (number > 9999)
					return 5;
				if (number > 999)
					return 4;
				if (number > 99)
					return 3;
				if (number > 9)
					return 2;
				return 1;
			}

			BYTE GetBitNum(int number, int bit) //bit:个位为1
			{
				return (number / PowOfTen(bit - 1)) % 10;
			}

			int PowOfTen(int x)
			{
				int v = 1;
				for (int i = 0; i < x; i++)
					v *= 10;
				return v;
			}

		private:

			int got;

			int total;

			int m_got;

			int m_total;
		};

	public:

		Drawer* dr;

		Material mat;

		HistoryNumber() :dr(AddComponent(new Drawer))
		{
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = { 1,1,1,1 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			dr->SetMaterial(mat);
			dr->SetTexture(src_ascii.Get());
		}
	};

public:

	SpellCardText(BossSpellCard* _spell, wstring _spellName) :
		sr(AddComponent(new SpriteRender)), frame(0), spell(_spell), deadframe(0), alpha(1), spellName(_spellName)
	{
		transform->scale = { 640,480,1 };
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,0 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
		sr->SetTexture(GameSceneCom::spellTextTarget.Get());
	}

private:

	wstring spellName;

	float alpha;

	int frame;

	int deadframe;

	LPGOBJ rtSet[2];

	SpriteRender* sr;

	Material mat;

	ComPtr<Text> text;

	ComPtr<Back> back;

	ComPtr<Bonus> bonus;

	ComPtr<BonusNumber> number;

	ComPtr<History> history;

	ComPtr<HistoryNumber> hisNumber;

	ComPtr<BossSpellCard> spell;

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::locator[20].Get());
		rtSet[0] = scene->AttachActor(new RenderTargetSet(GameSceneCom::spellTextTarget.Get(), 0, true));
		rtSet[0]->InsertBefore(this);
		rtSet[1] = scene->AttachActor(new RenderTargetSet(GameSceneCom::gameTarget.Get(), 0, false));
		rtSet[1]->InsertBefore(this);
		back = scene->AttachActor(new Back);
		back->InsertBefore(rtSet[1].Get());
		text = scene->AttachActor(new Text);
		text->tr->SetText(spellName);
		text->InsertBefore(rtSet[1].Get());
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
		}
		if (text)
		{
			text->Destroy();
			text.Reset();
		}
		if (back)
		{
			back->Destroy();
			back.Reset();
		}
		if (bonus)
		{
			bonus->Destroy();
			bonus.Reset();
		}
		if (number)
		{
			number->Destroy();
			number.Reset();
		}
		if (history)
		{
			history->Destroy();
			history.Reset();
		}
		if (hisNumber)
		{
			hisNumber->Destroy();
			hisNumber.Reset();
		}
		spell.Reset();
	}

	void OnUpdate() override
	{
		frame++;
		if (frame <= 10)
		{
			mat.diffuse.a += 0.1;
			sr->SetMaterial(mat);
		}
		else if (frame >= 60)
		{
			if (player->transform->location.x > -50 && abs(player->transform->location.y - (transform->location.y - 180)) < 50)
			{
				alpha -= 0.05;
				if (alpha < 0.2)
					alpha = 0.2;
				mat.diffuse.a = alpha;
				sr->SetMaterial(mat);
			}
			else
			{
				alpha += 0.05;
				if (alpha > 1)
					alpha = 1;
				mat.diffuse.a = alpha;
				sr->SetMaterial(mat);
			}
		}
		if (frame == 60)
		{
			bonus = scene->AttachActor(new Bonus);
			bonus->InsertBefore(rtSet[1].Get());
			number = scene->AttachActor(new BonusNumber);
			number->InsertBefore(rtSet[1].Get());
			history = scene->AttachActor(new History);
			history->InsertBefore(rtSet[1].Get());
			hisNumber = scene->AttachActor(new HistoryNumber);
			hisNumber->InsertBefore(rtSet[1].Get());
		}
		if (frame >= 60 && spell->IsValid())
		{
			transform->location = Lerp(transform->location, Vector3(0, 355, 0), 0.1f);
			number->dr->SetNumber(spell->score);
			hisNumber->dr->SetGot(SCHistory::GetHistoryGot(spell->cardID, difficulty));
			hisNumber->dr->SetTotal(SCHistory::GetHistoryTotal(spell->cardID, difficulty));
		}
		else if (!spell->IsValid())
		{
			transform->location = Lerp(transform->location, Vector3(300, 355, 0), 0.1f);
			deadframe++;
			if (deadframe == 15)
				Destroy();
		}
	}
};