#include "Player.h"
#include "GameGlobal.h"
#include "GameScene.h"
#include "SoundEffect.h"
#include "Source.h"
#include "Effect.h"
#include "EnemyBullet.h"
#include "ItemDeriver.h"
#include "ControllerDeriver.h"
#include "GSObjects.h"

class Player::GodModeFrame :public AComponent
{
public:

	Player* player;

	int remain;

	GodModeFrame(Player* _player, int frame) :player(_player), remain(frame)
	{
		player->godMode++;
	}

	void OnUpdate() override
	{
		remain--;
		if (remain < 0)
			gameObject->RemoveComponent(this);
	}

	void OnRemoved()
	{
		player->godMode--;
	}
};

Player::Player(const AnimationGroup& _animGroup, const PlayerAttribution& _attribution) :
	Character(_animGroup), playerFlags(PF_PeaceLoving), attribution(_attribution), controller(0), movframe(0),
	lastDirection(0), borderOfLife(0), respawnFrame(0), coldFrame(0), godMode(0)
{
	player = this;
	transform->location = { 0,-200,0 };
}

void Player::Miss()
{
	OnMissed();
}

void Player::OnMissed()
{
	if (!godMode)
	{
		spellFailed = true;
		se_pldead.Play();
		playerFlags |= PF_Dying;
		borderOfLife = 8;
		AddGodModeFrame(300);
	}
}

void Player::OnRespawn()
{
	animGroup.SetAnimIndex(0);
	transform->location = { 0,-260,0 };
	CleanAllEnemyBullets(CLEAN_HIT, 60);
}

void Player::OnDead()
{
	float angle = atan2(300, -transform->location.x);
	for (int i = 0; i < 7; i++)
	{
		auto item = scene->AttachActor(new Power(true));
		item->transform->location = transform->location;
		item->target = transform->location + GetVector((float)(-M_PI_2 + M_PI / 3 + i * M_PI / 18 + angle), 150) + Vector3(0, 30, 0);
	}
	scene->AttachActor(new PlayerDeadCircle)->transform->location = transform->location;
	scene->AttachActor(new InvColorEffect(120, 0, true))->transform->location = transform->location + Vector3(-40, 0, 0);
	scene->AttachActor(new InvColorEffect(120, 0, true))->transform->location = transform->location + Vector3(40, 0, 0);
	scene->AttachActor(new InvColorEffect(120, 0, true))->transform->location = transform->location + Vector3(0, -40, 0);
	scene->AttachActor(new InvColorEffect(120, 0, true))->transform->location = transform->location + Vector3(0, 40, 0);
	scene->AttachActor(new InvColorEffect(120, 0, true))->transform->location = transform->location;
	scene->AttachActor(new InvColorEffect(40, 0, false))->transform->location = transform->location + Vector3(-40, 0, 0);
	scene->AttachActor(new InvColorEffect(40, 0, false))->transform->location = transform->location + Vector3(40, 0, 0);
	scene->AttachActor(new InvColorEffect(40, 0, false))->transform->location = transform->location + Vector3(0, -40, 0);
	scene->AttachActor(new InvColorEffect(40, 0, false))->transform->location = transform->location + Vector3(0, 40, 0);
	scene->AttachActor(new InvColorEffect(40, 0, false, 0, true))->transform->location = transform->location;
	scene->AttachActor(new InvColorEffect(120, -20, true, 24, false))->transform->location = transform->location;

	playerData.life--;
	playerData.bomb = 3;
	playerData.power -= 100;
	if (playerData.power < 0)
		playerData.power = 0;

	if (playerData.life < 0)
	{
		playerData.life = 0;
		if (!isReplaying)
		{
			ComPtr<IScene> sTemp = scene;
			scene->AttachActor(new DelayInvoke(Delegate([=]()->void {
				timer->SetTimeStopLevel(1);
				PauseBgm();
				sTemp->AttachActor(new GameOverMenu);
				}), 10));
		}
	}
}

void Player::OnBomb() {}

