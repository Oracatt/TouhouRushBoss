#pragma once
#include "VirtualLib.h"
#include "SoundEffect.h"
#include "Source.h"
#include "BaseObject.h"
#include "SceneObjects.h"
#include "Replay.h"
#include "UserRenderer.h"
#include "Level.h"
#include "UIDeriver.h"

extern bool replayOver;
extern bool allowSaveReplay;
extern const char charArray[7][13];

class DelayInvoke :public GameObject
{
public:

	DelayInvoke(IDelegate* _delegate, int _frame) :
		del(_delegate), limit(_frame), frame(0)
	{
	}

private:

	ComPtr<IDelegate> del;

	int frame;

	int limit;

	void OnUpdate() override
	{
		frame++;
		if (frame >= limit)
		{
			del->Invoke();
			Destroy();
		}
	}

	void OnDestroy() override
	{
		if (del)
		{
			del.Reset();
		}
	}
};

class DelayCreate :public GameObject
{
public:

	DelayCreate(GameObject* _g, int _frame) :g(_g), frame(0), limit(_frame)
	{
	}

private:

	int frame;

	int limit;

	LPGOBJ g;

	void OnUpdate() override
	{
		frame++;
		if (frame >= limit)
		{
			scene->AttachActor(g.Get());
			g.Reset();
			Destroy();
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
};

class PauseMenu :public GameObject
{
protected:

	class BlurBackGround :public GameObject
	{
	public:

		BlurBackGround() :range(0), isLeft(false)
		{
			SetUpdateLevel(1);
			transform->scale = { 640,480,1 };
			SpriteRender* sr = new SpriteRender;
			sr->SetTexture(GameSceneCom::gameTarget.Get());
			ComPtr<BlurBackGround> ref = this;
			AddComponent(new CallbackSet::Setter(&blurCallback, [=]()->void {renderer->SendShaderData(SD_BLURSCALE, &ref->range, 0); }));
			AddComponent(sr);
			AddComponent(new CallbackSet::Setter(&mainCallback, 0));
		}

		void Leave()
		{
			isLeft = true;
		}

	private:

		float range;

		bool isLeft;

		void OnUpdate() override
		{
			if (!isLeft)
			{
				if (range < 1)
					range += 0.05;
			}
			else
			{
				range -= 0.05;
				if (range <= 0)
					Destroy();
			}
		}

		void OnCreated() override
		{
			InsertBefore(GameSceneCom::pauseLocator.Get());
		}
	};

	class Figure :public MovingObject //花纹
	{
	public:

		bool isInSubSelection;

		Figure() :frame(0), isInSubSelection(false)
		{
			SetUpdateLevel(1);
			mb->SetUpdateLevel(1);
			leaveEnter->SetUpdateLevel(1);
			transform->location = { -260,-100,0 };
			transform->scale = { 0.4f * 512 / 5.0f,0.62f * 704 * 0.55f,1 };
			transform->SetEulerAngles({ 0,0,0.07f + 0.07f * (float)sin(frame / 20.0f) });
			mat.diffuse = { 0.4f,0.75f,0.75f,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_pause.Get());
			sr->SetSourceRect({ 4 / 5.0f,1 * 0.45f,1 / 5.0f,1 * 0.55f });
			Enter(20, 0.75f);
		}

		void OnUpdate() override
		{
			frame++;
			transform->SetEulerAngles({ 0,0,0.07f + 0.07f * (float)sin(frame / 20.0f) });
			if (isInSubSelection)
			{
				mat.diffuse = { 0.75f,0.4f,0.4f,mat.diffuse.a };
				sr->SetMaterial(mat);
			}
			else
			{
				mat.diffuse = { 0.4f,0.75f,0.75f,mat.diffuse.a };
				sr->SetMaterial(mat);
			}
		}

		void OnCreated() override
		{
			InsertBefore(GameSceneCom::pauseLocator.Get());
		}

	private:

		int frame;
	};

	class PrimaryTitle :public MovingObject //标题
	{
	public:

		PrimaryTitle()
		{
			SetUpdateLevel(1);
			leaveEnter->SetUpdateLevel(1);
			mb->SetUpdateLevel(1);
			transform->location = { -250,-35,0 };
			transform->scale = { 0.7f * 512 / 3.0f,0.7f * 704 / 9.0f,1 };
			mat.diffuse.a = 0;
			sr->SetMaterial(mat);
			sr->SetTexture(src_pause.Get());
			sr->SetSourceRect({ 0,0,1 / 3.0f,1 / 9.0f });
			SetUpdateLevel(1);
			Enter(20, 1);
			Move({ -230,-35 }, 100, 50);
		}

		void OnCreated() override
		{
			InsertBefore(GameSceneCom::pauseLocator.Get());
		}
	};

	class SecondaryTitle :public MovingObject //选项
	{
	public:

		bool enabled;

		bool selected;

		int frame;

		SecondaryTitle() :selected(false), frame(0), enabled(true)
		{
			SetUpdateLevel(1);
			leaveEnter->SetUpdateLevel(1);
			mb->SetUpdateLevel(1);
			mat.diffuse.a = 0;
			sr->SetMaterial(mat);
			sr->SetTexture(src_pause.Get());
			SetUpdateLevel(1);
			Enter(20, 1);
			AnimSetLocalTransform(XMMatrixTranslation(0.5, 0, 0));
		}

		void OnCreated() override
		{
			InsertBefore(GameSceneCom::pauseLocator.Get());
		}

		void OnUpdate() override
		{
			frame++;
			if (enabled)
			{
				if (selected)
				{
					mat.diffuse.r = 0.8;
					mat.diffuse.g = mat.diffuse.b = 0.9f + 0.1f * (float)sin(frame / 12.0f);
				}
				else
				{
					mat.diffuse.r = mat.diffuse.g = mat.diffuse.b = 0.4;
				}
			}
			else
			{
				mat.diffuse.r = mat.diffuse.g = mat.diffuse.b = 0.1;
			}
			sr->SetMaterial(mat);
		}
	};

	class SpriteTextTitle :public SimpleMovingObject
	{
	public:

		int frame;

		SpriteTextRender* sr;

		SpriteTextTitle() :frame(0), sr(AddComponent(new SpriteTextRender))
		{
			transform->scale = { 16,16,1 };
			mb->SetUpdateLevel(1);
			SetUpdateLevel(1);
		}

		void OnCreated() override
		{
			InsertBefore(GameSceneCom::pauseLocator.Get());
		}
	};
};

class ReplayOverMenu :public PauseMenu
{
	class SecondaryImpl
	{
	public:

		class ReplayAgain :public SecondaryTitle
		{
		public:

			ReplayAgain()
			{
				transform->location = { -285,-85,0 };
				transform->scale = { 0.6f * 233.0f,0.6f * 59,1 };
				sr->SetSourceRect({ 0,432 / 704.0f,233 / 512.0f,59 / 704.0f });
				Move({ -265,-85 }, 100, 50);
			}
		};

		class ReturnTitle :public SecondaryTitle
		{
		public:

			ReturnTitle()
			{
				transform->location = { -285,-120,0 };
				transform->scale = { 0.6f * 246.0f,0.6f * 61.0f,1 };
				sr->SetSourceRect({ 0,137 / 704.0f,246 / 512.0f,61 / 704.0f });
				Move({ -265,-120 }, 100, 50);
			}
		};
	};

public:

	ComPtr<BlurBackGround> blurBackGround;

	ComPtr<Figure> figure;

	ComPtr<PrimaryTitle> pTitle;

	ComPtr<SecondaryTitle> sTitle[2];

	int frame;

	bool focused;

	int selectIndex;

	ReplayOverMenu() :frame(0), focused(true), selectIndex(0)
	{
		SetUpdateLevel(1);
	}

	void OnCreated() override
	{
		timer->SetPresentCount(1);
		InsertBefore(GameSceneCom::pauseLocator.Get());
		blurBackGround = scene->AttachActor(new BlurBackGround);
		figure = scene->AttachActor(new Figure);
		pTitle = scene->AttachActor(new PrimaryTitle);
		pTitle->sr->SetSourceRect({ 2 / 3.0f,0,1 / 3.0f,1 / 9.0f });
		sTitle[0] = new SecondaryImpl::ReplayAgain;
		sTitle[1] = new SecondaryImpl::ReturnTitle;
		for (int i = 0; i < 2; i++)
		{
			auto g = new DelayCreate(sTitle[i].Get(), i * 6);
			g->SetUpdateLevel(1);
			scene->AttachActor(g);
		}
		SetSelectedIndex(0);
	}

	void OnUpdate() override
	{
		frame++;
		if (frame > 25 && focused)
		{
			if (input->KeyPress(DIK_DOWNARROW))
			{
				se_select00.Play();
				SetSelectedIndex((selectIndex + 1) % 2);
			}
			else if (input->KeyPress(DIK_UPARROW))
			{
				se_select00.Play();
				SetSelectedIndex((selectIndex + 1) % 2);
			}
			else if (input->KeyPress(DIK_Z))
			{
				switch (selectIndex)
				{
				case 0:
				{
					focused = false;
					sTitle[0]->GListen(false);
					se_ok00.Play();
					scene->AttachActor(new ChangeLevel(nowLevel, true));
				}
				break;
				case 1:
				{
					focused = false;
					se_ok00.Play();
					sTitle[1]->GListen(false);
					scene->AttachActor(new ChangeLevel(LEVEL_TITLE, true));
				}
				break;
				}
			}
		}
	}

	void OnDestroy() override
	{
		blurBackGround.Reset();
		figure.Reset();
		pTitle.Reset();
		for (int i = 0; i < 2; i++)
			sTitle[i].Reset();
	}

	void SetSelectedIndex(int index)
	{
		selectIndex = index;
		for (int i = 0; i < 2; i++)
		{
			sTitle[i]->selected = i == index;
			if (i == index)
				sTitle[i]->Shake(1.5f / sTitle[i]->transform->scale.x);
		}
	}
};

class GamePauseMenu :public PauseMenu
{
	class SecondaryImpl
	{
	public:

		class ContinueGame :public SecondaryTitle
		{
		public:

			ContinueGame()
			{
				transform->location = { -285,-85,0 };
				transform->scale = { 0.6f * 246.0f,0.6f * 61.0f,1 };
				sr->SetSourceRect({ 0,78 / 704.0f,246 / 512.0f,61 / 704.0f });
				Move({ -265,-85 }, 100, 50);
			}
		};

		class ReturnTitle :public SecondaryTitle
		{
		public:

			ReturnTitle()
			{
				transform->location = { -285,-120,0 };
				transform->scale = { 0.6f * 246.0f,0.6f * 61.0f,1 };
				sr->SetSourceRect({ 0,137 / 704.0f,246 / 512.0f,61 / 704.0f });
				Move({ -265,-120 }, 100, 50);
			}
		};

		class SaveAndQuit :public SecondaryTitle
		{
		public:

			SaveAndQuit()
			{
				if (!allowSaveReplay)
					enabled = false;
				transform->location = { -285,-155,0 };
				transform->scale = { 0.6f * 363.0f,0.6f * 61.0f,1 };
				sr->SetSourceRect({ 0,255 / 704.0f,363 / 512.0f,61 / 704.0f });
				Move({ -265,-155 }, 100, 50);
			}
		};

		class RestartGame :public SecondaryTitle
		{
		public:

			RestartGame()
			{
				transform->location = { -285,-190,0 };
				transform->scale = { 0.6f * 203.0f,0.6f * 61.0f,1 };
				sr->SetSourceRect({ 0,196 / 704.0f,203 / 512.0f,61 / 704.0f });
				Move({ -265,-190 }, 100, 50);
			}
		};

		class Really :public SecondaryTitle
		{
		public:

			Really()
			{
				transform->location = { -50,-80,0 };
				transform->scale = { 0.6f * 240.0f,0.6f * 61.0f,1 };
				sr->SetSourceRect({ 0,314 / 704.0f,240 / 512.0f,61 / 704.0f });
			}

			void OnUpdate() override {}
		};

		class YesYes :public SecondaryTitle
		{
		public:

			YesYes()
			{
				transform->location = { -25,-120,0 };
				transform->scale = { 0.6f * 107.0f,0.6f * 61.0f,1 };
				sr->SetSourceRect({ 205 / 512.0f,196 / 704.0f,107 / 512.0f,61 / 704.0f });
			}
		};

		class NoNo :public SecondaryTitle
		{
		public:

			NoNo()
			{
				transform->location = { -25,-155,0 };
				transform->scale = { 0.6f * 86.0f,0.6f * 61.0f,1 };
				sr->SetSourceRect({ 342 / 512.0f,196 / 704.0f,86 / 512.0f,61 / 704.0f });
			}
		};
	};

	class Really :public GameObject
	{
	public:

		ComPtr<GamePauseMenu> menu;

		ComPtr<SecondaryTitle> sTitle[3];

		int frame;

		int selectIndex;

		bool focused;

		Really() :frame(0), selectIndex(1), focused(true)
		{
			SetUpdateLevel(1);
		}

		void OnCreated() override
		{
			InsertBefore(GameSceneCom::pauseLocator.Get());
			sTitle[0] = scene->AttachActor(new SecondaryImpl::Really);
			sTitle[1] = scene->AttachActor(new SecondaryImpl::YesYes);
			sTitle[2] = scene->AttachActor(new SecondaryImpl::NoNo);
			SetSelectedIndex(selectIndex);
		}

		void Leave()
		{
			menu->CancelReally();
			for (int i = 0; i < 3; i++)
				sTitle[i]->Leave(10);
			Destroy();
		}

		void OnUpdate() override
		{
			frame++;
			if (frame > 10 && focused)
			{
				if (input->KeyPress(DIK_DOWNARROW))
				{
					se_select00.Play();
					SetSelectedIndex(selectIndex == 1 ? 2 : 1);
				}
				else if (input->KeyPress(DIK_UPARROW))
				{
					se_select00.Play();
					SetSelectedIndex(selectIndex == 1 ? 2 : 1);
				}
				else if (input->KeyPress(DIK_Z))
				{
					switch (selectIndex)
					{
					case 1:
					{
						focused = false;
						sTitle[1]->GListen(false);
						se_ok00.Play();
						menu->result = true;
						auto g = new DelayInvoke(Delegate([=]()->void {Leave(); }), 20);
						g->SetUpdateLevel(1);
						scene->AttachActor(g);
					}
					break;
					case 2:
					{
						focused = false;
						sTitle[2]->GListen(false);
						se_ok00.Play();
						menu->result = false;
						auto g = new DelayInvoke(Delegate([=]()->void {Leave(); }), 20);
						g->SetUpdateLevel(1);
						scene->AttachActor(g);
					}
					break;
					}
				}
				else if (input->KeyPress(DIK_X))
				{
					se_cancel00.Play();
					menu->result = false;
					Leave();
				}
			}
		}

		void OnDestroy() override
		{
			menu.Reset();
			for (int i = 0; i < 3; i++)
				sTitle[i].Reset();
		}

		void SetSelectedIndex(int index)
		{
			selectIndex = index;
			for (int i = 0; i < 3; i++)
			{
				sTitle[i]->selected = i == index;
				if (i == index)
					sTitle[i]->Shake(1.5f / sTitle[i]->transform->scale.x);
			}
		}
	};

	class KeyboardMenu :public GameObject
	{
		class BlackCanvas :public AnimObject
		{
		public:
			
			BlackCanvas()
			{
				transform->scale = { 640,480,1 };
				SetUpdateLevel(1);
				leaveEnter->SetUpdateLevel(1);
				sr->SetTexture(src_dummy.Get());
				mat.diffuse = { 0,0,0,0 };
				sr->SetMaterial(mat);
				Enter(15, 1);
			}
		};

	public:

		ComPtr<SpriteTextTitle> keys[7][13];

		ComPtr<GamePauseMenu> menu;

		ComPtr<SpriteTextTitle> textRef;

		ComPtr<SpriteTextTitle> editor;

		int selectRow;

		int selectCol;

		char nowName[9];

		int rpyIndex;

		int wordNum;

		Date time;

		int lpFrame;

		int rpFrame;

		int upFrame;

		int dpFrame;

		bool focused;

		KeyboardMenu() :selectRow(0), selectCol(0), rpyIndex(0), wordNum(0), time(SyncDate()),
			lpFrame(0), rpFrame(0), upFrame(0), dpFrame(0), focused(true)
		{
			memset(nowName, 0, sizeof(nowName));
			strcpy(nowName, lastSaveName.c_str());
			for (int i = 0; i < 9; i++)
			{
				if (nowName[i])
					wordNum++;
			}
			SetUpdateLevel(1);
		}

		void OnCreated() override
		{
			InsertBefore(GameSceneCom::pauseLocator.Get());
			char w[2];
			w[1] = 0;
			for (int r = 0; r < 7; r++)
				for (int c = 0; c < 13; c++)
				{
					keys[r][c] = scene->AttachActor(new SpriteTextTitle);
					keys[r][c]->transform->location = { float(-135 + 22 * c),float(-60 - 22 * r),0 };
					w[0] = charArray[r][c];
					keys[r][c]->sr->enabled = false;
					keys[r][c]->sr->SetText(w);
				}
			SetSelectedCol(0);
			ResetText();

			editor = scene->AttachActor(new SpriteTextTitle);
			editor->sr->SetText("_");
			int x = wordNum + 1;
			if (x < 1)
				x = 1;
			if (x > 8)
				x = 8;
			editor->transform->location = { (float)(-145 + 10.8f * x),textRef->transform->location.y - 3,0 };
		}

		void OnUpdate() override
		{
			if (focused)
			{
				if (editor)
				{
					int x = wordNum + 1;
					if (x < 1)
						x = 1;
					if (x > 8)
						x = 8;
					editor->transform->location = { (float)(-145 + 10.8f * x),textRef->transform->location.y - 3,0 };
				}
				if (input->KeyPress(DIK_LEFT))
				{
					se_select00.Play();
					SetSelectedCol((selectCol + 12) % 13);
				}
				else if (input->KeyPress(DIK_RIGHT))
				{
					se_select00.Play();
					SetSelectedCol((selectCol + 1) % 13);
				}
				else if (input->KeyPress(DIK_UP))
				{
					se_select00.Play();
					SetSelectedRow((selectRow + 6) % 7);
				}
				else if (input->KeyPress(DIK_DOWN))
				{
					se_select00.Play();
					SetSelectedRow((selectRow + 1) % 7);
				}
				else if (input->KeyPress(DIK_X))
				{
					se_cancel00.Play();
					if (wordNum != 0)
						EnterWord(127);
					else
						LeaveForSaveMenu();
				}
				else if (input->KeyPress(DIK_Z))
				{
					se_ok00.Play();
					EnterWord(charArray[selectRow][selectCol]);
				}
				else
				{
					if (input->KeyDown(DIK_DOWN))
					{
						dpFrame++;
						if (dpFrame == 8)
						{
							se_select00.Play();
							SetSelectedRow((selectRow + 1) % 7);
							dpFrame = 0;
						}
					}
					else
					{
						dpFrame = 0;
					}
					if (input->KeyDown(DIK_UP))
					{
						upFrame++;
						if (upFrame == 8)
						{
							se_select00.Play();
							SetSelectedRow((selectRow + 6) % 7);
							upFrame = 0;
						}
					}
					else
					{
						upFrame = 0;
					}
					if (input->KeyDown(DIK_LEFT))
					{
						lpFrame++;
						if (lpFrame == 8)
						{
							se_select00.Play();
							SetSelectedCol((selectCol + 12) % 13);
							lpFrame = 0;
						}
					}
					else
					{
						lpFrame = 0;
					}
					if (input->KeyDown(DIK_RIGHT))
					{
						rpFrame++;
						if (rpFrame == 8)
						{
							se_select00.Play();
							SetSelectedCol((selectCol + 1) % 13);
							rpFrame = 0;
						}
					}
					else
					{
						rpFrame = 0;
					}
				}
			}
		}

		void OnDestroy() override
		{
			for (int r = 0; r < 7; r++)
				for (int c = 0; c < 13; c++)
				{
					keys[r][c].Reset();
				}
			menu.Reset();
			textRef.Reset();
			editor.Reset();
		}

	private:

		void ResetText()
		{
			char t[128];
			const char* d = 0;
			switch (difficulty)
			{
			case 0:
				d = "E";
				break;
			case 1:
				d = "N";
				break;
			case 2:
				d = "H";
				break;
			case 3:
				d = "L";
				break;
			}
			sprintf(t, "No.%02d %-8s %02d/%02d/%02d %-6s %-2s St.%d", rpyIndex, nowName, time.year % 100,
				time.month, time.day, playerIndex == 0 ? "Reimu" : "Marisa", d, nowStage + 1);
			textRef->sr->SetText(t);
		}

		void SetSelectedRow(int row)
		{
			selectRow = row;
			for (int r = 0; r < 7; r++)
				for (int c = 0; c < 13; c++)
			{
				if (keys[r][c])
				{
					if (r == selectRow&&c==selectCol)
						keys[r][c]->sr->highlight = true;
					else
						keys[r][c]->sr->highlight = false;
				}
			}
		}

		void SetSelectedCol(int col)
		{
			selectCol = col;
			for (int r = 0; r < 7; r++)
			{
				for (int c = 0; c < 13; c++)
				{
					if (keys[r][c])
					{
						if (r == selectRow && c == selectCol)
							keys[r][c]->sr->highlight = true;
						else
							keys[r][c]->sr->highlight = false;
					}
				}
			}
		}

		void EnterWord(unsigned char c)
		{
			if (c != 127 && c != 128)
			{
				if (wordNum < 8)
				{
					nowName[wordNum++] = c;
					ResetText();
				}
				if (wordNum == 8)
				{
					SetSelectedRow(6);
					SetSelectedCol(12);
				}
			}
			else if (c == 127)
			{
				if (wordNum > 0)
				{
					nowName[--wordNum] = 0;
					ResetText();
				}
			}
			else
			{
				focused = false;
				bgm_gameover->Stop();
				se_extend.Play();
				lastSaveName = nowName;
				StopRecording();
				char filename[128];
				sprintf(filename, "save_%d.rpy", rpyIndex);
				SaveReplay(filename, nowName);
				scene->Invoke(Delegate([=]()->void {
					replayList[rpyIndex].LoadFromFile(filename);
					}));
				scene->AttachActor(new BlackCanvas);
				scene->AttachActor(new ChangeLevel(LEVEL_TITLE, true));
			}
		}

		Date SyncDate()
		{
			time_t timep;
			struct tm* p;
			::time(&timep);
			p = gmtime(&timep);

			Date time;
			time.year = 1900 + p->tm_year;
			time.month = 1 + p->tm_mon;
			time.day = p->tm_mday;
			return time;
		}

		void LeaveForSaveMenu();
	};

	class SaveMenu :public GameObject
	{
	public:

		ComPtr<SpriteTextTitle> sTitle[25];

		ComPtr<GamePauseMenu> menu;

		int downPressFrame;

		int upPressFrame;

		SaveMenu() :selectIndex(0), downPressFrame(0), upPressFrame(0)
		{
			SetUpdateLevel(1);
		}

		void OnCreated() override
		{
			InsertBefore(GameSceneCom::pauseLocator.Get());
			for (int i = 0; i < 25; i++)
			{
				sTitle[i] = scene->AttachActor(new SpriteTextTitle);
				sTitle[i]->sr->SetText(GetReplayText(i + 1));
				sTitle[i]->transform->location = { -200,200,0 };
				sTitle[i]->Move({ -200,(float)(200 - 17 * i) }, 1600, 800);
				if (replayList[i + 1].GetPageAmount() == 0)
					sTitle[i]->sr->enabled = false;
				else
					sTitle[i]->sr->enabled = true;
			}
			SetSelectedIndex(0);
		}

		void OnUpdate() override
		{
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
				LeaveForMenu();
			}
			else if (input->KeyPress(DIK_Z))
			{
				se_ok00.Play();
				LeaveForKeyBoard();
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

		void OnDestroy() override
		{
			for (int i = 0; i < 25; i++)
				sTitle[i].Reset();
			menu.Reset();
		}

	private:

		int selectIndex;

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
					d = "E";
					break;
				case 1:
					d = "N";
					break;
				case 2:
					d = "H";
					break;
				case 3:
					d = "L";
					break;
				}
				sprintf(t, "No.%02d %-8s %02d/%02d/%02d %-6s %-2s St.%d", index, replayList[index].GetPlayerName().c_str(), header.time.year % 100,
					header.time.month, header.time.day, header.playerIndex == 0 ? "Reimu" : "Marisa", d, replayList[index].GetPageAmount());
			}
			else
			{
				sprintf(t, "No.%02d -------- --/--/-- ------ -- St.-", index);
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

		void LeaveForMenu()
		{
			for (int i = 0; i < 25; i++)
			{
				if (sTitle[i])
					sTitle[i]->Destroy();
			}
			menu->focused = true;
			menu->Show();
			Destroy();
		}

		void LeaveForKeyBoard()
		{
			sTitle[selectIndex]->Move({ -200 ,10 }, 1000, 500);
			sTitle[selectIndex]->sr->enabled = true;
			sTitle[selectIndex]->sr->highlight = false;
			for (int i = 0; i < 25; i++)
			{
				if (i != selectIndex)
				{
					sTitle[i]->Destroy();
				}
			}
			auto g = new KeyboardMenu;
			g->rpyIndex = selectIndex + 1;
			g->menu = menu;
			g->textRef = sTitle[selectIndex];
			scene->AttachActor(g);
			Destroy();
		}
	};

public:

	ComPtr<BlurBackGround> blurBackGround;

	ComPtr<Figure> figure;

	ComPtr<PrimaryTitle> pTitle;

	ComPtr<SecondaryTitle> sTitle[4];

	ComPtr<Really> really;

	int frame;

	bool focused;

	int selectIndex;

	bool result;

	GamePauseMenu() :frame(0), focused(true), selectIndex(0), result(false)
	{
		SetUpdateLevel(1);
	}

	void OnCreated() override
	{
		timer->SetPresentCount(1);
		InsertBefore(GameSceneCom::pauseLocator.Get());
		blurBackGround = scene->AttachActor(new BlurBackGround);
		figure = scene->AttachActor(new Figure);
		pTitle = scene->AttachActor(new PrimaryTitle);
		sTitle[0] = new SecondaryImpl::ContinueGame;
		sTitle[1] = new SecondaryImpl::ReturnTitle;
		sTitle[2] = new SecondaryImpl::SaveAndQuit;
		sTitle[3] = new SecondaryImpl::RestartGame;
		for (int i = 0; i < 4; i++)
		{
			auto g = new DelayCreate(sTitle[i].Get(), i * 6);
			g->SetUpdateLevel(1);
			scene->AttachActor(g);
		}
		SetSelectedIndex(0);
	}

	void OnUpdate() override
	{
		frame++;
		if (frame > 25 && focused)
		{
			if (input->KeyPress(DIK_DOWNARROW))
			{
				se_select00.Play();
				SetSelectedIndex((selectIndex + 1) % 4);
			}
			else if (input->KeyPress(DIK_UPARROW))
			{
				se_select00.Play();
				SetSelectedIndex((selectIndex + 3) % 4);
			}
			else if (input->KeyPress(DIK_Z))
			{
				switch (selectIndex)
				{
				case 0:
				{
					focused = false;
					se_ok00.Play();
					LeaveForContinue();
				}
				break;
				case 1:
				{
					focused = false;
					se_ok00.Play();
					sTitle[1]->GListen(false);
					auto g = new DelayInvoke(Delegate([=]()->void {ShowReally(); }), 20);
					g->SetUpdateLevel(1);
					scene->AttachActor(g);
				}
				break;
				case 2:
				{
					focused = false;
					se_ok00.Play();
					Hide();
					auto g = new SaveMenu;
					g->menu = this;
					scene->AttachActor(g);
				}
				break;
				case 3:
				{
					focused = false;
					se_ok00.Play();
					sTitle[3]->GListen(false);
					auto g = new DelayInvoke(Delegate([=]()->void {ShowReally(); }), 20);
					g->SetUpdateLevel(1);
					scene->AttachActor(g);
				}
				break;
				}
			}
			else if (input->KeyPress(DIK_X))
			{
				focused = false;
				se_cancel00.Play();
				LeaveForContinue();
			}
		}
	}

	void OnDestroy() override
	{
		blurBackGround.Reset();
		figure.Reset();
		pTitle.Reset();
		for (int i = 0; i < 4; i++)
			sTitle[i].Reset();
		really.Reset();
	}

	void Hide()
	{
		figure->Enter(1, 0);
		for (int i = 0; i < 4; i++)
		{
			if (sTitle[i])
				sTitle[i]->Enter(1, 0);
		}
		pTitle->Enter(1, 0);
	}

	void Show()
	{
		figure->Enter(10, 1);
		for (int i = 0; i < 4; i++)
		{
			if (sTitle[i])
				sTitle[i]->Enter(10, 1);
		}
		pTitle->Enter(10, 1);
	}

	void SetSelectedIndex(int index)
	{
		selectIndex = index;
		if (!sTitle[selectIndex]->enabled)
		{
			if (input->KeyPress(DIK_DOWNARROW))
				SetSelectedIndex((selectIndex + 1) % 4);
			else if (input->KeyPress(DIK_UPARROW))
				SetSelectedIndex((selectIndex + 3) % 4);
			return;
		}
		for (int i = 0; i < 4; i++)
		{
			sTitle[i]->selected = i == index;
			if (i == index)
				sTitle[i]->Shake(1.5f / sTitle[i]->transform->scale.x);
		}
	}

	void ShowReally()
	{
		really = scene->AttachActor(new Really);
		really->menu = this;
		figure->Move({ -20, -100 }, 2000, 2000);
		figure->isInSubSelection = true;
	}

	void CancelReally()
	{
		focused = true;
		figure->Move({ -260, -100 }, 2000, 2000);
		figure->isInSubSelection = false;
		if (result)
		{
			switch (selectIndex)
			{
			case 1:
			{
				focused = false;
				scene->AttachActor(new ChangeLevel(LEVEL_TITLE, true));
			}
			break;
			case 3:
			{
				focused = false;
				scene->AttachActor(new ChangeLevel(GetStageLevel((practiceMode || isReplaying) ? nowStage : 0), true));
			}
			break;
			}
		}
	}

	void LeaveForContinue()
	{
		blurBackGround->Leave();
		figure->Leave(20);
		pTitle->Leave(20);
		for (int i = 0; i < 4; i++)
			sTitle[i]->Leave(20);
		Destroy();
		auto g = new DelayInvoke(Delegate([=]()->void {timer->SetTimeStopLevel(0); ContinueBgm(); }), 20);
		g->SetUpdateLevel(1);
		scene->AttachActor(g);
	}
};

class GameOverMenu :public PauseMenu
{
	class SecondaryImpl
	{
	public:

		class Retry :public SecondaryTitle
		{
		public:

			Retry()
			{
				transform->location = { -285,-85,0 };
				transform->scale = { 0.6f * 118.0f,0.6f * 61.0f,1 };
				sr->SetSourceRect({ 232 / 512.0f,432 / 704.0f,118 / 512.0f,61 / 704.0f });
				Move({ -265,-85 }, 100, 50);
			}
		};

		class ReturnTitle :public SecondaryTitle
		{
		public:

			ReturnTitle()
			{
				transform->location = { -285,-120,0 };
				transform->scale = { 0.6f * 246.0f,0.6f * 61.0f,1 };
				sr->SetSourceRect({ 0,137 / 704.0f,246 / 512.0f,61 / 704.0f });
				Move({ -265,-120 }, 100, 50);
			}
		};

		class SaveAndQuit :public SecondaryTitle
		{
		public:

			SaveAndQuit()
			{
				if (!allowSaveReplay)
					enabled = false;
				transform->location = { -285,-155,0 };
				transform->scale = { 0.6f * 363.0f,0.6f * 61.0f,1 };
				sr->SetSourceRect({ 0,255 / 704.0f,363 / 512.0f,61 / 704.0f });
				Move({ -265,-155 }, 100, 50);
			}
		};

		class RestartGame :public SecondaryTitle
		{
		public:

			RestartGame()
			{
				transform->location = { -285,-190,0 };
				transform->scale = { 0.6f * 203.0f,0.6f * 61.0f,1 };
				sr->SetSourceRect({ 0,196 / 704.0f,203 / 512.0f,61 / 704.0f });
				Move({ -265,-190 }, 100, 50);
			}
		};

		class Really :public SecondaryTitle
		{
		public:

			Really()
			{
				transform->location = { -50,-80,0 };
				transform->scale = { 0.6f * 240.0f,0.6f * 61.0f,1 };
				sr->SetSourceRect({ 0,314 / 704.0f,240 / 512.0f,61 / 704.0f });
			}

			void OnUpdate() override {}
		};

		class YesYes :public SecondaryTitle
		{
		public:

			YesYes()
			{
				transform->location = { -25,-120,0 };
				transform->scale = { 0.6f * 107.0f,0.6f * 61.0f,1 };
				sr->SetSourceRect({ 205 / 512.0f,196 / 704.0f,107 / 512.0f,61 / 704.0f });
			}
		};

		class NoNo :public SecondaryTitle
		{
		public:

			NoNo()
			{
				transform->location = { -25,-155,0 };
				transform->scale = { 0.6f * 86.0f,0.6f * 61.0f,1 };
				sr->SetSourceRect({ 342 / 512.0f,196 / 704.0f,86 / 512.0f,61 / 704.0f });
			}
		};
	};

	class Really :public GameObject
	{
	public:

		ComPtr<GameOverMenu> menu;

		ComPtr<SecondaryTitle> sTitle[3];

		int frame;

		int selectIndex;

		bool focused;

		Really() :frame(0), selectIndex(1), focused(true)
		{
			SetUpdateLevel(1);
		}

		void OnCreated() override
		{
			InsertBefore(GameSceneCom::pauseLocator.Get());
			sTitle[0] = scene->AttachActor(new SecondaryImpl::Really);
			sTitle[1] = scene->AttachActor(new SecondaryImpl::YesYes);
			sTitle[2] = scene->AttachActor(new SecondaryImpl::NoNo);
			SetSelectedIndex(selectIndex);
		}

		void Leave()
		{
			menu->CancelReally();
			for (int i = 0; i < 3; i++)
				sTitle[i]->Leave(10);
			Destroy();
		}

		void OnUpdate() override
		{
			frame++;
			if (frame > 10 && focused)
			{
				if (input->KeyPress(DIK_DOWNARROW))
				{
					se_select00.Play();
					SetSelectedIndex(selectIndex == 1 ? 2 : 1);
				}
				else if (input->KeyPress(DIK_UPARROW))
				{
					se_select00.Play();
					SetSelectedIndex(selectIndex == 1 ? 2 : 1);
				}
				else if (input->KeyPress(DIK_Z))
				{
					switch (selectIndex)
					{
					case 1:
					{
						focused = false;
						sTitle[1]->GListen(false);
						se_ok00.Play();
						menu->result = true;
						auto g = new DelayInvoke(Delegate([=]()->void {Leave(); }), 20);
						g->SetUpdateLevel(1);
						scene->AttachActor(g);
					}
					break;
					case 2:
					{
						focused = false;
						sTitle[2]->GListen(false);
						se_ok00.Play();
						menu->result = false;
						auto g = new DelayInvoke(Delegate([=]()->void {Leave(); }), 20);
						g->SetUpdateLevel(1);
						scene->AttachActor(g);
					}
					break;
					}
				}
				else if (input->KeyPress(DIK_X))
				{
					se_cancel00.Play();
					menu->result = false;
					Leave();
				}
			}
		}

		void OnDestroy() override
		{
			menu.Reset();
			for (int i = 0; i < 3; i++)
				sTitle[i].Reset();
		}

		void SetSelectedIndex(int index)
		{
			selectIndex = index;
			for (int i = 0; i < 3; i++)
			{
				sTitle[i]->selected = i == index;
				if (i == index)
					sTitle[i]->Shake(1.5f / sTitle[i]->transform->scale.x);
			}
		}
	};


	class KeyboardMenu :public GameObject
	{
		class BlackCanvas :public AnimObject
		{
		public:

			BlackCanvas()
			{
				transform->scale = { 640,480,1 };
				SetUpdateLevel(1);
				leaveEnter->SetUpdateLevel(1);
				sr->SetTexture(src_dummy.Get());
				mat.diffuse = { 0,0,0,0 };
				sr->SetMaterial(mat);
				Enter(15, 1);
			}
		};

	public:

		ComPtr<SpriteTextTitle> keys[7][13];

		ComPtr<GameOverMenu> menu;

		ComPtr<SpriteTextTitle> textRef;

		ComPtr<SpriteTextTitle> editor;

		int selectRow;

		int selectCol;

		char nowName[9];

		int rpyIndex;

		int wordNum;

		Date time;

		int lpFrame;

		int rpFrame;

		int upFrame;

		int dpFrame;

		bool focused;

		KeyboardMenu() :selectRow(0), selectCol(0), rpyIndex(0), wordNum(0), time(SyncDate()),
			lpFrame(0), rpFrame(0), upFrame(0), dpFrame(0), focused(true)
		{
			memset(nowName, 0, sizeof(nowName));
			strcpy(nowName, lastSaveName.c_str());
			for (int i = 0; i < 9; i++)
			{
				if (nowName[i])
					wordNum++;
			}
			SetUpdateLevel(1);
		}

		void OnCreated() override
		{
			InsertBefore(GameSceneCom::pauseLocator.Get());
			char w[2];
			w[1] = 0;
			for (int r = 0; r < 7; r++)
				for (int c = 0; c < 13; c++)
				{
					keys[r][c] = scene->AttachActor(new SpriteTextTitle);
					keys[r][c]->transform->location = { float(-135 + 22 * c),float(-60 - 22 * r),0 };
					w[0] = charArray[r][c];
					keys[r][c]->sr->enabled = false;
					keys[r][c]->sr->SetText(w);
				}
			SetSelectedCol(0);
			ResetText();

			editor = scene->AttachActor(new SpriteTextTitle);
			editor->sr->SetText("_");
			int x = wordNum + 1;
			if (x < 1)
				x = 1;
			if (x > 8)
				x = 8;
			editor->transform->location = { (float)(-145 + 10.8f * x),textRef->transform->location.y - 3,0 };
		}

		void OnUpdate() override
		{
			if (focused)
			{
				if (editor)
				{
					int x = wordNum + 1;
					if (x < 1)
						x = 1;
					if (x > 8)
						x = 8;
					editor->transform->location = { (float)(-145 + 10.8f * x),textRef->transform->location.y - 3,0 };
				}
				if (input->KeyPress(DIK_LEFT))
				{
					se_select00.Play();
					SetSelectedCol((selectCol + 12) % 13);
				}
				else if (input->KeyPress(DIK_RIGHT))
				{
					se_select00.Play();
					SetSelectedCol((selectCol + 1) % 13);
				}
				else if (input->KeyPress(DIK_UP))
				{
					se_select00.Play();
					SetSelectedRow((selectRow + 6) % 7);
				}
				else if (input->KeyPress(DIK_DOWN))
				{
					se_select00.Play();
					SetSelectedRow((selectRow + 1) % 7);
				}
				else if (input->KeyPress(DIK_X))
				{
					se_cancel00.Play();
					if (wordNum != 0)
						EnterWord(127);
					else
						LeaveForSaveMenu();
				}
				else if (input->KeyPress(DIK_Z))
				{
					se_ok00.Play();
					EnterWord(charArray[selectRow][selectCol]);
				}
				else
				{
					if (input->KeyDown(DIK_DOWN))
					{
						dpFrame++;
						if (dpFrame == 8)
						{
							se_select00.Play();
							SetSelectedRow((selectRow + 1) % 7);
							dpFrame = 0;
						}
					}
					else
					{
						dpFrame = 0;
					}
					if (input->KeyDown(DIK_UP))
					{
						upFrame++;
						if (upFrame == 8)
						{
							se_select00.Play();
							SetSelectedRow((selectRow + 6) % 7);
							upFrame = 0;
						}
					}
					else
					{
						upFrame = 0;
					}
					if (input->KeyDown(DIK_LEFT))
					{
						lpFrame++;
						if (lpFrame == 8)
						{
							se_select00.Play();
							SetSelectedCol((selectCol + 12) % 13);
							lpFrame = 0;
						}
					}
					else
					{
						lpFrame = 0;
					}
					if (input->KeyDown(DIK_RIGHT))
					{
						rpFrame++;
						if (rpFrame == 8)
						{
							se_select00.Play();
							SetSelectedCol((selectCol + 1) % 13);
							rpFrame = 0;
						}
					}
					else
					{
						rpFrame = 0;
					}
				}
			}
		}

		void OnDestroy() override
		{
			for (int r = 0; r < 7; r++)
				for (int c = 0; c < 13; c++)
				{
					keys[r][c].Reset();
				}
			menu.Reset();
			textRef.Reset();
			editor.Reset();
		}

	private:

		void ResetText()
		{
			char t[128];
			const char* d = 0;
			switch (difficulty)
			{
			case 0:
				d = "E";
				break;
			case 1:
				d = "N";
				break;
			case 2:
				d = "H";
				break;
			case 3:
				d = "L";
				break;
			}
			sprintf(t, "No.%02d %-8s %02d/%02d/%02d %-6s %-2s St.%d", rpyIndex, nowName, time.year % 100,
				time.month, time.day, playerIndex == 0 ? "Reimu" : "Marisa", d, nowStage + 1);
			textRef->sr->SetText(t);
		}

		void SetSelectedRow(int row)
		{
			selectRow = row;
			for (int r = 0; r < 7; r++)
				for (int c = 0; c < 13; c++)
				{
					if (keys[r][c])
					{
						if (r == selectRow && c == selectCol)
							keys[r][c]->sr->highlight = true;
						else
							keys[r][c]->sr->highlight = false;
					}
				}
		}

		void SetSelectedCol(int col)
		{
			selectCol = col;
			for (int r = 0; r < 7; r++)
			{
				for (int c = 0; c < 13; c++)
				{
					if (keys[r][c])
					{
						if (r == selectRow && c == selectCol)
							keys[r][c]->sr->highlight = true;
						else
							keys[r][c]->sr->highlight = false;
					}
				}
			}
		}

		void EnterWord(unsigned char c)
		{
			if (c != 127 && c != 128)
			{
				if (wordNum < 8)
				{
					nowName[wordNum++] = c;
					ResetText();
				}
				if (wordNum == 8)
				{
					SetSelectedRow(6);
					SetSelectedCol(12);
				}
			}
			else if (c == 127)
			{
				if (wordNum > 0)
				{
					nowName[--wordNum] = 0;
					ResetText();
				}
			}
			else
			{
				focused = false;
				bgm_gameover->Stop();
				se_extend.Play();
				lastSaveName = nowName;
				StopRecording();
				char filename[128];
				sprintf(filename, "save_%d.rpy", rpyIndex);
				SaveReplay(filename, nowName);
				scene->Invoke(Delegate([=]()->void {
					replayList[rpyIndex].LoadFromFile(filename);
					}));
				scene->AttachActor(new BlackCanvas);
				scene->AttachActor(new ChangeLevel(LEVEL_TITLE, true));
			}
		}

		Date SyncDate()
		{
			time_t timep;
			struct tm* p;
			::time(&timep);
			p = gmtime(&timep);

			Date time;
			time.year = 1900 + p->tm_year;
			time.month = 1 + p->tm_mon;
			time.day = p->tm_mday;
			return time;
		}

		void LeaveForSaveMenu();
	};

	class SaveMenu :public GameObject
	{
	public:

		ComPtr<SpriteTextTitle> sTitle[25];

		ComPtr<GameOverMenu> menu;

		int downPressFrame;

		int upPressFrame;

		SaveMenu() :selectIndex(0), downPressFrame(0), upPressFrame(0)
		{
			SetUpdateLevel(1);
		}

		void OnCreated() override
		{
			InsertBefore(GameSceneCom::pauseLocator.Get());
			for (int i = 0; i < 25; i++)
			{
				sTitle[i] = scene->AttachActor(new SpriteTextTitle);
				sTitle[i]->sr->SetText(GetReplayText(i + 1));
				sTitle[i]->transform->location = { -200,200,0 };
				sTitle[i]->Move({ -200,(float)(200 - 17 * i) }, 1600, 800);
				if (replayList[i + 1].GetPageAmount() == 0)
					sTitle[i]->sr->enabled = false;
				else
					sTitle[i]->sr->enabled = true;
			}
			SetSelectedIndex(0);
		}

		void OnUpdate() override
		{
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
				LeaveForMenu();
			}
			else if (input->KeyPress(DIK_Z))
			{
				se_ok00.Play();
				LeaveForKeyBoard();
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

		void OnDestroy() override
		{
			for (int i = 0; i < 25; i++)
				sTitle[i].Reset();
			menu.Reset();
		}

	private:

		int selectIndex;

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
					d = "E";
					break;
				case 1:
					d = "N";
					break;
				case 2:
					d = "H";
					break;
				case 3:
					d = "L";
					break;
				}
				sprintf(t, "No.%02d %-8s %02d/%02d/%02d %-6s %-2s St.%d", index, replayList[index].GetPlayerName().c_str(), header.time.year % 100,
					header.time.month, header.time.day, header.playerIndex == 0 ? "Reimu" : "Marisa", d, replayList[index].GetPageAmount());
			}
			else
			{
				sprintf(t, "No.%02d -------- --/--/-- ------ -- St.-", index);
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

		void LeaveForMenu()
		{
			for (int i = 0; i < 25; i++)
			{
				if (sTitle[i])
					sTitle[i]->Destroy();
			}
			menu->focused = true;
			menu->Show();
			Destroy();
		}

		void LeaveForKeyBoard()
		{
			sTitle[selectIndex]->Move({ -200 ,10 }, 1000, 500);
			sTitle[selectIndex]->sr->enabled = true;
			sTitle[selectIndex]->sr->highlight = false;
			for (int i = 0; i < 25; i++)
			{
				if (i != selectIndex)
				{
					sTitle[i]->Destroy();
				}
			}
			auto g = new KeyboardMenu;
			g->rpyIndex = selectIndex + 1;
			g->menu = menu;
			g->textRef = sTitle[selectIndex];
			scene->AttachActor(g);
			Destroy();
		}
	};

public:

	ComPtr<BlurBackGround> blurBackGround;

	ComPtr<Figure> figure;

	ComPtr<PrimaryTitle> pTitle;

	ComPtr<SecondaryTitle> sTitle[4];

	ComPtr<Really> really;

	int frame;

	bool focused;

	int selectIndex;

	bool result;

	GameOverMenu() :frame(0), focused(true), selectIndex(0), result(false)
	{
		SetUpdateLevel(1);
	}

	void OnCreated() override
	{
		timer->SetPresentCount(1);
		bgm_gameover->Play();
		InsertBefore(GameSceneCom::pauseLocator.Get());
		blurBackGround = scene->AttachActor(new BlurBackGround);
		figure = scene->AttachActor(new Figure);
		pTitle = scene->AttachActor(new PrimaryTitle);
		pTitle->sr->SetSourceRect({ 1 / 3.0f,0,1 / 3.0f,1 / 9.0f });
		sTitle[0] = new SecondaryImpl::Retry;
		sTitle[1] = new SecondaryImpl::ReturnTitle;
		sTitle[2] = new SecondaryImpl::SaveAndQuit;
		sTitle[3] = new SecondaryImpl::RestartGame;
		for (int i = 0; i < 4; i++)
		{
			auto g = new DelayCreate(sTitle[i].Get(), i * 6);
			g->SetUpdateLevel(1);
			scene->AttachActor(g);
		}
		SetSelectedIndex(0);
	}

	void OnUpdate() override
	{
		frame++;
		if (frame > 25 && focused)
		{
			if (input->KeyPress(DIK_DOWNARROW))
			{
				se_select00.Play();
				SetSelectedIndex((selectIndex + 1) % 4);
			}
			else if (input->KeyPress(DIK_UPARROW))
			{
				se_select00.Play();
				SetSelectedIndex((selectIndex + 3) % 4);
			}
			else if (input->KeyPress(DIK_Z))
			{
				switch (selectIndex)
				{
				case 0:
				{
					focused = false;
					allowSaveReplay = false;
					retryTimes++;
					se_ok00.Play();
					sTitle[0]->GListen(false);
					auto g = new DelayInvoke(Delegate([=]()->void {
						ResetGameData();
						playerData.power = 400;
						LeaveForContinue(); 
						}), 20);
					g->SetUpdateLevel(1);
					scene->AttachActor(g);
				}
				break;
				case 1:
				{
					focused = false;
					se_ok00.Play();
					sTitle[1]->GListen(false);
					auto g = new DelayInvoke(Delegate([=]()->void {ShowReally(); }), 20);
					g->SetUpdateLevel(1);
					scene->AttachActor(g);
				}
				break;
				case 2:
				{
					focused = false;
					se_ok00.Play();
					Hide();
					auto g = new SaveMenu;
					g->menu = this;
					scene->AttachActor(g);
				}
				break;
				case 3:
				{
					focused = false;
					se_ok00.Play();
					sTitle[3]->GListen(false);
					auto g = new DelayInvoke(Delegate([=]()->void {ShowReally(); }), 20);
					g->SetUpdateLevel(1);
					scene->AttachActor(g);
				}
				break;
				}
			}
		}
	}

	void OnDestroy() override
	{
		blurBackGround.Reset();
		figure.Reset();
		pTitle.Reset();
		for (int i = 0; i < 4; i++)
			sTitle[i].Reset();
		really.Reset();
	}

	void SetSelectedIndex(int index)
	{
		selectIndex = index;
		if (!sTitle[selectIndex]->enabled)
		{
			if (input->KeyPress(DIK_DOWNARROW))
				SetSelectedIndex((selectIndex + 1) % 4);
			else if (input->KeyPress(DIK_UPARROW))
				SetSelectedIndex((selectIndex + 3) % 4);
			return;
		}
		for (int i = 0; i < 4; i++)
		{
			sTitle[i]->selected = i == index;
			if (i == index)
				sTitle[i]->Shake(1.5f / sTitle[i]->transform->scale.x);
		}
	}

	void Hide()
	{
		figure->Enter(1, 0);
		for (int i = 0; i < 4; i++)
		{
			if (sTitle[i])
				sTitle[i]->Enter(1, 0);
		}
		pTitle->Enter(1, 0);
	}

	void Show()
	{
		figure->Enter(10, 1);
		for (int i = 0; i < 4; i++)
		{
			if (sTitle[i])
				sTitle[i]->Enter(10, 1);
		}
		pTitle->Enter(10, 1);
	}

	void ShowReally()
	{
		really = scene->AttachActor(new Really);
		really->menu = this;
		figure->Move({ -20, -100 }, 2000, 2000);
		figure->isInSubSelection = true;
	}

	void CancelReally()
	{
		focused = true;
		figure->Move({ -260, -100 }, 2000, 2000);
		figure->isInSubSelection = false;
		if (result)
		{
			switch (selectIndex)
			{
			case 1:
			{
				focused = false;
				bgm_gameover->Stop();
				scene->AttachActor(new ChangeLevel(LEVEL_TITLE, true));
			}
			break;
			case 3:
			{
				focused = false;
				bgm_gameover->Stop();
				scene->AttachActor(new ChangeLevel(GetStageLevel(practiceMode ? nowStage : 0), true));
			}
			break;
			}
		}
	}

	void LeaveForContinue()
	{
		bgm_gameover->Stop();
		blurBackGround->Leave();
		figure->Leave(20);
		pTitle->Leave(20);
		for (int i = 0; i < 4; i++)
			sTitle[i]->Leave(20);
		Destroy();
		auto g = new DelayInvoke(Delegate([=]()->void {timer->SetTimeStopLevel(0); ContinueBgm(); }), 20);
		g->SetUpdateLevel(1);
		scene->AttachActor(g);
	}
};

class GamePauseListener :public GameObject
{
public:

	GamePauseListener() :frame(0)
	{
	}

	void OnUpdate() override
	{
		frame++;
		if (isReplaying && input->KeyDown(DIK_LCONTROL))
		{
			timer->SetUpdateCount(5);
		}
		else
		{
			timer->SetUpdateCount(1);
		}
		if (frame > 50)
		{
			if (input->KeyPress(DIK_ESCAPE) && canPause)
			{
				PauseBgm();
				se_pause.Play();
				timer->SetTimeStopLevel(1);
				timer->SetUpdateCount(1);
				scene->AttachActor(new GamePauseMenu);
			}
		}
	}

private:

	int frame;

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::pauseLocator.Get());
	}

