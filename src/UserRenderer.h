#pragma once
#include "VirtualLib.h"
#include "EngineGlobal.h"
#include "VertexBuffer.h"
#include "Source.h"

class CircleDrawer :public SpriteRender
{
public:

	CircleDrawer(float _minRadius, float _maxRadius, int _round) :
		needMap(false), minRadius(_minRadius), maxRadius(_maxRadius), percent(1), mPercent(1), round(_round)
	{
		D3D11_BUFFER_DESC vbd;
		D3D11_SUBRESOURCE_DATA vinitData;
		VertexPositionNormalTexture* verts = new VertexPositionNormalTexture[1026];
		for (int i = 0; i < 513; i++)
		{
			verts[i].pos = { (float)cos(i * M_PI / 256.0f) * maxRadius, (float)sin(i * M_PI / 256.0f) * maxRadius, 0 };
			verts[i].normal = { 0,0,-1 };
			verts[i].texCoord = { 1,i * round / 512.0f };
			verts[i + 513].pos = { (float)cos(i * M_PI / 256.0f) * minRadius,(float)sin(i * M_PI / 256.0f) * minRadius,0 };
			verts[i + 513].normal = { 0,0,-1 };
			verts[i + 513].texCoord = { 0,i * round / 512.0f };
		}
		vbd.Usage = D3D11_USAGE_DYNAMIC;
		vbd.ByteWidth = sizeof(VertexPositionNormalTexture) * 1026;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;
		vinitData.pSysMem = verts;
		Global::pDevice->CreateBuffer(&vbd, &vinitData, &vb);
		delete[] verts;

		D3D11_BUFFER_DESC ibd;
		D3D11_SUBRESOURCE_DATA iinitData;
		short* indices = new short[3078];
		int index = 0;
		for (int i = 0; i < 512; i++)
		{
			indices[index++] = (short)i;
			indices[index++] = (short)(i + 1);
			indices[index++] = (short)(i + 513);
			indices[index++] = (short)(i + 1);
			indices[index++] = (short)(i + 514);
			indices[index++] = (short)(i + 513);
		}
		indices[index++] = 512;
		indices[index++] = 0;
		indices[index++] = 1025;
		indices[index++] = 0;
		indices[index++] = 513;
		indices[index++] = 1025;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(short) * 3078;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;
		iinitData.pSysMem = indices;
		Global::pDevice->CreateBuffer(&ibd, &iinitData, &ib);
		delete[] indices;
	}

	int GetRound()
	{
		return round;
	}

	void SetRound(int r)
	{
		round = r;
		needMap = true;
	}

	float GetMinRadius()
	{
		return minRadius;
	}

	void SetMinRadius(float r)
	{
		minRadius = r;
		needMap = true;
	}

	float GetMaxRadius()
	{
		return maxRadius;
	}

	void SetMaxRadius(float r)
	{
		maxRadius = r;
		needMap = true;
	}

	float GetPercent()
	{
		return percent;
	}

	void SetPercent(float r)
	{
		percent = r;
	}

protected:

	bool needMap;

	int round;

	float minRadius;

	float maxRadius;

	float percent;

	ComPtr<ID3D11Buffer> vb;

	ComPtr<ID3D11Buffer> ib;

	void OnFetchData() override
	{
		__super::OnFetchData();
		mPercent = percent;
		if (needMap)
		{
			D3D11_MAPPED_SUBRESOURCE subRes;
			pContext->Map(vb.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes);
			VertexPositionNormalTexture* verts = (VertexPositionNormalTexture*)subRes.pData;
			for (int i = 0; i < 513; i++)
			{
				verts[i].pos = { (float)cos(i * M_PI / 256.0f) * maxRadius, (float)sin(i * M_PI / 256.0f) * maxRadius, 0 };
				verts[i].normal = { 0,0,-1 };
				verts[i].texCoord = { 1,i * round / 512.0f };
				verts[i + 513].pos = { (float)cos(i * M_PI / 256.0f) * minRadius,(float)sin(i * M_PI / 256.0f) * minRadius,0 };
				verts[i + 513].normal = { 0,0,-1 };
				verts[i + 513].texCoord = { 0,i * round / 512.0f };
			}
			pContext->Unmap(vb.Get(), 0);
			needMap = false;
		}
	}

