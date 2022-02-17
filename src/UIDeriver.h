#pragma once
#include "UI.h"
#include "Mathf.h"

extern ComPtr<UI> nowUI;

class RealTime :public UI
{
	class RealTimeText :public GameObject
	{
		class TimeRender :public SpriteRender
		{
		public:

			TimeRender(double time) :t(time), m_t(time) {}

		private:

			double t;

			double m_t;

			void OnFetchData() override
			{
				m_t = t;
				__super::OnFetchData();
			}

			void OnRender() override
			{
				UINT stride = 32;
				UINT offset = 0;

				const float m_size = 16;

				pContext->IASetVertexBuffers(0, 1, vb_Sprite.GetAddressOf(), &stride, &offset);

				if (mTexture)
					renderer->SendShaderData(SD_TEXTURE, mTexture->GetShaderResourceView(), 0);
				else
					renderer->SendShaderData(SD_TEXTURE, 0, 0);
				renderer->SendShaderData(SD_MATERIAL, &mMaterial, 0);

				float x = 17;
				const float y = -2;
				const float dx = 13;
				XMMATRIX matW;

				if (m_t >= 100)
				{
					matW = XMMatrixScaling(m_size, m_size, 1) * XMMatrixTranslation(x, y, 0);
					renderer->SetWorldMatrix(matW);
					renderer->SetTextureMatrix(GetTextureMat(((int)(m_t / 100)) % 10));
					renderer->StartRender();
					pContext->Draw(6, 0);
					x += dx;
				}

				if (m_t >= 10)
				{
					matW = XMMatrixScaling(m_size, m_size, 1) * XMMatrixTranslation(x, y, 0);
					renderer->SetWorldMatrix(matW);
					renderer->SetTextureMatrix(GetTextureMat(((int)(m_t / 10)) % 10));
					renderer->StartRender();
					pContext->Draw(6, 0);
					x += dx;
				}

				matW = XMMatrixScaling(m_size, m_size, 1) * XMMatrixTranslation(x, y, 0);
				renderer->SetWorldMatrix(matW);
				renderer->SetTextureMatrix(GetTextureMat((int)(m_t) % 10));
				renderer->StartRender();
				pContext->Draw(6, 0);
				x += dx;

				matW = XMMatrixScaling(m_size, m_size, 1) * XMMatrixTranslation(x, y, 0);
				renderer->SetWorldMatrix(matW);
				renderer->SetTextureMatrix(GetTextureMat(11));
				renderer->StartRender();
				pContext->Draw(6, 0);
				x += dx;

				matW = XMMatrixScaling(m_size, m_size, 1) * XMMatrixTranslation(x, y, 0);
				renderer->SetWorldMatrix(matW);
				renderer->SetTextureMatrix(GetTextureMat(((int)(m_t * 10)) % 10));
				renderer->StartRender();
				pContext->Draw(6, 0);
				x += dx;

				matW = XMMatrixScaling(m_size, m_size, 1) * XMMatrixTranslation(x, y, 0);
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

		Material mat;

		SpriteRender* sr;

		RealTimeText(double time) :sr(AddComponent(new TimeRender(time)))
		{
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = { 1,1,1,0 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_ascii.Get());
		}
	};

public:

	RealTime(double time) :frame(0), t(time)
	{
		transform->location = { -45,-2,0 };
		transform->scale = { 125 * 0.6f,34 * 0.6f,1 };
		mat.diffuse.a = 0;
		sr->SetMaterial(mat);
		sr->SetSourceRect({ 671 / 1024.0f,639 / 1024.0f,125 / 1024.0f,34 / 1024.0f });
		sr->SetTexture(src_front.Get());
	}

protected:

	int frame;

	double t;

	ComPtr<RealTimeText> text;

	void OnUpdate() override
	{
		frame++;
		if (frame <= 10)
		{
			mat.diffuse.a += 0.1;
			sr->SetMaterial(mat);
			text->sr->SetMaterial(mat);
		}
		if (frame > 120)
		{
			mat.diffuse.a -= 0.1;
			sr->SetMaterial(mat);
			text->sr->SetMaterial(mat);
		}
		if (frame == 130)
		{
			Destroy();
		}
	}

	void OnCreated() override
	{
		__super::OnCreated();
		text = scene->AttachActor(new RealTimeText(t));
		text->InsertAfter(this);
	}

	void OnDestroy() override
	{
		if (text)
		{
			text->Destroy();
			text.Reset();
		}
	}
};


class BreakTime :public UI
{
	class BreakTimeText :public GameObject
	{
		class TimeRender :public SpriteRender
		{
		public:

