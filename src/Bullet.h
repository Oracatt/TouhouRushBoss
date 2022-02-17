#pragma once
#include "VirtualLib.h"
#include "UserComponent.h"
#include "Character.h"

#define AR_NONE 0
#define AR_VELOCITY 1
#define AR_ROTATECW 2
#define AR_ROTATECCW 3

#define CLEAN_HIT 0
#define CLEAN_OUTOFRANGE 1
#define CLEAN_BONUS 2
#define CLEAN_BOMB 3

#define CLEAN_NOSPELL 4
#define CLEAN_SPELL 5
#define CLEAN_FINALSPELL 6

enum BulletFlag
{
	BF_NoCheck = 1,
	BF_NoDraw = 2
};

class Bullet:public GameObject
{
public:

	MoveBody* mb;

	bool cleanOnHit;

	bool cleanOnBomb;

	bool cleanOnOutOfRange;

	float outOfRangeTolerance;

	Bullet(BYTE _autoRotateMode);

	virtual void Clean(BYTE cleanType) = 0;

	void SetBulletFlags(DWORD flags);

	DWORD GetBulletFlags() const;

	class BulletRotator;

protected:

	float autoRotateOffset;

	BYTE autoRotateMode;

	DWORD bulletFlags;

	BulletRotator* rotator;

	SpriteRender* sr;

	Material mat;

	virtual void OnCheck() = 0;

	virtual void OnHit(Character* character) = 0;

	void OnUpdate() override;
};