	void OnDestroy() override
	{
		timer->SetTimeStopLevel(0);
		timer->SetUpdateCount(1);
	}
};

class ReplayOverListener :public GameObject
{
public:

	ReplayOverListener() :frame(0)
	{
	}

	void OnUpdate() override
	{
		frame++;
		if (frame > 50)
		{
			if (replayOver)
			{
				PauseBgm();
				se_pause.Play();
				timer->SetTimeStopLevel(1);
				timer->SetUpdateCount(1);
				scene->AttachActor(new ReplayOverMenu);
			}
		}
	}

private:

	int frame;

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::pauseLocator.Get());
	}

	void OnDestroy() override
	{
		timer->SetTimeStopLevel(0);
		timer->SetUpdateCount(1);
	}
};

class LevelCleaner :public GameObject
{
public:

	LevelCleaner(LevelType _nextLevel) :
		frame(0), sr(AddComponent(new SpriteRender)), nextLevel(_nextLevel),
		isResourcesLoaded(false), levelLoaded(false)
	{
		transform->scale = { 800,600,1 };
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 0,0,0,0 };
		mat.specular = { 0,0,0,0 };
		mat.power = 0;
		sr->SetMaterial(mat);
		sr->SetTexture(src_dummy.Get());
	}

	void OnUpdate() override
	{
		frame++;
		if (frame <= 120)
		{
			if (nowBgm)
				nowBgm->SetVolume(GetBgmVolume() * (1 - frame / 120.0f));
		}
		if (frame == 120)
		{
			StopBgm();
		}
		if (!isResourcesLoaded)
		{
			if (frame <= 50)
			{
				mat.diffuse.a += 0.02f;
				sr->SetMaterial(mat);
			}
			if (frame > 50 && frame <= 60)
			{
				for (LPACTOR g = GameSceneCom::locator[0]; g != GameSceneCom::locator[2]; g = g->GetNext())
				{
					if (g->GetName() == "BackGround")
						g->Destroy();
				}
			}
			if (frame == 90)
			{
				thread([=]()->void {
					LoadLevelResource(nextLevel, true);
					isResourcesLoaded = true;
					}).detach();
			}
		}
		else if (frame > 150)
		{
			if (!levelLoaded)
			{
				if (tag)
					tag->Leave();
				LoadLevel(scene.Get(), nextLevel);
				levelLoaded = true;
			}
		}
		if (levelLoaded)
		{
			mat.diffuse.a -= 0.03;
			sr->SetMaterial(mat);
			if (mat.diffuse.a <= 0)
				Destroy();
		}
	}

	void OnCreated() override
	{
		StopRecording();
		InsertBefore(GameSceneCom::locator[3].Get());
		tag = scene->AttachActor(new LoadingTag);
		tag->InsertBefore(GameSceneCom::pauseLocator.Get());
	}

	void OnDestroy() override
	{
		tag.Reset();
	}

