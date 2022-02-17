#pragma once
#include "VirtualLib.h"
#include "BaseObject.h"
#include "LevelResources.h"
#include "TitleScene.h"
#include "SceneObjects.h"
#include "Rand.h"
#include "UserComponent.h"
#include "EngineGlobal.h"
#include "GameGlobal.h"
#include "TitleObjects.h"
#include "SoundEffect.h"
#include "Replay.h"
#include "UserRenderer.h"
#include <sstream>

class Frame :public AnimObject
{
public:

	bool focused;

	Frame() :focused(true)
	{}

	void OnCreated() final
	{
		if (!TitleSceneCom::frameStack.empty())
			TitleSceneCom::frameStack.top()->focused = false;
		TitleSceneCom::frameStack.push(this);
		OnFrameCreated();
	}

	void OnUpdate() final
	{
		if (focused)
			OnFrameInput();
		if (IsValid())
			OnFrameThink();
	}

	void OnDestroy() final
	{
		TitleSceneCom::frameStack.pop();
		if (!TitleSceneCom::frameStack.empty())
			TitleSceneCom::frameStack.top()->focused = true;
		OnFrameDestroy();
	}

	virtual void OnFrameInput() {}

	virtual void OnFrameThink() {}

	virtual void OnFrameDestroy() {}

	virtual void OnFrameCreated() {}
};


class Frame_BackGround :public Frame
{
	class Star :public MovingObject
	{
		class StarEffect :public MovingObject
		{
		public:

			StarEffect() :frame(Random(0, 9))
			{
				transform->scale = { 9,9,1 };
				sr->SetTexture(src_star.Get());
				mb->velocity = GetVector(RandomAngle(), Random(-15.0f, 15.0f));
			}

			void OnUpdate() override
			{
				frame++;
				if (frame > 48)
				{
					Leave(20);
				}
				else if (frame % 5 == 0)
				{
					if ((frame / 5) % 2)
					{
						Enter(2, 0.8);
					}
					else
					{
						Enter(5, 0);
					}
				}
				transform->scale -= {0.15f, 0.15f, 0};
			}

		private:

			int frame;
		};

	public:

		Star() :frame(0)
		{
			transform->scale = { 12,12,1 };
			sr->SetTexture(src_star.Get());
			mb->velocity = { -250,-180,0 };
		}

		void OnCreated() override
		{
			InsertBefore(TitleSceneCom::locator[3].Get());
		}

		void OnUpdate() override
		{
			frame++;
			for (auto it = effects.begin(); it != effects.end();)
			{
				if ((*it)->IsValid())
					it++;
				else
					it = effects.erase(it);
			}
			auto eff = scene->AttachActor(new StarEffect);
			eff->InsertBefore(this);
			effects.push_back(eff);
			eff->transform->location = transform->location + GetVector(RandomAngle(), Random(0.0f, 3.0f));
			if (transform->location.x < -500)
				Destroy();
		}

		void OnDestroy() override
		{
			for (auto e : effects)
			{
				e->Leave(10);
			}
			effects.clear();
		}

	private:

		vector<ComPtr<StarEffect>> effects;

		int frame;
	};

public:

	Frame_BackGround() :frame(0)
	{
		transform->scale = { 640,480,1 };
		mat.diffuse.a = 0.5;
		sr->SetMaterial(mat);
		sr->SetTexture(src_titlebg_2.Get());
		Enter(8, 1);
	}

	void OnFrameCreated() override
	{
		InsertBefore(TitleSceneCom::locator[2].Get());
	}

	void OnFrameThink() override
	{
		frame++;
		if (frame <= 18)
		{
			transform->scale += {12 - frame / 3.0f, 0.75f * (12 - frame / 3.0f), 0};
		}
		if (IsLeft())
		{
			for (auto s : stars)
			{
				s->Leave(10);
			}
			stars.clear();
		}
		else if (frame % 60 == 10)
		{
			for (auto it = stars.begin(); it != stars.end();)
			{
				if ((*it)->IsValid())
					it++;
				else
					it = stars.erase(it);
			}
			auto star = scene->AttachActor(new Star);
			stars.push_back(star);
			star->transform->location = Vector3{ 320,240,0 }+Vector3(250, -180, 0) * Random(-1.0f, 1.0f);
		}
	}

	void OnFrameDestroy() override
	{
	}

private:

	int frame;

	vector<ComPtr<Star>> stars;
};

class Frame_Stage_StartGame :public Frame
{
public:

	int frame;

	int selectIndex;

	ComPtr<SelectTag> st_stage;

	ComPtr<Frame> parent;

	ComPtr<DifficultyTagAnim> dtagAnim;

	ComPtr<TitleStage> stageText[3];

	Frame_Stage_StartGame() :frame(0), selectIndex(0)
	{}

	void OnFrameCreated() override
	{
		InsertBefore(TitleSceneCom::locator[2].Get());
		st_stage = scene->AttachActor(new SelectTag(2));
		st_stage->transform->location = { 0,300,0 };
		st_stage->Move({ 0,170 }, 600, 100);
		for (int i = 0; i < 3; i++)
		{
			stageText[i] = scene->AttachActor(new TitleStage(i, { 0,70 - 30.0f * i,0 }));
			stageText[i]->transform->location = { 0,70 - 30.0f * i,0 };
		}
		stageText[selectIndex]->selected = true;
	}

	void OnFrameThink() override
	{
		frame++;
	}

	void OnFrameInput() override
	{
		if (frame >= 10)
		{
			if (input->KeyPress(DIK_DOWN))
			{
				se_select00.Play();
				SetSelectedIndex((selectIndex + 1) % 3);
			}
			else if (input->KeyPress(DIK_UP))
			{
				se_select00.Play();
				SetSelectedIndex((selectIndex + 2) % 3);
			}
			else if (input->KeyRelease(DIK_X))
			{
				se_cancel00.Play();
				LeaveForPlayerFrame();
			}
			else if (input->KeyPress(DIK_Z))
			{
				focused = false;
				stageText[selectIndex]->GListen(true);
				se_boon00.Play();
				se_ok00.Play();
				StopBgm();
				scene->AttachActor(new TDelayCreate(new EnterSwitch(GetStageLevel(selectIndex)), 12));
			}
		}
	}

