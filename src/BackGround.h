#pragma once
#include "VirtualLib.h"
#include "GameScene.h"

class BackGround :public GameObject
{
public:

	BackGround();

protected:

	void OnCreated() override;

	void OnDestroy() override;

	template <class T>
	T* CreateObject(T* g)
	{
		scene->AttachActor(g);
		g->SetName("BackGround");
		return g;
	}
};