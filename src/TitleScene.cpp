#include "TitleScene.h"
#include "SceneObjects.h"
#include "Level.h"
#include "TitleObjects.h"
#include "TitleFrame.h"

LPGOBJ TitleSceneCom::locator[6];
ComPtr<Camera> TitleSceneCom::camera;
stack<ComPtr<Frame>> TitleSceneCom::frameStack;

void TitleSceneCom::OnLoad()
{
	for (int i = 0; i < 6; i++)
		locator[i] = scene->AttachActor(new GameObject);

	scene->AttachActor(new BlendStateSet(BLEND_STATE_BRIGHTEN))->InsertBefore(locator[3].Get());
	scene->AttachActor(new BlendStateSet(BLEND_STATE_NORMAL))->InsertAfter(locator[3].Get());

	camera = scene->AttachActor(new Camera(CAMERA_TYPE_ORTHOGONAL, { 640,480 }));
	camera->transform->location = { 0,0,-10 };
	camera->InsertBefore(locator[0].Get());
	scene->AttachActor(new CameraSet(camera.Get()))->InsertBefore(locator[0].Get());

	LoadLevel(scene.Get(), LEVEL_TITLE);
}

void TitleSceneCom::OnClear()
{
	camera.Reset();
	for (int i = 0; i < 6; i++)
		locator[i].Reset();
}