	void OnFrameDestroy() override
	{
		st_stage.Reset();
		parent.Reset();
		dtagAnim.Reset();
		for (int i = 0; i < 3; i++)
			stageText[i].Reset();
	}

	void SetSelectedIndex(int index)
	{
		selectIndex = index;
		for (int i = 0; i < 3; i++)
		{
			if (i == selectIndex)
			{
				stageText[i]->Roll();
				stageText[i]->selected = true;
				transform->location = stageText[i]->transform->location;
			}
			else
			{
				stageText[i]->selected = false;
			}
		}
	}

	void LeaveForPlayerFrame();
};

class Frame_Player_StartGame :public Frame
{
	class PlayerIntro :public TransformingObject
	{
	public:

		PlayerIntro(int index)
		{
			transform->scale = { 367,285,1 };
			mat.diffuse = { 0,0,0,0 };
			mat.ambient = { 1,1,1,1 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_playerintro.Get());
			sr->SetSourceRect({ 0.5f * index,0,0.5f,1 });
		}
	};

	class PlayerSprite :public MovingObject
	{
	public:

		PlayerSprite(int index)
		{
			mat.diffuse = { 1,1,1,0 };
			sr->SetMaterial(mat);
			if (index == 0)
			{
				transform->scale = { 1071 * 0.4f,1170 * 0.4f,1 };
				sr->SetTexture(src_ps00.Get());
			}
			else if (index == 1)
			{
				transform->scale = { 1030 * 0.4f,1071 * 0.4f,1 };
				sr->SetTexture(src_ps01.Get());
			}
		}
	};

	class Arrow :public AnimObject
	{
	public:

		Arrow(bool isLeft)
		{
			transform->scale = { 45.0f * (isLeft ? 1 : -1) * 0.5f,83 * 0.5f,1 };
			mat.diffuse = { 1,1,1,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_arrow.Get());
		}
	};

public:

	bool isLeft;

	int selectIndex;

	int frame;

	ComPtr<Frame> parent;

	ComPtr<SelectTag> st_player;

	ComPtr<DifficultyTagAnim> dtagAnim;

	ComPtr<PlayerIntro> intros[2];

	ComPtr<PlayerIntro> currentIntro;

	ComPtr<PlayerSprite> sprites[2];

	ComPtr<PlayerSprite> currentSprite;

	ComPtr<Arrow> arrows[2];

	Frame_Player_StartGame() :selectIndex(playerIndex), isLeft(false), frame(0)
	{
	}

	void OnFrameCreated() override
	{
		InsertBefore(TitleSceneCom::locator[2].Get());
		st_player = scene->AttachActor(new SelectTag(1));
		st_player->transform->location = { 140,300,0 };
		st_player->Move({ 150,170 }, 600, 100);
		for (int i = 0; i < 2; i++)
			intros[i] = scene->AttachActor(new PlayerIntro(i));
		currentIntro = intros[selectIndex];
		intros[selectIndex]->transform->location = { 130,-40,0 };
		intros[selectIndex]->Enter(20);
		for (int i = 0; i < 2; i++)
		{
			sprites[i] = scene->AttachActor(new PlayerSprite(i));
			sprites[i]->transform->location = { -200,-10,0 };
		}
		currentSprite = sprites[selectIndex];
		sprites[selectIndex]->Enter(8);
		arrows[0] = scene->AttachActor(new Arrow(true));
		arrows[0]->transform->location = { -45,30,0 };
		arrows[0]->Enter(10);
		arrows[1] = scene->AttachActor(new Arrow(false));
		arrows[1]->transform->location = { 290,30,0 };
		arrows[1]->Enter(10);
	}

	void OnFrameThink() override
	{
		frame++;
	}

	void OnFrameInput() override
	{
		if (frame >= 10)
		{
			if (input->KeyPress(DIK_LEFT))
			{
				se_select00.Play();
				isLeft = true;
				SetSelectedIndex((selectIndex + 1) % 2);
			}
			else if (input->KeyPress(DIK_RIGHT))
			{
				se_select00.Play();
				isLeft = false;
				SetSelectedIndex((selectIndex + 1) % 2);
			}
			else if (input->KeyRelease(DIK_X))
			{
				se_cancel00.Play();
				LeaveForDifficultyFrame();
			}
			else if (input->KeyPress(DIK_Z))
			{
				playerIndex = selectIndex;
				focused = false;
				se_ok00.Play();
				currentSprite->GListen(false);
				currentIntro->GListen(true);
				if (practiceMode)
				{
					scene->AttachActor(new TDelayInvoke(Delegate([=]() {LeaveForStageFrame(); }), 20));
				}
				else
				{
					se_boon00.Play();
					StopBgm();
					scene->AttachActor(new TDelayCreate(new EnterSwitch(GetStageLevel(0)), 12));
				}
			}
		}
	}

	void OnFrameDestroy() override
	{
		st_player.Reset();
		parent.Reset();
		dtagAnim.Reset();
		for (int i = 0; i < 2; i++)
		{
			intros[i].Reset();
			sprites[i].Reset();
			arrows[i].Reset();
		}
	}

