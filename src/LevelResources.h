#pragma once
#include "VirtualLib.h"
#include "Level.h"

extern ComPtr<IDynamicAudioInstance> nowBgm;

//Global
extern ComPtr<IDynamicAudioInstance> bgm_gameover;
extern ComPtr<IDynamicAudioInstance> bgm_gamestart;

//Title
extern ComPtr<IDynamicAudioInstance> bgm_title;
extern ComPtr<ITexture> src_titlebg;
extern ComPtr<ITexture> src_titlebg_2;
extern ComPtr<ITexture> src_title_1;
extern ComPtr<ITexture> src_title_2;
extern ComPtr<ITexture> src_title;
extern ComPtr<ITexture> src_light;
extern ComPtr<ITexture> src_selector;
extern ComPtr<ITexture> src_difficulty;
extern ComPtr<ITexture> src_select;
extern ComPtr<ITexture> src_playerintro;
extern ComPtr<ITexture> src_ps00;
extern ComPtr<ITexture> src_ps01;
extern ComPtr<ITexture> src_arrow;
extern ComPtr<ITexture> src_star;
extern ComPtr<ITexture> src_replay;

//FrozenForest
extern ComPtr<IDynamicAudioInstance> bgm_frozenforest;
extern ComPtr<ITexture> src_forest1;
extern ComPtr<ITexture> src_snow;
extern ComPtr<ITexture> src_snow_ground;
extern ComPtr<ITexture> src_tree1;
extern ComPtr<ITexture> src_tree2;

//RiverSide
extern ComPtr<IDynamicAudioInstance> bgm_riverside;
extern ComPtr<ITexture> src_river_ground;
extern ComPtr<ITexture> src_water;
extern ComPtr<ITexture> src_cloud_1;
extern ComPtr<ITexture> src_cloud_2;

//GrassLand
extern ComPtr<IDynamicAudioInstance> bgm_grassland;
extern ComPtr<ITexture> src_grassland;
extern ComPtr<ITexture> src_leaf;
extern ComPtr<ITexture> src_sunlight;

void LoadLevelResource(LevelType type, bool forceReload = false);

void PlayBgm(IDynamicAudioInstance* pInstance);

void StopBgm();

void PauseBgm();

void ContinueBgm();