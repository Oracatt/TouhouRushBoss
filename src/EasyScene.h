#pragma once
#include "VirtualLib.h"
#include "EngineGlobal.h"

class EasyScene :public SComponent
{
protected:

	bool isFirstLoad = true;

	bool isCleared = false;

	MSGRESULT OnMessageBegin(BYTE& msgType, LPVOID& param1, LPVOID& param2) override;

	virtual void OnLoad() = 0;

	virtual void OnClear() = 0;
};