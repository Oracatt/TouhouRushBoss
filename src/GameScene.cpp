#include "GameScene.h"
#include "Reimu.h"
#include "SceneObjects.h"
#include "EnemyBulletDeriver.h"
#include "Callback.h"
#include "Rand.h"
#include "BossDeriver.h"
#include "BackGroundDeriver.h"
#include "GameGlobal.h"
#include "Mathf.h"
#include "Surface.h"
#include "LifeBar.h"
#include "BossSpellCardDeriver.h"
#include "UIDeriver.h"
#include "BossParticle.h"
#include "Balloon.h"
#include "Face.h"
#include "DialogDeriver.h"
#include "Level.h"
#include "ItemDeriver.h"
#include "Marisa.h"
#include "Replay.h"

ComPtr<GameCamera> GameSceneCom::gamecam;
ComPtr<Camera> GameSceneCom::backcam;
ComPtr<Camera> GameSceneCom::fixedcam;
ComPtr<Camera> GameSceneCom::fixed3dcam;
LPGOBJ GameSceneCom::locator[25];
LPGOBJ GameSceneCom::pauseLocator;
ComPtr<IRenderTarget> GameSceneCom::balloonTarget;
ComPtr<IRenderTarget> GameSceneCom::backTarget;
ComPtr<IRenderTarget> GameSceneCom::gameTarget;
ComPtr<IRenderTarget> GameSceneCom::spellTextTarget;

