#pragma once
#include "VirtualLib.h"
#include "EngineGlobal.h"
#include "CallBack.h"
#include "FogManager.h"
#include "GameScene.h"
#include "Rand.h"
#include "Mathf.h"
#include "Source.h"
#include "SoundEffect.h"
#include "Level.h"
#include "LevelResources.h"
#include "TitleScene.h"
#include <thread>

using namespace Global;

extern Vector2 switchOffset;
extern bool needSwitch;

class SetPresentTime :public GameObject
{
public:

	SetPresentTime(int _time, int recoverFrame) :time(_time), frame(0), limit(recoverFrame)
	{
	}

private:

	int limit;

	int frame;

	int time;

	void OnCreated() override
	{
		InsertBefore(GameSceneCom::locator[24].Get());
		timer->SetPresentCount(time);
	}

	void OnUpdate() override
	{
		frame++;
		if (frame >= limit)
		{
			Destroy();
		}
		else
		{
			timer->SetPresentCount(time);
		}
	}

	void OnDestroy() override
	{
		timer->SetPresentCount(1);
	}
};

class GameCamera :public Camera
{
public:

	GameCamera() :Camera(CAMERA_TYPE_ORTHOGONAL, { 640,480 }), frame(0), range(0), nowrange(0) {}

	void Shake(int _frame, float _range)
	{
		if (frame < _frame)
			frame = _frame;
		if (range < _range)
			range = _range;
		nowrange = Lerp(nowrange, range, 0.1);
		transform->location = GetVector(RandomAngle(), nowrange);
		transform->location.z = -10;
		range *= (frame + 19) / (float)(frame + 20);
		if (frame == 0)
		{
			range = 0;
			nowrange = 0;
		}
	}

private:

	int frame;

	float range;

	float nowrange;

	void OnUpdate() override
	{
		if (frame > 0)
			Shake(--frame, range);
		transform->location = Lerp(Vector3{ 0,0,-10 }, transform->location, 0.5);
	}
};

class CallbackSet :public GameObject
{
public:

	CallbackSet(EngineEventCallBack* pCallback, const function<void()>& func = function<void()>())
	{
		AddComponent(new Setter(pCallback, func));
	}

	class Setter :public RenderCom
	{
	public:

		EngineEventCallBack* pCallback;

		function<void()> func;

		Setter(EngineEventCallBack* _pCallback, const function<void()>& _func) :
			pCallback(_pCallback), func(_func) {}

		void OnRender() override
		{
			SetCurrentCallback(pCallback);
			if (func)
				func();
		}
	};
};

class FogSet :public GameObject
{
public:

	FogSet(bool enable)
	{
		AddComponent(new Setter(enable));
	}

	class Setter :public RenderCom
	{
	public:

		bool enable;

		Setter(bool _enable) :enable(_enable) {}

		void OnRender() override
		{
			if (enable)
			{
				renderer->SendShaderData(SD_FOGENABLED, (LPVOID)fogEnable, 0);
				renderer->SendShaderData(SD_FOGSTART, &fogStart, 0);
				renderer->SendShaderData(SD_FOGRANGE, &fogRange, 0);
				renderer->SendShaderData(SD_FOGCOLOR, &fogColor, 0);
			}
			else
			{
				renderer->SendShaderData(SD_FOGENABLED, (LPVOID)false, 0);
			}
		}
	};
};

class SamplerStateSet :public GameObject
{
public:

	SamplerStateSet(ID3D11SamplerState* state)
	{
		AddComponent(new Setter(state));
	}

	class Setter :public RenderCom
	{
	public:

		ID3D11SamplerState* state;

		Setter(ID3D11SamplerState* _state) :state(_state) {}

		void OnFetchData() override
		{
			m_state = state;
		}

		void OnRender() override
		{
			renderer->SendShaderData(SD_SAMPLER, m_state.Get(), 0);

			m_state.Reset();
		}

	private:

		ComPtr<ID3D11SamplerState> m_state;
	};
};

class RasterizerStateSet :public GameObject
{
public:

