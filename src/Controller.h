#pragma once
#include "VirtualLib.h"
#include "EngineGlobal.h"

using namespace Global;

__interface IControllable
{
	virtual void SetDirection(const Vector3& vec) = 0;

	virtual void ShiftPress() = 0;

	virtual void ShiftRelease() = 0;

	virtual void ZPress() = 0;

	virtual void ZRelease() = 0;

	virtual void XPress() = 0;

	virtual void XRelease() = 0;
};

class Controller :public GameObject
{
public:

	virtual bool IsZPressed() = 0;

	virtual bool IsLControlDown() = 0;

protected:

	IControllable* controllable;

	Controller(IControllable* _controllable) :controllable(_controllable) {}
};
