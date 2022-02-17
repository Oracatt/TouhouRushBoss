#pragma once
#include "Dialog.h"
#include "Boss.h"
#include "BossSpellCardDeriver.h"
#include "BossDeriver.h"

class Dialog_Reimu_Artia :public Dialog
{
	void StepChange(int step) override
	{
		switch (step)
		{
		case 0:
			left = scene->AttachActor(new Face_Reimu);
			left->Activate();
			left->SetFaceType(FACE_PUZZLED);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"������ʲô��ֵĵط�...");
			break;
		case 1:
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"һգ�۵Ĺ����Ȼ�������ϣ�������˭�ڸ��");
			break;
		case 2:
		{
			coldFrame = 120;
			auto boss = scene->AttachActor(new Artia);
			bossLife = 6;
			boss->transform->location = { 0,100,0 };
			boss->SetCharacterFlags(boss->GetCharacterFlags() | CharacterFlag::CF_ImmuneDamage);
			scene->AttachActor(new BossShowing(boss));
			left->Deactivate();
			left->SetFaceType(FACE_SURPRISE);
			SetBalloon(new Balloon_4({ 0,60 }, 5, false), L"������λ��·��С���ѣ�");
		}
			break;
		case 3:
			bossNameEffect = scene->AttachActor(new BossNameSprite(2));
			right = scene->AttachActor(new Face_Artia);
			right->Activate();
			right->SetFaceType(FACE_HAPPY);
			SetBalloon(new Balloon_4({ 150,-30 }, 5, true), L"��ѽ��û�뵽��Ȼ������С��");
			break;
		case 4:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_DISAPPOINT);
			right->SetFaceType(FACE_NOTICE);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"�ţ���֮ǰ�ɴ�������ʶ��");
			break;
		case 5:
			left->Deactivate();
			right->Activate();
			right->SetFaceType(FACE_HAPPY);
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"û��ϵ���������������С������˾���֪��");
			break;
		case 6:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_NOTICE);
			right->SetFaceType(FACE_NOTICE);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"���˻����磬�Ǿ�������������");
			break;
		case 7:
			left->Deactivate();
			right->Activate();
			left->SetFaceType(FACE_DISAPPOINT);
			right->SetFaceType(FACE_SWEAT);
			SetBalloon(new Balloon_4({ 150,-30 }, 3, true), L"�ǵ�Ҳ����");
			break;
		case 8:
			right->SetFaceType(FACE_DISAPPOINT);
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"��ʵ�ϣ������������ʲô���ӣ���Ҳû�м���");
			break;
		case 9:
			left->SetFaceType(FACE_SURPRISE);
			right->SetFaceType(FACE_HAPPY);
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"������֪�����������Щ���ǣ�����������Ļ�Ͽ���������");
			break;
		case 10:
			left->Activate();
			right->Deactivate();
			right->SetFaceType(FACE_NOTICE);
			SetBalloon(new Balloon_1({ -150,-30 }, 4, false), L"���ǣ����ڣ�");
			break;
		case 11:
			left->Deactivate();
			right->Activate();
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"û���������Ѿ����ܵ������ǵ��˷���");
			break;
		case 12:
			right->SetFaceType(FACE_HAPPY);
			SetBalloon(new Balloon_4({ 150,-30 }, 5, true), L"�⽫��һ���ޱȾ��ʵ�ֱ����");
			break;
		case 13:
			nowBoss->ShowMagicSquare();
			nowBoss->SetCharacterFlags(nowBoss->GetCharacterFlags() & ~CharacterFlag::CF_ImmuneDamage);
			nowBoss->AddComponent(new Artia_SC_1);
			player->SetPlayerFlags(player->GetPlayerFlags() & ~PF_PeaceLoving);
			Destroy();
			break;
		}
	}
};

