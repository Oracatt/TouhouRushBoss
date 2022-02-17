#pragma once
#include "VirtualLib.h"
#include "EasyScene.h"

class LoadingSceneCom :public EasyScene
{
protected:

	void OnLoad() override;

	void OnClear() override;

public:

	ComPtr<Camera> camera;

	ComPtr<ITexture> presrc_sig;

	ComPtr<ITexture> presrc_loading;
};