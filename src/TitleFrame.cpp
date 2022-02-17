#include "TitleFrame.h"

void Frame_Player_StartGame::LeaveForDifficultyFrame()
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
	dtagAnim->Leave();
	st_player->Leave(20);
	st_player->Move({ -150,300 }, 600, 100);
	auto _parent = parent;
	Destroy();
	scene->AttachActor(new Frame_Difficulty_StartGame)->parent = _parent;
}

void Frame_List_Replay::LeaveForStage()
{
	sTitle[selectIndex]->Move({ -210 ,162 }, 1000, 800);
	sTitle[selectIndex]->sr->enabled = true;
	sTitle[selectIndex]->sr->highlight = false;
	for (int i = 0; i < 25; i++)
	{
		if (i != selectIndex)
		{
			sTitle[i]->Destroy();
		}
	}
	auto g = new Frame_Stage_Replay;
	g->rpyIndex = selectIndex + 1;
	g->parent = parent;
	g->textRef = sTitle[selectIndex];
	scene->AttachActor(g);
	Destroy();
}

void Frame_Stage_Replay::LeaveForList()
{
	scene->AttachActor(new Frame_List_Replay)->parent = parent;
	textRef->Destroy();
	for(int i=0;i<stageNum;i++)
		stageText[i]->Destroy();
	Destroy();
}

void Frame_Stage_Replay::StartReplay()
{
	focused = false;
	se_ok00.Play();
	se_boon00.Play();
	StopBgm();
	auto header = currentReplay.GetHeader();
	playerIndex = header.playerIndex;
	difficulty = header.difficulty;
	scene->AttachActor(new EnterSwitch(GetStageLevel(selectIndex)));
}

void Frame_Stage_StartGame::LeaveForPlayerFrame()
{
	for (int i = 0; i < 3; i++)
	{
		stageText[i]->Leave(15);
	}
	st_stage->Leave(20);
	st_stage->Move({ 0,300 }, 600, 100);
	auto _parent = parent;
	auto _dtagAnim = dtagAnim;
	Destroy();
	auto f = scene->AttachActor(new Frame_Player_StartGame);
	f->parent = _parent;
	f->dtagAnim = _dtagAnim;
	f->dtagAnim->Enter(15, 1);
}