#pragma once
#include "VirtualLib.h"

class SCHistory
{
private:

	static int history[64][4][2];

public:

	static void SetHistoryGot(int cardID, int difficulty, int num);

	static int GetHistoryGot(int cardID, int difficulty);

	static void SetHistoryTotal(int cardID, int difficulty, int num);

	static int GetHistoryTotal(int cardID, int difficulty);

	static void ResetHistory();

	static void LoadHistory();

	static void SaveHistory();
};