			TimeRender(double time) :t(time), m_t(time) {}

		private:

			double t;

			double m_t;

			void OnFetchData() override
			{
				m_t = t;
				__super::OnFetchData();
			}

			void OnRender() override
			{
				UINT stride = 32;
				UINT offset = 0;

				const float m_size = 16;

				pContext->IASetVertexBuffers(0, 1, vb_Sprite.GetAddressOf(), &stride, &offset);

				if (mTexture)
					renderer->SendShaderData(SD_TEXTURE, mTexture->GetShaderResourceView(), 0);
				else
					renderer->SendShaderData(SD_TEXTURE, 0, 0);
				renderer->SendShaderData(SD_MATERIAL, &mMaterial, 0);

				float x = 17;
				const float y = 20;
				const float dx = 13;
				XMMATRIX matW;

				if (m_t >= 100)
				{
					matW = XMMatrixScaling(m_size, m_size, 1) * XMMatrixTranslation(x, y, 0);
					renderer->SetWorldMatrix(matW);
					renderer->SetTextureMatrix(GetTextureMat(((int)(m_t / 100)) % 10));
					renderer->StartRender();
					pContext->Draw(6, 0);
					x += dx;
				}

				if (m_t >= 10)
				{
					matW = XMMatrixScaling(m_size, m_size, 1) * XMMatrixTranslation(x, y, 0);
					renderer->SetWorldMatrix(matW);
					renderer->SetTextureMatrix(GetTextureMat(((int)(m_t / 10)) % 10));
					renderer->StartRender();
					pContext->Draw(6, 0);
					x += dx;
				}

				matW = XMMatrixScaling(m_size, m_size, 1) * XMMatrixTranslation(x, y, 0);
				renderer->SetWorldMatrix(matW);
				renderer->SetTextureMatrix(GetTextureMat((int)(m_t) % 10));
				renderer->StartRender();
				pContext->Draw(6, 0);
				x += dx;

				matW = XMMatrixScaling(m_size, m_size, 1) * XMMatrixTranslation(x, y, 0);
				renderer->SetWorldMatrix(matW);
				renderer->SetTextureMatrix(GetTextureMat(11));
				renderer->StartRender();
				pContext->Draw(6, 0);
				x += dx;

				matW = XMMatrixScaling(m_size, m_size, 1) * XMMatrixTranslation(x, y, 0);
				renderer->SetWorldMatrix(matW);
				renderer->SetTextureMatrix(GetTextureMat(((int)(m_t * 10)) % 10));
				renderer->StartRender();
				pContext->Draw(6, 0);
				x += dx;

				matW = XMMatrixScaling(m_size, m_size, 1) * XMMatrixTranslation(x, y, 0);
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

		Material mat;

		SpriteRender* sr;

		BreakTimeText(double time) :sr(AddComponent(new TimeRender(time)))
		{
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = { 1,1,1,0 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_ascii.Get());
		}
	};

public:

	BreakTime(double time) :frame(0), t(time)
	{
		transform->location = { -45,20,0 };
		transform->scale = { 125 * 0.6f,34 * 0.6f,1 };
		mat.diffuse.a = 0;
		sr->SetMaterial(mat);
		sr->SetSourceRect({ 671 / 1024.0f,606 / 1024.0f,125 / 1024.0f,34 / 1024.0f });
		sr->SetTexture(src_front.Get());
	}

protected:

	int frame;

	double t;

	ComPtr<BreakTimeText> text;

	void OnUpdate() override
	{
		frame++;
		if (frame <= 10)
		{
			mat.diffuse.a += 0.1;
			sr->SetMaterial(mat);
			text->sr->SetMaterial(mat);
		}
		if (frame > 120)
		{
			mat.diffuse.a -= 0.1;
			sr->SetMaterial(mat);
			text->sr->SetMaterial(mat);
		}
		if (frame == 130)
		{
			Destroy();
		}
	}

	void OnCreated() override
	{
		__super::OnCreated();
		text = scene->AttachActor(new BreakTimeText(t));
		text->InsertAfter(this);
	}

	void OnDestroy() override
	{
		if (text)
		{
			text->Destroy();
			text.Reset();
		}
	}
};

class BonusFailed :public UI
{
public:

