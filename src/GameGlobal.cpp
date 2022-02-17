#include "GameGlobal.h"
#include "LevelResources.h"
#include "Replay.h"

GameData playerData = { 2,3,0,0,100,10000,0 };

uint64_t showHighScore = 0;

ComPtr<Player> player;

ComPtr<Boss> nowBoss;

Vector2 warpCenter(10000, 10000);

volatile float warpLimit = 10000;

volatile int warpColor = 0;

wstring bossName = L"";

int bossLife = 0;

bool spellFailed = false;

int difficulty = 1;

int playerIndex = 0;

float bgmVolume = 1;

int nowStage = 0;

string lastSaveName;

int retryTimes = 0;

bool practiceMode = false;

bool canPause = false;

void ResetGameData()
{
	showHighScore = 0;
	uint64_t hiscore = playerData.hiscore;
	playerData = { 2,3,(uint64_t)min(retryTimes,9),hiscore,100,10000,0 };
	if (practiceMode)
	{
		playerData.life = 9;
		playerData.power = 400;
	}
}

void AddScore(long long score)
{
	playerData.score += score;
	if (isReplaying)
	{
		if (playerData.score > showHighScore)
			showHighScore = playerData.score;
	}
	else
	{
		if (playerData.score > playerData.hiscore)
			playerData.hiscore = playerData.score;
	}
}

void SetBgmVolume(float _volume)
{
	bgmVolume = _volume;
	if (bgm_title)
		bgm_title->SetVolume(bgmVolume);
	if (bgm_gameover)
		bgm_gameover->SetVolume(bgmVolume);
}

float GetBgmVolume()
{
	return bgmVolume;
}