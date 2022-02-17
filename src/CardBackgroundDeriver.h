#pragma once
#include "CardBackground.h"
#include "Source.h"

class Artia_CDBG :public CardBackground
{
public:

	class CDBG1 :public CardSurface
	{
	public:

		CDBG1() :CardSurface(BLEND_STATE_NORMAL, 0.02, 0.1, 1)
		{
			sr->SetTexture(src_artia_cdbg1.Get());
		}
	};

	class CDBG2 :public CardSurface
	{
	public:

		CDBG2() :CardSurface(BLEND_STATE_DARKEN, 0.02, 0.1, 1), y(0)
		{
			sr->SetTexture(src_artia_cdbg2.Get());
		}

		void OnUpdate() override
		{
			y += 0.005;
			sr->SetTextureTransform(XMMatrixScaling(2.5, 1.6, 1) * XMMatrixTranslation(0, y, 0));
			__super::OnUpdate();
		}

		float y;
	};

	Artia_CDBG(BossSpellCard* sc) :CardBackground(sc) {}

	void OnAddSurface() override
	{
		AddSurface(new CDBG1);
		AddSurface(new CDBG2);
	}
};

class Monstone_CDBG :public CardBackground
{
public:

	class CDBG1 :public CardSurface
	{
	public:

		CDBG1() :CardSurface(BLEND_STATE_NORMAL, 0.02, 0.1, 1)
		{
			sr->SetTexture(src_monstone_cdbg1.Get());
		}
	};

	class CDBG2 :public CardSurface
	{
	public:

		CDBG2() :CardSurface(BLEND_STATE_BRIGHTEN, 0.02, 0.1, 0.75f), y(0)
		{
			sr->SetTexture(src_monstone_cdbg2.Get());
		}

		void OnUpdate() override
		{
			y += 0.003;
			sr->SetTextureTransform(XMMatrixScaling(2.4, 1.8, 1) * XMMatrixTranslation(y, y, 0));
			__super::OnUpdate();
		}

		float y;
	};

	Monstone_CDBG(BossSpellCard* sc) :CardBackground(sc) {}

	void OnAddSurface() override
	{
		AddSurface(new CDBG1);
		AddSurface(new CDBG2);
	}
};

class SunnyMilk_CDBG :public CardBackground
{
public:

	class CDBG1 :public CardSurface
	{
	public:

		CDBG1() :CardSurface(BLEND_STATE_BRIGHTEN, 0.02, 0.1, 1), y(0)
		{
			sr->SetTexture(src_sunnymilk_cdbg1.Get());
		}

		void OnUpdate() override
		{
			y += 0.003;
			sr->SetTextureTransform(XMMatrixScaling(2.4, 1.8, 1) * XMMatrixTranslation(y, 0, 0));
			__super::OnUpdate();
		}

		float y;
	};

	class CDBG2 :public CardSurface
	{
	public:

		CDBG2() :CardSurface(BLEND_STATE_NORMAL, 0.02, 0.1, 1)
		{
			sr->SetTexture(src_sunnymilk_cdbg2.Get());
			sr->SetTextureTransform(XMMatrixScaling(1, 0.75, 1) * XMMatrixTranslation(0, 0.125, 0));
		}
	};

	class CDBG3 :public CardSurface
	{
	public:

		CDBG3() :CardSurface(BLEND_STATE_NORMAL, 0.02, 0.1, 1)
		{
			mat.diffuse = { 0,0,0,mat.diffuse.a };
			sr->SetMaterial(mat);
			sr->SetTexture(src_dummy.Get());
		}
	};

	SunnyMilk_CDBG(BossSpellCard* sc) :CardBackground(sc) {}

	void OnAddSurface() override
	{
		AddSurface(new CDBG3);
		AddSurface(new CDBG2);
		AddSurface(new CDBG1);
	}
};