class Dialog_Marisa_Artia :public Dialog
{
	void StepChange(int step) override
	{
		switch (step)
		{
		case 0:
			left = scene->AttachActor(new Face_Marisa);
			left->Activate();
			left->SetFaceType(FACE_PUZZLED);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"���䰡...����������");
			break;
		case 1:
			left->SetFaceType(FACE_ANGRY);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"�⸽��һ����ʲô����������");
			break;
		case 2:
		{
			coldFrame = 120;
			auto boss = scene->AttachActor(new Artia);
			bossLife = 6;
			boss->transform->location = { 0,100,0 };
			boss->SetCharacterFlags(boss->GetCharacterFlags() | CharacterFlag::CF_ImmuneDamage);
			scene->AttachActor(new BossShowing(boss));
			left->Deactivate();
			left->SetFaceType(FACE_SURPRISE);
			SetBalloon(new Balloon_4({ 0,60 }, 5, false), L"����ɨ��ļһ�ѵ���...");
		}
		break;
		case 3:
			bossNameEffect = scene->AttachActor(new BossNameSprite(2));
			right = scene->AttachActor(new Face_Artia);
			right->Activate();
			right->SetFaceType(FACE_HAPPY);
			SetBalloon(new Balloon_4({ 150,-30 }, 5, true), L"��������Ȼ�������һ��");
			break;
		case 4:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_DISAPPOINT);
			right->SetFaceType(FACE_NOTICE);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"�ȵȣ����ǲ���Ӧ�������ҽ���һ�£�");
			break;
		case 5:
			left->Deactivate();
			right->Activate();
			left->SetFaceType(FACE_NOTICE);
			right->SetFaceType(FACE_HAPPY);
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"ûʱ���ˣ�����Ĺ����Ƕ��Ѿ��Ȳ����ˣ�");
			break;
		case 6:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_SWEAT);
			right->SetFaceType(FACE_NOTICE);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"ʲô���ڣ���������ֻ������������");
			break;
		case 7:
			left->SetFaceType(FACE_ANGRY);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"��������˭�������������һ���Ѳ��˹�ϵ��");
			break;
		case 8:
			left->Deactivate();
			right->Activate();
			right->SetFaceType(FACE_ANGRY);
			SetBalloon(new Balloon_4({ 150,-30 }, 5, true), L"ԭ���Ǹ������鷳�ļһ�...");
			break;
		case 9:
			right->SetFaceType(FACE_ANGRY2);
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"ֻ��ϧ��ֻ��ʤ���߲����ʸ��ƿ�������");
			break;
		case 10:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_SWEAT);
			right->SetFaceType(FACE_ANGRY);
			SetBalloon(new Balloon_1({ -150,-30 }, 3, false), L"...");
			break;
		case 11:
			left->Deactivate();
			right->Activate();
			right->SetFaceType(FACE_NOTICE);
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"����������ڴ�һ��ħ����ĶԾ��ˣ��ó����ȫ��ʵ���ɣ�");
			break;
		case 12:
			nowBoss->ShowMagicSquare();
			nowBoss->SetCharacterFlags(nowBoss->GetCharacterFlags() & ~CharacterFlag::CF_ImmuneDamage);
			nowBoss->AddComponent(new Artia_SC_1);
			player->SetPlayerFlags(player->GetPlayerFlags() & ~PF_PeaceLoving);
			Destroy();
			break;
		}
	}
};

class Dialog_Reimu_Monstone :public Dialog
{
	void StepChange(int step) override
	{
		switch (step)
		{
		case 0:
			left = scene->AttachActor(new Face_Reimu);
			left->Activate();
			left->SetFaceType(FACE_PUZZLED);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"��������ʱҲû����ʲô����...");
			break;
		case 1:
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"�ѵ�֮ǰ�����ҵĴ����");
			break;
		case 2:
		{
			coldFrame = 120;
			auto boss = scene->AttachActor(new Monstone);
			bossLife = 4;
			boss->transform->location = { 0,100,0 };
			boss->SetCharacterFlags(boss->GetCharacterFlags() | CharacterFlag::CF_ImmuneDamage);
			scene->AttachActor(new BossShowing(boss));
			left->SetFaceType(FACE_SURPRISE);
			SetBalloon(new Balloon_1({ -150,-30 }, 4, false), L"ʲô�ˣ�");
		}
		break;
		case 3:
		{
			bossNameEffect = scene->AttachActor(new BossNameSprite(1));
			right = scene->AttachActor(new Face_Monstone);
			left->Deactivate();
			right->Activate();
			left->SetFaceType(FACE_SURPRISE);
			right->SetFaceType(FACE_LOSE);
			SetBalloon(new Balloon_4({ 150,-30 }, 4, true), L"#%#@&$*!%");
		}
		break;
		case 4:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_SWEAT);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"����ͷ��ʯͷ���ɾ�����");
			break;
		case 5:
			left->Deactivate();
			right->Activate();
			left->SetFaceType(FACE_SURPRISE);
			SetBalloon(new Balloon_4({ 150,-30 }, 5, true), L"С�һע����˵���Ŀ�����");
			break;
		case 6:
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"�����ӿɲ������˶��ܹ�ȥ�ģ�");
			break;
		case 7:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_NOTICE);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"ԭ������һ���˵����ʯͷ��");
			break;
		case 8:
			left->Deactivate();
			right->Activate();
			left->SetFaceType(FACE_DISAPPOINT);
			SetBalloon(new Balloon_4({ 150,-30 }, 4, true), L"�ҿɲ�����ͨ��ʯͷ��");
			break;
		case 9:
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"��������ӣ��Ȱ�����ֵǮ�Ķ������������ɣ�");
			break;
		case 10:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_NOTICE);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"ֵǮ�Ķ������������磬û�д�������");
			break;
		case 11:
			left->SetFaceType(FACE_ANGRY);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"������������ֹ��ĺ�����Ϊ�ɣ�");
			break;
		case 12:
			nowBoss->ShowMagicSquare();
			nowBoss->SetCharacterFlags(nowBoss->GetCharacterFlags() & ~CharacterFlag::CF_ImmuneDamage);
			nowBoss->AddComponent(new Monstone_SC_1);
			player->SetPlayerFlags(player->GetPlayerFlags() & ~PF_PeaceLoving);
			Destroy();
			StopBgm();
			PlayBgm(bgm_riverside.Get());
			scene->AttachActor(new BgmText(L"����Summer Rain"));
			break;
		}
	}
};