void Player::OnShoot() {}

void Player::OnUpdate()
{
	if (playerFlags & PlayerFlag::PF_Dying)
	{
		for (int i = 0; i < 3; i++)
			scene->AttachActor(new PlayerDeadSmallStar)->transform->location = transform->location;
		borderOfLife--;
		if (borderOfLife < 0)
		{
			playerFlags &= ~PlayerFlag::PF_Dying;
			playerFlags |= PlayerFlag::PF_Respawning;
			respawnFrame = 60;
			OnDead();
		}
	}
	else if (playerFlags & PlayerFlag::PF_Respawning)
	{
		if (respawnFrame >= 50)
		{
			for (int i = 0; i < 4; i++)
				scene->AttachActor(new PlayerDeadBigStar)->transform->location = transform->location;
			for (int i = 0; i < 3; i++)
				scene->AttachActor(new PlayerDeadSmallStar)->transform->location = transform->location;
		}
		respawnFrame--;
		if (respawnFrame < 0)
		{
			playerFlags &= ~PlayerFlag::PF_Respawning;
			playerFlags |= PlayerFlag::PF_Freezing;
			coldFrame = 40;
			OnRespawn();
		}
	}
	else if (playerFlags & PlayerFlag::PF_Freezing)
	{
		coldFrame--;
		transform->location.y += 1.5;
		if (coldFrame < 0)
		{
			playerFlags &= ~PlayerFlag::PF_Freezing;
		}
	}
	else
	{
		if (playerFlags & PlayerFlag::PF_SlowMode)
			transform->location += attribution.direction * (float)(timer->GetDeltaTime() * attribution.slowMoveSpeed);
		else
			transform->location += attribution.direction * (float)(timer->GetDeltaTime() * attribution.moveSpeed);
		if (transform->location.x < -296)
			transform->location.x = -296;
		if (transform->location.x > 296)
			transform->location.x = 296;
		if (transform->location.y < -210)
			transform->location.y = -210;
		if (transform->location.y > 210)
			transform->location.y = 210;
		if (attribution.direction.x < 0)
		{
			animGroup.SetAnimIndex(1);
			movframe = 4;
			lastDirection = -1;
		}
		else if (attribution.direction.x > 0)
		{
			animGroup.SetAnimIndex(2);
			movframe = 4;
			lastDirection = 1;
		}
		else
		{
			if (movframe > 0)
			{
				movframe--;
				if (lastDirection < 0)
					animGroup.SetAnimIndex(3);
				else if (lastDirection > 0)
					animGroup.SetAnimIndex(4);
			}
			else
			{
				animGroup.SetAnimIndex(0);
			}
		}

		if ((playerFlags & PlayerFlag::PF_Shooting) && !(playerFlags & PlayerFlag::PF_PeaceLoving))
		{
			OnShoot();
		}
	}
	if (godMode)
	{
		godModeColor++;
		if ((godModeColor / 2) % 2)
		{
			Material mat = sr->GetMaterial();
			mat.diffuse = { 0.5,0.5,1,1 };
			sr->SetMaterial(mat);
		}
		else
		{
			Material mat = sr->GetMaterial();
			mat.diffuse = { 1,1,1,1 };
			sr->SetMaterial(mat);
		}
	}
	else
	{
		Material mat = sr->GetMaterial();
		mat.diffuse = { 1,1,1,1 };
		sr->SetMaterial(mat);
	}
	if (playerFlags & PlayerFlag::PF_Dying || playerFlags & PlayerFlag::PF_Respawning)
		characterFlags |= CharacterFlag::CF_NoDraw;
	else
		characterFlags &= ~CharacterFlag::CF_NoDraw;
	if (transform->location.y > attribution.itemLine && !(playerFlags & PlayerFlag::PF_Respawning))
		CollectAllItems();
	__super::OnUpdate();
}

