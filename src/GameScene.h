#pragma once
#include "VirtualLib.h"
#include "EasyScene.h"

class GameSceneCom :public EasyScene
{
protected:

	void OnLoad() override;

	void OnClear() override;

	MSGRESULT OnMessageBegin(BYTE& msgType, LPVOID& param1, LPVOID& param2) override;

public:

	static ComPtr<class GameCamera> gamecam;

	static ComPtr<Camera> backcam;

	static ComPtr<Camera> fixedcam;

	static ComPtr<Camera> fixed3dcam;

	static LPGOBJ locator[25];

	static LPGOBJ pauseLocator;

	static ComPtr<IRenderTarget> balloonTarget;

	static ComPtr<IRenderTarget> backTarget;

	static ComPtr<IRenderTarget> gameTarget;

	static ComPtr<IRenderTarget> spellTextTarget;
};