#pragma once
#include "VirtualLib.h"
#include "EngineGlobal.h"

typedef struct
{
	Vector3 pos;
	Vector3 normal;
	Vector2 texCoord;
}VertexPositionNormalTexture;

extern ComPtr<ID3D11Buffer> vb_InvColor;
extern ComPtr<ID3D11Buffer> vb_Sprite;

void LoadVertexBuffer();