	BonusFailed(bool playSound, double breakTime, double realTime) :frame(0), bt(breakTime), rt(realTime), pls(playSound)
	{
		transform->scale = { 152,33.5,1 };
		transform->location = { 0,100,0 };
		mat.diffuse.a = 0;
		sr->SetMaterial(mat);
		sr->SetTexture(src_front.Get());
		sr->SetSourceRect({ 523 / 1024.0f,772 / 1024.0f,304 / 1024.0f,67 / 1024.0f });
	}

protected:

	int frame;

	double bt;

	double rt;

	bool pls;

	void OnUpdate() override
	{
		frame++;
		if (frame <= 10)
		{
			mat.diffuse.a += 0.1;
			sr->SetMaterial(mat);
		}
		if (frame > 120)
		{
			mat.diffuse.a -= 0.1;
			sr->SetMaterial(mat);
		}
		if (frame == 130)
		{
			Destroy();
		}
	}

	void OnCreated() override
	{
		__super::OnCreated();
		scene->AttachActor(new BreakTime(bt));
		scene->AttachActor(new RealTime(rt));
		if (pls)
			se_fault.Play();
	}
};

class GetSpellCardBonus :public UI
{
	class ScoreNumber :public GameObject
	{
		class Number :public GameObject
		{
		public:

			Number(BYTE num, int destroyFrame) :
				sr(AddComponent(new SpriteRender)), speed(90), aspeed(-15), frame(0), angle((float)M_PI / 2), desFrame(destroyFrame)
			{
				transform->SetEulerAngles({ 0,angle,0 });
				transform->scale = { 16,16,1 };
				mat.ambient = { 0,0,0,0 };
				mat.diffuse = { 1,1,1,1 };
				mat.power = 0;
				mat.specular = { 0,0,0,0 };
				sr->SetMaterial(mat);
				sr->SetSourceRect({ (32 * num) / 512.0f,288 / 512.0f,32 / 512.0f,32 / 512.0f });
				sr->SetTexture(src_ascii.Get());
			}

		private:

			int frame;

			SpriteRender* sr;

			Material mat;

			float speed;

			float aspeed;

			float angle;

			int desFrame;

			void OnUpdate() override
			{
				frame++;
				angle -= 0.1;
				if (angle < 0)
					angle = 0;
				transform->SetEulerAngles({ 0, angle, 0 });
				if (frame <= 21)
				{
					speed += aspeed;
					transform->location.y += speed * timer->GetDeltaTime();
				}
				if (frame < 36)
				{
					mat.diffuse.a = ((frame) / 4 % 2) ? 0.5 : 1;
					sr->SetMaterial(mat);
				}
				if (frame >= desFrame)
				{
					transform->scale.y -= 0.8;
					if (transform->scale.y <= 0)
						Destroy();
				}
			}
		};
	public:

		ScoreNumber(int score) :maxBit(GetMaxBit(score)), frame(0), createdBit(maxBit), number(score)
		{
		}

	private:

		int frame;

		int number;

		int maxBit;

		int createdBit;

		void OnUpdate() override
		{
			frame++;
			if (frame % 2 == 0)
			{
				if (createdBit)
				{
					const float width = 14;
					auto g = scene->AttachActor(new Number(GetBitNum(number, createdBit), 120 - frame));
					g->transform->location = { -width / 2 * (maxBit - 1) + width * (maxBit - createdBit),100,0 };
					g->InsertBefore(this);
					createdBit--;
				}
				else
				{
					Destroy();
				}
			}
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

	};

public:

	GetSpellCardBonus(int _score, double breakTime, double realTime) :frame(0), bt(breakTime), rt(realTime), score(_score)
	{
		transform->scale = { 225.5,32,1 };
		transform->location = { 0,100,0 };
		sr->SetTexture(src_front.Get());
		sr->SetSourceRect({ 528 / 1024.0f,198 / 1024.0f,451 / 1024.0f,64 / 1024.0f });
	}

protected:

	int frame;

	double bt;

	double rt;

	int score;

	void OnUpdate() override
	{
		frame++;
		if (frame < 24)
		{
			mat.diffuse.a = (frame / 2 % 2) ? 1 : 0.5;
			sr->SetMaterial(mat);
		}
		if (frame > 120)
		{
			transform->scale.y -= 1.6;
		}
		if (frame == 140)
		{
			Destroy();
		}
	}

	void OnCreated() override
	{
		__super::OnCreated();
		scene->AttachActor(new ScoreNumber(score))->InsertAfter(this);
		scene->AttachActor(new BreakTime(bt));
		scene->AttachActor(new RealTime(rt));
		se_cardget.Play();
	}
};

class FullPowerUp :public UI
{
public:

	FullPowerUp() :frame(0), alpha(0.5), baseAlpha(1)
	{
		transform->scale = { 308,16,1 };
		transform->location = { 0,100,0 };
		sr->SetTexture(src_front.Get());
		sr->SetSourceRect({ 1 / 2.0f + 0.001f,320 / 1024.0f,308 / 1024.0f,64 / 1024.0f });
	}

protected:

	int frame;

	float alpha;

	float baseAlpha;

	void OnCreated() override
	{
		__super::OnCreated();
		if (nowUI)
			nowUI->Destroy();
		nowUI = this;
	}

	void OnUpdate() override
	{
		if ((player->transform->location - transform->location).GetLength() < 70)
		{
			alpha -= 0.05;
			if (alpha < 0.2)
				alpha = 0.2;
		}
		else
		{
			alpha += 0.05;
			if (alpha > 1.0)
				alpha = 1.0;
		}

		frame++;
		if (frame <= 10)
			transform->scale = Lerp<Vector3>({ 308,16,1 }, { 154,32,1 }, frame / 10.0f);
		if (frame >= 80)
		{
			transform->scale += {3.85, 0.8, 0};
			baseAlpha -= 0.1;
		}
		mat.diffuse.a = baseAlpha * alpha;
		sr->SetMaterial(mat);
		if (frame == 90)
			Destroy();
	}

	void OnDestroy() override
	{
		if (nowUI.Get() == this)
			nowUI.Reset();
	}
};

class Extend :public UI
{
public:

	Extend() :frame(0), alpha(0.5), baseAlpha(1)
	{
		transform->scale = { 200,16,1 };
		transform->location = { 0,100,0 };
		sr->SetTexture(src_front.Get());
		sr->SetSourceRect({ 1 / 2.0f + 0.001f,448 / 1024.0f,200 / 1024.0f,64 / 1024.0f });
	}

protected:

	int frame;

	float alpha;

	float baseAlpha;
	
	void OnCreated() override
	{
		__super::OnCreated();
		if (nowUI)
			nowUI->Destroy();
		nowUI = this;
		se_extend.Play();
	}
	
	void OnUpdate() override
	{
		if ((player->transform->location - transform->location).GetLength() < 70)
		{
			alpha -= 0.05;
			if (alpha < 0.2)
				alpha = 0.2;
		}
		else
		{
			alpha += 0.05;
			if (alpha > 1.0)
				alpha = 1.0;
		}

		frame++;
		if (frame <= 10)
			transform->scale = Lerp<Vector3>({ 200,16,1 }, { 100,32,1 }, frame / 10.0f);
		if (frame >= 80)
		{
			transform->scale += {2.5, 0.8, 0};
			baseAlpha -= 0.1;
		}
		mat.diffuse.a = baseAlpha * alpha;
		sr->SetMaterial(mat);
		if (frame == 90)
			Destroy();
	}
	
	void OnDestroy() override
	{
		if (nowUI.Get() == this)
			nowUI.Reset();
	}
};

class ItemLine :public UI
{
public:

	ItemLine() :frame(0), line_sr(0)
	{
		mat.diffuse.a = 0;
		transform->location = { 0,player->GetAttribution().itemLine,0 };
		transform->scale = { 176,28,1 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_front.Get());
		sr->SetSourceRect({ 522 / 1024.0f,515 / 1024.0f,352 / 1024.0f,56 / 1024.0f });
		sr->SetLocalTransform(XMMatrixTranslation(0, 0.8f, 0));
	}

protected:

	int frame;

	ComPtr<UI> line;

	SpriteRender* line_sr;

	void OnCreated() override
	{
		__super::OnCreated();
		line = scene->AttachActor(new UI);
		line->transform->location = { 0,player->GetAttribution().itemLine,0 };
		line->transform->scale = { 640,4,1 };
		line_sr = line->GetComponent<SpriteRender>();
		line_sr->SetMaterial(mat);
		line_sr->SetTexture(src_front.Get());
		line_sr->SetSourceRect({ 512 / 1024.0f,577 / 1024.0f,335 / 1024.0f,14 / 1024.0f });
	}

	void OnUpdate() override
	{
		frame++;
		if (frame <= 10)
			mat.diffuse.a += 0.09;
		else if (frame >= 180)
			mat.diffuse.a -= 0.09;
		mat.ambient.r = mat.ambient.g = mat.ambient.b = (float)(0.4 + 0.4 * sin(frame * 0.15));
		sr->SetMaterial(mat);
		line_sr->SetMaterial(mat);
		if (frame == 190)
			Destroy();
	}

