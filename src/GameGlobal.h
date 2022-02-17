#pragma once
#include "Player.h"
#include "Boss.h"

struct GameData
{
	int life; //残机
	int bomb; //雷
	uint64_t score; //分数
	uint64_t hiscore; //最高分
	int power; //火力
	int maxPoint; //最大得点
	int graze; //擦弹
};

extern GameData playerData;

extern uint64_t showHighScore;

extern ComPtr<Player> player;

extern ComPtr<Boss> nowBoss;

extern Vector2 warpCenter;

extern volatile float warpLimit;

extern volatile int warpColor;

extern wstring bossName;

extern int bossLife;

extern bool spellFailed;

extern int difficulty; //0E3L

extern int playerIndex; //0reimu 1marisa

extern int nowStage;

extern string lastSaveName;

extern int retryTimes;

extern bool practiceMode;

extern bool canPause;

void ResetGameData();

void AddScore(long long score);

void SetBgmVolume(float volume);

float GetBgmVolume();