private:

	SpriteRender* sr;

	Material mat;

	int frame;

	bool isResourcesLoaded;

	bool levelLoaded;

	LevelType nextLevel;

	ComPtr<LoadingTag> tag;
};

class Victory :public PauseMenu
{
	class KeyboardMenu :public GameObject
	{
		class BlackCanvas :public AnimObject
		{
		public:

			BlackCanvas()
			{
				transform->scale = { 640,480,1 };
				SetUpdateLevel(1);
				leaveEnter->SetUpdateLevel(1);
				sr->SetTexture(src_dummy.Get());
				mat.diffuse = { 0,0,0,0 };
				sr->SetMaterial(mat);
				Enter(15, 1);
			}
		};

	public:

		ComPtr<SpriteTextTitle> keys[7][13];

		ComPtr<SpriteTextTitle> textRef;

		ComPtr<SpriteTextTitle> editor;

		int selectRow;

		int selectCol;

		char nowName[9];

		int rpyIndex;

		int wordNum;

		Date time;

		int lpFrame;

		int rpFrame;

		int upFrame;

		int dpFrame;

		bool focused;

		KeyboardMenu() :selectRow(0), selectCol(0), rpyIndex(0), wordNum(0), time(SyncDate()),
			lpFrame(0), rpFrame(0), upFrame(0), dpFrame(0), focused(true)
		{
			memset(nowName, 0, sizeof(nowName));
			strcpy(nowName, lastSaveName.c_str());
			for (int i = 0; i < 9; i++)
			{
				if (nowName[i])
					wordNum++;
			}
			SetUpdateLevel(1);
		}

