#pragma once
#include "VirtualLib.h"

enum LevelType
{
	LEVEL_NONE,
	LEVEL_TITLE,
	LEVEL_FROZENFOREST,
	LEVEL_RIVERSIDE,
	LEVEL_GRASSLAND
};

extern LevelType nowLevel;

void LoadLevel(IScene* scene, LevelType type);

LevelType GetStageLevel(int stage);

void ClearLevel();