	void SetSelectedIndex(int index)
	{
		if (selectIndex != index)
		{
			selectIndex = index;
			if (isLeft)
			{
				if (currentIntro)
				{
					currentIntro->Enter(20, 0);
					currentIntro->Roll({ 0,M_PI / 2.0f,0 }, 20);
					currentIntro->Transform(currentIntro->transform->scale, { 80,-40,0 }, 20);
				}
				currentIntro = intros[selectIndex];
				intros[selectIndex]->transform->location = { 180,-40,0 };
				intros[selectIndex]->Transform(currentIntro->transform->scale, { 130,-40,0 }, 20);
				intros[selectIndex]->Enter(20);
				intros[selectIndex]->transform->SetEulerAngles({ 0,M_PI * 3 / 2,0 });
				intros[selectIndex]->Roll({ 0,M_PI / 2.0f,0 }, 20);

				if (currentSprite)
				{
					currentSprite->Enter(8, 0);
					currentSprite->Move({ -250,-10 }, 300, 100);
				}
				currentSprite = sprites[selectIndex];
				sprites[selectIndex]->transform->location = { -150,-10,0 };
				sprites[selectIndex]->Move({ -200,-10 }, 300, 100);
				sprites[selectIndex]->Enter(8);
			}
			else
			{
				if (currentIntro)
				{
					currentIntro->Enter(20, 0);
					currentIntro->Roll({ 0,-M_PI / 2.0f,0 }, 20);
					currentIntro->Transform(currentIntro->transform->scale, { 180,-40,0 }, 20);
				}
				currentIntro = intros[selectIndex];
				intros[selectIndex]->transform->location = { 80,-40,0 };
				intros[selectIndex]->Transform(currentIntro->transform->scale, { 130,-40,0 }, 20);
				intros[selectIndex]->Enter(20);
				intros[selectIndex]->transform->SetEulerAngles({ 0,M_PI * 1 / 2,0 });
				intros[selectIndex]->Roll({ 0,-M_PI / 2.0f,0 }, 20);

				if (currentSprite)
				{
					currentSprite->Enter(8, 0);
					currentSprite->Move({ -150,-10 }, 300, 100);
				}
				currentSprite = sprites[selectIndex];
				sprites[selectIndex]->transform->location = { -250,-10,0 };
				sprites[selectIndex]->Move({ -200,-10 }, 300, 100);
				sprites[selectIndex]->Enter(8);
			}
		}
	}

	void LeaveForStageFrame()
	{
		for (int i = 0; i < 2; i++)
		{
			if (intros[i])
			{
				intros[i]->Leave(10);
			}
			if (sprites[i])
			{
				sprites[i]->Leave(10);
			}
			if (arrows[i])
			{
				arrows[i]->Leave(10);
			}
		}
		dtagAnim->Enter(15, 0);
		st_player->Leave(20);
		st_player->Move({ -150,300 }, 600, 100);
		auto _parent = parent;
		auto _dtagAnim = dtagAnim;
		Destroy();
		auto f = scene->AttachActor(new Frame_Stage_StartGame);
		f->parent = _parent;
		f->dtagAnim = _dtagAnim;
	}

	void LeaveForDifficultyFrame();
};

class Frame_Difficulty_StartGame :public Frame
{
public:

	class DifficultyTag :public MovingObject
	{
	public:

		int d;

		DifficultyTag(int _d) :d(_d)
		{
			transform->scale = { 477 * 0.8f,170 * 0.8f,1 };
			mat.diffuse.a = 0.5;
			sr->SetMaterial(mat);
			sr->SetSourceRect({ 0,d / 4.0f,1,1 / 4.0f });
			sr->SetTexture(src_difficulty.Get());
		}

	private:

		void OnCreated() override
		{
			InsertBefore(TitleSceneCom::locator[4].Get());
		}
	};

public:

	int selectIndex;

	ComPtr<Frame> parent;

	ComPtr<SelectTag> st_rank;

	Frame_Difficulty_StartGame() :frame(0), selectIndex(difficulty)
	{
	}

	void OnFrameCreated() override
	{
		InsertBefore(TitleSceneCom::locator[2].Get());
		st_rank = scene->AttachActor(new SelectTag(0));
		st_rank->transform->location = { -140,300,0 };
		st_rank->Move({ -150,170 }, 600, 100);
	}

	void OnFrameThink() override
	{
		frame++;
		if (frame == 2)
			SpawnTag(3);
		if (frame == 4)
			SpawnTag(2);
		if (frame == 6)
			SpawnTag(1);
		if (frame == 8)
			SpawnTag(0);
	}

	void OnFrameInput() override
	{
		if (frame > 8)
		{
			if (input->KeyPress(DIK_DOWN) || input->KeyPress(DIK_LEFT))
			{
				se_select00.Play();
				SetSelectedIndex((selectIndex + 3) % 4);
			}
			else if (input->KeyPress(DIK_UP) || input->KeyPress(DIK_RIGHT))
			{
				se_select00.Play();
				SetSelectedIndex((selectIndex + 1) % 4);
			}
			else if (input->KeyRelease(DIK_X))
			{
				se_cancel00.Play();
				difficulty = selectIndex;
				LeaveForTitle();
			}
			else if (input->KeyPress(DIK_Z))
			{
				focused = false;
				se_ok00.Play();
				difficulty = selectIndex;
				tags[selectIndex]->GListen(false);
				scene->AttachActor(new TDelayInvoke(Delegate([=]() {LeaveForPlayerFrame(); }), 20));
			}
		}
	}

	void OnFrameDestroy() override
	{
		for (int i = 0; i < 4; i++)
		{
			tags[i].Reset();
		}
		st_rank.Reset();
		parent.Reset();
	}

	void SpawnTag(int d)
	{
		tags[d] = scene->AttachActor(new DifficultyTag(d));
		tags[d]->transform->location = Vector3(180, 140, 0) * (d - selectIndex) + Vector3(-120, 120, 0);
		tags[d]->mat.diffuse.a= (d == selectIndex) ? 1.0f : 0.5f;
		tags[d]->sr->SetMaterial(tags[d]->mat);
		tags[d]->Move(Vector3(180, 140, 0) * (d - selectIndex) + Vector3(20, 0, 0), 1000, 100);
	}

	void SetSelectedIndex(int index)
	{
		selectIndex = index;
		for (int i = 0; i < 4; i++)
		{
			if (tags[i])
			{
				if (i == selectIndex)
				{
					tags[i]->mat.diffuse.a = 1;
					tags[i]->sr->SetMaterial(tags[i]->mat);
				}
				else
				{
					tags[i]->mat.diffuse.a = 0.5;
					tags[i]->sr->SetMaterial(tags[i]->mat);
				}
			}
		}
		MoveTags();
	}

