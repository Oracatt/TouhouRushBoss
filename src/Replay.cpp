#include "Replay.h"
#include "Rand.h"
#include "GSObjects.h"

bool replayOver = false; //��ReplayOverListener����
bool isReplaying = false; //�Ƿ�Ϊ�ط�״̬
bool recording = false; //�Ƿ�����¼��
bool allowSaveReplay = false; //������ط�

Replay currentReplay;
ReplayPage currentPage;
Replay replayList[32];

void StartRecording()
{
	if (!isReplaying)
	{
		if (!recording)
		{
			unsigned int seed = time(0);
			SetRandomSeed(seed);
			recording = true;
			player->GetScene()->Invoke(Delegate([=]()->void {
				player->SetPlayerFlags(player->GetPlayerFlags() & ~PF_PeaceLoving);
				ReplayPageHeader header;
				header.data = playerData;
				header.randseed = seed;
				header.stage = nowStage;
				header.playerAttribution = player->GetAttribution();
				header.playerFlags = player->GetPlayerFlags();
				header.playerLoc = player->transform->location;
				currentPage.ClearSequence();
				currentPage.SetHeader(header);
				}));
		}
	}
}

void StopRecording()
{
	if (!isReplaying)
	{
		if (recording)
		{
			recording = false;
			player->GetScene()->Invoke(Delegate([=]()->void {
				if (currentPage.GetSequenceAmount() > 0)
				{
					currentPage.AddEndTag();
					currentReplay.AddPage(currentPage);
				}
				}));
		}
	}
}

void SaveReplay(const string& filename, const string& playerName)
{
	if (!isReplaying)
	{
		player->GetScene()->Invoke(Delegate([=]()->void {
			currentReplay.SaveToFile(filename, playerName);
			}));
	}
}

void LoadReplayPage(int stage)
{
	if (isReplaying)
	{
		currentPage = currentReplay.GetPage(stage);
		ReplayPageHeader header=currentPage.GetHeader();
		SetRandomSeed(header.randseed);
		replayOver = false;
		player->GetScene()->Invoke(Delegate([=]()->void {
			player->SetPlayerFlags(player->GetPlayerFlags() & ~PF_PeaceLoving);
			uint64_t hs = playerData.hiscore;
			playerData = header.data;
			playerData.hiscore = hs;
			player->GetAttribution() = header.playerAttribution;
			player->SetPlayerFlags(header.playerFlags);
			player->transform->location = header.playerLoc;
			}));
	}
}