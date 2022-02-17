#include "Callback.h"

MainEventCallBack mainCallback;
ResizeCallBack resizeCallback;
WarpCallBack warpCallback;
BlurCallBack blurCallback;
WaterCallBack waterCallback;

EngineEventCallBack* GetCurrentCallback()
{
	vector<EngineEventCallBack*> cbs = engine->GetEventCallBacks();
	for (auto e : cbs)
	{
		if (e->GetActive())
			return e;
	}
	return 0;
}

void SetCurrentCallback(EngineEventCallBack* pCallBack)
{
	vector<EngineEventCallBack*> cbs = engine->GetEventCallBacks();
	for (auto e : cbs)
	{
		if (e == pCallBack)
			e->SetActive(true);
		else
			e->SetActive(false);
	}
}