void GameSceneCom::OnLoad()
{
	SetRandomSeed(0);
	canPause = true;
	if (!isReplaying)
	{
		replayOver = false;
		recording = false;
		allowSaveReplay = !practiceMode;
		currentReplay.ClearPages();
		currentPage.ClearSequence();
	}
	else
	{
		replayOver = false;
		recording = false;
		allowSaveReplay = false;
	}
	retryTimes = 0;

	RenderTargetDesc desc;
	desc.width = bufferSize.width;
	desc.height = bufferSize.height;
	desc.msaaEnabled = false;
	desc.support2D = true;
	desc.hasDepth = false;
	desc.multiSampleCount = 0;
	engine->CreateRenderTarget(desc, &backTarget);
	engine->CreateRenderTarget(desc, &gameTarget);

	desc.width = 640;
	desc.height = 480;
	engine->CreateRenderTarget(desc, &balloonTarget);
	engine->CreateRenderTarget(desc, &spellTextTarget);

	for (int i = 0; i < 25; i++)
		locator[i] = scene->AttachActor(new GameObject);
	pauseLocator = scene->AttachActor(new GameObject);

	backcam = scene->AttachActor(new Camera(CAMERA_TYPE_PROJECTIVE, { 0,0 }, (float)M_PI / 3));
	backcam->transform->location = { 0,0,0 };
	backcam->InsertBefore(locator[0].Get());
	scene->AttachActor(new CameraSet(backcam.Get()))->InsertBefore(locator[0].Get());

	gamecam = scene->AttachActor(new GameCamera);
	gamecam->transform->location = { 0,0,-10 };
	gamecam->InsertBefore(locator[3].Get());
	scene->AttachActor(new CameraSet(gamecam.Get()))->InsertBefore(locator[3].Get());
	scene->AttachActor(new CameraSet(gamecam.Get()))->InsertBefore(locator[19].Get());

	fixedcam = scene->AttachActor(new GameCamera);
	fixedcam->transform->location = { 0,0,-10 };
	fixedcam->InsertBefore(locator[24].Get());
	scene->AttachActor(new CameraSet(fixedcam.Get()))->InsertBefore(locator[24].Get());

	fixed3dcam = scene->AttachActor(new Camera(CAMERA_TYPE_PROJECTIVE));
	fixed3dcam->transform->location = { 0,0,0 };
	fixed3dcam->InsertBefore(locator[18].Get());
	scene->AttachActor(new CameraSet(fixed3dcam.Get()))->InsertBefore(locator[18].Get());

	scene->AttachActor(new RenderTargetSet(backTarget.Get()))->InsertBefore(locator[1].Get());
	scene->AttachActor(new RenderTargetSet(gameTarget.Get()))->InsertBefore(locator[5].Get());
	scene->AttachActor(new RenderTargetSet(0))->InsertBefore(locator[24].Get());

	LPGOBJ g = scene->AttachActor(new CallbackSet(&warpCallback, [=]()->void {
		float warpScale = (float)timer->GetGameTime() / 3.5f;
		float radius = 0.5;
		float limit = warpLimit;
		int color = warpColor;
		Vector2 center = Vector2(Lerp(0.0f, 640.0f, (320 + warpCenter.x * 0.8f) / 640.0f), Lerp(0.0f, 480.0f, (240 - warpCenter.y * 0.8f) / 480.0f));
		Vector2 vpSize = { 640,480 };
		renderer->SendShaderData(SD_WARPSCALE, &warpScale, 0);
		renderer->SendShaderData(SD_RADIUS, &radius, 0);
		renderer->SendShaderData(SD_LIMIT, &limit, 0);
		renderer->SendShaderData(SD_CENTER, &center, 0);
		renderer->SendShaderData(SD_VPSIZE, &vpSize, 0);
		renderer->SendShaderData(SD_COLORKEY, (LPVOID)warpColor, 0);
		}));
	g->InsertBefore(locator[5].Get());

	g = scene->AttachActor(new GameObject);
	g->transform->scale = { 800,600,1 };
	SpriteRender* sr = new SpriteRender;
	Material mat;
	mat.ambient = { 0,0,0,0 };
	mat.diffuse = { 1,1,1,1 };
	mat.power = 0;
	mat.specular = { 0,0,0,0 };
	sr->SetMaterial(mat);
	sr->SetTexture(backTarget.Get());
	g->AddComponent(sr);
	g->InsertBefore(locator[5].Get());

	g = scene->AttachActor(new CallbackSet(&mainCallback));
	g->InsertBefore(locator[5].Get());

	g = scene->AttachActor(new GameObject);
	g->transform->scale = { 640,480,1 };
	sr = new SpriteRender;
	mat.ambient = { 0,0,0,0 };
	mat.diffuse = { 1,1,1,1 };
	mat.power = 0;
	mat.specular = { 0,0,0,0 };
	sr->SetMaterial(mat);
	sr->SetTexture(gameTarget.Get());
	g->AddComponent(sr);
	g->InsertBefore(locator[24].Get());

	scene->AttachActor(new FogSet(true))->InsertBefore(locator[1].Get());
	scene->AttachActor(new FogSet(false))->InsertAfter(locator[2].Get());

	bool highLightData[25];
	memset(highLightData, 0, sizeof(highLightData));
	highLightData[2] = true;
	highLightData[4] = true;
	highLightData[6] = true;
	highLightData[12] = true;
	highLightData[13] = true;
	highLightData[14] = true;
	highLightData[15] = true;

	for (int i = 0; i < 25; i++)
	{
		if (highLightData[i])
			scene->AttachActor(new BlendStateSet(BLEND_STATE_BRIGHTEN))->InsertBefore(locator[i].Get());
		else
			scene->AttachActor(new BlendStateSet(BLEND_STATE_NORMAL))->InsertBefore(locator[i].Get());
	}

	scene->AttachActor(new SamplerStateSet(spPoint.Get()))->InsertBefore(locator[11].Get());
	scene->AttachActor(new SamplerStateSet(spPoint.Get()))->InsertBefore(locator[15].Get());
	scene->AttachActor(new SamplerStateSet(spPoint.Get()))->InsertBefore(locator[21].Get());
	scene->AttachActor(new SamplerStateSet(spAnis.Get()))->InsertAfter(locator[11].Get());
	scene->AttachActor(new SamplerStateSet(spAnis.Get()))->InsertAfter(locator[15].Get());
	scene->AttachActor(new SamplerStateSet(spAnis.Get()))->InsertAfter(locator[21].Get());

	switch (playerIndex)
	{
	case 0:
		scene->AttachActor(new Reimu);
		break;
	case 1:
		scene->AttachActor(new Marisa);
		break;
	}

	scene->AttachActor(new Surface);
	scene->AttachActor(new ItemLine);
	scene->AttachActor(new GamePauseListener);

	if (isReplaying)
		scene->AttachActor(new ReplayOverListener);

	ResetGameData();

	LoadLevel(scene.Get(), nowLevel);

	if (needSwitch)
		scene->AttachActor(new LeaveSwitch);
}

void GameSceneCom::OnClear()
{
	gamecam.Reset();
	backcam.Reset();
	fixedcam.Reset();
	backTarget.Reset();
	gameTarget.Reset();
	balloonTarget.Reset();
	spellTextTarget.Reset();
	for (int i = 0; i < 25; i++)
		locator[i].Reset();
	pauseLocator.Reset();
}

MSGRESULT GameSceneCom::OnMessageBegin(BYTE& msgType, LPVOID& param1, LPVOID& param2)
{
	__super::OnMessageBegin(msgType, param1, param2);
	
	if (msgType == SMSG_RESIZE)
	{
		renderer->Invoke(Delegate([=]() {
			RenderTargetDesc desc;
			desc.width = ((UINT)param1);
			desc.height = ((UINT)param2);
			desc.msaaEnabled = false;
			desc.support2D = true;
			desc.hasDepth = false;
			desc.multiSampleCount = 0;
			gameTarget->Resize(desc);
			backTarget->Resize(desc);
			}));
	}
	
	return MSGR_IGNORE;
}