class Dialog_Reimu_Monstone_2 :public Dialog
{
	void StepChange(int step) override
	{
		switch (step)
		{
		case 0:
			coldFrame = 120;
			left = scene->AttachActor(new Face_Reimu);
			left->Activate();
			left->SetFaceType(FACE_PUZZLED);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"�������ø���֮ǰ�����Ǿ�������Ҫ�������");
			break;
		case 1:
			Destroy();
			ClearLevel();
			break;
		}
	}
};

class Dialog_Marisa_Monstone :public Dialog
{
	void StepChange(int step) override
	{
		switch (step)
		{
		case 0:
			left = scene->AttachActor(new Face_Marisa);
			left->Activate();
			left->SetFaceType(FACE_PUZZLED);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"�ܸо���һ��ͻ��д��·���");
			break;
		case 1:
		{
			coldFrame = 120;
			auto boss = scene->AttachActor(new Monstone);
			bossLife = 4;
			boss->transform->location = { 0,100,0 };
			boss->SetCharacterFlags(boss->GetCharacterFlags() | CharacterFlag::CF_ImmuneDamage);
			scene->AttachActor(new BossShowing(boss));
			left->Deactivate();
			left->SetFaceType(FACE_SURPRISE);
			SetBalloon(new Balloon_4({ 0,60 }, 4, false), L"Ԥ��ͦ׼�С�һ�");
		}
		break;
		case 2:
		{
			bossNameEffect = scene->AttachActor(new BossNameSprite(1));
			right = scene->AttachActor(new Face_Monstone);
			left->Deactivate();
			right->Activate();
			left->SetFaceType(FACE_SURPRISE);
			right->SetFaceType(FACE_LOSE);
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"ʶ��Ļ��͸Ͻ���ֵǮ�Ķ�������������");
		}
		break;
		case 3:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_DISAPPOINT);
			SetBalloon(new Balloon_1({ -150,-30 }, 4, false), L"��˵�ɲ������...");
			break;
		case 4:
			left->SetFaceType(FACE_SWEAT);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"������һ��ʯͷ���ȷʵ��������֮���");
			break;
		case 5:
			left->Deactivate();
			right->Activate();
			left->SetFaceType(FACE_NOTICE);
			SetBalloon(new Balloon_4({ 150,-30 }, 5, true), L"��������ӣ�Ҫô������·��...");
			break;
		case 6:
			SetBalloon(new Balloon_4({ 150,-30 }, 5, true), L"Ҫô�ͱ��Ҷ��º���ι��ɣ�");
			break;
		case 7:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_SWEAT);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"��ֻ��һ��ʯͷ��Ҫ�ٶ��Ǯ������ʲô����");
			break;
		case 8:
			left->SetFaceType(FACE_ANGRY);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"���������Ұ����ͻ����ȥ�ĵط���");
			break;
		case 9:
			nowBoss->ShowMagicSquare();
			nowBoss->SetCharacterFlags(nowBoss->GetCharacterFlags() & ~CharacterFlag::CF_ImmuneDamage);
			nowBoss->AddComponent(new Monstone_SC_1);
			player->SetPlayerFlags(player->GetPlayerFlags() & ~PF_PeaceLoving);
			Destroy();
			StopBgm();
			PlayBgm(bgm_riverside.Get());
			scene->AttachActor(new BgmText(L"����Summer Rain"));
			break;
		}
	}
};

