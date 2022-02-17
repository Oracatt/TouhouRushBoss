#pragma once
#include "VirtualLib.h"

namespace Global
{
	extern ComPtr<IVirtualEngine> engine;
	extern ComPtr<IRenderer> renderer;
	extern ComPtr<IInput> input;
	extern ComPtr<ITimer> timer;
	extern ComPtr<IAudioPlayer> audio;
	extern ComPtr<ID3D11Device> pDevice;
	extern ComPtr<ID3D11DeviceContext> pContext;
	extern ComPtr<IDXGISwapChain> pSwapChain;
}

extern ComPtr<ID3D11DepthStencilState> dsState;
extern ComPtr<ID3D11SamplerState> spAnis;
extern ComPtr<ID3D11SamplerState> spPoint;
extern ComPtr<ID3D11SamplerState> spBorder;
extern ComPtr<ID3D11BlendState> enterBs;
extern ComPtr<ID3D11BlendState> leaveBs;

extern ComPtr<ITextFormat> tf_digit;
extern ComPtr<ITextFormat> tf_digit_center;
extern ComPtr<ITextFormat> tf_wryh;
extern ComPtr<ITextFormat> tf_wryh_big;
extern ComPtr<ITextFormat> tf_songti;
extern ComPtr<ITextFormat> tf_songti_spell;
extern ComPtr<ITextFormat> tf_wryh_volume;
extern ComPtr<ITextFormat> tf_youyuan;

extern ComPtr<ID2D1SolidColorBrush> sb_black;
extern ComPtr<ID2D1SolidColorBrush> sb_white;
extern ComPtr<ID2D1SolidColorBrush> sb_blue;

extern USize bufferSize;

extern DWORD mainThreadID;

extern HANDLE resourceLock;

#define SMSG_RESIZE 0x7f