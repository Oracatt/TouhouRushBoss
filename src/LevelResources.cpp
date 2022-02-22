#include "LevelResources.h"
#include "EngineGlobal.h"
#include "Source.h"
#include "GameGlobal.h"

using namespace Global;

ComPtr<IDynamicAudioInstance> nowBgm;

ComPtr<IDynamicAudioInstance> bgm_gameover;
ComPtr<IDynamicAudioInstance> bgm_gamestart;

ComPtr<IDynamicAudioInstance> bgm_title;
ComPtr<ITexture> src_titlebg;
ComPtr<ITexture> src_titlebg_2;
ComPtr<ITexture> src_title_1;
ComPtr<ITexture> src_title_2;
ComPtr<ITexture> src_title;
ComPtr<ITexture> src_light;
ComPtr<ITexture> src_selector;
ComPtr<ITexture> src_difficulty;
ComPtr<ITexture> src_select;
ComPtr<ITexture> src_playerintro;
ComPtr<ITexture> src_ps00;
ComPtr<ITexture> src_ps01;
ComPtr<ITexture> src_arrow;
ComPtr<ITexture> src_star;
ComPtr<ITexture> src_replay;

ComPtr<IDynamicAudioInstance> bgm_frozenforest;
ComPtr<ITexture> src_forest1;
ComPtr<ITexture> src_snow;
ComPtr<ITexture> src_snow_ground;
ComPtr<ITexture> src_tree1;
ComPtr<ITexture> src_tree2;

ComPtr<IDynamicAudioInstance> bgm_riverside;
ComPtr<ITexture> src_river_ground;
ComPtr<ITexture> src_water;
ComPtr<ITexture> src_cloud_1;
ComPtr<ITexture> src_cloud_2;

ComPtr<IDynamicAudioInstance> bgm_grassland;
ComPtr<ITexture> src_grassland;
ComPtr<ITexture> src_leaf;
ComPtr<ITexture> src_sunlight;

void FrozenForest_OnLoadResource()
{
	DynamicAudioDesc bgmDesc;
	bgmDesc.loopBegin = 2450504;
	bgmDesc.loopCount = -1;
	bgmDesc.loopEnd = 17569000;
	bgmDesc.playBegin = 0;
	bgmDesc.playEnd = 17569000;
	audio->CreateDynamicAudioInstance("bgm\\ÄýËªµÄ÷Ê»¨.wav", bgmDesc, &bgm_frozenforest);
	bgm_frozenforest->SetVolume(GetBgmVolume());

	SourceLoader loader;
	loader.AddSource(src_forest1, "image\\background\\FrozenForest\\forest1.png");
	loader.AddSource(src_snow, "image\\background\\FrozenForest\\snow.png");
	loader.AddSource(src_snow_ground, "image\\background\\FrozenForest\\snow_ground.png");
	loader.AddSource(src_tree1, "image\\background\\FrozenForest\\tree1.png");
	loader.AddSource(src_tree2, "image\\background\\FrozenForest\\tree2.png");
	loader.Load();
}

void RiverSide_OnLoadResource()
{
	DynamicAudioDesc bgmDesc;
	bgmDesc.loopBegin = 217500;
	bgmDesc.loopCount = -1;
	bgmDesc.loopEnd = 10800000;
	bgmDesc.playBegin = 0;
	bgmDesc.playEnd = 10800000;
	audio->CreateDynamicAudioInstance("bgm\\¤ª»Ý¤ß¥µ¥Þ©`¥ì¥¤¥½.wav", bgmDesc, &bgm_riverside);
	bgm_riverside->SetVolume(GetBgmVolume());

	SourceLoader loader;
	loader.AddSource(src_river_ground, "image\\background\\RiverSide\\ground.png");
	loader.AddSource(src_water, "image\\background\\RiverSide\\water.png");
	loader.AddSource(src_cloud_1, "image\\background\\RiverSide\\cloud1.png");
	loader.AddSource(src_cloud_2, "image\\background\\RiverSide\\cloud2.png");
	loader.Load();
}

void GrassLand_OnLoadResource()
{
	DynamicAudioDesc bgmDesc;
	bgmDesc.loopBegin = 159600;
	bgmDesc.loopCount = -1;
	bgmDesc.loopEnd = 13020000;
	bgmDesc.playBegin = 0;
	bgmDesc.playEnd = 13020000;
	audio->CreateDynamicAudioInstance("bgm\\¤¤¤¿¤º¤é¤ËÃü¤ò¤«¤±¤Æ.wav", bgmDesc, &bgm_grassland);
	bgm_grassland->SetVolume(GetBgmVolume());

	SourceLoader loader;
	loader.AddSource(src_grassland, "image\\background\\GrassLand\\grassland.png");
	loader.AddSource(src_leaf, "image\\background\\GrassLand\\leaf.png");
	loader.AddSource(src_sunlight, "image\\background\\GrassLand\\light.png");
	loader.Load();
}

