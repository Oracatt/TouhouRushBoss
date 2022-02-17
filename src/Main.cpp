#include <iostream>
#include "VirtualLib.h"
#include "LoadingScene.h"
#include "Source.h"
#include "Callback.h"
#include "SoundEffect.h"
#include "VertexBuffer.h"
#include "LevelResources.h"
#include "BossSpellCard.h"
#include "Replay.h"
#include "History.h"
#include "WinRC.h"

using namespace Global;

namespace Global
{
    ComPtr<IVirtualEngine> engine;
    ComPtr<IRenderer> renderer;
    ComPtr<IInput> input;
    ComPtr<ITimer> timer;
    ComPtr<IAudioPlayer> audio;
    ComPtr<ID3D11Device> pDevice;
    ComPtr<ID3D11DeviceContext> pContext;
    ComPtr<IDXGISwapChain> pSwapChain;
}

ComPtr<ID3D11DepthStencilState> dsState;
ComPtr<ID3D11SamplerState> spAnis;
ComPtr<ID3D11SamplerState> spPoint;
ComPtr<ID3D11SamplerState> spBorder;
ComPtr<ID3D11BlendState> enterBs;
ComPtr<ID3D11BlendState> leaveBs;

ComPtr<ITextFormat> tf_digit;
ComPtr<ITextFormat> tf_digit_center;
ComPtr<ITextFormat> tf_wryh;
ComPtr<ITextFormat> tf_wryh_big;
ComPtr<ITextFormat> tf_songti;
ComPtr<ITextFormat> tf_songti_spell;
ComPtr<ITextFormat> tf_wryh_volume;
ComPtr<ITextFormat> tf_youyuan;

ComPtr<ID2D1SolidColorBrush> sb_black;
ComPtr<ID2D1SolidColorBrush> sb_white;
ComPtr<ID2D1SolidColorBrush> sb_blue;

bool resourcesLoaded = false;

USize bufferSize = { 640,480 };

DWORD mainThreadID;

HANDLE resourceLock;

void LoadState()
{
    D3D11_RENDER_TARGET_BLEND_DESC rtBlendDesc;
    rtBlendDesc.BlendEnable = true;
    rtBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
    rtBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    rtBlendDesc.DestBlend = D3D11_BLEND_ZERO;
    rtBlendDesc.DestBlendAlpha = D3D11_BLEND_ONE;
    rtBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    rtBlendDesc.SrcBlend = D3D11_BLEND_DEST_ALPHA;
    rtBlendDesc.SrcBlendAlpha = D3D11_BLEND_ZERO;
    D3D11_BLEND_DESC blendDesc;
    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.IndependentBlendEnable = false;
    blendDesc.RenderTarget[0] = rtBlendDesc;
    pDevice->CreateBlendState(&blendDesc, &enterBs);

    rtBlendDesc.BlendEnable = true;
    rtBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
    rtBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_REV_SUBTRACT;
    rtBlendDesc.DestBlend = D3D11_BLEND_ONE;
    rtBlendDesc.DestBlendAlpha = D3D11_BLEND_ONE;
    rtBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    rtBlendDesc.SrcBlend = D3D11_BLEND_ZERO;
    rtBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.IndependentBlendEnable = false;
    blendDesc.RenderTarget[0] = rtBlendDesc;
    pDevice->CreateBlendState(&blendDesc, &leaveBs);

    D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());
    dsDesc.DepthEnable = false;
    dsDesc.StencilEnable = false;
    pDevice->CreateDepthStencilState(&dsDesc, &dsState);

    D3D11_SAMPLER_DESC spDesc = CD3D11_SAMPLER_DESC(CD3D11_DEFAULT());
    spDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    spDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    spDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    spDesc.MaxAnisotropy = 4;
    pDevice->CreateSamplerState(&spDesc, &spAnis);
    spDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    spDesc.MaxAnisotropy = 0;
    pDevice->CreateSamplerState(&spDesc, &spPoint);
    spDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
    spDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
    spDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    spDesc.MaxAnisotropy = 4;
    spDesc.BorderColor[0] = 0;
    spDesc.BorderColor[1] = 0;
    spDesc.BorderColor[2] = 0;
    spDesc.BorderColor[3] = 0;
    pDevice->CreateSamplerState(&spDesc, &spBorder);

    renderer->GetRenderTarget()->GetD2DRenderTarget()->CreateSolidColorBrush({ 0,0,0,1 }, &sb_black);
    renderer->GetRenderTarget()->GetD2DRenderTarget()->CreateSolidColorBrush({ 1,1,1,1 }, &sb_white);
    renderer->GetRenderTarget()->GetD2DRenderTarget()->CreateSolidColorBrush({ 0,0.1,1,1 }, &sb_blue);
}