		void OnCreated() override
		{
			InsertBefore(GameSceneCom::pauseLocator.Get());
			char w[2];
			w[1] = 0;
			for (int r = 0; r < 7; r++)
				for (int c = 0; c < 13; c++)
				{
					keys[r][c] = scene->AttachActor(new SpriteTextTitle);
					keys[r][c]->transform->location = { float(-135 + 22 * c),float(-60 - 22 * r),0 };
					w[0] = charArray[r][c];
					keys[r][c]->sr->enabled = false;
					keys[r][c]->sr->SetText(w);
				}
			SetSelectedCol(0);
			ResetText();

			editor = scene->AttachActor(new SpriteTextTitle);
			editor->sr->SetText("_");
			int x = wordNum + 1;
			if (x < 1)
				x = 1;
			if (x > 8)
				x = 8;
			editor->transform->location = { (float)(-145 + 10.8f * x),textRef->transform->location.y - 3,0 };
		}

		void OnUpdate() override
		{
			if (focused)
			{
				if (editor)
				{
					int x = wordNum + 1;
					if (x < 1)
						x = 1;
					if (x > 8)
						x = 8;
					editor->transform->location = { (float)(-145 + 10.8f * x),textRef->transform->location.y - 3,0 };
				}
				if (input->KeyPress(DIK_LEFT))
				{
					se_select00.Play();
					SetSelectedCol((selectCol + 12) % 13);
				}
				else if (input->KeyPress(DIK_RIGHT))
				{
					se_select00.Play();
					SetSelectedCol((selectCol + 1) % 13);
				}
				else if (input->KeyPress(DIK_UP))
				{
					se_select00.Play();
					SetSelectedRow((selectRow + 6) % 7);
				}
				else if (input->KeyPress(DIK_DOWN))
				{
					se_select00.Play();
					SetSelectedRow((selectRow + 1) % 7);
				}
				else if (input->KeyPress(DIK_X))
				{
					se_cancel00.Play();
					if (wordNum != 0)
						EnterWord(127);
					else
						LeaveForSaveMenu();
				}
				else if (input->KeyPress(DIK_Z))
				{
					se_ok00.Play();
					EnterWord(charArray[selectRow][selectCol]);
				}
				else
				{
					if (input->KeyDown(DIK_DOWN))
					{
						dpFrame++;
						if (dpFrame == 8)
						{
							se_select00.Play();
							SetSelectedRow((selectRow + 1) % 7);
							dpFrame = 0;
						}
					}
					else
					{
						dpFrame = 0;
					}
					if (input->KeyDown(DIK_UP))
					{
						upFrame++;
						if (upFrame == 8)
						{
							se_select00.Play();
							SetSelectedRow((selectRow + 6) % 7);
							upFrame = 0;
						}
					}
					else
					{
						upFrame = 0;
					}
					if (input->KeyDown(DIK_LEFT))
					{
						lpFrame++;
						if (lpFrame == 8)
						{
							se_select00.Play();
							SetSelectedCol((selectCol + 12) % 13);
							lpFrame = 0;
						}
					}
					else
					{
						lpFrame = 0;
					}
					if (input->KeyDown(DIK_RIGHT))
					{
						rpFrame++;
						if (rpFrame == 8)
						{
							se_select00.Play();
							SetSelectedCol((selectCol + 1) % 13);
							rpFrame = 0;
						}
					}
					else
					{
						rpFrame = 0;
					}
				}
			}
		}