	void MoveTags()
	{
		for (int i = 0; i < 4; i++)
		{
			if (tags[i])
				tags[i]->Move(Vector3(180, 140, 0) * (i - selectIndex) + Vector3(20, 0, 0), 1500, 100);
		}
	}

	void LeaveForTitle()
	{
		if (parent)
			parent->Leave(20);
		st_rank->Leave(20);
		st_rank->Move({ -150,300 }, 600, 100);
		for (int i = 0; i < 4; i++)
		{
			if (tags[i])
				tags[i]->Leave(20);
		}
		Destroy();
	}

	void LeaveForPlayerFrame()
	{
		st_rank->Leave(20);
		st_rank->Move({ -150,300 }, 600, 100);
		for (int i = 0; i < 4; i++)
		{
			if (tags[i])
				tags[i]->Leave(10);
		}
		auto _parent = parent;
		Destroy();
		auto f = scene->AttachActor(new Frame_Player_StartGame);
		f->parent = _parent;
		f->dtagAnim = scene->AttachActor(new DifficultyTagAnim(selectIndex));
	}

private:

	int frame;

	ComPtr<DifficultyTag> tags[4];
};

class Frame_BackGround_StartGame :public Frame_BackGround
{
	void OnFrameCreated() override
	{
		__super::OnFrameCreated();
		scene->AttachActor(new Frame_Difficulty_StartGame)->parent = this;
	}
};

class Frame_Option :public Frame
{
	class Number :public AnimObject
	{
	public:

		TextRender2D* tr;

		int value;

		int index;

		ComPtr<IRenderTarget> target;

		Number(int _index) :tr(0), value(100), index(_index)
		{
			if (index == 0)
				value = (int)(GetBgmVolume() * 100);
			else if (index == 1)
				value = (int)(GetSoundEffectVolume() * 100);

			transform->scale = { 640,480,1 };
			transform2D->scale = { 1,0.9 };
			transform2D->location = { 300,281 + 32.0f * index };

			RenderTargetDesc desc;
			desc.width = 640;
			desc.height = 480;
			desc.msaaEnabled = false;
			desc.support2D = true;
			desc.hasDepth = false;
			desc.multiSampleCount = 0;
			engine->CreateRenderTarget(desc, &target);

			AddComponent(new RenderTargetSet::Setter(target.Get(), 0, true));
			tr = AddComponent(new TextRender2D(false));
			AddComponent(new RenderTargetSet::Setter(0));

			tr->SetTextFormat(tf_wryh_volume.Get());
			tr->SetOutlineBrush(sb_blue.Get());
			tr->SetRect({ 0,0,200,200 });
			tr->SetOutlineEnabled(true);
			tr->SetOutlineWidth(2);
			SetValue(value);

			mat.diffuse.a = 0;
			sr->SetMaterial(mat);
			sr->SetPriority(90, true);
			sr->SetTexture(target.Get());

			Enter(10);
		}

		void Decrease()
		{
			SetValue(value - 5);
		}

		void Increase()
		{
			SetValue(value + 5);
		}

		void SetValue(int _value)
		{
			value = _value;
			if (value < 0)
				value = 0;
			if (value > 100)
				value = 100;
			wchar_t text[16];
			wsprintf(text, L"%3d%%", value);
			tr->SetText(text);

			if (index == 0)
			{
				SetBgmVolume(value / 100.0f);
			}
			else if (index == 1)
			{
				SetSoundEffectVolume(value / 100.0f);
			}
		}

	private:

		void OnCreated() override
		{
			InsertBefore(TitleSceneCom::locator[1].Get());
		}
	};

public:

	int selectIndex;

	int frame;

	ComPtr<Title> titles[3];

	ComPtr<Number> number[2];

	Frame_Option() :selectIndex(0), frame(0)
	{
	}

	void OnFrameCreated() override
	{
		InsertBefore(TitleSceneCom::locator[0].Get());
		int types[] = { 10,11,9 };
		for (int i = 0; i < 3; i++)
		{
			titles[i] = new Title(types[i], { -120,-54 - 32.0f * i,0 });
			titles[i]->transform->location = Vector3{ -220,46 - 32.0f * i,0 };
			scene->AttachActor(new TDelayCreate(titles[i].Get(), i * 6));
		}
		titles[0]->selected = true;
		transform->location = titles[0]->transform->location;
		number[0] = scene->AttachActor(new Number(0));
		number[1] = scene->AttachActor(new Number(1));
	}

	void OnFrameInput() override
	{
		if (input->KeyPress(DIK_DOWN))
		{
			se_select00.Play();
			SetSelectedIndex((selectIndex + 1) % 3);
		}
		else if (input->KeyPress(DIK_UP))
		{
			se_select00.Play();
			SetSelectedIndex((selectIndex + 2) % 3);
		}
		else if (input->KeyPress(DIK_LEFT))
		{
			if (selectIndex < 2)
			{
				se_select00.Play();
				number[selectIndex]->Decrease();
			}
		}
		else if (input->KeyPress(DIK_RIGHT))
		{
			if (selectIndex < 2)
			{
				se_select00.Play();
				number[selectIndex]->Increase();
			}
		}
		else if (input->KeyPress(DIK_Z))
		{
			se_ok00.Play();
			switch (selectIndex)
			{
			case 0:
			case 1:
				break;
			case 2:
			{
				LeaveForTitle();
			}
			break;
			}
		}
		else if (input->KeyPress(DIK_X))
		{
			se_cancel00.Play();
			SetSelectedIndex(2);
		}
	}

	void SetSelectedIndex(int index)
	{
		selectIndex = index;
		for (int i = 0; i < 3; i++)
		{
			if (i == selectIndex)
			{
				titles[i]->Roll();
				titles[i]->selected = true;
				transform->location = titles[i]->transform->location;
			}
			else
			{
				titles[i]->selected = false;
			}
		}
	}

	void LeaveForTitle()
	{
		for (int i = 0; i < 3; i++)
		{
			if (titles[i])
			{
				titles[i]->Leave(10);
				titles[i].Reset();
			}
		}
		for (int i = 0; i < 2; i++)
		{
			if (number[i])
			{
				number[i]->Leave(10);
				number[i].Reset();
			}
		}
		Destroy();
	}
};