void LoadGlobalResources()
{
    WaitForSingleObject(resourceLock, INFINITE);
    ComPtr<IBuffer> digitBuffer;
    engine->CreateBuffer("font\\digifaw.ttf", &digitBuffer);
    engine->CreateTextFormatFromBuffer(digitBuffer.Get(), "DigifaceWide", 12, &tf_digit);
    engine->CreateTextFormatFromBuffer(digitBuffer.Get(), "DigifaceWide", 18, &tf_digit_center);
    engine->CreateTextFormatFromFile("font\\simyou.ttf", "YouYuan", 14, &tf_youyuan);
    engine->CreateTextFormat("微软雅黑", 12, &tf_wryh);
    engine->CreateTextFormat("微软雅黑", 16, &tf_wryh_big);
    engine->CreateTextFormat("宋体", 20, &tf_songti_spell);
    engine->CreateTextFormat("宋体", 20, &tf_songti);
    engine->CreateTextFormat("微软雅黑", 20, &tf_wryh_volume);
    tf_digit_center->SetAlignment(ALIGNMENT_TYPE_CENTER);
    tf_songti_spell->SetAlignment(ALIGNMENT_TYPE_BOTTOM | ALIGNMENT_TYPE_RIGHT);
    tf_youyuan->SetAlignment(ALIGNMENT_TYPE_TOP);
    LoadGameSource();
    LoadSoundEffect();
    LoadVertexBuffer();
    ReleaseMutex(resourceLock);

    LoadLevelResource(LEVEL_TITLE);

    resourcesLoaded = true;
}

int StartGame()
{
    MountPackageFile("thsrc.smx");
    VideoDesc videoDesc;
    videoDesc.bufferWidth = 640;
    videoDesc.bufferHeight = 480;
    videoDesc.refreshRate = 60;
    videoDesc.vSync = true;
    videoDesc.msaaEnabled = false;
    videoDesc.multiSampleCount = 4;
    videoDesc.windowed = true;
    videoDesc.altEnter = true;
    videoDesc.fixedAspect = true;
    bufferSize = { videoDesc.bufferWidth,videoDesc.bufferHeight };
    PhysXDesc physxDesc;
    physxDesc.cpuDispatcher = 0;
    InputDesc inputDesc;
    inputDesc.keyboardLevel = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;
    inputDesc.mouseLevel = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;
    EngineDescription engineDesc;
    engineDesc.inputDesc = inputDesc;
    engineDesc.videoDesc = videoDesc;
    engineDesc.errorCallBack = 0;
    engineDesc.hInstance = GetModuleHandle(0);
    engineDesc.outputWindow = 0;
    engineDesc.windowName = "东方RushBoss";
    engineDesc.className = "东方RushBoss";
    engineDesc.physxDesc = physxDesc;
    engineDesc.attributes.backColor = { 0,0,0,1 };
    engineDesc.attributes.autoClear = true;
    engineDesc.attributes.lockDeltaTime = true;
    HRESULT hr = 0;
    if (FAILED(hr = CreateVirtualEngine(VENGINE_SDK_VERSION, engineDesc, &engine)))
    {
        wchar_t msg[64];
        wsprintf(msg, L"create engine failed(%d)", hr);
        MessageBox(0, msg, L"Fatal Error", 0);
        return 0;
    }
    engine.As(&renderer);
    engine.As(&input);
    engine.As(&timer);
    engine.As(&audio);
    renderer->GetPointer(IID_ID3D11Device, &pDevice);
    renderer->GetPointer(IID_ID3D11DeviceContext, &pContext);
    renderer->GetPointer(IID_IDXGISwapChain, &pSwapChain);

    HICON hIcon = LoadIcon(engineDesc.hInstance, MAKEINTRESOURCE(IDI_ICON1));
    SendMessage(engineDesc.outputWindow, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

    LoadState();

    engine->AddEventCallBack(&mainCallback);
    engine->AddEventCallBack(&resizeCallback);
    engine->AddEventCallBack(&warpCallback);
    engine->AddEventCallBack(&blurCallback);
    engine->AddEventCallBack(&waterCallback);
    SetCurrentCallback(&mainCallback);
    ComPtr<IScene> pScene;
    engine->CreateScene(0, &pScene);
    pScene->AddComponent(new LoadingSceneCom);
    engine->Start(pScene.Get());
    engine->Destroy();
    engine.Reset();

    return 0;
}

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    if (_access("replay", 0) != -1)
    {
        for (int i = 1; i <= 25; i++)
        {
            char filename[128];
            sprintf(filename, "save_%d.rpy", i);
            replayList[i].LoadFromFile(filename);
        }
    }
    mainThreadID = GetCurrentThreadId();
    resourceLock = CreateMutex(0, FALSE, L"resourceLock");
    SCHistory::LoadHistory();
    int code = StartGame();
    SCHistory::SaveHistory();
    WaitForSingleObject(resourceLock, INFINITE);
    CloseHandle(resourceLock);
    return code;
}