		void OnDestroy() override
		{
			for (int r = 0; r < 7; r++)
				for (int c = 0; c < 13; c++)
				{
					keys[r][c].Reset();
				}
			textRef.Reset();
			editor.Reset();
		}

	private:

		void ResetText()
		{
			char t[128];
			const char* d = 0;
			switch (difficulty)
			{
			case 0:
				d = "E";
				break;
			case 1:
				d = "N";
				break;
			case 2:
				d = "H";
				break;
			case 3:
				d = "L";
				break;
			}
			sprintf(t, "No.%02d %-8s %02d/%02d/%02d %-6s %-2s St.%d", rpyIndex, nowName, time.year % 100,
				time.month, time.day, playerIndex == 0 ? "Reimu" : "Marisa", d, nowStage + 1);
			textRef->sr->SetText(t);
		}

		void SetSelectedRow(int row)
		{
			selectRow = row;
			for (int r = 0; r < 7; r++)
				for (int c = 0; c < 13; c++)
				{
					if (keys[r][c])
					{
						if (r == selectRow && c == selectCol)
							keys[r][c]->sr->highlight = true;
						else
							keys[r][c]->sr->highlight = false;
					}
				}
		}

		void SetSelectedCol(int col)
		{
			selectCol = col;
			for (int r = 0; r < 7; r++)
			{
				for (int c = 0; c < 13; c++)
				{
					if (keys[r][c])
					{
						if (r == selectRow && c == selectCol)
							keys[r][c]->sr->highlight = true;
						else
							keys[r][c]->sr->highlight = false;
					}
				}
			}
		}