	void OnDestroy() override
	{
		if (line)
		{
			line->Destroy();
			line.Reset();
		}
	}
};

class ItemNumber :public UI
{
	class Number :public UI
	{
	public:

		Number(BYTE _num, int destroyFrame, bool isMax) :frame(0), num(_num), desFrame(destroyFrame)
		{
			transform->scale = { 10,10,1 };
			mat.diffuse.a = 0.5;
			if (isMax)
				mat.diffuse.b = 0;
			sr->SetTexture(src_ascii.Get());
			sr->SetSourceRect({ num * 16 / 512.0f,496 / 512.0f, 16 / 512.0f,16 / 512.0f });
		}

	private:

		BYTE num;

		int frame;

		int desFrame;

		void OnUpdate() override
		{
			frame++;
			transform->location.y += 1;
			if (frame <= 20)
			{
				mat.diffuse.a += 0.025;
				sr->SetMaterial(mat);
			}
			if (frame == desFrame)
			{
				sr->SetSourceRect({ num * 16 / 512.0f,480 / 512.0f, 16 / 512.0f,16 / 512.0f });
			}
			if (frame == desFrame + 6)
			{
				Destroy();
			}
		}
	};

public:

	ItemNumber(int _score, const Vector2& _loc, bool _isMax) :
		score(_score), loc(_loc), maxBit(GetMaxBit(score)), createdBit(maxBit), isMax(_isMax)
	{
	}

private:

	int score;

	Vector2 loc;

	int maxBit;

	int createdBit;

	bool isMax;

	void OnCreated() override
	{
		__super::OnCreated();
		while (createdBit)
		{
			const float width = 10;
			auto g = scene->AttachActor(new Number(GetBitNum(score, createdBit), 48 - createdBit, isMax));
			g->transform->location = { -width / 2 * (maxBit - 1) + width * (maxBit - createdBit) + loc.x,loc.y,0 };
			g->InsertBefore(this);
			createdBit--;
		}
		Destroy();
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
};

class StageClear :public UI
{
	class ScoreNumber :public GameObject
	{
		class Number :public GameObject
		{
		public:

			Number(BYTE num) :
				sr(AddComponent(new SpriteRender)), frame(0)
			{
				transform->scale = { 24,24,1 };
				mat.ambient = { 0,0,0,0 };
				mat.diffuse = { 1,1,1,0 };
				mat.power = 0;
				mat.specular = { 0,0,0,0 };
				sr->SetMaterial(mat);
				sr->SetSourceRect({ (32 * num) / 512.0f,288 / 512.0f,32 / 512.0f,32 / 512.0f });
				sr->SetTexture(src_ascii.Get());
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
					mat.diffuse.a += 0.05f;
					sr->SetMaterial(mat);
				}
				if (frame > 180)
				{
					mat.diffuse.a -= 0.05f;
					sr->SetMaterial(mat);
				}
				if (frame == 200)
				{
					Destroy();
				}
			}
		};
	public:

		ScoreNumber(int score) :maxBit(GetMaxBit(score)), frame(0), createdBit(maxBit), number(score)
		{
		}

	private:

		int frame;

		int number;

		int maxBit;

		int createdBit;

		void OnUpdate() override
		{
			while (createdBit)
			{
				const float width = 20;
				auto g = scene->AttachActor(new Number(GetBitNum(number, createdBit)));
				g->transform->location = { -width / 2 * (maxBit - 1) + width * (maxBit - createdBit),35,0 };
				g->InsertBefore(this);
				createdBit--;
			}
			Destroy();
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

	};

public:

	StageClear(int _score) :frame(0), score(_score)
	{
		mat.diffuse.a = 0;
		sr->SetMaterial(mat);
		transform->scale = { 256 * 1.2f,128 * 1.2f,1 };
		transform->location = { 0,90,0 };
		sr->SetTexture(src_stageclear.Get());
	}

protected:

	int frame;

	int score;

	void OnUpdate() override
	{
		frame++;
		if (frame <= 20)
		{
			mat.diffuse.a += 0.05;
			sr->SetMaterial(mat);
		}
		if (frame > 180)
		{
			mat.diffuse.a -= 0.05;
			sr->SetMaterial(mat);
		}
		if (frame == 200)
		{
			Destroy();
		}
	}

	void OnCreated() override
	{
		__super::OnCreated();
		scene->AttachActor(new ScoreNumber(score))->InsertAfter(this);
	}
};