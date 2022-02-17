#pragma once
#include "VirtualLib.h"
#include "UserComponent.h"
#include "Mathf.h"

class BaseObject :public GameObject
{
public:

	SpriteRender* sr;

	Material mat;

	BaseObject() :
		sr(AddComponent(new SpriteRender))
	{
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,1 };
		mat.specular = { 0,0,0,0 };
		mat.power = 0;
		sr->SetMaterial(mat);
	}
};

class AnimObject :public BaseObject
{
	class LeaveEnter :public AComponent
	{
	public:

		AnimObject* object;

		float oldAlpha;

		float targetAlpha;

		int maxFrame;

		int frame;

		int glFrame;

		bool leaving;

		bool entering;

		Color oldGlColor;

		bool isGlAmbient;

		int shakeFrame;

		float shakeRange;

		XMMATRIX localmat;

		LeaveEnter(AnimObject* obj) :
			object(obj), maxFrame(0), frame(-1), leaving(false), oldAlpha(1), entering(false), glFrame(-1),
			isGlAmbient(false), shakeFrame(-1), shakeRange(0), targetAlpha(0), localmat(XMMatrixIdentity())
		{
		}

	private:

		void OnUpdate() override
		{
			if (shakeFrame >= 0)
			{
				XMMATRIX mat = localmat;
				if (shakeFrame > 3)
					mat = XMMatrixTranslation(shakeRange * (7 - shakeFrame), 0, 0) * mat;
				else
					mat = XMMatrixTranslation(shakeRange * shakeFrame, 0, 0) * mat;
				object->sr->SetLocalTransform(mat);
				shakeFrame--;
			}
			if (glFrame > 0)
			{
				if (isGlAmbient)
				{
					if ((glFrame / 2) % 2)
						object->mat.ambient.r = object->mat.ambient.g = object->mat.ambient.b = 0.5;
					else
						object->mat.ambient.r = object->mat.ambient.g = object->mat.ambient.b = 1;
				}
				else
				{
					if ((glFrame / 2) % 2)
						object->mat.diffuse.r = object->mat.diffuse.g = object->mat.diffuse.b = 0.5;
					else
						object->mat.diffuse.r = object->mat.diffuse.g = object->mat.diffuse.b = 1;
				}
				object->sr->SetMaterial(object->mat);
				glFrame--;
			}
			else if (glFrame == 0)
			{
				if (isGlAmbient)
					object->mat.ambient = oldGlColor;
				else
					object->mat.diffuse = oldGlColor;
				object->sr->SetMaterial(object->mat);
				glFrame--;
			}
			if (frame >= 0)
			{
				if (leaving)
				{
					object->mat.diffuse.a = Lerp(oldAlpha, targetAlpha, (maxFrame - frame) / (float)maxFrame);
					object->sr->SetMaterial(object->mat);
				}
				else if (entering)
				{
					object->mat.diffuse.a = Lerp(oldAlpha, targetAlpha, (maxFrame - frame) / (float)maxFrame);
					object->sr->SetMaterial(object->mat);
				}
				frame--;
			}
			else if (leaving)
			{
				object->Destroy();
			}
		}
	};

public:

	AnimObject() :
		leaveEnter(AddComponent(new LeaveEnter(this))), isLeft(false)
	{
	}

	bool IsLeft()
	{
		return isLeft;
	}

	void AnimSetLocalTransform(const XMMATRIX& mat)
	{
		sr->SetLocalTransform(mat);
		leaveEnter->localmat = mat;
	}

	virtual void Enter(int frame, float _targetAlpha = 1)
	{
		if (!isLeft)
		{
			leaveEnter->frame = frame;
			leaveEnter->maxFrame = frame;
			leaveEnter->entering = true;
			leaveEnter->leaving = false;
			leaveEnter->oldAlpha = mat.diffuse.a;
			leaveEnter->targetAlpha = _targetAlpha;
		}
	}

	virtual void Leave(int frame, float _targetAlpha = 0)
	{
		if (!isLeft)
		{
			isLeft = true;
			leaveEnter->frame = frame;
			leaveEnter->maxFrame = frame;
			leaveEnter->entering = false;
			leaveEnter->leaving = true;
			leaveEnter->oldAlpha = mat.diffuse.a;
			leaveEnter->targetAlpha = _targetAlpha;
		}
	}

	virtual void GListen(bool ambient)
	{
		if (!isLeft)
		{
			leaveEnter->entering = false;
			leaveEnter->leaving = false;
			leaveEnter->glFrame = 20;
			leaveEnter->isGlAmbient = ambient;
			if (ambient)
				leaveEnter->oldGlColor = mat.ambient;
			else
				leaveEnter->oldGlColor = mat.diffuse;
		}
	}

	virtual void Shake(float range = 0.01f)
	{
		leaveEnter->shakeFrame = 6;
		leaveEnter->shakeRange = range;
	}

protected:

	LeaveEnter* leaveEnter;

private:

	bool isLeft;
};

class MovingObject :public AnimObject
{
private:

	class PrivateMoveBody :public MoveBody
	{
	public:

		PrivateMoveBody(MovingObject* obj) :object(obj)
		{
		}

	private:

		MovingObject* object;

