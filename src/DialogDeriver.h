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
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"这又是什么奇怪的地方...");
			break;
		case 1:
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"一眨眼的功夫居然到了晚上，究竟是谁在搞鬼啊");
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
			SetBalloon(new Balloon_4({ 0,60 }, 5, false), L"这是哪位迷路的小朋友？");
		}
			break;
		case 3:
			bossNameEffect = scene->AttachActor(new BossNameSprite(2));
			right = scene->AttachActor(new Face_Artia);
			right->Activate();
			right->SetFaceType(FACE_HAPPY);
			SetBalloon(new Balloon_4({ 150,-30 }, 5, true), L"啊呀，没想到竟然是灵梦小姐");
			break;
		case 4:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_DISAPPOINT);
			right->SetFaceType(FACE_NOTICE);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"嗯？我之前可从来不认识你");
			break;
		case 5:
			left->Deactivate();
			right->Activate();
			right->SetFaceType(FACE_HAPPY);
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"没关系，在我们那里，灵梦小姐可是人尽皆知呢");
			break;
		case 6:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_NOTICE);
			right->SetFaceType(FACE_NOTICE);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"除了幻想乡，那就是外面的世界喽");
			break;
		case 7:
			left->Deactivate();
			right->Activate();
			left->SetFaceType(FACE_DISAPPOINT);
			right->SetFaceType(FACE_SWEAT);
			SetBalloon(new Balloon_4({ 150,-30 }, 3, true), L"那倒也不是");
			break;
		case 8:
			right->SetFaceType(FACE_DISAPPOINT);
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"事实上，外面的世界是什么样子，我也没有见过");
			break;
		case 9:
			left->SetFaceType(FACE_SURPRISE);
			right->SetFaceType(FACE_HAPPY);
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"但是我知道，外面的那些人们，现在正在屏幕上看着我们呢");
			break;
		case 10:
			left->Activate();
			right->Deactivate();
			right->SetFaceType(FACE_NOTICE);
			SetBalloon(new Balloon_1({ -150,-30 }, 4, false), L"我们？现在？");
			break;
		case 11:
			left->Deactivate();
			right->Activate();
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"没错，而且我已经感受到观众们的兴奋了");
			break;
		case 12:
			right->SetFaceType(FACE_HAPPY);
			SetBalloon(new Balloon_4({ 150,-30 }, 5, true), L"这将是一次无比精彩的直播！");
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
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"好冷啊...这诡异的天气");
			break;
		case 1:
			left->SetFaceType(FACE_ANGRY);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"这附近一定有什么人在作妖吧");
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
			SetBalloon(new Balloon_4({ 0,60 }, 5, false), L"骑着扫帚的家伙？难道是...");
		}
		break;
		case 3:
			bossNameEffect = scene->AttachActor(new BossNameSprite(2));
			right = scene->AttachActor(new Face_Artia);
			right->Activate();
			right->SetFaceType(FACE_HAPPY);
			SetBalloon(new Balloon_4({ 150,-30 }, 5, true), L"啊哈！果然和我想的一样");
			break;
		case 4:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_DISAPPOINT);
			right->SetFaceType(FACE_NOTICE);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"等等，你是不是应该先自我介绍一下？");
			break;
		case 5:
			left->Deactivate();
			right->Activate();
			left->SetFaceType(FACE_NOTICE);
			right->SetFaceType(FACE_HAPPY);
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"没时间了，外面的观众们都已经等不及了！");
			break;
		case 6:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_SWEAT);
			right->SetFaceType(FACE_NOTICE);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"什么观众？这里明明只有我们两个人");
			break;
		case 7:
			left->SetFaceType(FACE_ANGRY);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"不管你是谁，这鬼天气跟你一定脱不了关系吧");
			break;
		case 8:
			left->Deactivate();
			right->Activate();
			right->SetFaceType(FACE_ANGRY);
			SetBalloon(new Balloon_4({ 150,-30 }, 5, true), L"原来是个来找麻烦的家伙...");
			break;
		case 9:
			right->SetFaceType(FACE_ANGRY2);
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"只可惜，只有胜利者才有资格掌控主场！");
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
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"观众们早就期待一场魔法间的对决了，拿出你的全部实力吧！");
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
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"河面上暂时也没发现什么异样...");
			break;
		case 1:
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"难道之前都是我的错觉吗？");
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
			SetBalloon(new Balloon_1({ -150,-30 }, 4, false), L"什么人？");
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
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"这年头，石头都成精了吗");
			break;
		case 5:
			left->Deactivate();
			right->Activate();
			left->SetFaceType(FACE_SURPRISE);
			SetBalloon(new Balloon_4({ 150,-30 }, 5, true), L"小家伙，注意你说话的口气！");
			break;
		case 6:
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"这条河可不是人人都能过去的！");
			break;
		case 7:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_NOTICE);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"原来还是一块会说话的石头啊");
			break;
		case 8:
			left->Deactivate();
			right->Activate();
			left->SetFaceType(FACE_DISAPPOINT);
			SetBalloon(new Balloon_4({ 150,-30 }, 4, true), L"我可不是普通的石头！");
			break;
		case 9:
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"想过这条河，先把身上值钱的东西都交出来吧！");
			break;
		case 10:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_NOTICE);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"值钱的东西都留在神社，没有带出来呢");
			break;
		case 11:
			left->SetFaceType(FACE_ANGRY);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"正好让我先制止你的胡作非为吧！");
			break;
		case 12:
			nowBoss->ShowMagicSquare();
			nowBoss->SetCharacterFlags(nowBoss->GetCharacterFlags() & ~CharacterFlag::CF_ImmuneDamage);
			nowBoss->AddComponent(new Monstone_SC_1);
			player->SetPlayerFlags(player->GetPlayerFlags() & ~PF_PeaceLoving);
			Destroy();
			StopBgm();
			PlayBgm(bgm_riverside.Get());
			scene->AttachActor(new BgmText(L"恩惠Summer Rain"));
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
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"在情况变得更糟之前，还是尽快解决主要的问题吧");
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
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"总感觉下一秒就会有大事发生");
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
			SetBalloon(new Balloon_4({ 0,60 }, 4, false), L"预感挺准嘛，小家伙");
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
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"识相的话就赶紧把值钱的东西都交出来！");
		}
		break;
		case 3:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_DISAPPOINT);
			SetBalloon(new Balloon_1({ -150,-30 }, 4, false), L"我说可不是这个...");
			break;
		case 4:
			left->SetFaceType(FACE_SWEAT);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"不过被一块石头打劫确实是在意料之外的");
			break;
		case 5:
			left->Deactivate();
			right->Activate();
			left->SetFaceType(FACE_NOTICE);
			SetBalloon(new Balloon_4({ 150,-30 }, 5, true), L"想过这条河，要么交出过路费...");
			break;
		case 6:
			SetBalloon(new Balloon_4({ 150,-30 }, 5, true), L"要么就被我丢下河里喂鱼吧！");
			break;
		case 7:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_SWEAT);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"你只是一块石头，要再多的钱财又有什么用呢");
			break;
		case 8:
			left->SetFaceType(FACE_ANGRY);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"还是先让我把你送回你该去的地方吧");
			break;
		case 9:
			nowBoss->ShowMagicSquare();
			nowBoss->SetCharacterFlags(nowBoss->GetCharacterFlags() & ~CharacterFlag::CF_ImmuneDamage);
			nowBoss->AddComponent(new Monstone_SC_1);
			player->SetPlayerFlags(player->GetPlayerFlags() & ~PF_PeaceLoving);
			Destroy();
			StopBgm();
			PlayBgm(bgm_riverside.Get());
			scene->AttachActor(new BgmText(L"恩惠Summer Rain"));
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
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"居然和一块石头浪费了这么久的时间");
			break;
		case 1:
			coldFrame = 120;
			left->SetFaceType(FACE_PUZZLED);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"还是抓紧时间赶路吧");
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
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"只有这种远离人烟的地方才不会发生那些怪事吧");
			break;
		case 1:
			left->SetFaceType(FACE_NOTICE);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"这么说，来这里也找不到什么线索");
			break;
		case 2:
			left->SetFaceType(FACE_DISAPPOINT);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"还是赶紧找到出去的路吧");
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
			SetBalloon(new Balloon_4({ 0,60 }, 4, false), L"干嘛这么着急离开啊");
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
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"好久没遇到人类了，我可准备了好多的恶作剧...");
		}
		break;
		case 5:
			right->SetFaceType(FACE_SWEAT);
			SetBalloon(new Balloon_4({ 150,-30 }, 4, true), L"...不对，是礼物");
			break;
		case 6:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_NOTICE);
			right->SetFaceType(FACE_NOTICE);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"虽然你只是一只妖精，但我还是想问一下");
			break;
		case 7:
			left->SetFaceType(FACE_SWEAT);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"你知道出去的路吗");
			break;
		case 8:
			left->Deactivate();
			right->Activate();
			left->SetFaceType(FACE_DISAPPOINT);
			right->SetFaceType(FACE_DISAPPOINT);
			SetBalloon(new Balloon_4({ 150,-30 }, 5, true), L"人类可真是没有幽默感啊");
			break;
		case 9:
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"既然都来了，那就比试一下吧，打赢了我就告诉你！");
			StopBgm();
			PlayBgm(bgm_grassland.Get());
			scene->AttachActor(new BgmText(L"赌上性命去恶作剧"));
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
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"像你这种妖精待在森林里真的不会引发火灾吗");
			break;
		case 1:
			coldFrame = 120;
			left->SetFaceType(FACE_DISAPPOINT);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"算了，已经没有时间考虑这些了");
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
			SetBalloon(new Balloon_1({ -150,-30 }, 7, false), L"来时的路这么快就已经长满了树，已经完全找不到出去的路了");
			break;
		case 1:
			left->SetFaceType(FACE_DISAPPOINT);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"这片森林处处都让人觉得诡异啊");
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
			SetBalloon(new Balloon_4({ 0,60 }, 4, false), L"真是细心的人类啊~");
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
			SetBalloon(new Balloon_4({ 150,-30 }, 6, true), L"不过话说回来，这么快就打算离开了吗");
		}
		break;
		case 4:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_NOTICE2);
			right->SetFaceType(FACE_PUZZLED);
			SetBalloon(new Balloon_1({ -150,-30 }, 4, false), L"已经改变主意了");
			break;
		case 5:
			left->SetFaceType(FACE_HAPPY);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"毕竟没有比幕后黑手就在眼前更让人兴奋的事了");
			break;
		case 6:
			left->Deactivate();
			right->Activate();
			left->SetFaceType(FACE_DISAPPOINT);
			right->SetFaceType(FACE_SURPRISE);
			SetBalloon(new Balloon_4({ 150,-30 }, 5, true), L"啊喂，你在说什么啊，完全听不懂");
			break;
		case 7:
			right->SetFaceType(FACE_ANGRY);
			SetBalloon(new Balloon_4({ 150,-30 }, 4, true), L"一点幽默感都没有");
			break;
		case 8:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_SWEAT);
			right->SetFaceType(FACE_PUZZLED);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"妖精们都把这种所谓的恶作剧视为幽默吗");
			break;
		case 9:
			left->SetFaceType(FACE_ANGRY);
			SetBalloon(new Balloon_1({ -150,-30 }, 5, false), L"看来有必要退治一下了！");
			StopBgm();
			PlayBgm(bgm_grassland.Get());
			scene->AttachActor(new BgmText(L"赌上性命去恶作剧"));
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
			SetBalloon(new Balloon_4({ 150,-30 }, 5, true), L"呜呜，那些都只是你看到的假象啦~");
			break;
		case 1:
			left->Activate();
			right->Deactivate();
			left->SetFaceType(FACE_NOTICE2);
			SetBalloon(new Balloon_1({ -150,-30 }, 4, false), L"路果然通了呢");
			break;
		case 2:
			coldFrame = 120;
			left->SetFaceType(FACE_NOTICE);
			SetBalloon(new Balloon_1({ -150,-30 }, 6, false), L"不过，这一切应该不止妖精的恶作剧这么简单吧");
			break;
		case 3:
			Destroy();
			ClearLevel();
			break;
		}
	}
};