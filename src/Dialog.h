#pragma once
#include "Face.h"
#include "Balloon.h"
#include "GameScene.h"
#include "SoundEffect.h"
#include "GameGlobal.h"

class Dialog:public GameObject
{
public:

	ComPtr<Face> left;

	ComPtr<Face> right;

	ComPtr<Balloon> balloon;

	LPGOBJ bossNameEffect;

	Dialog() :step(-1), coldFrame(30), autoNextFrame(300)
	{
	}

protected:

	int autoNextFrame;

	int coldFrame;

	void OnUpdate() override
	{
		if (coldFrame == 0)
		{
			if (player->GetController()->IsLControlDown())
			{
				coldFrame = 2;
				autoNextFrame = 500;
				se_plst.Play();
				StepChange(++step);
			}
			else if (player->GetController()->IsZPressed())
			{
				coldFrame = 2;
				autoNextFrame = 500;
				se_plst.Play();
				StepChange(++step);
			}
			else if (autoNextFrame == 0)
			{
				coldFrame = 2;
				autoNextFrame = 500;
				se_plst.Play();
				StepChange(++step);
			}
			else
			{
				autoNextFrame--;
			}
		}
		else
		{
			coldFrame--;
		}
	}

	void OnCreated() override
	{
		player->SetPlayerFlags(player->GetPlayerFlags() | PF_PeaceLoving);
		InsertBefore(GameSceneCom::locator[20].Get());
		se_plst.Play();
		StepChange(++step);
	}

	void OnDestroy() override
	{
		if (left)
		{
			left->Deactivate();
			left->Leave();
			left.Reset();
		}
		if (right)
		{
			right->Deactivate();
			right->Leave();
			right.Reset();
		}
		if (balloon)
		{
			balloon->Destroy();
			balloon.Reset();
		}
		if (bossNameEffect)
		{
			bossNameEffect->Destroy();
			bossNameEffect.Reset();
		}
	}

	virtual void SetBalloon(Balloon* _balloon, const wstring& text)
	{
		if (balloon)
			balloon->Destroy();
		scene->AttachActor(_balloon)->SetText(text);
		balloon = _balloon;
	}

	virtual void StepChange(int step) = 0;

private:

	int step;
};

void ShowEndDialog_SunnyMilk();

void ShowEndDialog_Monstone();