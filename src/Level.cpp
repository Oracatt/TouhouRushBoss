#include "Level.h"
#include "LevelResources.h"
#include "BackGround.h"
#include "DialogDeriver.h"
#include "SceneObjects.h"
#include "BackGroundDeriver.h"
#include "TitleObjects.h"
#include "GSObjects.h"
#include "TitleObjects.h"
#include "TitleFrame.h"
#include "Replay.h"

LevelType nowLevel = LEVEL_NONE;

void GrassLand_OnLoad(IScene* scene)
{
	nowStage = 0;
	if (!isReplaying)
		StartRecording();
	else
		LoadReplayPage(nowStage);

	PlayBgm(bgm_gamestart.Get());
	scene->AttachActor(new GrassLand);
	if (playerIndex == 0)
		scene->AttachActor(new DelayCreate(new Dialog_Reimu_SunnyMilk, 60));
	else if (playerIndex == 1)
		scene->AttachActor(new DelayCreate(new Dialog_Marisa_SunnyMilk, 60));
}

void RiverSide_OnLoad(IScene* scene)
{
	nowStage = 1;
	if (!isReplaying)
		StartRecording();
	else
		LoadReplayPage(nowStage);

	PlayBgm(bgm_gamestart.Get());
	scene->AttachActor(new RiverSide);
	if (playerIndex == 0)
		scene->AttachActor(new DelayCreate(new Dialog_Reimu_Monstone, 60));
	else if (playerIndex == 1)
		scene->AttachActor(new DelayCreate(new Dialog_Marisa_Monstone, 60));
}

void FrozenForest_OnLoad(IScene* scene)
{
	nowStage = 2;
	if (!isReplaying)
		StartRecording();
	else
		LoadReplayPage(nowStage);

	PlayBgm(bgm_frozenforest.Get());
	scene->AttachActor(new FrozenForest);
	if (playerIndex == 0)
		scene->AttachActor(new DelayCreate(new Dialog_Reimu_Artia, 60));
	else if (playerIndex == 1)
		scene->AttachActor(new DelayCreate(new Dialog_Marisa_Artia, 60));

	scene->AttachActor(new DelayCreate(new BgmText(L"ÄýËªµÄ÷Ê»¨ ~ Profound Spirit"), 30));
}

void Title_OnLoad(IScene* scene)
{
	PlayBgm(bgm_title.Get());
	scene->AttachActor(new TitleBackground);
	scene->AttachActor(new BlackCanvas);
	scene->AttachActor(new TitleTag_1);
	scene->AttachActor(new TitleTag_2);
	scene->AttachActor(new TitleParticleSpawner);

	scene->AttachActor(new TDelayInvoke(Delegate([=]()->void {
		scene->AttachActor(new Frame_Title);
		}), 10));
}

void LoadLevel(IScene* scene, LevelType type)
{
	LoadLevelResource(type);
	switch (type)
	{
	case LEVEL_TITLE:
		Title_OnLoad(scene);
		break;
	case LEVEL_FROZENFOREST:
		FrozenForest_OnLoad(scene);
		break;
	case LEVEL_RIVERSIDE:
		RiverSide_OnLoad(scene);
		break;
	case LEVEL_GRASSLAND:
		GrassLand_OnLoad(scene);
		break;
	}
}

LevelType GetStageLevel(int stage)
{
	LevelType level = LEVEL_NONE;
	switch (stage)
	{
	case 0:
		level = LEVEL_GRASSLAND;
		break;
	case 1:
		level = LEVEL_RIVERSIDE;
		break;
	case 2:
		level = LEVEL_FROZENFOREST;
		break;
	}
	return level;
}

void ClearLevel()
{
	if (practiceMode)
	{
		player->GetScene()->AttachActor(new DelayInvoke(Delegate([=]()->void {
			StopBgm();
			timer->SetTimeStopLevel(1);
			player->GetScene()->AttachActor(new ChangeLevel(LEVEL_TITLE));
			}), 150));
	}
	else
	{
		if (nowStage < 2)
		{
			int s = 1000000 * (1 + playerData.life);
			player->GetScene()->AttachActor(new StageClear(s));
			AddScore(s);
			if (!isReplaying)
				player->GetScene()->AttachActor(new DelayCreate(new LevelCleaner(GetStageLevel(nowStage + 1)), 200));
		}
		else
		{
			player->GetScene()->AttachActor(new DelayInvoke(Delegate([=]()->void {
				int s = 1000000 * (1 + playerData.life);
				player->GetScene()->AttachActor(new StageClear(s));
				AddScore(s);
				if (!isReplaying)
					player->GetScene()->AttachActor(new DelayCreate(new Victory, 200));
				}), 150));
		}
	}
}