	RasterizerStateSet(ID3D11RasterizerState* state)
	{
		AddComponent(new Setter(state));
	}

	class Setter :public RenderCom
	{
	public:

		ID3D11RasterizerState* state;

		Setter(ID3D11RasterizerState* _state) :state(_state) {}

		void OnFetchData() override
		{
			m_state = state;
		}

		void OnRender() override
		{
			pContext->RSSetState(m_state.Get());

			m_state.Reset();
		}

	private:

		ComPtr<ID3D11RasterizerState> m_state;
	};
};

class DepthStencilStateSet :public GameObject
{
public:

	DepthStencilStateSet(ID3D11DepthStencilState* state)
	{
		AddComponent(new Setter(state));
	}

	class Setter :public RenderCom
	{
	public:

		ID3D11DepthStencilState* state;

		void OnFetchData() override
		{
			m_state = state;
		}

		Setter(ID3D11DepthStencilState* _state) :state(_state) {}

		void OnRender() override
		{
			pContext->OMSetDepthStencilState(m_state.Get(), 0);

			m_state.Reset();
		}

	private:

		ComPtr<ID3D11DepthStencilState> m_state;
	};
};

class BlendStateSet :public GameObject
{
public:

	BlendStateSet(BLEND_STATE state = BLEND_STATE_NORMAL, ID3D11BlendState* blend = 0, const Color& factor = {0,0,0,0})
	{
		AddComponent(new Setter(state, blend, factor));
	}

	class Setter :public RenderCom
	{
	public:

		BLEND_STATE state;

		Color factor;

		ID3D11BlendState* blend;

		Setter(BLEND_STATE _state = BLEND_STATE_NORMAL, ID3D11BlendState* _blend = 0, const Color& _factor = {0,0,0,0}) :state(_state), blend(_blend), factor(_factor) {}

		void OnFetchData() override
		{
			m_blend = blend;
		}

		void OnRender() override
		{
			if (!m_blend)
			{
				renderer->SetBlendState(state);
			}
			else
			{
				pContext->OMSetBlendState(m_blend.Get(), (float*)&factor, 0xffffffff);
				m_blend.Reset();
			}
		}

	private:

		ComPtr<ID3D11BlendState> m_blend;
	};
};

class CameraSet :public GameObject
{
public:

	CameraSet(ICamera* camera)
	{
		AddComponent(new Setter(camera));
	}

	class Setter :public RenderCom
	{
	public:

		ICamera* camera;

		Setter(ICamera* _camera) :camera(_camera) {}

		void OnFetchData() override
		{
			m_cam = camera;
		}

		void OnRender() override
		{
			renderer->SendShaderData(SD_UPDATEMATRIX, m_cam.Get(), 0);

			m_cam.Reset();
		}

	private:

		ComPtr<ICamera> m_cam;
	};
};

class RenderTargetSet :public GameObject
{
public:

	RenderTargetSet(IRenderTarget* target, float clearAlpha = 1, bool clear = true)
	{
		AddComponent(new Setter(target, clearAlpha, clear));
	}

	class Setter :public RenderCom
	{
	public:

		ComPtr<IRenderTarget> target;

		Setter(IRenderTarget* _target, float clearAlpha = 1, bool _clear = true) :
			target(_target), alpha(clearAlpha), clear(_clear) {}

		void OnRender() override
		{
			if (target)
			{
				renderer->SendShaderData(SD_TEXTURE, 0, 0);
				renderer->StartRender();
				if (clear)
					renderer->ClearRenderTarget(target.Get(), { 0,0,0,alpha });
				renderer->SetRenderTarget(target.Get());
			}
			else
				renderer->SetRenderTarget(0);
		}

	private:

		float alpha;

		bool clear;
	};
};

class LoadingTag :public GameObject
{
public:

