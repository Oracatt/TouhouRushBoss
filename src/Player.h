#pragma once
#include "VirtualLib.h"
#include "Character.h"
#include "Controller.h"

/********************
AnimGroup:
0:��ֹ
1:����
2:����
3:����ֹͣ
4:����ֹͣ
********************/

struct PlayerAttribution
{
	float moveSpeed; //�ƶ��ٶ�
	float slowMoveSpeed; //����ģʽ�ƶ��ٶ�
	float itemLine; //���߻����߸߶�
	float checkRadius; //�ж��뾶
	float grazeRadius; //�����뾶
	float itemRadius; //���߻��հ뾶
	Vector3 direction; //��ǰ����
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
