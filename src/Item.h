#pragma once
#include "VirtualLib.h"
#include "UserComponent.h"
#include "GameGlobal.h"
#include "Rand.h"
#include "Mathf.h"

class Item :public GameObject
{
public:

	MoveBody* mb;

	Vector3 target;

	bool following;

	Item(bool _highLight, bool _roll, bool gravity = true, bool isPlayerDrop = false);

protected:

	SpriteRender* sr;

	GameObject* tag;

	bool isPlayerDrop;

	bool highLight;

	bool roll;

	int rollFrame;

	float checkRadius;

	Material mat;

	void OnCreated() override;

	void OnUpdate() override;

	void OnDestroy() override;

	virtual void OnEat() = 0;

	virtual GameObject* OnShowTag() = 0;
};

void CollectAllItems();

template <class T,class... Arg>
void DropItem(int num, const Vector2& loc, float range, Arg&&... arg)
{
	for (int i = 0; i < num; i++)
	{
		T* g = player->GetScene()->AttachActor(new T(std::forward<Arg>(arg)...));
		g->transform->location = loc + GetVector(RandomAngle(), Random(0.0f, range));
		g->mb->velocity = { 0,200,0 };
	}
}