class Frame_BackGround_Manual :public Frame_BackGround
{
	class Tag :public MovingObject
	{
	public:

		TextRender2D* tr;

		ComPtr<IRenderTarget> target;

		bool selected;

		Tag(const wstring& str) :tr(AddComponent(new TextRender2D(false))), selected(false)
		{
			RenderTargetDesc desc;
			desc.hasDepth = false;
			desc.height = 100;
			desc.width = 200;
			desc.msaaEnabled = false;
			desc.multiSampleCount = 0;
			desc.support2D = true;
			engine->CreateRenderTarget(desc, &target);

			tr->SetTextFormat(tf_wryh_big.Get());
			tr->SetOutlineEnabled(true);
			tr->SetRect({ 0,0,200,100 });
			tr->SetText(str);
			tr->SetPriority(102, false);

			AddComponent(new RenderTargetSet::Setter(target.Get(), 0, true))->SetPriority(103, false);
			AddComponent(new RenderTargetSet::Setter(0))->SetPriority(101, false);
			SortComponents();

			transform2D->scale = { 1.2,1.2 };
			transform->scale = { 200,100 ,1 };
			mat.diffuse.a = 0;
			sr->SetMaterial(mat);
			sr->SetTexture(target.Get());
		}

		void OnCreated() override
		{
			InsertBefore(TitleSceneCom::locator[4].Get());
		}

		void OnUpdate() override
		{
			if (selected)
			{
				mat.diffuse.r = mat.diffuse.g = mat.diffuse.b = 1;
				sr->SetMaterial(mat);
			}
			else
			{
				mat.diffuse.r = mat.diffuse.g = mat.diffuse.b = 0.5;
				sr->SetMaterial(mat);
			}
		}

		void OnDestroy() override
		{
			target.Reset();
		}
	};

	class Description :public MovingObject
	{
	public:

		TextRender2D* tr;

		ComPtr<IRenderTarget> target;

		Description(const wstring& str) :tr(AddComponent(new TextRender2D(false)))
		{
			RenderTargetDesc desc;
			desc.hasDepth = false;
			desc.height = 500;
			desc.width = 470;
			desc.msaaEnabled = false;
			desc.multiSampleCount = 0;
			desc.support2D = true;
			engine->CreateRenderTarget(desc, &target);

			tr->SetTextFormat(tf_wryh_big.Get());
			tr->SetOutlineEnabled(true);
			tr->SetRect({ 0,0,440,500 });
			tr->SetText(str);

			transform2D->scale = { 1.0,1.0 };
			transform->scale = { 470,500 ,1 };
			mat.diffuse.a = 0;
			sr->SetMaterial(mat);
			sr->SetTexture(target.Get());
		}

		void OnCreated() override
		{
			InsertBefore(TitleSceneCom::locator[4].Get());

			ComPtr<IRenderTarget> temp_target = target;
			ComPtr<IScene> temp_scene = scene;
			ComPtr<RenderCom> temp_tr = tr;
			renderer->Invoke(Delegate([=]()->void {
				renderer->SetRenderTarget(temp_target.Get());
				temp_tr->OnFetchData();
				temp_tr->OnRender();
				temp_tr->SetActive(false);
				ComPtr<RenderCom> internal_tr = temp_tr;
				temp_scene->Invoke(Delegate([=]()->void {internal_tr->Destroy(); }));
				}));
		}

		void OnDestroy() override
		{
			target.Reset();
		}
	};

	class DescTitle :public MovingObject
	{
	public:

		TextRender2D* tr;

		ComPtr<IRenderTarget> target;

		DescTitle(const wstring& str) :tr(AddComponent(new TextRender2D(false)))
		{
			RenderTargetDesc desc;
			desc.hasDepth = false;
			desc.height = 100;
			desc.width = 200;
			desc.msaaEnabled = false;
			desc.multiSampleCount = 0;
			desc.support2D = true;
			engine->CreateRenderTarget(desc, &target);

			tr->SetTextFormat(tf_wryh_big.Get());
			tr->SetOutlineEnabled(true);
			tr->SetRect({ 0,0,200,100 });
			tr->SetText(str);
			tr->SetPriority(102, false);

			AddComponent(new RenderTargetSet::Setter(target.Get(), 0, true))->SetPriority(103, false);
			AddComponent(new RenderTargetSet::Setter(0))->SetPriority(101, false);
			SortComponents();

			transform2D->scale = { 1.5,1.5 };
			transform->scale = { 200,100 ,1 };
			mat.diffuse.a = 0;
			sr->SetMaterial(mat);
			sr->SetTexture(target.Get());
		}

		void OnCreated() override
		{
			InsertBefore(TitleSceneCom::locator[4].Get());
		}

		void OnDestroy() override
		{
			target.Reset();
		}
	};

public:

	int selectIndex;

	int frame;

	ComPtr<Tag> tags[4];

	ComPtr<Description> desc;

	ComPtr<DescTitle> title;

	bool isUpper;

	wstring descText[4];

