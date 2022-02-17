#include "VertexBuffer.h"

using namespace Global;

ComPtr<ID3D11Buffer> vb_InvColor;
ComPtr<ID3D11Buffer> vb_Sprite;

void LoadVertexBuffer()
{
	D3D11_BUFFER_DESC vbd;
	D3D11_SUBRESOURCE_DATA vinitData;
	VertexPositionNormalTexture* verts = new VertexPositionNormalTexture[768];
	for (int i = 0; i < 256; i++)
	{
		float rad_1 = -(float)(M_PI * 2 * (i - 1) / 256.0f);
		float rad_2 = -(float)(M_PI * 2 * i / 256.0f);
		verts[3 * i].pos = { 0,0,0 };
		verts[3 * i].normal = { 0,0,-1 };
		verts[3 * i].texCoord = { 0.5f,0.5f };
		verts[3 * i + 1].pos = { cos(rad_1),sin(rad_1),0 };
		verts[3 * i + 1].normal = { 0,0,-1 };
		verts[3 * i + 1].texCoord = { 0.5f + 0.5f * cos(rad_1),0.5f + 0.5f * sin(rad_1) };
		verts[3 * i + 2].pos = { cos(rad_2),sin(rad_2),0 };
		verts[3 * i + 2].normal = { 0,0,-1 };
		verts[3 * i + 2].texCoord = { 0.5f + 0.5f * cos(rad_2),0.5f + 0.5f * sin(rad_2) };
	}
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexPositionNormalTexture) * 768;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	vinitData.pSysMem = verts;
	pDevice->CreateBuffer(&vbd, &vinitData, &vb_InvColor);
	delete[] verts;

	verts = new VertexPositionNormalTexture[6]{
		{{-0.5,0.5,0},{0,0,-1},{0,0}},
		{{0.5,0.5,0},{0,0,-1},{1,0}},
		{{-0.5,-0.5,0},{0,0,-1},{0,1}},
		{{-0.5,-0.5,0},{0,0,-1},{0,1}},
		{{0.5,0.5,0},{0,0,-1},{1,0}},
		{{0.5,-0.5,0},{0,0,-1},{1,1}}
	};
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexPositionNormalTexture) * 6;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	vinitData.pSysMem = verts;
	pDevice->CreateBuffer(&vbd, &vinitData, &vb_Sprite);
	delete[] verts;
}