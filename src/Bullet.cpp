#include "Bullet.h"
#include "SceneObjects.h"
#include "Mathf.h"

class Bullet::BulletRotator :public AComponent
{
public:

	Bullet* bullet;

	BulletRotator(Bullet* _bullet) :bullet(_bullet) {}

	void OnUpdate() override
	{
		if (bullet->autoRotateMode == AR_VELOCITY)
		{
			if (bullet->mb->velocity.GetLength() != 0)
				bullet->transform->SetEulerAngles({ 0,0,bullet->autoRotateOffset + atan2(bullet->mb->velocity.y, bullet->mb->velocity.x) - (float)M_PI_2 });
		}
		else if (bullet->autoRotateMode == AR_ROTATECCW)
		{
			bullet->transform->Rotate({ 0,0,bullet->autoRotateOffset + 0.05f }, false);
		}
		else if (bullet->autoRotateMode == AR_ROTATECW)
		{
			bullet->transform->Rotate({ 0,0,bullet->autoRotateOffset - 0.05f }, false);
		}
	}
};

Bullet::Bullet(BYTE _autoRotateMode) :
	autoRotateMode(_autoRotateMode), bulletFlags(0), mb(AddComponent(new MoveBody)),
	rotator(AddComponent(new BulletRotator(this))), sr(0), autoRotateOffset(0),
	cleanOnHit(true), cleanOnOutOfRange(true), outOfRangeTolerance(20), cleanOnBomb(true)
{
	mat.ambient = { 0,0,0,0 };
	mat.diffuse = { 1,1,1,1 };
	mat.power = 0;
	mat.specular = { 0,0,0,0 };
	sr = AddComponent(new SpriteRender);
	sr->SetMaterial(mat);
}

void Bullet::OnUpdate()
{
	if (bulletFlags & BulletFlag::BF_NoDraw)
		sr->SetActive(false);
	else
		sr->SetActive(true);
	if (!(bulletFlags & BulletFlag::BF_NoCheck))
		OnCheck();
	if (cleanOnOutOfRange && IsOutOfRange(transform->location, outOfRangeTolerance))
		Clean(CLEAN_OUTOFRANGE);
}

void Bullet::SetBulletFlags(DWORD flags)
{
	bulletFlags = flags;
}

DWORD Bullet::GetBulletFlags() const
{
	return bulletFlags;
}