		void EnterWord(unsigned char c)
		{
			if (c != 127 && c != 128)
			{
				if (wordNum < 8)
				{
					nowName[wordNum++] = c;
					ResetText();
				}
				if (wordNum == 8)
				{
					SetSelectedRow(6);
					SetSelectedCol(12);
				}
			}
			else if (c == 127)
			{
				if (wordNum > 0)
				{
					nowName[--wordNum] = 0;
					ResetText();
				}
			}
			else
			{
				focused = false;
				bgm_gameover->Stop();
				se_extend.Play();
				lastSaveName = nowName;
				StopRecording();
				char filename[128];
				sprintf(filename, "save_%d.rpy", rpyIndex);
				SaveReplay(filename, nowName);
				scene->Invoke(Delegate([=]()->void {
					replayList[rpyIndex].LoadFromFile(filename);
					}));
				scene->AttachActor(new BlackCanvas);
				scene->AttachActor(new ChangeLevel(LEVEL_TITLE, true));
			}
		}

		Date SyncDate()
		{
			time_t timep;
			struct tm* p;
			::time(&timep);
			p = gmtime(&timep);

			Date time;
			time.year = 1900 + p->tm_year;
			time.month = 1 + p->tm_mon;
			time.day = p->tm_mday;
			return time;
		}