		void OnUpdate() override
		{
			float deltaTime = 1 / 60.0f;
			if (object->moving)
			{
				float distance = (object->targetPosition - object->transform->location).GetLength();
				if (distance < object->moveSpeed * deltaTime)
				{
					object->transform->location = object->targetPosition;
					object->moving = false;
				}
				else
				{
					object->transform->location += (object->targetPosition - object->transform->location).GetNormalized() * object->moveSpeed * deltaTime;
					float value = 1 - distance / object->targetMaxDistance;
					object->moveSpeed = Lerp(object->maxSpeed, object->minSpeed, value);
				}
			}
			else
			{
				__super::OnUpdate();
			}
		}
	};

public:

	MoveBody* mb;

	MovingObject() :mb(AddComponent(new PrivateMoveBody(this))), moveSpeed(0), maxSpeed(0),
		minSpeed(0), targetMaxDistance(0), moving(false)
	{
	}

	virtual void Move(const Vector2& pos, float maxspeed, float minspeed)
	{
		moving = true;
		targetPosition = pos;
		maxSpeed = maxspeed;
		minSpeed = minspeed;
		moveSpeed = maxSpeed;
		targetMaxDistance = (targetPosition - transform->location).GetLength();
	}

	virtual void Move(const Vector2& pos, float minspeed = 50)
	{
		Move(pos, fmax((transform->location - pos).GetLength(), minspeed), minspeed);
	}

private:

	bool moving;

	float moveSpeed;

	float maxSpeed;

	float minSpeed;

	Vector2 targetPosition;

	float targetMaxDistance;
};

class TransformingObject :public AnimObject
{
	class Transformer :public AComponent
	{
	public:

		int frame;

		int maxFrame;

		int rollFrame;

		int maxRollFrame;

		Vector3 oldScale;

		Vector3 oldLoc;

		Vector3 targetScale;

		Vector3 targetLoc;

		Euler oldEuler;

		Euler deltaEuler;

		Transformer(TransformingObject* obj) :object(obj), frame(-1), maxFrame(-1), rollFrame(-1), maxRollFrame(-1)
		{
		}

	private:

		TransformingObject* object;

		void OnUpdate() override
		{
			if (frame >= 0)
			{
				object->transform->location = Lerp(oldLoc, targetLoc, (maxFrame - frame) / (float)maxFrame);
				object->transform->scale = Lerp(oldScale, targetScale, (maxFrame - frame) / (float)maxFrame);
				frame--;
			}
			if (rollFrame >= 0)
			{
				object->transform->SetEulerAngles(Lerp(oldEuler, oldEuler + deltaEuler, (maxRollFrame - rollFrame) / (float)maxRollFrame));
				rollFrame--;
			}
		}
	};

public:

	TransformingObject() :transformer(AddComponent(new Transformer(this)))
	{
	}

	virtual void Transform(const Vector3& _targetScale, const Vector3& _targetLoc, int _frame)
	{
		transformer->maxFrame = _frame;
		transformer->frame = _frame;
		transformer->oldScale = transform->scale;
		transformer->oldLoc = transform->location;
		transformer->targetScale = _targetScale;
		transformer->targetLoc = _targetLoc;
	}

	virtual void Roll(const Euler& deltaEuler, int _frame)
	{
		transformer->maxRollFrame = _frame;
		transformer->rollFrame = _frame;
		transformer->deltaEuler = deltaEuler;
		transformer->oldEuler = transform->GetEulerAngles();
	}

protected:

	Transformer* transformer;
};

class SimpleMovingObject :public GameObject
{
private:

	class PrivateMoveBody :public MoveBody
	{
	public:

		PrivateMoveBody(SimpleMovingObject* obj) :object(obj)
		{
		}

	private:

		SimpleMovingObject* object;

		void OnUpdate() override
		{
			float deltaTime = 1 / 60.0f;
			if (object->moving)
			{
				float distance = (object->targetPosition - object->transform->location).GetLength();
				if (distance < object->moveSpeed * deltaTime)
				{
					object->transform->location = object->targetPosition;
					object->moving = false;
				}
				else
				{
					object->transform->location += (object->targetPosition - object->transform->location).GetNormalized() * object->moveSpeed * deltaTime;
					float value = 1 - distance / object->targetMaxDistance;
					object->moveSpeed = Lerp(object->maxSpeed, object->minSpeed, value);
				}
			}
			else
			{
				__super::OnUpdate();
			}
		}
	};

public:

	MoveBody* mb;

	SimpleMovingObject() :mb(AddComponent(new PrivateMoveBody(this))), moveSpeed(0), maxSpeed(0),
		minSpeed(0), targetMaxDistance(0), moving(false)
	{
	}

	virtual void Move(const Vector2& pos, float maxspeed, float minspeed)
	{
		moving = true;
		targetPosition = pos;
		maxSpeed = maxspeed;
		minSpeed = minspeed;
		moveSpeed = maxSpeed;
		targetMaxDistance = (targetPosition - transform->location).GetLength();
	}

	virtual void Move(const Vector2& pos, float minspeed = 50)
	{
		Move(pos, fmax((transform->location - pos).GetLength(), minspeed), minspeed);
	}

private:

	bool moving;

	float moveSpeed;

	float maxSpeed;

	float minSpeed;

	Vector2 targetPosition;

	float targetMaxDistance;
};