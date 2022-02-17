#include "Dialog.h"
#include "DialogDeriver.h"

void ShowEndDialog_SunnyMilk()
{
	if (playerIndex == 0)
		player->GetScene()->AttachActor(new DelayCreate(new Dialog_Reimu_SunnyMilk_2, 50));
	else if (playerIndex == 1)
		player->GetScene()->AttachActor(new DelayCreate(new Dialog_Marisa_SunnyMilk_2, 50));
}

void ShowEndDialog_Monstone()
{
	if (playerIndex == 0)
		player->GetScene()->AttachActor(new DelayCreate(new Dialog_Reimu_Monstone_2, 50));
	else if (playerIndex == 1)
		player->GetScene()->AttachActor(new DelayCreate(new Dialog_Marisa_Monstone_2, 50));
}