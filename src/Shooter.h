#pragma once
#include "EnemyBullet.h"
#include "Effect.h"
#include "GameGlobal.h"
#include <functional>

template<class T,class... Arg>
void ShootAround(Arg... arg, int num, const Vector3& loc, float speed, float startrad = 0, float offset = 0, function<void(T*)> func = 0)
{
	for (int i = 0; i < num; i++)
	{
		auto d = new T(forward<Arg>(arg)...);
		d->transform->location = loc + GetVector(startrad + float(i * M_PI * 2 / num), offset);
		d->mb->velocity = GetVector(startrad + float(i * M_PI * 2 / num), speed);
		if (func)
			func(d);
		player->GetScene()->AttachActor(new EnemyBulletFog(d));
	}
}

template<class T, class... Arg>
void ShootSnipe(Arg&&... arg, int num, const Vector3& loc, float speed, float startrad, float deltarad = 0, float offset = 0, function<void(T*)> func = 0)
{
	for (int i = 0; i < num; i++)
	{
		float angle = startrad - (num - 1) * deltarad / 2 + deltarad * i;
		auto d = new T(forward<Arg>(arg)...);
		d->transform->location = loc + GetVector(angle, offset);
		d->mb->velocity = GetVector(angle, speed);
		if (func)
			func(d);
		player->GetScene()->AttachActor(new EnemyBulletFog(d));
	}
}

class Shooter :public EnemyBullet
{
public:
	
	Shooter(bool _highLight = false);

	void Clean(BYTE cleanType);

protected:

	int frame;

	void OnCheck() override;

	void OnGrazed() override;

	virtual void OnShoot() = 0;

private:

	void OnUpdate() override;

	void SetColor(int c) override;
};