	Frame_BackGround_Manual() :selectIndex(0), frame(0), isUpper(false)
	{
		wstringstream str;
		str << L"��������" << endl;
		str << L"  ���������������ɫ�ƶ�" << endl;
		str << L"  ��Z���������ӵ���������" << endl;
		str << L"  ��X����ʹ�÷��������У�" << endl;
		str << L"  ��Shift�����������ģʽ����ʾ�ж���" << endl;
		str << L"  ��Ctrl���������Ի�" << endl;
		str << L"  ��Esc������ͣ��Ϸ" << endl;
		str << endl;
		str << L"�������" << endl;
		str << L"  ��Ctrl�����ӿ�ط��ٶ�" << endl;
		str << L"  ��Alt+Enter�����л�ȫ��/����ģʽ" << endl;
		descText[0] = str.str();
		str.str(L"");

		str << L"��������������˺ܶ���¡�" << endl;
		str << endl;
		str << L"������յ����죬��һ��ͱ�������ܲ���֦Ҷ��ï�����֣�ȴͻȻ�����ѩ��������ˣ�ʱ��ĸ���Ҳ���ģ�����壬\
			��Ҿ�����������һ����Լ�������ʲô���ͺ������˿����˻������һ�С�" << endl;
		str << endl;
		str << L"����ֵ��ǣ����ܻ������Ѿ����ⳡ�����һ�����ˣ���Ҷ�������Щ��û��ʲô���Ծ���ÿ��ļƻ�Ҳ���ճ����С�\
			���������ԭ�������������ֺ�����Щ��ֵ��������û��������" << endl;
		str << endl;
		str << L"��һ�з·������ˡ���������Ϊ��ѡ��֮�ˣ����κ�ħ��ɳ�ܿ췢������Щ�쳣���������ǣ�����ȫ�������ʣ�����һͬ��������Դͷ�����ˡ�" << endl;
		descText[1] = str.str();
		str.str(L"");

		str << L"�����Ͻ�        ����Ϊ��ǰBOSS����" << endl;
		str << L"                   ���ǵ�����ΪBOSSʣ�����" << endl;
		str << endl;
		str << L"�����Ͻ�        HI SCOREΪ��ʷ��߷�" << endl;
		str << L"                   SCOREΪ��ǰ����" << endl;
		str << endl;
		str << L"�����½�        ��ɫ��������Ϊ��ǰ����ֵ" << endl;
		str << L"                   ��ɫ�õ����Ϊ��ǰ���õ�" << endl;
		str << endl;
		str << L"�����½�        ����Ϊ��ǰ�л������е������ٲл�" << endl;
		str << L"                   ����Ϊ��ǰ��������ʹ�÷���������" << endl;
		descText[2] = str.str();
		str.str(L"");

		str << L"�ӷֵ���" << endl;
		str << L"  ����ɫ�������ߣ�������ʱ����10000�֣�δ��ʱ����100��" << endl;
		str << L"  ����ɫ�õ���ߣ�����һ���������������Ϊ���õ㣬���һ���ݵ�ǰ����߶�˥�������˥����25%������λ��Խ�ߵ÷�Խ��" << endl;
		str << L"  �������������ߣ�����2000��" << endl;
		str << endl;
		str << L"���ߵ���" << endl;
		str << L"  ������һ��BOSS�����ɹ̶�����һЩ����" << endl;
		str << L"  �������ɵ��佱���������ߣ�������10���õ㡣ÿ����Ļֻ�ܵ���һ�ε���" << endl;
		str << L"  �����Missʱ������7����ɫ��������" << endl;
		str << L"  ������BOSS������Ƿ�ʱ�����ڵĵ�Ļ��ת��Ϊ������������" << endl;
		str << endl;
		str << L"�����ӷ���" << endl;
		str << L"  ��ÿ�Ե������һ���˺�����10��" << endl;
		str << L"  ���ɹ���ȡBOSS���������Ӵ�������" << endl;
		descText[3] = str.str();
	}

	void OnFrameCreated() override
	{
		__super::OnFrameCreated();

		const wchar_t* texts[] = { L"1.��������",L"2.��������",L"3.��Ϸ����",L"4.��Ϸ����" };

		for (int i = 0; i < 4; i++)
		{
			tags[i] = new Tag(texts[i]);
			tags[i]->transform->location = Vector3{ -280,210 - 36.0f * i,0 };
			tags[i]->Move({ -180,110 - 36.0f * i }, 1000, 100);
			tags[i]->Enter(30);
			scene->AttachActor(new TDelayCreate(tags[i].Get(), i * 5));
		}

		SetSelectedIndex(selectIndex);
	}

	void OnFrameInput() override
	{
		__super::OnFrameInput();
		if (input->KeyPress(DIK_DOWN))
		{
			se_select00.Play();
			isUpper = false;
			SetSelectedIndex((selectIndex + 1) % 4);
		}
		else if (input->KeyPress(DIK_UP))
		{
			se_select00.Play();
			isUpper = true;
			SetSelectedIndex((selectIndex + 3) % 4);
		}
		else if (input->KeyPress(DIK_X))
		{
			focused = false;
			se_cancel00.Play();
			LeaveForTitle();
		}
	}

	void OnFrameDestroy() override
	{
		__super::OnFrameDestroy();
		for (int i = 0; i < 4; i++)
		{
			tags[i].Reset();
		}
		desc.Reset();
		title.Reset();
	}

	void SetSelectedIndex(int index)
	{
		selectIndex = index;
		for (int i = 0; i < 4; i++)
		{
			if (tags[i])
			{
				if (i == selectIndex)
				{
					tags[i]->selected = true;
					tags[i]->Shake();
					SetDescriptionIndex(i);
				}
				else
				{
					tags[i]->selected = false;
				}
			}
		}
	}

	void SetDescriptionIndex(int index)
	{
		switch (index)
		{
		case 0:
		{
			SetTitle(L"����������");
			SetDescription(descText[0]);
		}
		break;
		case 1:
		{
			SetTitle(L"����������");
			SetDescription(descText[1]);
		}
		break;
		case 2:
		{
			SetTitle(L"����Ϸ����");
			SetDescription(descText[2]);
		}
		break;
		case 3:
		{
			SetTitle(L"����Ϸ����");
			SetDescription(descText[3]);
		}
		break;
		}
	}

	void SetDescription(const wstring& str)
	{
		if (desc)
			desc->Leave(10);
		desc = scene->AttachActor(new Description(str));
		desc->Enter(15);
		if (isUpper)
		{
			desc->transform->location = { 85,-142,0 };
			desc->Move({ 85,-92 }, 300, 100);
		}
		else
		{
			desc->transform->location = { 85,-42,0 };
			desc->Move({ 85,-92 }, 300, 100);
		}
	}