	LoadingTag() :
		sr_1(AddComponent(new SpriteRender)), sr_2(AddComponent(new SpriteRender)), frame(0), isLeft(false)
	{
		SetUpdateLevel(1);
		transform->location = { 210,-178,0 };
		transform->scale = { 128,32,1 };
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,1 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr_1->SetMaterial(mat);
		sr_1->SetTexture(src_loading.Get());
		sr_1->SetSourceRect({ 0,0,1,0.25 });
		sr_2->SetMaterial(mat);
		sr_2->SetTexture(src_loading.Get());
		sr_2->SetSourceRect({ 0,0.25,1,0.25 });
		sr_2->SetLocalTransform(XMMatrixTranslation(0, -0.5, 0));
	}

	void Leave()
	{
		isLeft = true;
	}

private:

	int frame;

	SpriteRender* sr_1;

	SpriteRender* sr_2;

	Material mat;

	bool isLeft;

	void OnUpdate() override
	{
		frame++;
		if (!isLeft)
		{
			mat.diffuse.a = 0.75f + 0.25f * sin(frame / 8.0f);
			sr_1->SetMaterial(mat);
			sr_2->SetMaterial(mat);
		}
		else
		{
			mat.diffuse.a -= 0.12f;
			sr_1->SetMaterial(mat);
			sr_2->SetMaterial(mat);
			transform->location.x += 3;
			if (mat.diffuse.a <= 0)
				Destroy();
		}
	}
};

class EnterSwitch :public GameObject
{
	class Switch :public GameObject
	{
	public:

		Switch(bool isLeft, float _angle) :sr(AddComponent(new SpriteRender)), isleft(isLeft), frame(0), angle(0)
		{
			if (isLeft)
			{
				angle = -M_PI / 12.0f + _angle;
				transform->Rotate({ 0,0,(float)(-M_PI / 2 + angle) }, true);
				transform->scale = { 280,1100,1 };
			}
			else
			{
				angle = M_PI * 13.0 / 12.0f - _angle;
				transform->Rotate({ 0,0,(float)(-M_PI / 2 + angle) }, true);
				transform->scale = { 280,1100,1 };
			}
			Material mat;
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = { 1,1,1,1 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_switch.Get());
			if (!isleft)
				sr->SetSourceRect({ 1,0,-1,1 });
		}

	private:

		float angle;

		int frame;

		bool isleft;

		SpriteRender* sr;

		void OnUpdate() override
		{
			frame++;
			if (frame <= 20)
			{
				transform->Translate({ (float)cos(angle) * 40, (float)sin(angle) * 40, 0 });
			}
		}
	};

	class Back :public GameObject
	{
	public:

		Vector2 offset;

		Back(const Vector2& _offset = {}) :
			sr(AddComponent(new SpriteRender)), offset(_offset)
		{
			transform->location = { 0,0,0 };
			transform->scale = { 640,640,1 };
			Material mat;
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = { 1,1,1,1 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_switchbg.Get());
			sr->SetTextureTransform(XMMatrixScaling(1.5, 1.5, 1.5) * XMMatrixTranslation(offset.x, offset.y, 0));
		}

	private:

		SpriteRender* sr;

		void OnUpdate() override
		{
			offset += {-0.001, -0.001};
			sr->SetTextureTransform(XMMatrixScaling(1.5, 1.5, 1.5) * XMMatrixTranslation(offset.x, offset.y, 0));
		}
	};

public:

	EnterSwitch(LevelType _type, bool _forceReload = false) :frame(0), sr(AddComponent(new SpriteRender)),
		isResourcesLoaded(false), type(_type), fl(_forceReload), executed(false), exeframe(-1)
	{
		transform->location = { 0,0,0 };
		transform->scale = { 640,480,1 };
		Material mat;
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,1 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
	}

private:

	int frame;

	SpriteRender* sr;

	ComPtr<IRenderTarget> target;

	LPGOBJ rtSet[2];

	LPGOBJ bsSet[2];

	LPGOBJ ssSet[2];

	vector<ComPtr<Switch>> switches;

	ComPtr<Back> back;

	ComPtr<LoadingTag> tag;

	bool isResourcesLoaded;

	LevelType type;

	bool fl;

	bool executed;

	int exeframe;

