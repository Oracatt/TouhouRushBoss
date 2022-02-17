#pragma once
#include "VirtualLib.h"
#include "EngineGlobal.h"

using namespace Global;

class MoveBody :public AComponent
{
public:

	Vector3 velocity;

	Vector3 force;

	Vector3 drag;

	MoveBody() :velocity(0, 0, 0), force(0, 0, 0), drag(0, 0, 0) {}

	void OnUpdate() override
	{
		double deltaTime = timer->GetDeltaTime();
		float sqv2 = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
		Vector3 d = Vector3(drag.x * sqv2 * velocity.x, drag.y * sqv2 * velocity.y, drag.z * sqv2 * velocity.z) / 100;
		velocity += (force - d) * deltaTime;
		gameObject->transform->location += velocity * deltaTime;
	}

private:

	float sgn(float x)
	{
		if (x < 0)
			return -1;
		else
			return 1;
	}
};