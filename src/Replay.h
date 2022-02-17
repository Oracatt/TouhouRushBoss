#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include <direct.h>
#include "GameGlobal.h"
#include "Player.h"

using namespace std;

typedef short Sequence;

#define SQ_NONE           0
#define SQ_ZPRESS         1
#define SQ_ZRELEASE       2
#define SQ_XPRESS         4
#define SQ_XRELEASE       8
#define SQ_LSHIFTPRESS    16
#define SQ_LSHIFTRELEASE  32
#define SQ_LEFTDOWN       64
#define SQ_RIGHTDOWN      128
#define SQ_UPDOWN         256
#define SQ_DOWNDOWN       512
#define SQ_LCTRLDOWN      1024
#define SQ_OVER           2048

extern bool replayOver;
extern bool isReplaying;
extern bool recording;
extern bool allowSaveReplay;

extern class Replay currentReplay;
extern class ReplayPage currentPage;
extern class Replay replayList[32]; //下标为No

struct Date
{
	int year;

	int month;

	int day;
};

struct ReplayPageHeader
{
	unsigned int randseed;

	int stage;

	GameData data;

	PlayerAttribution playerAttribution;

	DWORD playerFlags;

	Vector3 playerLoc;

	long long finalScore;
};

struct ReplayHeader
{
	int playerIndex;

	int difficulty;

	Date time;
};

class ReplayPage
{
	friend class Replay;

public:

	ReplayPage() :seq_ptr(0)
	{
	}

	void SetHeader(const ReplayPageHeader& _header)
	{
		header = _header;
	}

	ReplayPageHeader GetHeader()
	{
		return header;
	}

	int GetSequenceAmount()
	{
		return sequences.size();
	}

	void AddSequence(Sequence seq)
	{
		sequences.push_back(seq);
	}

	void ClearSequence()
	{
		sequences.clear();
		seq_ptr = 0;
	}

	Sequence ReadSequence()
	{
		return sequences[seq_ptr++];
	}

	void AddEndTag()
	{
		if (sequences.size() != 0)
			sequences[sequences.size() - 1] |= SQ_OVER;
		else
			sequences.push_back(SQ_OVER);
	}

private:

	vector<Sequence> sequences;

	int seq_ptr;

	ReplayPageHeader header;
};

/*
	玩家名长度
	玩家名
	Replay头
	(
	ReplayPage头
	SequenceArray
	)*
*/
class Replay
{
public:

	Replay()
	{
	}

	void SetHeader(const ReplayHeader& _header)
	{
		header = _header;
	}

	ReplayHeader GetHeader()
	{
		return header;
	}

	string GetPlayerName()
	{
		return playerName;
	}

	void AddPage(const ReplayPage& page)
	{
		ReplayPage p = page;
		auto h = p.GetHeader();
		h.finalScore = playerData.score;
		p.SetHeader(h);
		pages.push_back(p);
	}

	int GetPageAmount()
	{
		return pages.size();
	}

	ReplayPage GetPage(int index)
	{
		return pages[index];
	}

	void ClearPages()
	{
		pages.clear();
	}

	void LoadFromFile(const string& filename)
	{
		if (_access("replay", 0) == -1)
			return;
		if (_access((string("replay/") + filename).c_str(), 0) == -1)
			return;

		int nameLength;
		FileStream fs((string("replay/") + filename).c_str(), FileStream::Mode::Open);
		fs.Read(&nameLength, 0, sizeof(nameLength));
		char* name = new char[nameLength + 1];
		fs.Read(name, 0, nameLength);
		name[nameLength] = 0;
		playerName = name;
		delete[] name;
		fs.Read(&header, 0, sizeof(header));
		pages.clear();
		while (fs.GetPosition() != fs.GetLength())
		{
			ReplayPageHeader pageHeader;
			ReplayPage page;
			Sequence seq;
			fs.Read(&pageHeader, 0, sizeof(pageHeader));
			page.SetHeader(pageHeader);
			while (true)
			{
				fs.Read(&seq, 0, sizeof(seq));
				page.AddSequence(seq);
				if (seq & SQ_OVER)
					break;
			}
			pages.push_back(page);
		}
		fs.Close();
	}

	void SaveToFile(const string& filename, const string& _playerName)
	{
		if (_access("replay", 0) == -1)
			_mkdir("replay");
		
		playerName = _playerName;
		int length = playerName.length();
		SyncDate();
		header.difficulty = difficulty;
		header.playerIndex = playerIndex;
		FileStream fs((string("replay/") + filename).c_str(), FileStream::Mode::Create);
		fs.Write(&length, 0, sizeof(length));
		fs.Write((LPVOID)playerName.c_str(), 0, length);
		fs.Write(&header, 0, sizeof(header));
		for (auto page : pages)
		{
			ReplayPageHeader h;
			h = page.GetHeader();
			fs.Write(&h, 0, sizeof(h));
			for (auto seq : page.sequences)
				fs.Write(&seq, 0, sizeof(seq));
		}
		fs.Close();
	}

private:

	vector<ReplayPage> pages;

	ReplayHeader header;

	string playerName;

	void SyncDate()
	{
		time_t timep;
		struct tm* p;
		time(&timep);
		p = gmtime(&timep);

		header.time.year = 1900 + p->tm_year;
		header.time.month = 1 + p->tm_mon;
		header.time.day = p->tm_mday;
	}
};

void StartRecording();

void StopRecording();

void SaveReplay(const string& filename, const string& playerName);

void LoadReplayPage(int stage);