	void OnCreated() override
	{
		needSwitch = true;
		RenderTargetDesc desc;
		desc.width = 640;
		desc.height = 480;
		desc.msaaEnabled = false;
		desc.support2D = true;
		desc.hasDepth = false;
		desc.multiSampleCount = 0;
		engine->CreateRenderTarget(desc, &target);
		sr->SetTexture(target.Get());
		rtSet[0] = scene->AttachActor(new RenderTargetSet(target.Get(), 0, true));
		rtSet[0]->InsertBefore(this);
		ssSet[0] = scene->AttachActor(new SamplerStateSet(spBorder.Get()));
		ssSet[0]->InsertBefore(this);
		bsSet[0] = scene->AttachActor(new BlendStateSet((BLEND_STATE)0, enterBs.Get()));
		bsSet[0]->InsertBefore(this);
		ssSet[1] = scene->AttachActor(new SamplerStateSet(spAnis.Get()));
		ssSet[1]->InsertBefore(this);
		back = scene->AttachActor(new Back);
		back->InsertBefore(this);
		bsSet[1] = scene->AttachActor(new BlendStateSet(BLEND_STATE_NORMAL));
		bsSet[1]->InsertBefore(this);
		rtSet[1] = scene->AttachActor(new RenderTargetSet(0));
		rtSet[1]->InsertBefore(this);
		tag = scene->AttachActor(new LoadingTag);
		tag->InsertAfter(this);

		thread([=]()->void {
			LoadLevelResource(type, fl);
			isResourcesLoaded = true;
			}).detach();
	}

	void OnUpdate() override
	{
		frame++;
		if (frame == 2)
		{
			auto g = scene->AttachActor(new Switch(true, 0));
			g->transform->location = { -580.0f,450 ,0 };
			g->InsertBefore(bsSet[0].Get());
			switches.push_back(g);
		}
		else if (frame == 3)
		{
			auto g = scene->AttachActor(new Switch(true, 0));
			g->transform->location = { -600.0f,320 ,0 };
			g->InsertBefore(bsSet[0].Get());
			switches.push_back(g);
		}
		else if (frame == 8)
		{
			auto g = scene->AttachActor(new Switch(true, 0));
			g->transform->location = { -620.0f,250 ,0 };
			g->InsertBefore(bsSet[0].Get());
			switches.push_back(g);
		}
		else if (frame == 9)
		{
			auto g = scene->AttachActor(new Switch(true, 1 / 8.0f));
			g->transform->location = { -640.0f,120 ,0 };
			g->InsertBefore(bsSet[0].Get());
			switches.push_back(g);
		}
		else if (frame == 14)
		{
			auto g = scene->AttachActor(new Switch(true, 1 / 7.0f));
			g->transform->location = { -660.0f,50 ,0 };
			g->InsertBefore(bsSet[0].Get());
			switches.push_back(g);
		}
		else if (frame == 15)
		{
			auto g = scene->AttachActor(new Switch(true, 1 / 6.0f));
			g->transform->location = { -680.0f,-50 ,0 };
			g->InsertBefore(bsSet[0].Get());
			switches.push_back(g);
		}
		else if (frame == 19)
		{
			auto g = scene->AttachActor(new Switch(true, 1 / 5.0f));
			g->transform->location = { -700.0f,-150 ,0 };
			g->InsertBefore(bsSet[0].Get());
			switches.push_back(g);
		}
		if (frame == 6)
		{
			auto g = scene->AttachActor(new Switch(false, 0));
			g->transform->location = { 580.0f,450 ,0 };
			g->InsertBefore(bsSet[0].Get());
			switches.push_back(g);
		}
		else if (frame == 7)
		{
			auto g = scene->AttachActor(new Switch(false, 0));
			g->transform->location = { 600.0f,320 ,0 };
			g->InsertBefore(bsSet[0].Get());
			switches.push_back(g);
		}
		else if (frame == 12)
		{
			auto g = scene->AttachActor(new Switch(false, 0));
			g->transform->location = { 620.0f,250 ,0 };
			g->InsertBefore(bsSet[0].Get());
			switches.push_back(g);
		}
		else if (frame == 13)
		{
			auto g = scene->AttachActor(new Switch(false, 1 / 8.0f));
			g->transform->location = { 640.0f,120 ,0 };
			g->InsertBefore(bsSet[0].Get());
			switches.push_back(g);
		}
		else if (frame == 21)
		{
			auto g = scene->AttachActor(new Switch(false, 1 / 7.0f));
			g->transform->location = { 660.0f,50 ,0 };
			g->InsertBefore(bsSet[0].Get());
			switches.push_back(g);
		}
		else if (frame == 22)
		{
			auto g = scene->AttachActor(new Switch(false, 1 / 6.0f));
			g->transform->location = { 680.0f,-50 ,0 };
			g->InsertBefore(bsSet[0].Get());
			switches.push_back(g);
		}
		else if (frame == 18)
		{
			auto g = scene->AttachActor(new Switch(false, 1 / 5.0f));
			g->transform->location = { 700.0f,-150 ,0 };
			g->InsertBefore(bsSet[0].Get());
			switches.push_back(g);
		}

		if (frame >= 60 && isResourcesLoaded && !executed)
		{
			executed = true;
			exeframe = 10;
			if (tag)
			{
				tag->Leave();
				tag.Reset();
			}
		}
		if (exeframe >= 0)
		{
			exeframe--;
			if (exeframe == 0)
			{
				if (type == LEVEL_TITLE)
				{
					scene->Destroy();
					ComPtr<IScene> mscene;
					engine->CreateScene(0, &mscene);
					mscene->AddComponent(new TitleSceneCom);
					engine->SetNowScene(mscene.Get());
				}
				else
				{
					scene->Destroy();
					ComPtr<IScene> mscene;
					engine->CreateScene(0, &mscene);
					mscene->AddComponent(new GameSceneCom);
					engine->SetNowScene(mscene.Get());
				}
			}
		}
	}