	void SetTitle(const wstring& str)
	{
		if (title)
			title->Leave(10);
		title = scene->AttachActor(new DescTitle(str));
		title->Enter(15);
		if (isUpper)
		{
			title->transform->location = { -50,108,0 };
			title->Move({ -50,158 }, 300, 100);
		}
		else
		{
			title->transform->location = { -50,208,0 };
			title->Move({ -50,158 }, 300, 100);
		}
	}

	void LeaveForTitle()
	{
		Leave(20);
		for (int i = 0; i < 4; i++)
		{
			if (tags[i])
				tags[i]->Leave(10);
		}
		if (desc)
			desc->Leave(10);
		if (title)
			title->Leave(10);
	}
};

class Frame_List_Replay :public Frame
{
public:

	ComPtr<SpriteTextTitle> sTitle[25];

	ComPtr<Frame> parent;

	int selectIndex;

	int upPressFrame;

	int downPressFrame;

	Frame_List_Replay() :selectIndex(0), upPressFrame(0), downPressFrame(0)
	{
	}

	void OnFrameCreated() override
	{
		__super::OnFrameCreated();
		for (int i = 0; i < 25; i++)
		{
			sTitle[i] = scene->AttachActor(new SpriteTextTitle);
			sTitle[i]->sr->SetText(GetReplayText(i + 1));
			sTitle[i]->transform->location = { -210,162,0 };
			sTitle[i]->Move({ -210,(float)(162 - 15 * i) }, 1600, 800);
			if (replayList[i + 1].GetPageAmount() == 0)
				sTitle[i]->sr->enabled = false;
			else
				sTitle[i]->sr->enabled = true;
		}
		SetSelectedIndex(0);
	};

	void OnFrameInput() override
	{
		__super::OnFrameInput();
		if (input->KeyPress(DIK_DOWNARROW))
		{
			se_select00.Play();
			SetSelectedIndex((selectIndex + 1) % 25);
		}
		else if (input->KeyPress(DIK_UPARROW))
		{
			se_select00.Play();
			SetSelectedIndex((selectIndex + 24) % 25);
		}
		else if (input->KeyPress(DIK_X))
		{
			se_cancel00.Play();
			LeaveForTitle();
		}
		else if (input->KeyPress(DIK_Z))
		{
			if (sTitle[selectIndex]->sr->enabled)
			{
				se_ok00.Play();
				currentReplay = replayList[selectIndex + 1];
				LeaveForStage();
			}
		}
		else
		{
			if (input->KeyDown(DIK_DOWNARROW))
			{
				downPressFrame++;
				if (downPressFrame == 9)
				{
					se_select00.Play();
					SetSelectedIndex((selectIndex + 1) % 25);
					downPressFrame = 0;
				}
			}
			else
			{
				downPressFrame = 0;
			}
			if (input->KeyDown(DIK_UPARROW))
			{
				upPressFrame++;
				if (upPressFrame == 9)
				{
					se_select00.Play();
					SetSelectedIndex((selectIndex + 24) % 25);
					upPressFrame = 0;
				}
			}
			else
			{
				upPressFrame = 0;
			}
		}
	}

	void OnFrameDestroy() override
	{
		__super::OnFrameDestroy();
		for (int i = 0; i < 25; i++)
			sTitle[i].Reset();
		parent.Reset();
		Leave(20);
	}

private:

	string GetReplayText(int index)
	{
		char t[128];
		if (replayList[index].GetPageAmount() != 0)
		{
			ReplayHeader header = replayList[index].GetHeader();
			const char* d = 0;
			switch (header.difficulty)
			{
			case 0:
				d = "Easy";
				break;
			case 1:
				d = "Normal";
				break;
			case 2:
				d = "Hard";
				break;
			case 3:
				d = "Lunatic";
				break;
			}
			sprintf(t, "No.%02d %-8s %02d/%02d/%02d %-6s %-7s St.%d", index, replayList[index].GetPlayerName().c_str(), header.time.year % 100,
				header.time.month, header.time.day, header.playerIndex == 0 ? "Reimu" : "Marisa", d, replayList[index].GetPageAmount());
		}
		else
		{
			sprintf(t, "No.%02d -------- --/--/-- ------ ------- St.-", index);
		}
		return t;
	}

	void SetSelectedIndex(int index)
	{
		selectIndex = index;
		for (int i = 0; i < 25; i++)
		{
			if (sTitle[i])
			{
				if (i == selectIndex)
					sTitle[i]->sr->highlight = true;
				else
					sTitle[i]->sr->highlight = false;
			}
		}
	}

	void LeaveForTitle()
	{
		if (parent)
			parent->Leave(20);
		for (int i = 0; i < 25; i++)
		{
			if (sTitle[i])
				sTitle[i]->Destroy();
		}
		Destroy();
	}

	void LeaveForStage();
};

class Frame_Stage_Replay :public Frame
{
public:

	int stageNum; //��Ҫ�޸������С

	int rpyIndex;

	int frame;

	int selectIndex;

	ComPtr<Frame> parent;

	ComPtr<SpriteTextTitle> textRef;

	ComPtr<SpriteTextTitle> stageText[3];

	Frame_Stage_Replay() :rpyIndex(0), frame(0), stageNum(3), selectIndex(0)
	{
	}

	void OnFrameCreated() override
	{
		__super::OnFrameCreated();
	}

	void OnFrameInput() override
	{
		if (frame > 10)
		{
			if (input->KeyPress(DIK_DOWNARROW))
			{
				se_select00.Play();
				SetSelectedIndex((selectIndex + 1) % stageNum);
			}
			else if (input->KeyPress(DIK_UPARROW))
			{
				se_select00.Play();
				SetSelectedIndex((selectIndex + stageNum - 1) % stageNum);
			}
			else if (input->KeyPress(DIK_X))
			{
				se_cancel00.Play();
				LeaveForList();
			}
			else if (input->KeyPress(DIK_Z))
			{
				if (stageText[selectIndex]->sr->enabled)
				{
					se_ok00.Play();
					StartReplay();
				}
			}
		}
	}