void Title_OnLoadResource()
{
	if (!bgm_gameover)
	{
		DynamicAudioDesc bgmDesc;
		bgmDesc.loopBegin = 0;
		bgmDesc.loopCount = -1;
		bgmDesc.loopEnd = 6683972;
		bgmDesc.playBegin = 0;
		bgmDesc.playEnd = 6683972;
		audio->CreateDynamicAudioInstance("bgm\\Player's Score.wav", bgmDesc, &bgm_gameover);
		bgm_gameover->SetVolume(GetBgmVolume());
	}

	if (!bgm_gamestart)
	{
		DynamicAudioDesc bgmDesc;
		bgmDesc.loopBegin = 0;
		bgmDesc.loopCount = -1;
		bgmDesc.loopEnd = 13805768;
		bgmDesc.playBegin = 0;
		bgmDesc.playEnd = 13805768;
		audio->CreateDynamicAudioInstance("bgm\\»¨¤ÎÓ³¤ë‰V.wav", bgmDesc, &bgm_gamestart);
		bgm_gamestart->SetVolume(GetBgmVolume());
	}

	DynamicAudioDesc bgmDesc;
	bgmDesc.loopBegin = 549000;
	bgmDesc.loopCount = -1;
	bgmDesc.loopEnd = 15450000;
	bgmDesc.playBegin = 0;
	bgmDesc.playEnd = 15450000;
	audio->CreateDynamicAudioInstance("bgm\\»ê¤Î»¨.wav", bgmDesc, &bgm_title);
	bgm_title->SetVolume(GetBgmVolume());

	SourceLoader loader;
	loader.AddSource(src_titlebg, "image\\title\\titlebg.png");
	loader.AddSource(src_titlebg_2, "image\\title\\titlebg_2.png");
	loader.AddSource(src_title_1, "image\\title\\title_1.png");
	loader.AddSource(src_title_2, "image\\title\\title_2.png");
	loader.AddSource(src_title, "image\\title\\title.png");
	loader.AddSource(src_light, "image\\effect\\light.png");
	loader.AddSource(src_selector, "image\\title\\selector.png");
	loader.AddSource(src_difficulty, "image\\title\\difficulty.png");
	loader.AddSource(src_select, "image\\title\\select.png");
	loader.AddSource(src_playerintro, "image\\title\\player_intro.png");
	loader.AddSource(src_ps00, "image\\title\\ps_00.png");
	loader.AddSource(src_ps01, "image\\title\\ps_01.png");
	loader.AddSource(src_arrow, "image\\title\\arrow.png");
	loader.AddSource(src_star, "image\\title\\star.png");
	loader.AddSource(src_replay, "image\\title\\replay.png");
	loader.Load();
}

void UnloadLevelResource(LevelType ecp)
{
	if (ecp != LEVEL_TITLE)
	{
		bgm_title.Reset();
		src_titlebg.Reset();
		src_title_1.Reset();
		src_title_2.Reset();
		src_title.Reset();
		src_light.Reset();
		src_selector.Reset();
		src_difficulty.Reset();
		src_select.Reset();
		src_playerintro.Reset();
		src_ps00.Reset();
		src_ps01.Reset();
		src_arrow.Reset();
		src_replay.Reset();
	}
	if (ecp != LEVEL_FROZENFOREST)
	{
		bgm_frozenforest.Reset();
		src_forest1.Reset();
		src_snow.Reset();
		src_snow_ground.Reset();
		src_tree1.Reset();
		src_tree2.Reset();
	}
	if (ecp != LEVEL_RIVERSIDE)
	{
		bgm_riverside.Reset();
		src_river_ground.Reset();
		src_water.Reset();
		src_cloud_1.Reset();
		src_cloud_2.Reset();
	}
	if (ecp != LEVEL_GRASSLAND)
	{
		bgm_grassland.Reset();
		src_grassland.Reset();
		src_leaf.Reset();
		src_light.Reset();
	}
}

void LoadLevelResource(LevelType type, bool forceReload)
{
	WaitForSingleObject(resourceLock, INFINITE);
	if (forceReload)
		nowLevel = LEVEL_NONE;
	if (nowLevel != type)
	{
		nowLevel = type;
		UnloadLevelResource(type);
		switch (type)
		{
		case LEVEL_RIVERSIDE:
			RiverSide_OnLoadResource();
			break;
		case LEVEL_GRASSLAND:
			GrassLand_OnLoadResource();
			break;
		case LEVEL_FROZENFOREST:
			FrozenForest_OnLoadResource();
			break;
		case LEVEL_TITLE:
			Title_OnLoadResource();
			break;
		}
	}
	ReleaseMutex(resourceLock);
}

void PlayBgm(IDynamicAudioInstance* pInstance)
{
	StopBgm();
	pInstance->SetVolume(GetBgmVolume());
	pInstance->Play();
	nowBgm = pInstance;
}

void StopBgm()
{
	if (nowBgm)
	{
		nowBgm->Stop();
		nowBgm.Reset();
	}
}

void PauseBgm()
{
	if (nowBgm)
		nowBgm->Pause();
}

void ContinueBgm()
{
	if (nowBgm)
		nowBgm->Play();
}