	void OnDestroy() override
	{
		target.Reset();
		if (back)
		{
			switchOffset = back->offset;
			back->Destroy();
			back.Reset();
		}
		if (tag)
		{
			tag->Destroy();
			tag.Reset();
		}
		for (int i = 0; i < 2; i++)
		{
			if (rtSet[i])
			{
				rtSet[i]->Destroy();
				rtSet[i].Reset();
			}
			if (bsSet[i])
			{
				bsSet[i]->Destroy();
				bsSet[i].Reset();
			}
			if (ssSet[i])
			{
				ssSet[i]->Destroy();
				ssSet[i].Reset();
			}
		}
		for (auto g : switches)
		{
			g->Destroy();
		}
		switches.clear();
	}
};


class LeaveSwitch :public GameObject
{
	class Switch :public GameObject
	{
	public:

		Switch(bool isLeft, float _angle, int _leaveFrame) :
			sr(AddComponent(new SpriteRender)), isleft(isLeft), frame(0), angle(0), leaveFrame(_leaveFrame)
		{
			if (isLeft)
			{
				angle = -M_PI / 12.0f + _angle;
				transform->Rotate({ 0,0,(float)(-M_PI / 2 + angle) }, true);
				transform->scale = { 350,-1000,1 };
			}
			else
			{
				angle = M_PI * 13.0 / 12.0f - _angle;
				transform->Rotate({ 0,0,(float)(-M_PI / 2 + angle) }, true);
				transform->scale = { 350,-1000,1 };
			}
			Material mat;
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = { 1,1,1,1 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_switch.Get());
			if (!isleft)
				sr->SetSourceRect({ 1,0,-1,1 });
		}

	private:

		int leaveFrame;

		float angle;

		int frame;

		bool isleft;

		SpriteRender* sr;

		void OnCreated() override
		{
			transform->Translate({ (float)cos(angle) * 800, (float)sin(angle) * 400, 0 });
		}

		void OnUpdate() override
		{
			frame++;
			if (frame > leaveFrame)
				transform->Translate({ (float)cos(angle) * 60, (float)sin(angle) * 60, 0 });
		}
	};

	class Back :public GameObject
	{
	public:

		Vector2 offset;