		void LeaveForSaveMenu();
	};

	class SaveMenu :public GameObject
	{
	public:

		ComPtr<SpriteTextTitle> sTitle[25];

		int downPressFrame;

		int upPressFrame;

		SaveMenu() :selectIndex(0), downPressFrame(0), upPressFrame(0)
		{
			SetUpdateLevel(1);
		}

		void OnCreated() override
		{
			InsertBefore(GameSceneCom::pauseLocator.Get());
			for (int i = 0; i < 25; i++)
			{
				sTitle[i] = scene->AttachActor(new SpriteTextTitle);
				sTitle[i]->sr->SetText(GetReplayText(i + 1));
				sTitle[i]->transform->location = { -200,200,0 };
				sTitle[i]->Move({ -200,(float)(200 - 17 * i) }, 1600, 800);
				if (replayList[i + 1].GetPageAmount() == 0)
					sTitle[i]->sr->enabled = false;
				else
					sTitle[i]->sr->enabled = true;
			}
			SetSelectedIndex(0);
		}

		void OnUpdate() override
		{
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
			else if (input->KeyPress(DIK_Z))
			{
				se_ok00.Play();
				LeaveForKeyBoard();
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

		void OnDestroy() override
		{
			for (int i = 0; i < 25; i++)
				sTitle[i].Reset();
		}

	private:

		int selectIndex;

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
					d = "E";
					break;
				case 1:
					d = "N";
					break;
				case 2:
					d = "H";
					break;
				case 3:
					d = "L";
					break;
				}
				sprintf(t, "No.%02d %-8s %02d/%02d/%02d %-6s %-2s St.%d", index, replayList[index].GetPlayerName().c_str(), header.time.year % 100,
					header.time.month, header.time.day, header.playerIndex == 0 ? "Reimu" : "Marisa", d, replayList[index].GetPageAmount());
			}
			else
			{
				sprintf(t, "No.%02d -------- --/--/-- ------ -- St.-", index);
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

		void LeaveForKeyBoard()
		{
			sTitle[selectIndex]->Move({ -200 ,10 }, 1000, 500);
			sTitle[selectIndex]->sr->enabled = true;
			sTitle[selectIndex]->sr->highlight = false;
			for (int i = 0; i < 25; i++)
			{
				if (i != selectIndex)
				{
					sTitle[i]->Destroy();
				}
			}
			auto g = new KeyboardMenu;
			g->rpyIndex = selectIndex + 1;
			g->textRef = sTitle[selectIndex];
			scene->AttachActor(g);
			Destroy();
		}
	};

