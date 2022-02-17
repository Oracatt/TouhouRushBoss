#pragma once
#include "VirtualLib.h"
#include "EasyScene.h"
#include <stack>

class TitleSceneCom :public EasyScene
{
protected:

	void OnLoad() override;

	void OnClear() override;

public:

	static LPGOBJ locator[6];

	static ComPtr<Camera> camera;

	static stack<ComPtr<class Frame>> frameStack;
};