		Back(const Vector2& _offset = {}) :
			sr(AddComponent(new SpriteRender)), offset(_offset)
		{
			transform->location = { 0,0,0 };
			transform->scale = { 640,640,1 };
			Material mat;
			mat.ambient = { 0,0,0,0 };
			mat.diffuse = { 1,1,1,1 };
			mat.power = 0;
			mat.specular = { 0,0,0,0 };
			sr->SetMaterial(mat);
			sr->SetTexture(src_switchbg.Get());
			sr->SetTextureTransform(XMMatrixScaling(1.5, 1.5, 1.5) * XMMatrixTranslation(offset.x, offset.y, 0));
		}

	private:

		SpriteRender* sr;

		void OnUpdate() override
		{
			offset += {-0.001, -0.001};
			sr->SetTextureTransform(XMMatrixScaling(1.5, 1.5, 1.5) * XMMatrixTranslation(offset.x, offset.y, 0));
		}
	};

public:

	LeaveSwitch() :frame(0), sr(AddComponent(new SpriteRender))
	{
		transform->location = { 0,0,0 };
		transform->scale = { 640,480,1 };
		Material mat;
		mat.ambient = { 0,0,0,0 };
		mat.diffuse = { 1,1,1,1 };
		mat.power = 0;
		mat.specular = { 0,0,0,0 };
		sr->SetMaterial(mat);
	}

private:

	int frame;

	SpriteRender* sr;

	ComPtr<IRenderTarget> target;

	LPGOBJ rtSet[2];

	LPGOBJ bsSet[2];

	LPGOBJ ssSet[2];

	vector<ComPtr<Switch>> switches;

	ComPtr<Back> back;

	void OnCreated() override
	{
		needSwitch = false;
		RenderTargetDesc desc;
		desc.width = 640;
		desc.height = 480;
		desc.msaaEnabled = false;
		desc.support2D = true;
		desc.hasDepth = false;
		desc.multiSampleCount = 0;
		engine->CreateRenderTarget(desc, &target);
		sr->SetTexture(target.Get());
		rtSet[0] = scene->AttachActor(new RenderTargetSet(target.Get(), 0, true));
		rtSet[0]->InsertBefore(this);
		ssSet[0] = scene->AttachActor(new SamplerStateSet(spBorder.Get()));
		ssSet[0]->InsertBefore(this);
		bsSet[0] = scene->AttachActor(new BlendStateSet((BLEND_STATE)0, enterBs.Get()));
		bsSet[0]->InsertBefore(this);
		ssSet[1] = scene->AttachActor(new SamplerStateSet(spAnis.Get()));
		ssSet[1]->InsertBefore(this);
		back = scene->AttachActor(new Back(switchOffset));
		back->InsertBefore(this);
		bsSet[1] = scene->AttachActor(new BlendStateSet(BLEND_STATE_NORMAL));
		bsSet[1]->InsertBefore(this);
		rtSet[1] = scene->AttachActor(new RenderTargetSet(0));
		rtSet[1]->InsertBefore(this);

		for (int i = 0; i < 8; i++)
		{
			auto g = scene->AttachActor(new Switch(i % 2 == 0, 0, 5 * i));
			g->transform->Translate({ -680.0f * ((i % 2 == 0) ? 1 : -1),500 - 100.0f * i ,0 });
			g->InsertBefore(bsSet[0].Get());
			switches.push_back(g);
			g = scene->AttachActor(new Switch(i % 2 == 0, 0, 5 * i));
			g->transform->Translate({ -680.0f * ((i % 2 == 0) ? 1 : -1),400 - 100.0f * i ,0 });
			g->InsertBefore(bsSet[0].Get());
			switches.push_back(g);
		}
	}

	void OnUpdate() override
	{
		frame++;
		if (frame == 60)
		{
			Destroy();
		}
	}

	void OnDestroy() override
	{
		target.Reset();
		if (back)
		{
			back->Destroy();
			back.Reset();
		}
		for (int i = 0; i < 2; i++)
		{
			if (rtSet[i])
			{
				rtSet[i]->Destroy();
				rtSet[i].Reset();
			}
			if (bsSet[i])
			{
				bsSet[i]->Destroy();
				bsSet[i].Reset();
			}
			if (ssSet[i])
			{
				ssSet[i]->Destroy();
				ssSet[i].Reset();
			}
		}
		for (auto g : switches)
		{
			g->Destroy();
		}
		switches.clear();
	}
};