	void OnRender() override
	{
		UINT stride = 32;
		UINT offset = 0;

		pContext->IASetVertexBuffers(0, 1, vb.GetAddressOf(), &stride, &offset);
		pContext->IASetIndexBuffer(ib.Get(), DXGI_FORMAT_R16_UINT, 0);

		renderer->SetWorldMatrix(mlocalTransform * transform);
		renderer->SetTextureMatrix(mTexTransform);
		if (mTexture)
			renderer->SendShaderData(SD_TEXTURE, mTexture->GetShaderResourceView(), 0);
		else
			renderer->SendShaderData(SD_TEXTURE, 0, 0);
		renderer->SendShaderData(SD_MATERIAL, &mMaterial, 0);
		renderer->StartRender();
		pContext->DrawIndexed(((int)(1026 * mPercent)) * 3, 0, 0);
	}

private:

	float mPercent;
};

class SpriteTextRender :public SpriteRender
{
public:

	bool enabled;

	bool highlight;

	SpriteTextRender() :needCopy(false), margin(0.68), m_margin(0.68),enabled(true), m_enabled(true)
	{
		Material mat;
		mat.diffuse = { 1,1,1,1 };
		mat.ambient = { 0,0,0,0 };
		mat.specular = { 0,0,0,0 };
		mat.power = 0;
		SetMaterial(mat);
		SetTexture(src_ascii.Get());
	}

	string GetText()
	{
		return str;
	}

	void SetText(const string& _str)
	{
		str = _str;
		needCopy = true;
	}

	void SetMargin(float _margin)
	{
		margin = _margin;
	}

protected:

	string str;

	bool needCopy;

	void OnFetchData() override
	{
		__super::OnFetchData();
		if (needCopy)
		{
			m_str = str;
			needCopy = false;
		}
		m_margin = margin;
		m_highlight = highlight;
		m_enabled = enabled;
	}

	void OnRender() override
	{
		UINT stride = 32;
		UINT offset = 0;

		pContext->IASetVertexBuffers(0, 1, vb_Sprite.GetAddressOf(), &stride, &offset);

		if (mTexture)
			renderer->SendShaderData(SD_TEXTURE, mTexture->GetShaderResourceView(), 0);
		else
			renderer->SendShaderData(SD_TEXTURE, 0, 0);

		Material hlmat;
		hlmat.diffuse = { 1,1,0,1 };
		hlmat.ambient = { 0,0,0,0 };
		hlmat.specular = { 0,0,0,0 };
		hlmat.power = 0;

		Material disableMat;
		disableMat.diffuse = { 0.5,0.5,0.5,1 };
		disableMat.ambient = { 0,0,0,0 };
		disableMat.specular = { 0,0,0,0 };
		disableMat.power = 0;

		Material shadowMat;
		shadowMat.diffuse = { 0.15,0.15,0.15,1 };
		shadowMat.ambient = { 0,0,0,0 };
		shadowMat.specular = { 0,0,0,0 };
		shadowMat.power = 0;

		renderer->SendShaderData(SD_MATERIAL, &shadowMat, 0);
		
		for (int i = 0; i < m_str.length(); i++)
		{
			RectangleF rect = GetCharSource(m_str[i]);
			XMMATRIX texMat = XMMatrixIdentity();
			texMat *= XMMatrixScaling(rect.width / 1, rect.height / 1, 1);
			texMat *= XMMatrixTranslation(rect.x, rect.y, 0);

			renderer->SetWorldMatrix(mlocalTransform * XMMatrixTranslation(m_margin * i + 0.075f, -0.075f, 0) * transform);
			renderer->SetTextureMatrix(texMat);
			renderer->StartRender();
			pContext->Draw(6, 0);
		}
		
		if (m_highlight)
			renderer->SendShaderData(SD_MATERIAL, &hlmat, 0);
		else if (m_enabled)
			renderer->SendShaderData(SD_MATERIAL, &mMaterial, 0);
		else
			renderer->SendShaderData(SD_MATERIAL, &disableMat, 0);

		for (int i = 0; i < m_str.length(); i++)
		{
			RectangleF rect = GetCharSource(m_str[i]);
			XMMATRIX texMat = XMMatrixIdentity();
			texMat *= XMMatrixScaling(rect.width / 1, rect.height / 1, 1);
			texMat *= XMMatrixTranslation(rect.x, rect.y, 0);

			renderer->SetWorldMatrix(mlocalTransform * XMMatrixTranslation(m_margin * i, 0, 0) * transform);
			renderer->SetTextureMatrix(texMat);
			renderer->StartRender();
			pContext->Draw(6, 0);
		}
	}

private:

	string m_str;

	float margin;

	float m_margin;

	bool m_highlight;

	bool m_enabled;

	RectangleF GetCharSource(unsigned char c)
	{
		int row = (c - 32) / 18;
		int column = (c - 32) % 18;
		return { column * 28 / 512.0f,row * 28 / 512.0f + 0.001f,28 / 512.0f,28 / 512.0f - 0.002f };
	}
};