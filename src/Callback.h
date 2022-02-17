#pragma once
#include "VirtualLib.h"
#include "EngineGlobal.h"

using namespace Global;

/*
	data1:ID3D11SamplerState*
*/
#define SD_SAMPLER   0x00001000L
#define SD_WARPSCALE 0x00001001L
#define SD_RADIUS    0x00001002L
#define SD_LIMIT     0x00001003L
#define SD_CENTER    0x00001004L
#define SD_VPSIZE    0x00001005L
#define SD_COLORKEY  0x00001006L

#define SD_BLURSCALE 0x00001007L

#define SD_TIMER     0x00001008L

class MainEventCallBack :public DefaultEngineEventCallBack
{
public:

	ComPtr<ID3DX11EffectSamplerVariable> mSampler;

	void OnEngineInitialized(IVirtualEngine* eng) override
	{
		engine = eng;
		engine.As(&renderer);
		renderer->GetPointer(IID_ID3D11Device, (void**)&pDevice);
		renderer->GetPointer(IID_ID3D11DeviceContext, (void**)&pContext);
		if (SUCCEEDED(LoadShader()))
		{
			D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};
			D3DX11_PASS_DESC passDesc;
			mPass->GetDesc(&passDesc);
			pDevice->CreateInputLayout(vertexDesc, 3, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &mInputLayout);
			pContext->IASetInputLayout(mInputLayout.Get());
			pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}
	}

	void OnEngineDestroy() override
	{
		mSampler.Reset();
		__super::OnEngineDestroy();
	}

	void RecvShaderData(UINT type, LPVOID data1, LPVOID data2) override
	{
		__super::RecvShaderData(type, data1, data2);
		if (type == SD_SAMPLER)
		{
			mSampler->SetSampler(0, (ID3D11SamplerState*)data1);
		}
	}

	HRESULT LoadShader()
	{
		DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		shaderFlags |= D3D10_SHADER_DEBUG;
		shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
		ComPtr<ID3DBlob> compilationMsgs = 0;
		vector<BYTE> buffer;
		LoadResourceFromFile("shader\\default.fx", buffer, 0);
		HRESULT hr = D3DX11CompileEffectFromMemory(buffer.data(), buffer.size(), "tech.fx", 0,
			0, 0, 0, pDevice.Get(), &effect, &compilationMsgs);
		bool isContain(string str1, string str2);
		if (compilationMsgs != 0 && isContain((char*)compilationMsgs->GetBufferPointer(), "error"))
		{
			engine->ReportFatalError((char*)compilationMsgs->GetBufferPointer(), FE_UNKNOWN);
			return ERR_UNKNOWN;
		}

		mTech = effect->GetTechniqueByName("T0");
		mPass = mTech->GetPassByIndex(0);
		mMatWVP = effect->GetVariableByName("gWorldViewProj")->AsMatrix();
		mLights = effect->GetVariableByName("gLights");
		mLightCount = effect->GetVariableByName("gLightCount")->AsScalar();
		mMaterial = effect->GetVariableByName("gMaterial");
		mTexture = effect->GetVariableByName("gDiffuseMap")->AsShaderResource();
		mFogStart = effect->GetVariableByName("gFogStart")->AsScalar();
		mFogRange = effect->GetVariableByName("gFogRange")->AsScalar();
		mFogColor = effect->GetVariableByName("gFogColor")->AsVector();
		mFogEnabled = effect->GetVariableByName("gFogEnabled")->AsScalar();
		mEyePos = effect->GetVariableByName("gEyePosW");
		mMatWorld = effect->GetVariableByName("gWorld")->AsMatrix();
		mMatWIT = effect->GetVariableByName("gWorldInvTranspose")->AsMatrix();
		mMatTexture = effect->GetVariableByName("gTexTransform")->AsMatrix();
		mInvEnabled = effect->GetVariableByName("gInvColor")->AsScalar();
		mSampler = effect->GetVariableByName("samp")->AsSampler();

		return SUCCEED_OPERATE;
	}
};

class ResizeCallBack :public EngineEventCallBack
{
	void OnEngineInitialized(struct IVirtualEngine* engine) {}

	void OnEngineDestroy() {}

	void OnStartRender() {}

	void OnBufferResized(UINT width, UINT height)
	{
		bufferSize = { width,height };
		engine->BroadcastMessage(SMSG_RESIZE, (LPVOID)width, (LPVOID)height);
	}

	void RecvShaderData(UINT type, LPVOID data1, LPVOID data2) {}
};

class WarpCallBack :public DefaultEngineEventCallBack
{
	ComPtr<ID3DX11EffectSamplerVariable> mSampler;

	ComPtr<ID3DX11EffectScalarVariable> mWarpScale;

	ComPtr<ID3DX11EffectScalarVariable> mRadius;