void Player::OnCreated()
{
	__super::OnCreated();
	InsertBefore(GameSceneCom::locator[9].Get());
	if (!isReplaying)
		controller = scene->AttachActor(new KeyController(this));
	else
		controller = scene->AttachActor(new ReplayController(this));
	controller->InsertBefore(this);
	scene->AttachActor(new SlowEffect(false, this));
	scene->AttachActor(new SlowEffect(true, this));
}

void Player::OnDestroy()
{
	__super::OnDestroy();
	controller->Destroy();
	if (player.Get() == this)
		player.Reset();
}

void Player::SetDirection(const Vector3& vec)
{
	attribution.direction = vec;
}

void Player::ShiftPress()
{
	playerFlags |= PlayerFlag::PF_SlowMode;
}

void Player::ShiftRelease()
{
	playerFlags &= ~PlayerFlag::PF_SlowMode;
}

void Player::ZPress()
{
	playerFlags |= PlayerFlag::PF_Shooting;
}

void Player::ZRelease()
{
	playerFlags &= ~PlayerFlag::PF_Shooting;
}

void Player::XPress()
{
	if (!(playerFlags & PF_UsingBomb) && !(playerFlags & PF_Respawning) && !(playerFlags & PF_Freezing)
		&& playerData.bomb > 0 && !(playerFlags & PlayerFlag::PF_PeaceLoving))
	{
		spellFailed = true;
		playerData.bomb--;
		playerFlags |= PlayerFlag::PF_UsingBomb;
		playerFlags &= ~PlayerFlag::PF_Dying;
		OnBomb();
	}
}

void Player::XRelease()
{
	
}

PlayerAttribution& Player::GetAttribution()
{
	return attribution;
}

void Player::SetPlayerFlags(DWORD flags)
{
	playerFlags = flags;
}

DWORD Player::GetPlayerFlags() const
{
	return playerFlags;
}

void Player::AddGodModeFrame(int frame)
{
	AddComponent(new GodModeFrame(this, frame));
}

bool Player::IsGodMode()
{
	return godMode > 0;
}

Controller* Player::GetController()
{
	return controller;
}

SlowEffect::SlowEffect(bool _clockWise, Player* _player) :
	clockWise(_clockWise), player(_player), sr(AddComponent(new SpriteRender)), slowFrame(0), fastFrame(8), slowMode(false)
{
	mat.ambient = { 0,0,0,0 };
	mat.diffuse = { 1,1,1,0 };
	mat.power = 0;
	mat.specular = { 0,0,0,0 };
	sr->SetMaterial(mat);
	sr->SetTexture(src_slowEffect.Get());
	sr->SetSourceRect({ 0,0,0.5,1 });
	transform->scale = { 64,64,1 };
}

void SlowEffect::OnCreated()
{
	InsertBefore(GameSceneCom::locator[19].Get());
}

void SlowEffect::OnDestroy()
{
	player.Reset();
}

void SlowEffect::OnUpdate()
{
	if (clockWise)
		transform->Rotate({ 0,0,-0.05 }, false);
	else
		transform->Rotate({ 0,0,0.05 }, false);
	if (!(player->GetPlayerFlags() & PF_Respawning)&& !(player->GetPlayerFlags() & PF_Freezing))
	{
		transform->location = player->transform->location;
		if (player->GetPlayerFlags() & PF_SlowMode)
			slowMode = true;
		else
			slowMode = false;
	}
	if (slowMode)
	{
		fastFrame = 0;
		slowFrame++;
		if (slowFrame > 8)
			slowFrame = 8;
		if (clockWise)
			transform->scale = { 96 - 4.0f * slowFrame,96 - 4.0f * slowFrame ,1 };
		else
			transform->scale = { 32 + 4.0f * slowFrame,32 + 4.0f * slowFrame ,1 };
		mat.diffuse.a = 1;
		sr->SetMaterial(mat);
	}
	else
	{
		slowFrame = 0;
		fastFrame++;
		if (fastFrame > 8)
			fastFrame = 8;
		mat.diffuse.a = 1 - 0.125f * fastFrame;
		sr->SetMaterial(mat);
	}
}