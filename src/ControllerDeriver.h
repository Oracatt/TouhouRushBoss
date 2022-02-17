#pragma once
#include "Controller.h"
#include "Replay.h"

class KeyController :public Controller
{
public:

	KeyController(IControllable* _controllable) :Controller(_controllable), zpressed(false), lcontroldown(false) 
	{
	}

	void OnUpdate() override
	{
		Sequence seq = SQ_NONE;
		if (input->KeyPress(DIK_LSHIFT))
		{
			controllable->ShiftPress();
			seq |= SQ_LSHIFTPRESS;
		}
		else if (input->KeyRelease(DIK_LSHIFT))
		{
			controllable->ShiftRelease();
			seq |= SQ_LSHIFTRELEASE;
		}
		if (input->KeyPress(DIK_Z))
		{
			controllable->ZPress();
			seq |= SQ_ZPRESS;
		}
		else if (input->KeyRelease(DIK_Z))
		{
			controllable->ZRelease();
			seq |= SQ_ZRELEASE;
		}
		if (input->KeyPress(DIK_X))
		{
			controllable->XPress();
			seq |= SQ_XPRESS;
		}
		else if (input->KeyRelease(DIK_X))
		{
			controllable->XRelease();
			seq |= SQ_XRELEASE;
		}
		int x = 0, y = 0;
		if (input->KeyDown(DIK_LEFT))
		{
			x--;
			seq |= SQ_LEFTDOWN;
		}
		if (input->KeyDown(DIK_RIGHT))
		{
			x++;
			seq |= SQ_RIGHTDOWN;
		}
		if (input->KeyDown(DIK_UP))
		{
			y++;
			seq |= SQ_UPDOWN;
		}
		if (input->KeyDown(DIK_DOWN))
		{
			y--;
			seq |= SQ_DOWNDOWN;
		}
		if (x || y)
			controllable->SetDirection(Vector3(x, y, 0).GetNormalized());
		else
			controllable->SetDirection({ 0, 0 ,0 });

		zpressed = input->KeyPress(DIK_Z);
		lcontroldown = input->KeyDown(DIK_LCONTROL);
		if (lcontroldown)
			seq |= SQ_LCTRLDOWN;
		if (recording)
			currentPage.AddSequence(seq);
	}

	bool IsZPressed() override
	{
		return zpressed;
	}

	bool IsLControlDown() override
	{
		return lcontroldown;
	}

private:

	bool zpressed;

	bool lcontroldown;
};

class ReplayController :public Controller
{
public:

	ReplayController(IControllable* _controllable) :
		Controller(_controllable), zpressed(false), lcontroldown(false) 
	{
	}

	void OnUpdate() override
	{
		Sequence seq = currentPage.ReadSequence();
		if (seq & SQ_LSHIFTPRESS)
			controllable->ShiftPress();
		else if (seq & SQ_LSHIFTRELEASE)
			controllable->ShiftRelease();
		if (seq & SQ_ZPRESS)
			controllable->ZPress();
		else if (seq & SQ_ZRELEASE)
			controllable->ZRelease();
		if (seq & SQ_XPRESS)
			controllable->XPress();
		else if (seq & SQ_XRELEASE)
			controllable->XRelease();
		int x = 0, y = 0;
		if (seq & SQ_LEFTDOWN)
			x--;
		if (seq & SQ_RIGHTDOWN)
			x++;
		if (seq & SQ_UPDOWN)
			y++;
		if (seq & SQ_DOWNDOWN)
			y--;
		if (x || y)
			controllable->SetDirection(Vector3(x, y, 0).GetNormalized());
		else
			controllable->SetDirection({ 0, 0 ,0 });

		zpressed = (seq & SQ_ZPRESS);
		lcontroldown = (seq & SQ_LCTRLDOWN);

		if (seq & SQ_OVER)
			replayOver = true;
	}

	bool IsZPressed() override
	{
		return zpressed;
	}

	bool IsLControlDown() override
	{
		return lcontroldown;
	}

private:

	bool zpressed;

	bool lcontroldown;
};