class ChangeLevel :public GameObject
{
public:

	LevelType type;

	bool isResourcesLoaded;

	bool fl;

	int frame;

	ChangeLevel(LevelType _type, bool _forceReload = false) :type(_type), isResourcesLoaded(false), fl(_forceReload),
		frame(0)
	{
		SetUpdateLevel(1);
	}

	void OnCreated() override
	{
		scene->AttachActor(new LoadingTag);
		thread([=]()->void {
			LoadLevelResource(type, fl);
			isResourcesLoaded = true;
			}).detach();
	}

	void OnUpdate() override
	{
		frame++;
		if (isResourcesLoaded && frame >= 60)
		{
			if (type == LEVEL_TITLE)
			{
				scene->Destroy();
				ComPtr<IScene> mscene;
				engine->CreateScene(0, &mscene);
				mscene->AddComponent(new TitleSceneCom);
				engine->SetNowScene(mscene.Get());
			}
			else
			{
				scene->Destroy();
				ComPtr<IScene> mscene;
				engine->CreateScene(0, &mscene);
				mscene->AddComponent(new GameSceneCom);
				engine->SetNowScene(mscene.Get());
			}
		}
	}
};

class DebugController :public GameObject
{
public:

	LPGOBJ target;

	DebugController(GameObject* g) :target(g)
	{

	}

	void OnUpdate() override
	{
		if (input->KeyPress(DIK_LEFT))
		{
			target->transform->location.x--;
			printf("%f %f\n", target->transform->location.x, target->transform->location.y);
		}
		if (input->KeyPress(DIK_RIGHT))
		{
			target->transform->location.x++;
			printf("%f %f\n", target->transform->location.x, target->transform->location.y);
		}
		if (input->KeyPress(DIK_UP))
		{
			target->transform->location.y++;
			printf("%f %f\n", target->transform->location.x, target->transform->location.y);
		}
		if (input->KeyPress(DIK_DOWN))
		{
			target->transform->location.y--;
			printf("%f %f\n", target->transform->location.x, target->transform->location.y);
		}
	}

	void OnDestroy() override
	{
		target.Reset();
	}
};

class Debugger :public GameObject
{
public:

	Debugger()
	{
		SetName("debugger");
		SetUpdateLevel(1);
	}

	void OnUpdate() override
	{
		if (input->KeyPress(DIK_P))
		{
			/*
			FILE* fp = fopen("debug.txt", "w+");
			for (LPACTOR actor = scene->GetFirstActor(); actor; actor = actor->GetNext())
			{
				fprintf(fp, "%s\n", ((GameObject*)actor.Get())->name.c_str());
			}
			*/
			timer->SetTimeStopLevel(1);
		}
		if (input->KeyPress(DIK_L))
		{
			timer->SetTimeStopLevel(0);
		}
		if (input->KeyPress(DIK_R))
		{
			
			scene->Destroy();
			ComPtr<IScene> mscene;
			engine->CreateScene(0, &mscene);
			mscene->AddComponent(new GameSceneCom);
			engine->SetNowScene(mscene.Get());
			
			//scene->AttachActor(new EnterSwitch(LEVEL_FROZENFOREST));
		}
		if (input->KeyDown(DIK_W))
			GameSceneCom::backcam->transform->Rotate({ -0.01,0,0 }, false);
		if (input->KeyDown(DIK_S))
			GameSceneCom::backcam->transform->Rotate({ 0.01,0,0 }, false);
		if (input->KeyDown(DIK_A))
			GameSceneCom::backcam->transform->Rotate({ 0,-0.01,0 }, true);
		if (input->KeyDown(DIK_D))
			GameSceneCom::backcam->transform->Rotate({ 0,0.01,0 }, true);
		int num = 0;
		for (LPACTOR g = scene->GetFirstActor(); g; g = g->GetNext())
		{
			num++;
		}
		//cout << num << endl;
	}
};