	ComPtr<ID3DX11EffectScalarVariable> mLimit;

	ComPtr<ID3DX11EffectVariable> mCenter;

	ComPtr<ID3DX11EffectVariable> mVpSize;

	ComPtr<ID3DX11EffectScalarVariable> mColorKey;

	void OnEngineInitialized(struct IVirtualEngine* eng)
	{
		engine = eng;
		engine.As(&renderer);
		renderer->GetPointer(IID_ID3D11Device, (void**)&pDevice);
		renderer->GetPointer(IID_ID3D11DeviceContext, (void**)&pContext);
		LoadShader();
	}

	void OnEngineDestroy()
	{
		mSampler.Reset();
		mWarpScale.Reset();
		mRadius.Reset();
		mLimit.Reset();
		mCenter.Reset();
		mVpSize.Reset();
		mColorKey.Reset();

		__super::OnEngineDestroy();
	}

	void RecvShaderData(UINT type, LPVOID data1, LPVOID data2)
	{
		switch (type)
		{
		case SD_TEXTURE:
			mTexture->SetResource((ID3D11ShaderResourceView*)data1);
			break;
		case SD_SAMPLER:
			mSampler->SetSampler(0, (ID3D11SamplerState*)data1);
			break;
		case SD_WARPSCALE:
			mWarpScale->SetFloat(*((float*)data1));
			break;
		case SD_RADIUS:
			mRadius->SetFloat(*((float*)data1));
			break;
		case SD_LIMIT:
			mLimit->SetFloat(*((float*)data1));
			break;
		case SD_CENTER:
			mCenter->SetRawValue(data1, 0, sizeof(Vector2));
			break;
		case SD_VPSIZE:
			mVpSize->SetRawValue(data1, 0, sizeof(Vector2));
			break;
		case SD_COLORKEY:
			mColorKey->SetInt((int)data1);
			break;
		}
	}

	HRESULT LoadShader()
	{
		DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		shaderFlags |= D3D10_SHADER_DEBUG;
		shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
		ComPtr<ID3DBlob> compilationMsgs = 0;
		vector<BYTE> buffer;
		LoadResourceFromFile("shader\\warp.fx", buffer, 0);
		HRESULT hr = D3DX11CompileEffectFromMemory(buffer.data(), buffer.size(), "tech.fx", 0,
			0, 0, 0, pDevice.Get(), &effect, &compilationMsgs);
		bool isContain(string str1, string str2);
		if (compilationMsgs != 0 && isContain((char*)compilationMsgs->GetBufferPointer(), "error"))
		{
			engine->ReportFatalError((char*)compilationMsgs->GetBufferPointer(), FE_UNKNOWN);
			return ERR_UNKNOWN;
		}

		mTech = effect->GetTechniqueByName("T0");
		mPass = mTech->GetPassByIndex(0);
		mTexture = effect->GetVariableByName("gDiffuseMap")->AsShaderResource();
		mSampler = effect->GetVariableByName("samp")->AsSampler();
		mWarpScale = effect->GetVariableByName("warpScale")->AsScalar();
		mRadius = effect->GetVariableByName("radius")->AsScalar();
		mLimit = effect->GetVariableByName("limit")->AsScalar();
		mCenter = effect->GetVariableByName("center");
		mVpSize = effect->GetVariableByName("vpSize");
		mMatWVP = effect->GetVariableByName("gWorldViewProj")->AsMatrix();
		mMatWorld = effect->GetVariableByName("gWorld")->AsMatrix();
		mMatWIT = effect->GetVariableByName("gWorldInvTranspose")->AsMatrix();
		mMatTexture = effect->GetVariableByName("gTexTransform")->AsMatrix();
		mColorKey = effect->GetVariableByName("colorKey")->AsScalar();

		return SUCCEED_OPERATE;
	}
};

class BlurCallBack :public DefaultEngineEventCallBack
{
	ComPtr<ID3DX11EffectSamplerVariable> mSampler;

	ComPtr<ID3DX11EffectScalarVariable> mScale;

	void OnEngineInitialized(struct IVirtualEngine* eng)
	{
		engine = eng;
		engine.As(&renderer);
		renderer->GetPointer(IID_ID3D11Device, (void**)&pDevice);
		renderer->GetPointer(IID_ID3D11DeviceContext, (void**)&pContext);
		LoadShader();
	}

	void OnEngineDestroy()
	{
		mSampler.Reset();
		mScale.Reset();

		__super::OnEngineDestroy();
	}

	void RecvShaderData(UINT type, LPVOID data1, LPVOID data2)
	{
		switch (type)
		{
		case SD_TEXTURE:
			mTexture->SetResource((ID3D11ShaderResourceView*)data1);
			break;
		case SD_SAMPLER:
			mSampler->SetSampler(0, (ID3D11SamplerState*)data1);
			break;
		case SD_BLURSCALE:
			mScale->SetFloat(*((float*)data1));
			break;
		}
	}