	void OnFrameThink() override
	{
		frame++;
		if (frame == 10)
		{
			for (int i = 0; i < stageNum; i++)
			{
				stageText[i] = scene->AttachActor(new SpriteTextTitle);
				stageText[i]->transform->location = { -92,(float)(100 - 20 * i),0 };
				stageText[i]->sr->SetText(GetStageText(i));
				stageText[i]->transform->scale = { 16,16,1 };
				stageText[i]->sr->SetMargin(0.6);
				if (i >= currentReplay.GetPageAmount())
					stageText[i]->sr->enabled = false;
			}
			SetSelectedIndex(0);
		}
	}

	void OnFrameDestroy() override
	{
		__super::OnFrameDestroy();
		parent.Reset();
		textRef.Reset();
		for (int i = 0; i < stageNum; i++)
			stageText[i].Reset();
	}

private:

	string GetStageText(int index)
	{
		char t[128];
		if (index < currentReplay.GetPageAmount())
		{
			auto page = currentReplay.GetPage(index);
			auto header = page.GetHeader();
			sprintf(t, "Stage %d  %-9lld", index + 1, header.finalScore);
		}
		else
		{
			sprintf(t, "Stage %d  ---------", index + 1);
		}
		return t;
	}

	void SetSelectedIndex(int index)
	{
		selectIndex = index;
		for (int i = 0; i < stageNum; i++)
		{
			if (stageText[i])
			{
				if (i == selectIndex)
					stageText[i]->sr->highlight = true;
				else
					stageText[i]->sr->highlight = false;
			}
		}
	}

	void  LeaveForList();

	void StartReplay();
};

class Frame_BackGround_Replay :public Frame_BackGround
{
	class ReplayTitle :public MovingObject
	{
	public:

		ReplayTitle()
		{
			transform->scale = { 197 * 3 / 4.0f,60 * 3 / 4.0f,1 };
			sr->SetTexture(src_replay.Get());
		}

		void OnCreated() override
		{
			InsertBefore(TitleSceneCom::locator[4].Get());
		}
	};

public:

	ComPtr<ReplayTitle> replayTitle;

	void OnFrameCreated() override
	{
		__super::OnFrameCreated();
		scene->AttachActor(new Frame_List_Replay)->parent = this;
		replayTitle = scene->AttachActor(new ReplayTitle);
		replayTitle->transform->location = { 0,300,0 };
		replayTitle->Move({ 0,200 }, 500, 200);
	}

	void OnFrameDestroy() override
	{
		__super::OnFrameDestroy();
		replayTitle.Reset();
	}

	void Leave(int frame, float targetAlpha) override
	{
		__super::Leave(frame, targetAlpha);
		replayTitle->Move({ 0,300 }, 500, 200);
		replayTitle->Leave(15, 0);
	}
};

class Frame_Title :public Frame
{
public:

	int selectIndex;

	int frame;

	ComPtr<Title> titles[6];

	Frame_Title() :selectIndex(0), frame(0)
	{
		transform->scale = { 400,24,1 };
		mat.diffuse = { 1,1,1,0.0 };
		sr->SetMaterial(mat);
		sr->SetTexture(src_selector.Get());
	}

	void OnFrameCreated() override
	{
		InsertBefore(TitleSceneCom::locator[0].Get());
		int types[] = { 0,2,4,7,8,9 };
		for (int i = 0; i < 6; i++)
		{
			titles[i] = new Title(types[i], { -240,10 - 32.0f * i,0 });
			titles[i]->transform->location = Vector3{ -340,110 - 32.0f * i,0 };
			scene->AttachActor(new TDelayCreate(titles[i].Get(), i * 6 + 10));
		}
		titles[0]->selected = true;
		transform->location = titles[0]->transform->location;
	}

	void OnFrameInput() override
	{
		if (input->KeyPress(DIK_DOWN))
		{
			se_select00.Play();
			SetSelectedIndex((selectIndex + 1) % 6);
		}
		else if (input->KeyPress(DIK_UP))
		{
			se_select00.Play();
			SetSelectedIndex((selectIndex + 5) % 6);
		}
		else if (input->KeyPress(DIK_Z))
		{
			se_ok00.Play();
			switch (selectIndex)
			{
			case 0: //start game
			{
				practiceMode = false;
				focused = false;
				isReplaying = false;
				titles[0]->GListen(true);
				scene->AttachActor(new TDelayCreate(new Frame_BackGround_StartGame, 20));
			}
			break;
			case 1: //practice start
			{
				practiceMode = true;
				focused = false;
				isReplaying = false;
				titles[1]->GListen(true);
				scene->AttachActor(new TDelayCreate(new Frame_BackGround_StartGame, 20));
			}
			break;
			case 2: //replay
			{
				practiceMode = false;
				focused = false;
				isReplaying = true;
				titles[2]->GListen(true);
				scene->AttachActor(new TDelayCreate(new Frame_BackGround_Replay, 20));
			}
			break;
			case 3: //option
			{
				focused = false;
				titles[3]->GListen(true);
				scene->AttachActor(new TDelayCreate(new Frame_Option, 20));
			}
			break;
			case 4: //manual
			{
				focused = false;
				titles[4]->GListen(true);
				scene->AttachActor(new TDelayCreate(new Frame_BackGround_Manual, 20));
			}
			break;
			case 5: //exit
			{
				PostThreadMessage(mainThreadID, WM_QUIT, 0, 0);
			}
			break;
			}
		}
		else if (input->KeyPress(DIK_X))
		{
			se_cancel00.Play();
			SetSelectedIndex(5);
		}
	}

	void OnFrameThink() override
	{
		frame++;
		if (frame > 30)
		{
			if (frame <= 80)
			{
				mat.diffuse.a += 0.01;
				sr->SetMaterial(mat);
				transform->location = titles[selectIndex]->transform->location;
			}
		}
	}

	void SetSelectedIndex(int index)
	{
		selectIndex = index;
		for (int i = 0; i < 6; i++)
		{
			if (i == selectIndex)
			{
				titles[i]->Roll();
				titles[i]->selected = true;
				transform->location = titles[i]->transform->location;
			}
			else
			{
				titles[i]->selected = false;
			}
		}
	}
};