	class ThankYouForPlaying :public AnimObject
	{
	public:

		ThankYouForPlaying()
		{
			transform->scale = { 717 * 0.5,327 * 0.1f,1 };
			mat.diffuse.a = 0;
			sr->SetMaterial(mat);
			sr->SetTexture(src_victory.Get());
			leaveEnter->SetUpdateLevel(1);
			sr->SetSourceRect({ 0,0,1,0.2 });
		}
	};

	class Tip :public AnimObject
	{
	public:

		Tip(bool canSave)
		{
			transform->scale = { 717 * 0.5,327 * 0.1f,1 };
			mat.diffuse.a = 0;
			sr->SetMaterial(mat);
			sr->SetTexture(src_victory.Get());
			leaveEnter->SetUpdateLevel(1);
			sr->SetSourceRect({ 0,canSave ? 0.4f : 0.2f,1,0.2 });
		}
	};

	class SubTitle :public AnimObject
	{
	public:

		SubTitle(bool positive)
		{
			transform->scale = { 717 * 0.5,327 * 0.1f,1 };
			mat.diffuse.a = 0;
			sr->SetMaterial(mat);
			sr->SetTexture(src_victory.Get());
			leaveEnter->SetUpdateLevel(1);
			sr->SetSourceRect({ 0,positive ? 0.6f : 0.8f,1,0.2 });
		}
	};

public:

	Victory() :sr(AddComponent(new SpriteRender)), frame(0), focused(true), selectIndex(0)
	{
		transform->location = { 0,0,0 };
		transform->scale = { 640,480,1 };
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 0,0,0,0 };
		mat.specular = { 0,0,0,0 };
		mat.power = 0;
		sr->SetMaterial(mat);
		sr->SetTexture(src_victorybg.Get());
	}

private:

	LPGOBJ locator;

	SpriteRender* sr;

	Material mat;

	int frame;

	ComPtr<ThankYouForPlaying> tyfp;

	ComPtr<Tip> tip;

	ComPtr<SubTitle> titles[2];

	bool focused;

	int selectIndex;

	void OnCreated() override
	{
		StopRecording();
		canPause = false;
		InsertBefore(GameSceneCom::locator[22].Get());
		locator = scene->AttachActor(new GameObject);
		locator->InsertAfter(this);
	}

	void OnUpdate() override
	{
		frame++;
		if (frame <= 120)
		{
			if (nowBgm)
				nowBgm->SetVolume(GetBgmVolume() * (1 - frame / 120.0f));
		}
		if (frame == 120)
		{
			StopBgm();
		}
		if (frame <= 50)
		{
			mat.diffuse.a += 0.02f;
			sr->SetMaterial(mat);
		}
		if (frame == 100)
		{
			timer->SetTimeStopLevel(1);
			SetUpdateLevel(1);
		}
		if (frame > 90 && frame <= 130)
		{
			mat.diffuse.r += 0.025f;
			mat.diffuse.g += 0.025f;
			mat.diffuse.b += 0.025f;
			sr->SetMaterial(mat);
		}
		if (frame == 110)
		{
			tyfp = scene->AttachActor(new ThankYouForPlaying);
			tyfp->InsertBefore(locator.Get());
			tyfp->Enter(20, 1);
		}
		if (frame == 190)
		{
			tyfp->Leave(20);
		}
		if (frame == 200)
		{
			if (allowSaveReplay)
			{
				tip = scene->AttachActor(new Tip(true));
				tip->transform->location = { 0,30,0 };
				tip->InsertBefore(locator.Get());
				tip->Enter(20, 1);

				for (int i = 0; i < 2; i++)
				{
					titles[i] = scene->AttachActor(new SubTitle(i == 0));
					titles[i]->transform->location = { 0,-30.0f * i,0 };
					titles[i]->InsertBefore(locator.Get());
					titles[i]->Enter(20, 1);
				}
				SetSelectedIndex(0);
			}
			else
			{
				tip = scene->AttachActor(new Tip(false));
				tip->transform->location = { 0,0,0 };
				tip->InsertBefore(locator.Get());
				tip->Enter(20, 1);
			}
		}
		if (frame > 200 && focused && allowSaveReplay)
		{
			if (input->KeyPress(DIK_DOWN) || input->KeyPress(DIK_UP))
			{
				se_select00.Play();
				SetSelectedIndex((selectIndex + 1) % 2);
			}
			else if (input->KeyPress(DIK_Z))
			{
				if (selectIndex == 0)
				{
					focused = false;
					se_ok00.Play();
					scene->AttachActor(new SaveMenu);
					tip->Destroy();
					for (int i = 0; i < 2; i++)
						titles[i]->Destroy();
				}
				else
				{
					se_ok00.Play();
					frame = 279;
					allowSaveReplay = false;
				}
			}
		}
		if (frame == 280 && !allowSaveReplay)
		{
			tip->Leave(20);
			for (int i = 0; i < 2; i++)
			{
				if (titles[i])
					titles[i]->Leave(20);
			}
		}
		if (frame > 280 && frame <= 330 && !allowSaveReplay)
		{
			mat.diffuse.r -= 0.02f;
			mat.diffuse.g -= 0.02f;
			mat.diffuse.b -= 0.02f;
			sr->SetMaterial(mat);
		}
		if (frame == 350 && !allowSaveReplay)
		{
			scene->AttachActor(new ChangeLevel(LEVEL_TITLE));
		}
	}

	void OnDestroy() override
	{
		locator.Reset();
		tyfp.Reset();
		tip.Reset();
		for (int i = 0; i < 2; i++)
			titles[i].Reset();
	}

	void SetSelectedIndex(int index)
	{
		selectIndex = index;
		for (int i = 0; i < 2; i++)
		{
			if (i == selectIndex)
			{
				titles[i]->mat.diffuse = { 1,1,1,1 };
				titles[i]->sr->SetMaterial(titles[i]->mat);
			}
			else
			{
				titles[i]->mat.diffuse = { 0.5,0.5,0.5,1 };
				titles[i]->sr->SetMaterial(titles[i]->mat);
			}
		}
	}
};

class BgmText :public GameObject
{
public:

	TextRender2D* tr;

	SpriteRender* sr;

	int frame;

	ComPtr<IRenderTarget> target;

	BgmText(const wstring& bgmName) :tr(new TextRender2D(false)), frame(0), sr(new SpriteRender)
	{
		transform->scale = { 640,480,1 };
		transform2D->location = { 320,488 };
		tr->SetOutlineWidth(2);
		tr->SetOutlineEnabled(true);
		tr->SetTextFormat(tf_youyuan.Get());
		tr->SetRect({ -160,0,160,100 });
		tr->SetText(L"BGM:" + bgmName);

		RenderTargetDesc desc;
		desc.width = 640;
		desc.height = 480;
		desc.msaaEnabled = false;
		desc.support2D = true;
		desc.hasDepth = true;
		desc.multiSampleCount = 0;
		engine->CreateRenderTarget(desc, &target);

		AddComponent(new RenderTargetSet::Setter(target.Get(), 0, true));
		AddComponent(tr);
		AddComponent(new RenderTargetSet::Setter(GameSceneCom::gameTarget.Get(), 0, false));

		Material mat;
		mat.diffuse = { 1,1,1,1 };
		mat.ambient = { 0,0,0,0 };
		mat.specular = { 0,0,0,0 };
		mat.power = 0;
		sr->SetMaterial(mat);
		sr->SetTexture(target.Get());
		AddComponent(sr);
	}

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::locator[20].Get());
	}

	void OnUpdate() override
	{
		frame++;
		if (frame < 20)
		{
			transform2D->location.y -= 1.5;
		}
		if (frame > 150)
		{
			transform2D->location.y += 1.5;
		}
		if (frame == 200)
		{
			Destroy();
		}
	}

	void OnDestroy() override
	{
		target.Reset();
	}
};

class BossNameSprite :public GameObject
{
public:

	SpriteRender* sr;

	Material mat;

	int frame;

	BossNameSprite(int index) :sr(AddComponent(new SpriteRender)), frame(0)
	{
		transform->location = { 90,-30,0 };
		transform->scale = { 0.75f * 392,0.75f * 383 / 3.0f,1 };
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,0 };
		mat.specular = { 0,0,0,0 };
		mat.power = 0;
		sr->SetMaterial(mat);
		sr->SetTexture(src_bossname.Get());
		sr->SetSourceRect({ 0,index / 3.0f,1,1 / 3.0f });
	}

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::locator[23].Get());
	}

	void OnUpdate() override
	{
		frame++;
		if (frame < 40)
		{
			mat.diffuse.a += 0.025;
			sr->SetMaterial(mat);
		}
		if (frame > 180)
		{
			mat.diffuse.a -= 0.025;
			sr->SetMaterial(mat);
		}
		if (frame == 220)
		{
			Destroy();
		}
	}
};