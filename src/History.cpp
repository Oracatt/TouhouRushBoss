#include "History.h"
#include "GameGlobal.h"
#include "SoundEffect.h"

int SCHistory::history[64][4][2];

void SCHistory::SetHistoryGot(int cardID, int difficulty, int num)
{
	history[cardID][difficulty][0] = num;
}

int SCHistory::GetHistoryGot(int cardID, int difficulty)
{
	return history[cardID][difficulty][0];
}

void SCHistory::SetHistoryTotal(int cardID, int difficulty, int num)
{
	history[cardID][difficulty][1] = num;
}

int SCHistory::GetHistoryTotal(int cardID, int difficulty)
{
	return history[cardID][difficulty][1];
}

void SCHistory::ResetHistory()
{
	memset(history, 0, sizeof(history));
}

void SCHistory::LoadHistory()
{
	ResetHistory();
	if (_access("score.dat", 0) != -1)
	{
		FileStream fs("score.dat", FileStream::Mode::OpenOrCreate);
		fs.Read(history, 0, sizeof(history));
		fs.Read(&playerData.hiscore, 0, sizeof(playerData.hiscore));
		showHighScore = playerData.hiscore;
		int nameLen;
		fs.Read(&nameLen, 0, sizeof(nameLen));
		char* name = new char[nameLen + 1];
		fs.Read(name, 0, nameLen);
		name[nameLen] = 0;
		lastSaveName = name;
		delete[] name;
		float bgmVol, sndVol;
		fs.Read(&bgmVol, 0, sizeof(bgmVol));
		fs.Read(&sndVol, 0, sizeof(sndVol));
		SetBgmVolume(bgmVol);
		SetSoundEffectVolume(sndVol);
		fs.Close();
	}
}

void SCHistory::SaveHistory()
{
	FileStream fs("score.dat", FileStream::Mode::Create);
	fs.Write(history, 0, sizeof(history));
	fs.Write(&playerData.hiscore, 0, sizeof(playerData.hiscore));
	int nameLen = (int)lastSaveName.size();
	fs.Write(&nameLen, 0, sizeof(nameLen));
	fs.Write((LPVOID)lastSaveName.c_str(), 0, nameLen);
	float bgmVol = GetBgmVolume();
	float sndVol = GetSoundEffectVolume();
	fs.Write(&bgmVol, 0, sizeof(bgmVol));
	fs.Write(&sndVol, 0, sizeof(sndVol));
	fs.Close();
}
