#include "BackGround.h"

BackGround::BackGround()
{
	SetName("BackGround");
}

void BackGround::OnCreated()
{
	InsertBefore(GameSceneCom::locator[2].Get());
}

void BackGround::OnDestroy()
{
}