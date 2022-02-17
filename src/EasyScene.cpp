#include "EasyScene.h"

MSGRESULT EasyScene::OnMessageBegin(BYTE& msgType, LPVOID& param1, LPVOID& param2)
{
	if (isFirstLoad && msgType == SMSG_SCENEAWAKE)
	{
		isFirstLoad = false;
		OnLoad();
	}
	if (!isCleared && msgType == SMSG_DESTROYSCENE)
	{
		isCleared = true;
		OnClear();
	}
	return MSGR_IGNORE;
}