	HRESULT LoadShader()
	{
		DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		shaderFlags |= D3D10_SHADER_DEBUG;
		shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
		ComPtr<ID3DBlob> compilationMsgs = 0;
		vector<BYTE> buffer;
		LoadResourceFromFile("shader\\blur.fx", buffer, 0);
		HRESULT hr = D3DX11CompileEffectFromMemory(buffer.data(), buffer.size(), "tech.fx", 0,
			0, 0, 0, pDevice.Get(), &effect, &compilationMsgs);
		bool isContain(string str1, string str2);
		if (compilationMsgs != 0 && isContain((char*)compilationMsgs->GetBufferPointer(), "error"))
		{
			engine->ReportFatalError((char*)compilationMsgs->GetBufferPointer(), FE_UNKNOWN);
			return ERR_UNKNOWN;
		}

		mTech = effect->GetTechniqueByName("T0");
		mPass = mTech->GetPassByIndex(0);
		mTexture = effect->GetVariableByName("gDiffuseMap")->AsShaderResource();
		mSampler = effect->GetVariableByName("samp")->AsSampler();
		mScale = effect->GetVariableByName("scale")->AsScalar();
		mMatWVP = effect->GetVariableByName("gWorldViewProj")->AsMatrix();
		mMatWorld = effect->GetVariableByName("gWorld")->AsMatrix();
		mMatWIT = effect->GetVariableByName("gWorldInvTranspose")->AsMatrix();
		mMatTexture = effect->GetVariableByName("gTexTransform")->AsMatrix();

		return SUCCEED_OPERATE;
	}
};

class WaterCallBack :public DefaultEngineEventCallBack
{
	ComPtr<ID3DX11EffectSamplerVariable> mSampler;

	ComPtr<ID3DX11EffectScalarVariable> mTimer;

	void OnEngineInitialized(struct IVirtualEngine* eng)
	{
		engine = eng;
		engine.As(&renderer);
		renderer->GetPointer(IID_ID3D11Device, (void**)&pDevice);
		renderer->GetPointer(IID_ID3D11DeviceContext, (void**)&pContext);
		LoadShader();
	}

	void OnEngineDestroy()
	{
		mSampler.Reset();
		mTimer.Reset();

		__super::OnEngineDestroy();
	}

	void RecvShaderData(UINT type, LPVOID data1, LPVOID data2)
	{
		switch (type)
		{
		case SD_TEXTURE:
			mTexture->SetResource((ID3D11ShaderResourceView*)data1);
			break;
		case SD_SAMPLER:
			mSampler->SetSampler(0, (ID3D11SamplerState*)data1);
			break;
		case SD_TIMER:
			mTimer->SetFloat(*((float*)data1));
			break;
		}
	}

	HRESULT LoadShader()
	{
		DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		shaderFlags |= D3D10_SHADER_DEBUG;
		shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
		ComPtr<ID3DBlob> compilationMsgs = 0;
		vector<BYTE> buffer;
		LoadResourceFromFile("shader\\water.fx", buffer, 0);
		HRESULT hr = D3DX11CompileEffectFromMemory(buffer.data(), buffer.size(), "tech.fx", 0,
			0, 0, 0, pDevice.Get(), &effect, &compilationMsgs);
		bool isContain(string str1, string str2);
		if (compilationMsgs != 0 && isContain((char*)compilationMsgs->GetBufferPointer(), "error"))
		{
			engine->ReportFatalError((char*)compilationMsgs->GetBufferPointer(), FE_UNKNOWN);
			return ERR_UNKNOWN;
		}

		mTech = effect->GetTechniqueByName("T0");
		mPass = mTech->GetPassByIndex(0);
		mTexture = effect->GetVariableByName("gDiffuseMap")->AsShaderResource();
		mSampler = effect->GetVariableByName("samp")->AsSampler();
		mTimer = effect->GetVariableByName("timer")->AsScalar();
		mMatWVP = effect->GetVariableByName("gWorldViewProj")->AsMatrix();
		mMatWorld = effect->GetVariableByName("gWorld")->AsMatrix();
		mMatWIT = effect->GetVariableByName("gWorldInvTranspose")->AsMatrix();
		mMatTexture = effect->GetVariableByName("gTexTransform")->AsMatrix();

		return SUCCEED_OPERATE;
	}
};

extern MainEventCallBack mainCallback;
extern ResizeCallBack resizeCallback;
extern WarpCallBack warpCallback;
extern BlurCallBack blurCallback;
extern WaterCallBack waterCallback;

EngineEventCallBack* GetCurrentCallback();

void SetCurrentCallback(EngineEventCallBack* pCallBack);