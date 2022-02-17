#pragma once
#include "VirtualLib.h"
#include "Character.h"
#include "Controller.h"

/********************
AnimGroup:
0:静止
1:左移
2:右移
3:左移停止
4:右移停止
********************/

struct PlayerAttribution
{
	float moveSpeed; //移动速度
	float slowMoveSpeed; //低速模式移动速度
	float itemLine; //道具回收线高度
	float checkRadius; //判定半径
	float grazeRadius; //擦弹半径
	float itemRadius; //道具回收半径
	Vector3 direction; //当前方向
};

enum PlayerFlag
{
	PF_UsingBomb = 1,
	PF_Shooting = 2,
	PF_ReadyToBomb = 4,
	PF_Dying = 8,
	PF_Respawning = 16,
	PF_SlowMode = 32,
	PF_Freezing = 64,
	PF_PeaceLoving = 128
};

class Player :public Character, public IControllable
{
public:

	Player(const AnimationGroup& _animGroup, const PlayerAttribution& _attribution);

	void Miss();

	PlayerAttribution& GetAttribution();

	void SetPlayerFlags(DWORD flags);

	DWORD GetPlayerFlags() const;

	void AddGodModeFrame(int frame);

	bool IsGodMode();

	Controller* GetController();

	class GodModeFrame;

protected:

	DWORD playerFlags;

	PlayerAttribution attribution;

	Controller* controller;

	int godMode;

	int godModeColor;

	virtual void OnMissed();

	virtual void OnDead();

	virtual void OnRespawn();

	virtual void OnBomb();

	virtual void OnShoot();

	void OnUpdate() override;

	void OnCreated() override;

	void OnDestroy() override;

	void SetDirection(const Vector3& vec) override;

	void ShiftPress() override;

	void ShiftRelease() override;

	void ZPress() override;

	void ZRelease() override;

	void XPress() override;

	void XRelease() override;

private:

	int movframe;

	int lastDirection;

	int borderOfLife;

	int respawnFrame;

	int coldFrame;
};

class SlowEffect :public GameObject
{
public:

	SlowEffect(bool _clockWise, Player* _player);

	void OnCreated() override;

	void OnDestroy() override;

	void OnUpdate() override;

private:

	bool clockWise;

	ComPtr<Player> player;

	SpriteRender* sr;

	int slowFrame;

	int fastFrame;

	Material mat;

	bool slowMode;
};