class Dialog_Marisa_Monstone_2 :public Dialog
{
	void StepChange(int step) override
	{
		switch (step)
		{
		case 0:
			left = scene->AttachActor(new Face_Marisa);
			left->Activate();
			left->SetFaceType(FACE_DISAPPOINT);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"��Ȼ��һ��ʯͷ�˷�����ô�õ�ʱ��");
			break;
		case 1:
			coldFrame = 120;
			left->SetFaceType(FACE_PUZZLED);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"����ץ��ʱ���·��");
			break;
		case 2:
			Destroy();
			ClearLevel();
			break;
		}
	}
};

class Dialog_Reimu_SunnyMilk :public Dialog
{
	void StepChange(int step) override
	{
		switch (step)
		{
		case 0:
			left = scene->AttachActor(new Face_Reimu);
			left->Activate();
			left->SetFaceType(FACE_HAPPY);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"ֻ������Զ�����̵ĵط��Ų��ᷢ����Щ���°�");
			break;
		case 1:
			left->SetFaceType(FACE_NOTICE);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"��ô˵��������Ҳ�Ҳ���ʲô����");
			break;
		case 2:
			left->SetFaceType(FACE_DISAPPOINT);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"���ǸϽ��ҵ���ȥ��·��");
			break;
		case 3:
		{
			coldFrame = 120;
			auto boss = scene->AttachActor(new SunnyMilk);
			bossLife = 3;
			boss->transform->location = { 0,100,0 };
			boss->SetCharacterFlags(boss->GetCharacterFlags() | CharacterFlag::CF_ImmuneDamage);
			scene->AttachActor(new BossShowing(boss));
			left->Deactivate();
			left->SetFaceType(FACE_SURPRISE);
			SetBalloon(new Balloon_4({ 0,60 }, 4, false), L"������ô�ż��뿪��");
		}
		break;
		case 4:
		{
			bossNameEffect = scene->AttachActor(new BossNameSprite(0));
			right = scene->AttachActor(new Face_SunnyMilk);
			left->Deactivate();
			right->Activate();
			left->SetFaceType(FACE_SURPRISE);
			right->SetFaceType(FACE_HAPPY);
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"�þ�û���������ˣ��ҿ�׼���˺ö�Ķ�����...");
		}
		break;
		case 5:
			right->SetFaceType(FACE_SWEAT);
			SetBalloon(new Balloon_4({ 150,-30 }, 4, true), L"...���ԣ�������");
			break;
		case 6:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_NOTICE);
			right->SetFaceType(FACE_NOTICE);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"��Ȼ��ֻ��һֻ���������һ�������һ��");
			break;
		case 7:
			left->SetFaceType(FACE_SWEAT);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"��֪����ȥ��·��");
			break;
		case 8:
			left->Deactivate();
			right->Activate();
			left->SetFaceType(FACE_DISAPPOINT);
			right->SetFaceType(FACE_DISAPPOINT);
			SetBalloon(new Balloon_4({ 150,-30 }, 5, true), L"���������û����Ĭ�а�");
			break;
		case 9:
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"��Ȼ�����ˣ��Ǿͱ���һ�°ɣ���Ӯ���Ҿ͸����㣡");
			StopBgm();
			PlayBgm(bgm_grassland.Get());
			scene->AttachActor(new BgmText(L"��������ȥ������"));
			break;
		case 10:
			nowBoss->ShowMagicSquare();
			nowBoss->SetCharacterFlags(nowBoss->GetCharacterFlags() & ~CharacterFlag::CF_ImmuneDamage);
			nowBoss->AddComponent(new SunnyMilk_SC_1);
			player->SetPlayerFlags(player->GetPlayerFlags() & ~PF_PeaceLoving);
			Destroy();
			break;
		}
	}
};

class Dialog_Reimu_SunnyMilk_2 :public Dialog
{
	void StepChange(int step) override
	{
		switch (step)
		{
		case 0:
			left = scene->AttachActor(new Face_Reimu);
			right = scene->AttachActor(new Face_SunnyMilk);
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_SWEAT);
			right->SetFaceType(FACE_LOSE);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"����������������ɭ������Ĳ�������������");
			break;
		case 1:
			coldFrame = 120;
			left->SetFaceType(FACE_DISAPPOINT);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"���ˣ��Ѿ�û��ʱ�俼����Щ��");
			break;
		case 2:
			Destroy();
			ClearLevel();
			break;
		}
	}
};

class Dialog_Marisa_SunnyMilk :public Dialog
{
	void StepChange(int step) override
	{
		switch (step)
		{
		case 0:
			left = scene->AttachActor(new Face_Marisa);
			left->Activate();
			left->SetFaceType(FACE_PUZZLED);
			SetBalloon(new Balloon_1({ -150,-30 }, 7, false), L"��ʱ��·��ô����Ѿ������������Ѿ���ȫ�Ҳ�����ȥ��·��");
			break;
		case 1:
			left->SetFaceType(FACE_DISAPPOINT);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"��Ƭɭ�ִ��������˾��ù��찡");
			break;
		case 2:
		{
			coldFrame = 120;
			auto boss = scene->AttachActor(new SunnyMilk);
			bossLife = 3;
			boss->transform->location = { 0,100,0 };
			boss->SetCharacterFlags(boss->GetCharacterFlags() | CharacterFlag::CF_ImmuneDamage);
			scene->AttachActor(new BossShowing(boss));
			left->Deactivate();
			left->SetFaceType(FACE_SURPRISE);
			SetBalloon(new Balloon_4({ 0,60 }, 4, false), L"����ϸ�ĵ����డ~");
		}
		break;
		case 3:
		{
			bossNameEffect = scene->AttachActor(new BossNameSprite(0));
			right = scene->AttachActor(new Face_SunnyMilk);
			left->Deactivate();
			right->Activate();
			left->SetFaceType(FACE_SURPRISE);
			right->SetFaceType(FACE_DISAPPOINT);
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"������˵��������ô��ʹ����뿪����");
		}
		break;
		case 4:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_NOTICE2);
			right->SetFaceType(FACE_PUZZLED);
			SetBalloon(new Balloon_1({ -150,-30 }, 4, false), L"�Ѿ��ı�������");
			break;
		case 5:
			left->SetFaceType(FACE_HAPPY);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"�Ͼ�û�б�Ļ����־�����ǰ�������˷ܵ�����");
			break;
		case 6:
			left->Deactivate();
			right->Activate();
			left->SetFaceType(FACE_DISAPPOINT);
			right->SetFaceType(FACE_SURPRISE);
			SetBalloon(new Balloon_4({ 150,-30 }, 5, true), L"��ι������˵ʲô������ȫ������");
			break;
		case 7:
			right->SetFaceType(FACE_ANGRY);
			SetBalloon(new Balloon_4({ 150,-30 }, 4, true), L"һ����Ĭ�ж�û��");
			break;
		case 8:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_SWEAT);
			right->SetFaceType(FACE_PUZZLED);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"�����Ƕ���������ν�Ķ�������Ϊ��Ĭ��");
			break;
		case 9:
			left->SetFaceType(FACE_ANGRY);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"�����б�Ҫ����һ���ˣ�");
			StopBgm();
			PlayBgm(bgm_grassland.Get());
			scene->AttachActor(new BgmText(L"��������ȥ������"));
			break;
		case 10:
			nowBoss->ShowMagicSquare();
			nowBoss->SetCharacterFlags(nowBoss->GetCharacterFlags() & ~CharacterFlag::CF_ImmuneDamage);
			nowBoss->AddComponent(new SunnyMilk_SC_1);
			player->SetPlayerFlags(player->GetPlayerFlags() & ~PF_PeaceLoving);
			Destroy();
			break;
		}
	}
};

class Dialog_Marisa_SunnyMilk_2 :public Dialog
{
	void StepChange(int step) override
	{
		switch (step)
		{
		case 0:
			left = scene->AttachActor(new Face_Marisa);
			right = scene->AttachActor(new Face_SunnyMilk);
			left->Deactivate();
			right->Activate();
			left->SetFaceType(FACE_HAPPY);
			right->SetFaceType(FACE_LOSE);
			SetBalloon(new Balloon_4({ 150,-30 }, 5, true), L"���أ���Щ��ֻ���㿴���ļ�����~");
			break;
		case 1:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_NOTICE2);
			SetBalloon(new Balloon_1({ -150,-30 }, 4, false), L"·��Ȼͨ����");
			break;
		case 2:
			coldFrame = 120;
			left->SetFaceType(FACE_NOTICE);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"��������һ��Ӧ�ò�ֹ�����Ķ�������ô�򵥰�");
			break;
		case 3:
			Destroy();
			ClearLevel();
			break;
		}
	}
};